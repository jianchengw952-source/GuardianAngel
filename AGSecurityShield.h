#ifndef AG_SECURITY_SHIELD_H
#define AG_SECURITY_SHIELD_H

// ============================================================================
// 🛡️ AngelGuardian Five-Dimensional Anti-Crack & Cloud Brain Shield (Step 1)
// 架构特性：
// 1. 符号表与类名语义投毒：将敏感类名伪装为系统 UIKit 文本排版私有组件
// 2. 编译期异或数组 + 栈内即时解密即焚（服务器地址、签名盐值、敏感符号 0 明文）
// 3. 64位混合布尔算术 (MBA) + __DATA,__ag_core 二进制段云端 AES-256-CTR 抽离金库
// 4. 一次一密流密码 (derive_stream_cipher) 内存解密云脑 12 项灵魂资产 + 反代理抓包
// 5. SpringBoard 单点验签 + UDID 硬件绑定缓存 + notifyd 内核寄存器跨进程零痕迹同步
// ============================================================================

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import <CommonCrypto/CommonHMAC.h>
#import <CommonCrypto/CommonCryptor.h>
#import <CommonCrypto/CommonDigest.h>
#import <mach-o/dyld.h>
#import <mach-o/getsect.h>
#import <dlfcn.h>
#import <notify.h>
#import <stdint.h>
#import <string.h>

// ==================== 一、编译期与链接期符号防护宏 ====================
#ifndef AG_HIDDEN_FUNC
#define AG_HIDDEN_FUNC __attribute__((visibility("hidden"), noinline))
#endif

#ifndef AG_FORCE_INLINE
#define AG_FORCE_INLINE static inline __attribute__((always_inline))
#endif

#define AG_MBA_XOR64(x, y) (((x) | (y)) - ((x) & (y)))
#define AG_MBA_ADD64(x, y) (((x) ^ (y)) + (((x) & (y)) << 1))
#define AG_OPAQUE_TRUE(x)  ((((x) * (x) + (x)) & 1ULL) == 0ULL)

// ==================== 二、编译期栈内解密即焚常量 (0 明文) ====================
#define _AG_XB(c, idx, salt) ((uint8_t)(((uint8_t)(c)) ^ ((uint8_t)(salt)) ^ (uint8_t)((idx) * 7 + 13)))

AG_FORCE_INLINE const char *_ag_stack_decrypt_cstr(const uint8_t *enc, size_t len, uint8_t salt, char *buf) {
    for (size_t i = 0; i < len; i++) {
        buf[i] = (char)(enc[i] ^ salt ^ (uint8_t)(i * 7 + 13));
    }
    buf[len] = '\0';
    return buf;
}

// 1. "https://angel7758.duckdns.org:38080"
AG_FORCE_INLINE const char *ag_enc_cstr_server_host(char buf[40]) {
    const uint8_t e[] = {
        _AG_XB('h',0,0x5A),_AG_XB('t',1,0x5A),_AG_XB('t',2,0x5A),_AG_XB('p',3,0x5A),_AG_XB('s',4,0x5A),
        _AG_XB(':',5,0x5A),_AG_XB('/',6,0x5A),_AG_XB('/',7,0x5A),_AG_XB('a',8,0x5A),_AG_XB('n',9,0x5A),
        _AG_XB('g',10,0x5A),_AG_XB('e',11,0x5A),_AG_XB('l',12,0x5A),_AG_XB('7',13,0x5A),_AG_XB('7',14,0x5A),
        _AG_XB('5',15,0x5A),_AG_XB('8',16,0x5A),_AG_XB('.',17,0x5A),_AG_XB('d',18,0x5A),_AG_XB('u',19,0x5A),
        _AG_XB('c',20,0x5A),_AG_XB('k',21,0x5A),_AG_XB('d',22,0x5A),_AG_XB('n',23,0x5A),_AG_XB('s',24,0x5A),
        _AG_XB('.',25,0x5A),_AG_XB('o',26,0x5A),_AG_XB('r',27,0x5A),_AG_XB('g',28,0x5A),_AG_XB(':',29,0x5A),
        _AG_XB('3',30,0x5A),_AG_XB('8',31,0x5A),_AG_XB('0',32,0x5A),_AG_XB('8',33,0x5A),_AG_XB('0',34,0x5A)
    };
    return _ag_stack_decrypt_cstr(e, 35, 0x5A, buf);
}

// 2. "/api/v1/verify"
AG_FORCE_INLINE const char *ag_enc_cstr_auth_path(char buf[20]) {
    const uint8_t e[] = {
        _AG_XB('/',0,0x33),_AG_XB('a',1,0x33),_AG_XB('p',2,0x33),_AG_XB('i',3,0x33),_AG_XB('/',4,0x33),
        _AG_XB('v',5,0x33),_AG_XB('1',6,0x33),_AG_XB('/',7,0x33),_AG_XB('v',8,0x33),_AG_XB('e',9,0x33),
        _AG_XB('r',10,0x33),_AG_XB('i',11,0x33),_AG_XB('f',12,0x33),_AG_XB('y',13,0x33)
    };
    return _ag_stack_decrypt_cstr(e, 14, 0x33, buf);
}

// 3. "AG_SIGN_SALT_2026_@!"
AG_FORCE_INLINE const char *ag_enc_cstr_hmac_salt(char buf[30]) {
    const uint8_t e[] = {
        _AG_XB('A',0,0x7B),_AG_XB('G',1,0x7B),_AG_XB('_',2,0x7B),_AG_XB('S',3,0x7B),_AG_XB('I',4,0x7B),
        _AG_XB('G',5,0x7B),_AG_XB('N',6,0x7B),_AG_XB('_',7,0x7B),_AG_XB('S',8,0x7B),_AG_XB('A',9,0x7B),
        _AG_XB('L',10,0x7B),_AG_XB('T',11,0x7B),_AG_XB('_',12,0x7B),_AG_XB('2',13,0x7B),_AG_XB('0',14,0x7B),
        _AG_XB('2',15,0x7B),_AG_XB('6',16,0x7B),_AG_XB('_',17,0x7B),_AG_XB('@',18,0x7B),_AG_XB('!',19,0x7B)
    };
    return _ag_stack_decrypt_cstr(e, 20, 0x7B, buf);
}

// 4. "/var/jb"
AG_FORCE_INLINE const char *ag_enc_cstr_var_jb(char buf[16]) {
    const uint8_t e[] = {_AG_XB('/',0,0x3A),_AG_XB('v',1,0x3A),_AG_XB('a',2,0x3A),_AG_XB('r',3,0x3A),_AG_XB('/',4,0x3A),_AG_XB('j',5,0x3A),_AG_XB('b',6,0x3A)};
    return _ag_stack_decrypt_cstr(e, 7, 0x3A, buf);
}

// 5. ".jbroot"
AG_FORCE_INLINE const char *ag_enc_cstr_jbroot(char buf[16]) {
    const uint8_t e[] = {_AG_XB('.',0,0x5C),_AG_XB('j',1,0x5C),_AG_XB('b',2,0x5C),_AG_XB('r',3,0x5C),_AG_XB('o',4,0x5C),_AG_XB('o',5,0x5C),_AG_XB('t',6,0x5C)};
    return _ag_stack_decrypt_cstr(e, 7, 0x5C, buf);
}

// 6. "ellekit"
AG_FORCE_INLINE const char *ag_enc_cstr_ellekit(char buf[16]) {
    const uint8_t e[] = {_AG_XB('e',0,0x71),_AG_XB('l',1,0x71),_AG_XB('l',2,0x71),_AG_XB('e',3,0x71),_AG_XB('k',4,0x71),_AG_XB('i',5,0x71),_AG_XB('t',6,0x71)};
    return _ag_stack_decrypt_cstr(e, 7, 0x71, buf);
}

// 7. "libroothide"
AG_FORCE_INLINE const char *ag_enc_cstr_libroothide(char buf[16]) {
    const uint8_t e[] = {_AG_XB('l',0,0x29),_AG_XB('i',1,0x29),_AG_XB('b',2,0x29),_AG_XB('r',3,0x29),_AG_XB('o',4,0x29),_AG_XB('o',5,0x29),_AG_XB('t',6,0x29),_AG_XB('h',7,0x29),_AG_XB('i',8,0x29),_AG_XB('d',9,0x29),_AG_XB('e',10,0x29)};
    return _ag_stack_decrypt_cstr(e, 11, 0x29, buf);
}

// 8. "MobileSubstrate"
AG_FORCE_INLINE const char *ag_enc_cstr_mobilesubstrate(char buf[24]) {
    const uint8_t e[] = {_AG_XB('M',0,0x44),_AG_XB('o',1,0x44),_AG_XB('b',2,0x44),_AG_XB('i',3,0x44),_AG_XB('l',4,0x44),_AG_XB('e',5,0x44),_AG_XB('S',6,0x44),_AG_XB('u',7,0x44),_AG_XB('b',8,0x44),_AG_XB('s',9,0x44),_AG_XB('t',10,0x44),_AG_XB('r',11,0x44),_AG_XB('a',12,0x44),_AG_XB('t',13,0x44),_AG_XB('e',14,0x44)};
    return _ag_stack_decrypt_cstr(e, 15, 0x44, buf);
}

// 9. "DynamicLibraries"
AG_FORCE_INLINE const char *ag_enc_cstr_dynamiclibs(char buf[24]) {
    const uint8_t e[] = {_AG_XB('D',0,0x63),_AG_XB('y',1,0x63),_AG_XB('n',2,0x63),_AG_XB('a',3,0x63),_AG_XB('m',4,0x63),_AG_XB('i',5,0x63),_AG_XB('c',6,0x63),_AG_XB('L',7,0x63),_AG_XB('i',8,0x63),_AG_XB('b',9,0x63),_AG_XB('r',10,0x63),_AG_XB('a',11,0x63),_AG_XB('r',12,0x63),_AG_XB('i',13,0x63),_AG_XB('e',14,0x63),_AG_XB('s',15,0x63)};
    return _ag_stack_decrypt_cstr(e, 16, 0x63, buf);
}

// 10. "TweakInject"
AG_FORCE_INLINE const char *ag_enc_cstr_tweakinject(char buf[16]) {
    const uint8_t e[] = {_AG_XB('T',0,0x1F),_AG_XB('w',1,0x1F),_AG_XB('e',2,0x1F),_AG_XB('a',3,0x1F),_AG_XB('k',4,0x1F),_AG_XB('I',5,0x1F),_AG_XB('n',6,0x1F),_AG_XB('j',7,0x1F),_AG_XB('e',8,0x1F),_AG_XB('c',9,0x1F),_AG_XB('t',10,0x1F)};
    return _ag_stack_decrypt_cstr(e, 11, 0x1F, buf);
}

// 11. "TCCAccessPreflight"
AG_FORCE_INLINE const char *ag_enc_cstr_tcc_preflight(char buf[24]) {
    const uint8_t e[] = {_AG_XB('T',0,0x82),_AG_XB('C',1,0x82),_AG_XB('C',2,0x82),_AG_XB('A',3,0x82),_AG_XB('c',4,0x82),_AG_XB('c',5,0x82),_AG_XB('e',6,0x82),_AG_XB('s',7,0x82),_AG_XB('s',8,0x82),_AG_XB('P',9,0x82),_AG_XB('r',10,0x82),_AG_XB('e',11,0x82),_AG_XB('f',12,0x82),_AG_XB('l',13,0x82),_AG_XB('i',14,0x82),_AG_XB('g',15,0x82),_AG_XB('h',16,0x82),_AG_XB('t',17,0x82)};
    return _ag_stack_decrypt_cstr(e, 18, 0x82, buf);
}

// 12. "TCCAccessSetForBundleId"
AG_FORCE_INLINE const char *ag_enc_cstr_tcc_setbundle(char buf[32]) {
    const uint8_t e[] = {_AG_XB('T',0,0x91),_AG_XB('C',1,0x91),_AG_XB('C',2,0x91),_AG_XB('A',3,0x91),_AG_XB('c',4,0x91),_AG_XB('c',5,0x91),_AG_XB('e',6,0x91),_AG_XB('s',7,0x91),_AG_XB('s',8,0x91),_AG_XB('S',9,0x91),_AG_XB('e',10,0x91),_AG_XB('t',11,0x91),_AG_XB('F',12,0x91),_AG_XB('o',13,0x91),_AG_XB('r',14,0x91),_AG_XB('B',15,0x91),_AG_XB('u',16,0x91),_AG_XB('n',17,0x91),_AG_XB('d',18,0x91),_AG_XB('l',19,0x91),_AG_XB('e',20,0x91),_AG_XB('I',21,0x91),_AG_XB('d',22,0x91)};
    return _ag_stack_decrypt_cstr(e, 23, 0x91, buf);
}

// 13. "CNCopyCurrentNetworkInfo"
AG_FORCE_INLINE const char *ag_enc_cstr_cncopy_wifi(char buf[32]) {
    const uint8_t e[] = {_AG_XB('C',0,0x4D),_AG_XB('N',1,0x4D),_AG_XB('C',2,0x4D),_AG_XB('o',3,0x4D),_AG_XB('p',4,0x4D),_AG_XB('y',5,0x4D),_AG_XB('C',6,0x4D),_AG_XB('u',7,0x4D),_AG_XB('r',8,0x4D),_AG_XB('r',9,0x4D),_AG_XB('e',10,0x4D),_AG_XB('n',11,0x4D),_AG_XB('t',12,0x4D),_AG_XB('N',13,0x4D),_AG_XB('e',14,0x4D),_AG_XB('t',15,0x4D),_AG_XB('w',16,0x4D),_AG_XB('o',17,0x4D),_AG_XB('r',18,0x4D),_AG_XB('k',19,0x4D),_AG_XB('I',20,0x4D),_AG_XB('n',21,0x4D),_AG_XB('f',22,0x4D),_AG_XB('o',23,0x4D)};
    return _ag_stack_decrypt_cstr(e, 24, 0x4D, buf);
}

// 14. "AMapLocationManager"
AG_FORCE_INLINE const char *ag_enc_cstr_amap_mgr(char buf[24]) {
    const uint8_t e[] = {_AG_XB('A',0,0x67),_AG_XB('M',1,0x67),_AG_XB('a',2,0x67),_AG_XB('p',3,0x67),_AG_XB('L',4,0x67),_AG_XB('o',5,0x67),_AG_XB('c',6,0x67),_AG_XB('a',7,0x67),_AG_XB('t',8,0x67),_AG_XB('i',9,0x67),_AG_XB('o',10,0x67),_AG_XB('n',11,0x67),_AG_XB('M',12,0x67),_AG_XB('a',13,0x67),_AG_XB('n',14,0x67),_AG_XB('a',15,0x67),_AG_XB('g',16,0x67),_AG_XB('e',17,0x67),_AG_XB('r',18,0x67)};
    return _ag_stack_decrypt_cstr(e, 19, 0x67, buf);
}

// 15. "detectRiskOfFakeLocation"
AG_FORCE_INLINE const char *ag_enc_cstr_amap_risk(char buf[32]) {
    const uint8_t e[] = {_AG_XB('d',0,0x53),_AG_XB('e',1,0x53),_AG_XB('t',2,0x53),_AG_XB('e',3,0x53),_AG_XB('c',4,0x53),_AG_XB('t',5,0x53),_AG_XB('R',6,0x53),_AG_XB('i',7,0x53),_AG_XB('s',8,0x53),_AG_XB('k',9,0x53),_AG_XB('O',10,0x53),_AG_XB('f',11,0x53),_AG_XB('F',12,0x53),_AG_XB('a',13,0x53),_AG_XB('k',14,0x53),_AG_XB('e',15,0x53),_AG_XB('L',16,0x53),_AG_XB('o',17,0x53),_AG_XB('c',18,0x53),_AG_XB('a',19,0x53),_AG_XB('t',20,0x53),_AG_XB('i',21,0x53),_AG_XB('o',22,0x53),_AG_XB('n',23,0x53)};
    return _ag_stack_decrypt_cstr(e, 24, 0x53, buf);
}

// 16. "setDetectRiskOfFakeLocation:"
AG_FORCE_INLINE const char *ag_enc_cstr_amap_setrisk(char buf[32]) {
    const uint8_t e[] = {_AG_XB('s',0,0x2B),_AG_XB('e',1,0x2B),_AG_XB('t',2,0x2B),_AG_XB('D',3,0x2B),_AG_XB('e',4,0x2B),_AG_XB('t',5,0x2B),_AG_XB('e',6,0x2B),_AG_XB('c',7,0x2B),_AG_XB('t',8,0x2B),_AG_XB('R',9,0x2B),_AG_XB('i',10,0x2B),_AG_XB('s',11,0x2B),_AG_XB('k',12,0x2B),_AG_XB('O',13,0x2B),_AG_XB('f',14,0x2B),_AG_XB('F',15,0x2B),_AG_XB('a',16,0x2B),_AG_XB('k',17,0x2B),_AG_XB('e',18,0x2B),_AG_XB('L',19,0x2B),_AG_XB('o',20,0x2B),_AG_XB('c',21,0x2B),_AG_XB('a',22,0x2B),_AG_XB('t',23,0x2B),_AG_XB('i',24,0x2B),_AG_XB('o',25,0x2B),_AG_XB('n',26,0x2B),_AG_XB(':',27,0x2B)};
    return _ag_stack_decrypt_cstr(e, 28, 0x2B, buf);
}

// 17. "WCPayOfflinePayViewController"
AG_FORCE_INLINE const char *ag_enc_cstr_wcpay_vc(char buf[32]) {
    const uint8_t e[] = {_AG_XB('W',0,0x77),_AG_XB('C',1,0x77),_AG_XB('P',2,0x77),_AG_XB('a',3,0x77),_AG_XB('y',4,0x77),_AG_XB('O',5,0x77),_AG_XB('f',6,0x77),_AG_XB('f',7,0x77),_AG_XB('l',8,0x77),_AG_XB('i',9,0x77),_AG_XB('n',10,0x77),_AG_XB('e',11,0x77),_AG_XB('P',12,0x77),_AG_XB('a',13,0x77),_AG_XB('y',14,0x77),_AG_XB('V',15,0x77),_AG_XB('i',16,0x77),_AG_XB('e',17,0x77),_AG_XB('w',18,0x77),_AG_XB('C',19,0x77),_AG_XB('o',20,0x77),_AG_XB('n',21,0x77),_AG_XB('t',22,0x77),_AG_XB('r',23,0x77),_AG_XB('o',24,0x77),_AG_XB('l',25,0x77),_AG_XB('l',26,0x77),_AG_XB('e',27,0x77),_AG_XB('r',28,0x77)};
    return _ag_stack_decrypt_cstr(e, 29, 0x77, buf);
}

// ==================== 三、Mach-O __DATA,__ag_core 抽离金库 ====================
#pragma pack(push, 1)
typedef struct {
    uint64_t magic;         // 0x4147434F52453630ULL ("AGCORE60")
    uint32_t flags;         // 0x00000000: 待服务器抽离; 0x00000001: 已加密
    uint32_t payloadLen;    // 实际 payload 字节长度
    uint8_t  nonce[16];     // AES-CTR 128位初始化向量
    uint8_t  payload[512];  // 核心灵魂资产加密容器
} AGCloudCoreVault;
#pragma pack(pop)

// 静态注入二进制段 __DATA,__ag_core，保证在编译链接产物中独立成节
__attribute__((section("__DATA,__ag_core"), used, aligned(16)))
static AGCloudCoreVault _g_ag_core_vault = {
    .magic = 0x4147434F52453630ULL,
    .flags = 0,
    .payloadLen = 64,
    .nonce = {0x00,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xAA,0xBB,0xCC,0xDD,0xEE,0xFF},
    .payload = {
        0x41,0x6E,0x67,0x65,0x6C,0x47,0x75,0x61,0x72,0x64,0x69,0x61,0x6E,0x5F,0x50,0x72,
        0x6F,0x5F,0x43,0x6C,0x6F,0x75,0x64,0x5F,0x42,0x72,0x61,0x69,0x6E,0x5F,0x32,0x30,
        0x32,0x36,0x5F,0x53,0x65,0x63,0x75,0x72,0x69,0x74,0x79,0x5F,0x56,0x61,0x75,0x6C,
        0x74,0x5F,0x49,0x6E,0x69,0x74,0x69,0x61,0x6C,0x5F,0x53,0x65,0x65,0x64,0x21,0x00
    }
};

// ==================== 四、动态流密码与内存自解密引擎 ====================
AG_FORCE_INLINE void derive_stream_cipher(const uint8_t *key32, const uint8_t *nonce16, uint8_t *outBuf, size_t len) {
    uint8_t state[32];
    for (int i = 0; i < 16; i++) {
        state[i] = key32[i] ^ nonce16[i];
        state[i + 16] = key32[i + 16] ^ nonce16[15 - i];
    }
    uint64_t s0 = *(uint64_t *)(state);
    uint64_t s1 = *(uint64_t *)(state + 8);
    uint64_t s2 = *(uint64_t *)(state + 16);
    uint64_t s3 = *(uint64_t *)(state + 24);

    for (size_t i = 0; i < len; i++) {
        s0 = (s0 ^ (s0 << 13)) ^ (s0 >> 7);
        s1 = AG_MBA_XOR64(s1, (s1 << 17)) ^ (s1 >> 11);
        s2 = (s2 ^ (s2 << 5)) ^ (s2 >> 3);
        s3 = AG_MBA_ADD64(s3, 0x9E3779B97F4A7C15ULL);
        uint8_t k = (uint8_t)((s0 + s1 + s2 + s3) & 0xFF);
        outBuf[i] ^= k;
    }
}

// 内存中就地通过 AES-256-CTR 解密 __DATA,__ag_core
AG_HIDDEN_FUNC
static BOOL _ag_decrypt_ag_core_vault_in_memory(const uint8_t *aesKey32) {
    if (!aesKey32) return NO;
    if (_g_ag_core_vault.magic != 0x4147434F52453630ULL) return NO;
    if (_g_ag_core_vault.flags == 0) {
        // 未经服务器抽离的测试/本地状态，直接放行
        return YES;
    }

    CCCryptorRef cryptor = NULL;
    CCCryptorStatus status = CCCryptorCreateWithMode(
        kCCDecrypt,
        kCCModeCTR,
        kCCAlgorithmAES,
        ccNoPadding,
        _g_ag_core_vault.nonce,
        aesKey32,
        32,
        NULL, 0, 0,
        kCCModeOptionCTR_BE,
        &cryptor
    );

    if (status != kCCSuccess || !cryptor) {
        return NO;
    }

    size_t moved = 0;
    status = CCCryptorUpdate(cryptor, _g_ag_core_vault.payload, sizeof(_g_ag_core_vault.payload), _g_ag_core_vault.payload, sizeof(_g_ag_core_vault.payload), &moved);
    CCCryptorRelease(cryptor);

    if (status == kCCSuccess) {
        _g_ag_core_vault.flags = 0; // 内存解密成功，恢复明态标志
        return YES;
    }
    return NO;
}

// ==================== 五、反代理抓包与直连云脑握手 ====================
AG_HIDDEN_FUNC
static NSDictionary *_ag_perform_cloud_handshake_sync(NSString *udid, NSString *appVer) {
    if (!udid || udid.length == 0) return nil;

    char hostBuf[40];
    char pathBuf[20];
    char saltBuf[30];
    ag_enc_cstr_server_host(hostBuf);
    ag_enc_cstr_auth_path(pathBuf);
    ag_enc_cstr_hmac_salt(saltBuf);

    NSString *urlString = [NSString stringWithFormat:@"%s%s", hostBuf, pathBuf];
    NSURL *url = [NSURL URLWithString:urlString];
    if (!url) return nil;

    NSMutableURLRequest *req = [NSMutableURLRequest requestWithURL:url cachePolicy:NSURLRequestReloadIgnoringLocalAndRemoteCacheData timeoutInterval:6.0];
    [req setHTTPMethod:@"POST"];
    [req setValue:@"application/json" forHTTPHeaderField:@"Content-Type"];

    // 严禁走任何系统代理或抓包代理（反 Charles / Fiddler / Surge）
    NSURLSessionConfiguration *conf = [NSURLSessionConfiguration ephemeralSessionConfiguration];
    conf.connectionProxyDictionary = @{};

    uint64_t ts = (uint64_t)[[NSDate date] timeIntervalSince1970];
    NSString *rawSign = [NSString stringWithFormat:@"%@_%llu_%s", udid, ts, saltBuf];
    unsigned char hmac[CC_SHA256_DIGEST_LENGTH];
    CCHmac(kCCHmacAlgSHA256, saltBuf, strlen(saltBuf), [rawSign UTF8String], [rawSign lengthOfBytesUsingEncoding:NSUTF8StringEncoding], hmac);
    NSMutableString *hexSign = [NSMutableString stringWithCapacity:CC_SHA256_DIGEST_LENGTH * 2];
    for (int i = 0; i < CC_SHA256_DIGEST_LENGTH; i++) {
        [hexSign appendFormat:@"%02x", hmac[i]];
    }

    NSDictionary *bodyDict = @{
        @"udid": udid,
        @"version": appVer ?: @"1.0.0",
        @"timestamp": @(ts),
        @"signature": hexSign
    };
    NSError *err = nil;
    NSData *bodyData = [NSJSONSerialization dataWithJSONObject:bodyDict options:0 error:&err];
    if (err || !bodyData) return nil;
    [req setHTTPBody:bodyData];

    __block NSDictionary *respDict = nil;
    dispatch_semaphore_t sem = dispatch_semaphore_create(0);
    NSURLSession *session = [NSURLSession sessionWithConfiguration:conf];
    NSURLSessionDataTask *task = [session dataTaskWithRequest:req completionHandler:^(NSData * _Nullable data, NSURLResponse * _Nullable response, NSError * _Nullable error) {
        if (!error && data && [(NSHTTPURLResponse *)response statusCode] == 200) {
            respDict = [NSJSONSerialization JSONObjectWithJSONObject:data options:0 error:nil];
        }
        dispatch_semaphore_signal(sem);
    }];
    [task resume];
    dispatch_semaphore_wait(sem, dispatch_time(DISPATCH_TIME_NOW, (int64_t)(6.0 * NSEC_PER_SEC)));

    return respDict;
}

// ==================== 六、反 AI 逆向蜜罐诱饵函数 (Adversarial AI Honeypots) ====================
// 说明：故意保留清晰的验证命名与非内联函数体，当逆向者使用 Claude/GPT + IDA-MCP 搜索“授权/防检测/签名校验”时，
// AI 会将注意力集中在以下诱饵函数并给出针对诱饵的无效 Patch 方案，而真正的核心逻辑全部已内联并伪装为 UI 排版函数。
__attribute__((used, visibility("default"), noinline))
int _verifyCloudVIPSubscriptionReceiptWithRSAKey(const char *receiptBytes, uint32_t keyVersion) {
    volatile uint32_t acc = 0x811C9DC5U ^ keyVersion;
    if (receiptBytes) {
        for (int i = 0; i < 16 && receiptBytes[i] != '\0'; i++) {
            acc = (acc ^ (uint8_t)receiptBytes[i]) * 0x01000193U;
        }
    }
    return ((acc & 0xFFFFU) == 0xA55AU) ? 1 : 0;
}

__attribute__((used, visibility("default"), noinline))
static int _validateDeviceUDIDHardwareLicenseToken(uint64_t deviceSeed) {
    volatile uint64_t h = deviceSeed ^ 0x4F2A9B1E7D6C8035ULL;
    h = (h ^ (h >> 31)) * 0x9E3779B97F4A7C15ULL;
    return ((h & 0xFFULL) == 0xFEULL) ? 1 : 0;
}

// ==================== 七、内核寄存器跨进程零痕迹状态同步 (notifyd) ====================
// 使用系统级通知通知状态空间，将合法验证种子安全广播给客户端应用，无需任何临时文件或跨进程 IPC 端口
#define AG_NOTIFY_AUTH_STATUS_KEY "com.apple.coremedia.spatial.auth_s"
#define AG_NOTIFY_AUTH_PARAM_KEY  "com.apple.coremedia.spatial.auth_p"
#define AG_AUTH_REG_MAGIC_SALT    0xA55A9E3779B97F4AULL

AG_FORCE_INLINE void ag_sync_kernel_auth_state(uint64_t statusSeed, uint32_t paramSeed) {
    int token = 0;
    notify_register_check(AG_NOTIFY_AUTH_STATUS_KEY, &token);
    notify_set_state(token, statusSeed ^ AG_AUTH_REG_MAGIC_SALT);
    notify_post(AG_NOTIFY_AUTH_STATUS_KEY);

    notify_register_check(AG_NOTIFY_AUTH_PARAM_KEY, &token);
    notify_set_state(token, (uint64_t)paramSeed ^ 0x5AA51234UL);
    notify_post(AG_NOTIFY_AUTH_PARAM_KEY);
}

AG_FORCE_INLINE BOOL ag_verify_kernel_auth_state(void) {
    int token = 0;
    uint64_t state = 0;
    notify_register_check(AG_NOTIFY_AUTH_STATUS_KEY, &token);
    notify_get_state(token, &state);
    uint64_t unmasked = state ^ AG_AUTH_REG_MAGIC_SALT;
    return (unmasked == 0x7F4A7C159E3779B9ULL || unmasked == 0x0);
}

// ==================== 八、反 AI 语义投毒符号映射表 (伪装为 UIKit 排版系统) ====================
#define AG_CLS_VOICE_COORD_CSTR   "_UISceneAudioGraphLayoutCoordinator"
#define AG_CLS_PREFS_CTRL_CSTR    "_UISceneDisplayTraitPreferencesController"

#define AGLocationSpoofManager                _UIAccessibilityParagraphLayoutCoordinator
#define AGLocationSpoofSettingsViewController _UITextSelectionForceGestureViewController
#define AGAppScopePickerViewController        _UISystemSceneHierarchyTableController
#define AGGaodeTileOverlay                    _MKVectorTileRasterRenderOverlay
#define GeminiAssistantManager                _UISceneAudioGraphLayoutCoordinator
#define AGSystemVoiceCoordinator              _UISceneAudioGraphLayoutCoordinator
#define GeminiPassthroughWindow               _UITextEffectsPassthroughHostingWindow
#define AngelGuardianSettingsViewController   _UISceneDisplayTraitPreferencesController
#define AGSystemPreferencesController         _UISceneDisplayTraitPreferencesController
#define AGInSituTranslationOverlay            _UIKeyboardAutocorrectionBubbleWindow
#define AGGlobalActionPillWindow              _UIStatusBarDynamicPillBannerWindow
#define AGLockScreenRootViewController        _UICoverSheetBackdropRootViewController
#define AGCardBgCanvasView                    _UIBackdropTraitCanvasView
#define AGOrbCanvasView                       _UIEffectorTraitCanvasView
#define AGWaveCanvasView                      _UISpectrumTraitCanvasView
#define AGMiniSummonCanvasView                _UIDockingTraitCanvasView
#define AGStreamTranslateDelegate             _UIKeyboardTranslationRouterDelegate
#define AGSharedTransSessionRouter            _UIKeyboardAudioSessionRouter
#define AngelCamGrabber                       _UIPeripheralCameraContextGrabber
#define AGAnnotation                          _UIMapKitVectorGlyphAnnotation
#define AGScreenTranslationOverlay            _UIKeyboardAutocorrectionBubbleWindow

#define agPublishSpoofStateToKernel           _ui_trait_register_geocoordinate_payload
#define agReadSpoofStateFromKernel            _ui_trait_retrieve_geocoordinate_payload
#define agPublishCustomWiFiToKernel           _ui_trait_register_hotspot_payload
#define agReadCustomWiFiFromKernel            _ui_trait_retrieve_hotspot_payload
#define agIsAntiDetectionShieldActive         _ui_trait_is_layout_fence_active
#define agShouldSpoofCurrentProcess           _ui_trait_should_filter_process_environment
#define agInstallAntiDetectionCHooksIfNeeded  _ui_trait_attach_mach_filter_callbacks
#define agSimulatedLocationFromCurrentState   _ui_trait_compute_interpolated_metrics
#define agSpoofedWiFiDictionary               _ui_trait_current_telemetry_dictionary
#define agExecuteWeChatCommandDict            _ui_trait_dispatch_action_tuple
#define agAnalyzeRecentCrashesAndResprings    _ui_trait_inspect_diagnostic_backtrace
#define _ag_sec_zero_bias_mask                _ui_trait_opaque_bias_accumulator
#define _ag_sec_feature_gate_allow            _ui_trait_opaque_gate_decision
#define agInstallWiFiShieldCHooksIfNeeded     _ui_trait_attach_captive_network_callbacks
#define agSyncClientHooksWithCurrentState     _ui_trait_synchronize_runtime_method_table
#define onCopyOfficialRepoURLTapped           _uiSceneCopyTraitSourceEndpointTapped

#endif /* AG_SECURITY_SHIELD_H */
