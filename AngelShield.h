// ==============================================================================
// File: AngelShield.h (Top-Tier Commercial DRM & Section-Extraction Edition)
// Target: Latest Dopamine 2.x Rootless (/var/jb, iphoneos-arm64, ElleKit)
// ==============================================================================

#ifndef AngelShield_h
#define AngelShield_h

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import <CommonCrypto/CommonCryptor.h>
#import <CommonCrypto/CommonHMAC.h>
#import <mach-o/dyld.h>
#import <mach-o/getsect.h>
#import <dlfcn.h>
#import <sys/stat.h>
#import <os/log.h>
#import <os/lock.h>

// 1. 核心读写数据段宏定义（位于 __DATA 段，天然具备 RW 权限，100% 兼容 Dopamine PPL 保护）
#define ANGEL_CORE_DATA __attribute__((section("__DATA,__ag_core"), used, aligned(16)))
#define ANGEL_INLINE    static inline __attribute__((always_inline))

// Dopamine 无根配置路径与系统共享路径
static NSString * const kAGDopaminePrefsPath = @"/var/jb/var/mobile/Library/Preferences/com.custom.geminisirioverlay.plist";
static NSString * const kAGSystemPrefsPath   = @"/var/mobile/Library/Preferences/com.custom.geminisirioverlay.plist";
static NSString * const kAGCloudCardPrefsKey = @"AG_CloudBrain_CardKey";
static NSString * const kAGCloudCipherKey    = @"AG_CloudBrain_CoreCipher";

// 段内完整性金丝雀魔数（未加密调试态或解密成功后为 0xA6590C0E；云脑抽离加密后变为密文）
ANGEL_CORE_DATA static uint32_t gAngelCoreCanaryMagic = 0xA6590C0E;

static BOOL gAngelIsAuthorized = NO;
static uint64_t gAngelRuntimeTicket = 0;
static NSInteger gAngelRemainingDays = 0;
static os_unfair_lock gAngelCryptLock = OS_UNFAIR_LOCK_INIT;
static UIWindow *gAngelAuthAlertWindow = nil;

@interface AGCloudAuthAlertWindow : UIWindow
@end
@implementation AGCloudAuthAlertWindow
- (BOOL)_canShowWhileLocked { return YES; }
@end

// ==================== 防线 1：栈上动态还原 URL 与 SECRET_SALT（IDA Strings 零暴露） ====================
// "https://angel7758.duckdns.org:38080/api/v1/verify" ^ 0x5A
ANGEL_INLINE NSString *Angel_DecodeVerifyURL(void) {
    const uint8_t enc[] = {
        0x32,0x2E,0x2E,0x2A,0x29,0x60,0x75,0x75,0x3B,0x34,0x3D,0x3F,0x36,0x6D,0x6D,0x6F,
        0x62,0x74,0x3E,0x2F,0x39,0x31,0x3E,0x34,0x29,0x74,0x35,0x28,0x3D,0x60,0x69,0x62,
        0x6A,0x62,0x6A,0x75,0x3B,0x2A,0x33,0x75,0x2C,0x6B,0x75,0x2C,0x3F,0x28,0x33,0x3C,0x23
    };
    char buf[sizeof(enc) + 1];
    for (size_t i = 0; i < sizeof(enc); i++) buf[i] = (char)(enc[i] ^ 0x5A);
    buf[sizeof(enc)] = '\0';
    NSString *s = [NSString stringWithUTF8String:buf];
    memset(buf, 0, sizeof(buf));
    return s;
}

// "AG_v590_US_Cloud_Brain_MasterKey_2026!" ^ 0x5A
ANGEL_INLINE void Angel_DecodeSecretSaltToBuf(char outBuf[64]) {
    const uint8_t enc[] = {
        0x1B,0x1D,0x05,0x2C,0x6F,0x63,0x6A,0x05,0x0F,0x09,0x05,0x19,0x36,0x35,0x2F,0x3E,
        0x05,0x18,0x28,0x3B,0x33,0x34,0x05,0x17,0x3B,0x29,0x2E,0x3F,0x28,0x11,0x3F,0x23,
        0x05,0x68,0x6A,0x68,0x6C,0x7B
    };
    for (size_t i = 0; i < sizeof(enc); i++) outBuf[i] = (char)(enc[i] ^ 0x5A);
    outBuf[sizeof(enc)] = '\0';
}

ANGEL_INLINE uint8_t ag_hexCharToNibble(char c) {
    if (c >= '0' && c <= '9') return (uint8_t)(c - '0');
    if (c >= 'a' && c <= 'f') return (uint8_t)(c - 'a' + 10);
    if (c >= 'A' && c <= 'F') return (uint8_t)(c - 'A' + 10);
    return 0;
}

ANGEL_INLINE BOOL ag_hexStringToBytes(const char *hexStr, uint8_t *outBytes, size_t maxBytes, size_t *outActualLen) {
    if (!hexStr || !outBytes) return NO;
    size_t hexLen = strlen(hexStr);
    if (hexLen < 2 || (hexLen % 2) != 0) return NO;
    size_t byteLen = hexLen / 2;
    if (byteLen > maxBytes) byteLen = maxBytes;
    for (size_t i = 0; i < byteLen; i++) {
        outBytes[i] = (ag_hexCharToNibble(hexStr[i * 2]) << 4) | ag_hexCharToNibble(hexStr[i * 2 + 1]);
    }
    if (outActualLen) *outActualLen = byteLen;
    return YES;
}

// ==================== 防线 4：核心段原地流解密 + 过期/盗版回填内存粉碎自毁 ====================
ANGEL_INLINE uint8_t *Angel_LocateCoreSection(unsigned long *outSize) {
    Dl_info dylibInfo;
    memset(&dylibInfo, 0, sizeof(dylibInfo));
    if (dladdr((const void *)&gAngelCryptLock, &dylibInfo) == 0 || !dylibInfo.dli_fbase) {
        return NULL;
    }
    const struct mach_header_64 *header = (const struct mach_header_64 *)dylibInfo.dli_fbase;
    return getsectiondata(header, "__DATA", "__ag_core", outSize);
}

// 当服务器返回 401/402 时，即使破解者曾用内存 Dump 回填过 .dylib，也当场把内存中的 __DATA,__ag_core 粉碎为乱码！
ANGEL_INLINE void Angel_ScrambleCoreSectionOnExpire(void) {
    os_unfair_lock_lock(&gAngelCryptLock);
    gAngelIsAuthorized = NO;
    gAngelRuntimeTicket = 0;
    unsigned long secSize = 0;
    uint8_t *secPtr = Angel_LocateCoreSection(&secSize);
    if (secPtr && secSize > 0) {
        for (unsigned long i = 0; i < secSize; i++) {
            secPtr[i] ^= (uint8_t)(0xA7 + (i & 0x3F));
        }
    }
    os_unfair_lock_unlock(&gAngelCryptLock);
}

ANGEL_INLINE BOOL Angel_DecryptCoreSection(const char *hexKey) {
    if (!hexKey || strlen(hexKey) < 64) {
        return NO;
    }

    os_unfair_lock_lock(&gAngelCryptLock);

    if (gAngelCoreCanaryMagic == 0xA6590C0E) {
        gAngelIsAuthorized = YES;
        gAngelRuntimeTicket = ((uint64_t)gAngelCoreCanaryMagic << 16) ^ 0x5F3759DFULL;
        os_unfair_lock_unlock(&gAngelCryptLock);
        return YES;
    }

    uint8_t rawBytes[48];
    memset(rawBytes, 0, sizeof(rawBytes));
    size_t parsedLen = 0;
    if (!ag_hexStringToBytes(hexKey, rawBytes, sizeof(rawBytes), &parsedLen) || parsedLen < 32) {
        os_unfair_lock_unlock(&gAngelCryptLock);
        return NO;
    }

    uint8_t aesKey[kCCKeySizeAES256];
    uint8_t ctrIV[kCCBlockSizeAES128];
    memcpy(aesKey, rawBytes, kCCKeySizeAES256);
    if (parsedLen >= 48) {
        memcpy(ctrIV, rawBytes + 32, kCCBlockSizeAES128);
    } else {
        memset(ctrIV, 0, kCCBlockSizeAES128);
    }

    unsigned long sectionSize = 0;
    uint8_t *sectionPtr = Angel_LocateCoreSection(&sectionSize);
    if (!sectionPtr || sectionSize == 0) {
        memset(aesKey, 0, sizeof(aesKey));
        memset(rawBytes, 0, sizeof(rawBytes));
        os_unfair_lock_unlock(&gAngelCryptLock);
        return NO;
    }

    CCCryptorRef cryptor = NULL;
    CCCryptorStatus status = CCCryptorCreateWithMode(
        kCCDecrypt,
        kCCModeCTR,
        kCCAlgorithmAES,
        ccNoPadding,
        ctrIV,
        aesKey,
        kCCKeySizeAES256,
        NULL,
        0,
        0,
        kCCModeOptionCTR_BE,
        &cryptor
    );

    if (status == kCCSuccess && cryptor != NULL) {
        size_t dataOutMoved = 0;
        status = CCCryptorUpdate(cryptor, sectionPtr, (size_t)sectionSize, sectionPtr, (size_t)sectionSize, &dataOutMoved);
        CCCryptorRelease(cryptor);
    }

    memset(aesKey, 0, sizeof(aesKey));
    memset(rawBytes, 0, sizeof(rawBytes));

    if (status == kCCSuccess) {
        gAngelIsAuthorized = YES;
        gAngelRuntimeTicket = ((uint64_t)gAngelCoreCanaryMagic << 16) ^ 0x5F3759DFULL;
    }
    os_unfair_lock_unlock(&gAngelCryptLock);
    return (status == kCCSuccess);
}

// ==================== 获取设备真实硬件指纹 UDID ====================
ANGEL_INLINE NSString *Angel_GetHardwareUDID(void) {
    static NSString *sCachedUDID = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        void *gestalt = dlopen("/usr/lib/libMobileGestalt.dylib", RTLD_LAZY);
        if (gestalt) {
            typedef CFPropertyListRef (*MGCopyAnswer_fn)(CFStringRef prop);
            MGCopyAnswer_fn mgCopy = (MGCopyAnswer_fn)dlsym(gestalt, "MGCopyAnswer");
            if (mgCopy) {
                CFStringRef udidRef = (CFStringRef)mgCopy(CFSTR("UniqueDeviceID"));
                if (udidRef && CFGetTypeID(udidRef) == CFStringGetTypeID()) {
                    sCachedUDID = [(__bridge NSString *)udidRef copy];
                    CFRelease(udidRef);
                }
            }
        }
        if (!sCachedUDID || sCachedUDID.length < 8) {
            sCachedUDID = [[[UIDevice currentDevice] identifierForVendor] UUIDString];
        }
        if (!sCachedUDID || sCachedUDID.length < 8) {
            sCachedUDID = @"UNKNOWN_DOPAMINE_UDID";
        }
    });
    return sCachedUDID;
}

ANGEL_INLINE NSString *Angel_GenerateNonce16(void) {
    static const char kChars[] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    char buf[17];
    for (int i = 0; i < 16; i++) {
        buf[i] = kChars[arc4random_uniform((uint32_t)(sizeof(kChars) - 1))];
    }
    buf[16] = '\0';
    return [NSString stringWithUTF8String:buf];
}

ANGEL_INLINE NSString *Angel_ComputeHMACSHA256WithHiddenSalt(NSString *payload) {
    char saltBuf[64];
    Angel_DecodeSecretSaltToBuf(saltBuf);
    const char *cData = [payload UTF8String];
    if (!cData) {
        memset(saltBuf, 0, sizeof(saltBuf));
        return @"";
    }
    unsigned char cHMAC[CC_SHA256_DIGEST_LENGTH];
    CCHmac(kCCHmacAlgSHA256, saltBuf, strlen(saltBuf), cData, strlen(cData), cHMAC);
    memset(saltBuf, 0, sizeof(saltBuf));

    NSMutableString *hexOut = [NSMutableString stringWithCapacity:CC_SHA256_DIGEST_LENGTH * 2];
    for (int i = 0; i < CC_SHA256_DIGEST_LENGTH; i++) {
        [hexOut appendFormat:@"%02x", cHMAC[i]];
    }
    return hexOut;
}

// ==================== 防线 3：硬件 UDID 绑定的跨进程 core_key 加密缓存（防 Filza 查看与跨机拷贝） ====================
ANGEL_INLINE NSString *Angel_TransformCoreKeyWithHardwareUDID(NSString *inputHexStr) {
    if (!inputHexStr || inputHexStr.length < 64) return nil;
    NSString *udidMaskHex = Angel_ComputeHMACSHA256WithHiddenSalt([NSString stringWithFormat:@"HW_BIND_%@", Angel_GetHardwareUDID()]);
    if (udidMaskHex.length < 64) return nil;

    uint8_t inBytes[48], maskBytes[32], outBytes[48];
    size_t inLen = 0, maskLen = 0;
    if (!ag_hexStringToBytes([inputHexStr UTF8String], inBytes, sizeof(inBytes), &inLen) ||
        !ag_hexStringToBytes([udidMaskHex UTF8String], maskBytes, sizeof(maskBytes), &maskLen) ||
        maskLen < 32) {
        return nil;
    }
    for (size_t i = 0; i < inLen; i++) {
        outBytes[i] = inBytes[i] ^ maskBytes[i % 32] ^ (uint8_t)(0x3C + i);
    }
    NSMutableString *outHex = [NSMutableString stringWithCapacity:inLen * 2];
    for (size_t i = 0; i < inLen; i++) {
        [outHex appendFormat:@"%02x", outBytes[i]];
    }
    memset(inBytes, 0, sizeof(inBytes));
    memset(outBytes, 0, sizeof(outBytes));
    return outHex;
}

static NSDictionary *Angel_ReadSharedPrefsDict(void) {
    NSDictionary *d = [NSDictionary dictionaryWithContentsOfFile:kAGDopaminePrefsPath];
    if ([d isKindOfClass:[NSDictionary class]] && d.count > 0) return d;
    d = [NSDictionary dictionaryWithContentsOfFile:kAGSystemPrefsPath];
    return [d isKindOfClass:[NSDictionary class]] ? d : @{};
}

static void Angel_WriteSharedPrefsKeyValues(NSDictionary *kvPairs, NSArray<NSString *> *keysToRemove) {
    NSArray<NSString *> *paths = @[kAGDopaminePrefsPath, kAGSystemPrefsPath];
    for (NSString *p in paths) {
        NSString *dir = [p stringByDeletingLastPathComponent];
        if (![[NSFileManager defaultManager] fileExistsAtPath:dir]) {
            [[NSFileManager defaultManager] createDirectoryAtPath:dir withIntermediateDirectories:YES attributes:nil error:nil];
        }
        NSMutableDictionary *mut = [NSMutableDictionary dictionaryWithContentsOfFile:p] ?: [NSMutableDictionary dictionary];
        for (NSString *rmKey in keysToRemove) {
            [mut removeObjectForKey:rmKey];
        }
        if (kvPairs) {
            [mut addEntriesFromDictionary:kvPairs];
        }
        [mut writeToFile:p atomically:YES];
        chmod([p UTF8String], 0644);
    }
}

static NSString *Angel_LoadSavedCardKey(void) {
    NSDictionary *dict = Angel_ReadSharedPrefsDict();
    NSString *saved = [dict[kAGCloudCardPrefsKey] stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceAndNewlineCharacterSet]];
    return (saved && saved.length > 0) ? saved : @"AUTO";
}

static void Angel_SaveCardKeyToDisk(NSString *cardKey) {
    if (!cardKey || cardKey.length == 0) return;
    NSString *clean = [cardKey stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceAndNewlineCharacterSet]];
    Angel_WriteSharedPrefsKeyValues(@{kAGCloudCardPrefsKey: clean}, nil);
}

ANGEL_INLINE BOOL Angel_TryFastDecryptFromCachedSession(void) {
    NSDictionary *dict = Angel_ReadSharedPrefsDict();
    NSString *cipherHex = dict[kAGCloudCipherKey];
    if ([cipherHex isKindOfClass:[NSString class]] && cipherHex.length >= 64) {
        NSString *recoveredKey = Angel_TransformCoreKeyWithHardwareUDID(cipherHex);
        if (recoveredKey.length >= 64) {
            return Angel_DecryptCoreSection([recoveredKey UTF8String]);
        }
    }
    return NO;
}

static void Angel_PerformCloudHandshake(NSString *overrideCardKey, void (^onSuccessBlock)(void));
static void Angel_PresentActivationAlertOnMainThread(NSString *serverTipMsg, void (^onSuccessBlock)(void));

// ==================== iOS 原生风格到期/未激活卡密输入弹窗 ====================
static void Angel_PresentActivationAlertOnMainThread(NSString *serverTipMsg, void (^onSuccessBlock)(void)) {
    dispatch_async(dispatch_get_main_queue(), ^{
        NSString *bid = [[NSBundle mainBundle] bundleIdentifier];
        if (![bid isEqualToString:@"com.apple.springboard"] && ![bid isEqualToString:@"com.apple.Preferences"]) {
            return;
        }

        NSString *msgText = (serverTipMsg && serverTipMsg.length > 0)
            ? [NSString stringWithFormat:@"%@\n试用已到期，请输入正式卡密激活继续使用", serverTipMsg]
            : @"试用已到期，请输入正式卡密激活继续使用";

        UIAlertController *alert = [UIAlertController alertControllerWithTitle:@"💎 天使守护 · 授权激活"
                                                                       message:msgText
                                                                preferredStyle:UIAlertControllerStyleAlert];

        [alert addTextFieldWithConfigurationHandler:^(UITextField * _Nonnull textField) {
            textField.placeholder = @"请输入您的正式授权卡密";
            textField.clearButtonMode = UITextFieldViewModeWhileEditing;
            textField.autocapitalizationType = UITextAutocapitalizationTypeAllCharacters;
            textField.autocorrectionType = UITextAutocorrectionTypeNo;
            NSString *saved = Angel_LoadSavedCardKey();
            if (![saved isEqualToString:@"AUTO"]) {
                textField.text = saved;
            }
        }];

        UIAlertAction *activateAction = [UIAlertAction actionWithTitle:@"立即激活"
                                                                 style:UIAlertActionStyleDefault
                                                               handler:^(UIAlertAction * _Nonnull action) {
            NSString *inputKey = [alert.textFields.firstObject.text stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceAndNewlineCharacterSet]];
            if (gAngelAuthAlertWindow) {
                gAngelAuthAlertWindow.hidden = YES;
                gAngelAuthAlertWindow = nil;
            }
            if (!inputKey || inputKey.length == 0) {
                Angel_PresentActivationAlertOnMainThread(@"⚠️ 卡密不能为空，请重新输入", onSuccessBlock);
                return;
            }
            Angel_SaveCardKeyToDisk(inputKey);
            Angel_PerformCloudHandshake(inputKey, onSuccessBlock);
        }];

        UIAlertAction *laterAction = [UIAlertAction actionWithTitle:@"稍后"
                                                              style:UIAlertActionStyleCancel
                                                            handler:^(UIAlertAction * _Nonnull action) {
            if (gAngelAuthAlertWindow) {
                gAngelAuthAlertWindow.hidden = YES;
                gAngelAuthAlertWindow = nil;
            }
        }];

        [alert addAction:laterAction];
        [alert addAction:activateAction];

        UIViewController *topVC = nil;
        if ([bid isEqualToString:@"com.apple.Preferences"]) {
            for (UIWindow *win in [UIApplication sharedApplication].windows) {
                if (!win.hidden && win.alpha > 0.01 && win.rootViewController) {
                    topVC = win.rootViewController;
                    while (topVC.presentedViewController) {
                        topVC = topVC.presentedViewController;
                    }
                    break;
                }
            }
        }

        if (!topVC) {
            UIWindowScene *activeScene = nil;
            for (UIScene *sc in [UIApplication sharedApplication].connectedScenes) {
                if ([sc isKindOfClass:[UIWindowScene class]] && sc.activationState == UISceneActivationStateForegroundActive) {
                    activeScene = (UIWindowScene *)sc;
                    break;
                }
            }
            if (!activeScene) {
                for (UIScene *sc in [UIApplication sharedApplication].connectedScenes) {
                    if ([sc isKindOfClass:[UIWindowScene class]]) {
                        activeScene = (UIWindowScene *)sc;
                        break;
                    }
                }
            }
            gAngelAuthAlertWindow = activeScene
                ? [[AGCloudAuthAlertWindow alloc] initWithWindowScene:activeScene]
                : [[AGCloudAuthAlertWindow alloc] initWithFrame:[UIScreen mainScreen].bounds];
            gAngelAuthAlertWindow.frame = [UIScreen mainScreen].bounds;
            gAngelAuthAlertWindow.windowLevel = UIWindowLevelAlert + 2500.0;
            gAngelAuthAlertWindow.backgroundColor = [UIColor clearColor];
            UIViewController *hostVC = [[UIViewController alloc] init];
            hostVC.view.backgroundColor = [UIColor clearColor];
            gAngelAuthAlertWindow.rootViewController = hostVC;
            [gAngelAuthAlertWindow makeKeyAndVisible];
            topVC = hostVC;
        }

        [topVC presentViewController:alert animated:YES completion:nil];
    });
}

// ==================== 防线 2：绕过一切抓包代理的云脑 POST /api/v1/verify 握手 ====================
static void Angel_PerformCloudHandshake(NSString *overrideCardKey, void (^onSuccessBlock)(void)) {
    NSString *udid    = Angel_GetHardwareUDID();
    NSString *cardKey = (overrideCardKey && overrideCardKey.length > 0) ? overrideCardKey : Angel_LoadSavedCardKey();
    NSString *ts      = [NSString stringWithFormat:@"%ld", (long)[[NSDate date] timeIntervalSince1970]];
    NSString *nonce   = Angel_GenerateNonce16();

    // 签名规则：HMAC-SHA256(SECRET_SALT, udid + "|" + card_key + "|" + ts + "|" + nonce)
    NSString *signRaw = [NSString stringWithFormat:@"%@|%@|%@|%@", udid, cardKey, ts, nonce];
    NSString *signHex = Angel_ComputeHMACSHA256WithHiddenSalt(signRaw);

    NSURL *url = [NSURL URLWithString:Angel_DecodeVerifyURL()];
    if (!url) return;

    NSMutableURLRequest *req = [NSMutableURLRequest requestWithURL:url];
    req.HTTPMethod = @"POST";
    req.timeoutInterval = 12.0;
    [req setValue:@"application/json; charset=utf-8" forHTTPHeaderField:@"Content-Type"];
    [req setValue:@"AngelGuardian/5.9.0 (Dopamine-Rootless; arm64)" forHTTPHeaderField:@"User-Agent"];

    NSDictionary *bodyDict = @{
        @"udid": udid,
        @"card_key": cardKey,
        @"ts": ts,
        @"nonce": nonce,
        @"sign": signHex
    };
    req.HTTPBody = [NSJSONSerialization dataWithJSONObject:bodyDict options:0 error:nil];

    // 强制设置 connectionProxyDictionary = @{}，物理级绕过 Thor / Reqable / Charles / Stream 等一切抓包软件！
    NSURLSessionConfiguration *cfg = [NSURLSessionConfiguration ephemeralSessionConfiguration];
    cfg.connectionProxyDictionary = @{};
    cfg.requestCachePolicy = NSURLRequestReloadIgnoringLocalAndRemoteCacheData;

    NSURLSession *session = [NSURLSession sessionWithConfiguration:cfg];
    [[session dataTaskWithRequest:req completionHandler:^(NSData * _Nullable data, NSURLResponse * _Nullable response, NSError * _Nullable error) {
        if (error || !data) {
            return;
        }

        NSDictionary *json = [NSJSONSerialization JSONObjectWithData:data options:0 error:nil];
        if (![json isKindOfClass:[NSDictionary class]]) return;

        NSInteger code = [json[@"code"] integerValue];
        NSDictionary *dataObj = [json[@"data"] isKindOfClass:[NSDictionary class]] ? json[@"data"] : json;
        NSString *msg = json[@"msg"] ?: (json[@"message"] ?: @"");

        if (code == 200) {
            NSString *coreKey = dataObj[@"core_key"] ?: json[@"core_key"];
            NSNumber *daysNum = dataObj[@"days_left"] ?: (dataObj[@"remain_days"] ?: json[@"days_left"]);
            gAngelRemainingDays = daysNum ? [daysNum integerValue] : -1;

            if ([coreKey isKindOfClass:[NSString class]] && coreKey.length >= 64) {
                Angel_DecryptCoreSection([coreKey UTF8String]);
                NSString *hwCipher = Angel_TransformCoreKeyWithHardwareUDID(coreKey);
                if (hwCipher.length >= 64) {
                    Angel_WriteSharedPrefsKeyValues(@{kAGCloudCipherKey: hwCipher}, @[@"AG_CloudBrain_CoreSession"]);
                }
            } else {
                gAngelIsAuthorized = YES;
                gAngelRuntimeTicket = ((uint64_t)gAngelCoreCanaryMagic << 16) ^ 0x5F3759DFULL;
            }

            os_log(OS_LOG_DEFAULT, "[AngelShield] ✅ Cloud Brain Authorized! Remaining days: %ld", (long)gAngelRemainingDays);

            if (onSuccessBlock) {
                dispatch_async(dispatch_get_main_queue(), onSuccessBlock);
            }
        } else if (code == 401 || code == 402) {
            // 到期或未激活：当场将内存中的 __DATA,__ag_core 段覆写粉碎为乱码（彻底废除内存 Dump 回填破解包），并擦除缓存弹出卡密框
            Angel_ScrambleCoreSectionOnExpire();
            Angel_WriteSharedPrefsKeyValues(@{@"Gemini_Enabled": @NO, @"AG_LocSpoof_Active": @NO}, @[kAGCloudCipherKey, @"AG_CloudBrain_CoreSession"]);

            NSTimeInterval delaySec = [[[NSBundle mainBundle] bundleIdentifier] isEqualToString:@"com.apple.springboard"] ? 3.0 : 0.4;
            dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(delaySec * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
                Angel_PresentActivationAlertOnMainThread(msg, onSuccessBlock);
            });
        }
    }] resume];
}

#endif /* AngelShield_h */
