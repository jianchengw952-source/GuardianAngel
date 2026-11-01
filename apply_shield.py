#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import os

INPUT_FILE = "dopamine测试版本.txt" if os.path.exists("dopamine测试版本.txt") else "Tweak.x"
OUTPUT_FILE = "Tweak.xm"

with open(INPUT_FILE, "r", encoding="utf-8") as f:
    code = f.read()

# ==============================================================================
# 1. 引入 AngelShield.h 并声明 __DATA,__ag_core 受保护核心数据段
# ==============================================================================
header_target = "#include <dirent.h>" if "#include <dirent.h>" in code else "#import <dirent.h>"
header_replacement = """#import <dirent.h>
#import "AngelShield.h"

// ==================== [云脑 __DATA,__ag_core 段抽离核心常量] ====================
ANGEL_CORE_DATA static uint64_t kAGProtectedKernelMagic = 0xA5A5000000000000ULL;
ANGEL_CORE_DATA static uint64_t kAGProtectedMixPrime1   = 0x9E3779B97F4A7C15ULL;
ANGEL_CORE_DATA static uint64_t kAGProtectedMixPrime2   = 0xBF58476D1CE4E5B9ULL;
ANGEL_CORE_DATA static uint64_t kAGProtectedMixPrime3   = 0x94D049BB133111EBULL;
ANGEL_CORE_DATA static double   kAGProtectedEarthAxisA  = 6378245.0;
ANGEL_CORE_DATA static double   kAGProtectedEarthEE     = 0.00669342162296594323;
ANGEL_CORE_DATA static uint32_t kAGProtectedVIPOffset   = 0x1A8;
ANGEL_CORE_DATA static char     kAGProtectedHookAMapCls[32] = "AMapLocationManager";
"""
assert header_target in code, "未找到 #include <dirent.h> 或 #import <dirent.h>"
code = code.replace(header_target, header_replacement, 1)

# ==============================================================================
# 2. 【修复 1】修复 AGCLDaemonLocation 32字节栈溢出 (188B -> 156B 严格对齐 + 栈安全联合体)
# ==============================================================================
struct_target = """    double ellipsoidalAltitude;          // 0x90
    BOOL fromSimulationController;       // 0x98
    uint8_t _reservedPadding[32];        // 必须保留！兼容 iOS 16/17 尾部扩展字节，防止 gOrig_CLLoc_clientLoc 写穿栈帧
} AGCLDaemonLocation;
#pragma pack(pop)"""

struct_replacement = """    double ellipsoidalAltitude;          // 0x90
    BOOL fromSimulationController;       // 0x98
    uint8_t _tailAlign[3];               // 0x99..0x9B (严格对齐 iOS 原生 0x9C = 156 字节，防止 return 写穿调用方栈帧)
} AGCLDaemonLocation;

typedef union {
    AGCLDaemonLocation loc;
    uint8_t rawBytes[sizeof(AGCLDaemonLocation) + 64]; // 本地栈安全垫：承接原函数在 iOS 16/17 可能多出的尾部字节
} AGCLDaemonLocationSafeBuffer;
#pragma pack(pop)"""
assert struct_target in code, "未找到 AGCLDaemonLocation 结构体定义"
code = code.replace(struct_target, struct_replacement, 1)

client_loc_target = """static AGCLDaemonLocation agDyn_CLLoc_clientLoc(CLLocation *self, SEL _cmd) {
    AGCLDaemonLocation raw;
    memset(&raw, 0, sizeof(raw));
    if (gOrig_CLLoc_clientLoc) {
        raw = ((AGCLDaemonLocation(*)(id, SEL))gOrig_CLLoc_clientLoc)(self, _cmd);
    }"""

client_loc_replacement = """static AGCLDaemonLocation agDyn_CLLoc_clientLoc(CLLocation *self, SEL _cmd) {
    AGCLDaemonLocationSafeBuffer safeBuf;
    memset(&safeBuf, 0, sizeof(safeBuf));
    if (gOrig_CLLoc_clientLoc) {
        safeBuf = ((AGCLDaemonLocationSafeBuffer(*)(id, SEL))gOrig_CLLoc_clientLoc)(self, _cmd);
    }
    AGCLDaemonLocation raw = safeBuf.loc;"""
assert client_loc_target in code, "未找到 agDyn_CLLoc_clientLoc 函数入口"
code = code.replace(client_loc_target, client_loc_replacement, 1)

coord_setter_target = """            static SEL sSetRefSel = NULL;
            static dispatch_once_t onceToken;
            dispatch_once(&onceToken, ^{
                sSetRefSel = sel_registerName("setReferenceFrame:");
            });
            if ([self respondsToSelector:sSetRefSel]) {
                ((void(*)(id, SEL, int))objc_msgSend)(self, sSetRefSel, 1);
            }
            return CLLocationCoordinate2DMake(tel.lat, tel.lon);"""
coord_setter_replacement = """            return CLLocationCoordinate2DMake(tel.lat, tel.lon);"""
assert coord_setter_target in code, "未找到 -coordinate 内部的 setReferenceFrame 块"
code = code.replace(coord_setter_target, coord_setter_replacement, 1)

# ==============================================================================
# 3. 【修复 2】修复音频中断 in=none 时强行重启导致的 5s+5s=10s 看门狗死锁注销
# ==============================================================================
sync_stop_target = """        if (eng || pl) {
            dispatch_sync(self.audioPlayQueue, ^{
                @try {
                    if (pl) [pl stop];
                    if (eng) {
                        [eng.inputNode removeTapOnBus:0];
                        [eng stop];
                    }
                } @catch (NSException *e) {}
            });
        }"""
sync_stop_replacement = """        if (eng || pl) {
            dispatch_async(self.audioPlayQueue, ^{
                @try {
                    if (pl) [pl stop];
                    if (eng) {
                        [eng.inputNode removeTapOnBus:0];
                        [eng stop];
                    }
                } @catch (NSException *e) {}
            });
        }"""
assert sync_stop_target in code, "未找到 stopAllVoiceEngines 中的 dispatch_sync"
code = code.replace(sync_stop_target, sync_stop_replacement, 1)

audio_route_target = """- (void)handleAudioSessionInterruption:(NSNotification *)note {
    NSUInteger type = [note.userInfo[AVAudioSessionInterruptionTypeKey] unsignedIntegerValue];
    agLiveDiagLog(@"AUDIO_INTERRUPT", @"AVAudioSession interruption type=%lu", (unsigned long)type);
    dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(0.40 * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
        if (!self.isDismissed && self.overlayWindow && !self.overlayWindow.hidden && self.activeModeTab != AGConfigTabPureText) {
            if (!self.liveAudioEngine || !self.liveAudioEngine.isRunning) {
                [self setupAndStartFullDuplexPCMAudioEngine];
            }
        }
    });
}

- (void)handleAudioSessionRouteChange:(NSNotification *)note {
    if (self.isDismissed || !self.overlayWindow || self.overlayWindow.hidden) return;
    NSUInteger reason = [note.userInfo[AVAudioSessionRouteChangeReasonKey] unsignedIntegerValue];
    AVAudioSession *sess = [AVAudioSession sharedInstance];
    NSString *inPort = sess.currentRoute.inputs.firstObject.portType ?: @"none";
    NSString *outPort = sess.currentRoute.outputs.firstObject.portType ?: @"none";
    agLiveDiagLog(@"AUDIO_ROUTE", @"Route changed reason=%lu | in=%@ | out=%@ | engineRunning=%d",
                  (unsigned long)reason, inPort, outPort, (self.liveAudioEngine ? self.liveAudioEngine.isRunning : 0));

    if (self.activeModeTab != AGConfigTabPureText) {
        dispatch_async(agAudioSerialQueue(), ^{
            if (self.isDismissed || !self.liveAudioEngine) return;
            if ([outPort isEqualToString:AVAudioSessionPortBuiltInReceiver]) {
                [sess overrideOutputAudioPort:AVAudioSessionPortOverrideSpeaker error:nil];
            }
            if (!self.liveAudioEngine.isRunning) {
                NSError *err = nil;
                [self.liveAudioEngine startAndReturnError:&err];
                agLiveDiagLog(@"AUDIO_ROUTE_HEAL", @"Restarted engine after route change (running=%d, err=%@)", self.liveAudioEngine.isRunning, err.localizedDescription ?: @"none");
            }
            if (self.pcmPlayerNode && !self.pcmPlayerNode.isPlaying) {
                [self.pcmPlayerNode play];
            }
            [self primeFullDuplexAudioEngineWithSilentPulse:@"ROUTE_CHANGE"];
        });
    }
}"""

audio_route_replacement = """- (void)handleAudioSessionInterruption:(NSNotification *)note {
    NSUInteger type = [note.userInfo[AVAudioSessionInterruptionTypeKey] unsignedIntegerValue];
    agLiveDiagLog(@"AUDIO_INTERRUPT", @"AVAudioSession interruption type=%lu", (unsigned long)type);
    dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(0.55 * NSEC_PER_SEC)), agAudioSerialQueue(), ^{
        if (self.isDismissed || !self.overlayWindow || self.overlayWindow.hidden || self.activeModeTab == AGConfigTabPureText) return;
        AVAudioSession *sess = [AVAudioSession sharedInstance];
        [sess setActive:YES withOptions:0 error:nil];
        NSString *inPort = sess.currentRoute.inputs.firstObject.portType;
        if (!inPort || inPort.length == 0) return; // 无麦克风输入路由(in=none)时不强拉引擎，防止 mediaserverd 5s 阻塞
        if (!self.liveAudioEngine || !self.liveAudioEngine.isRunning) {
            dispatch_async(dispatch_get_main_queue(), ^{
                [self setupAndStartFullDuplexPCMAudioEngine];
            });
        }
    });
}

- (void)handleAudioSessionRouteChange:(NSNotification *)note {
    if (self.isDismissed || !self.overlayWindow || self.overlayWindow.hidden) return;
    NSUInteger reason = [note.userInfo[AVAudioSessionRouteChangeReasonKey] unsignedIntegerValue];
    if (self.activeModeTab != AGConfigTabPureText) {
        dispatch_async(agAudioSerialQueue(), ^{
            if (self.isDismissed || !self.liveAudioEngine) return;
            AVAudioSession *sess = [AVAudioSession sharedInstance];
            NSString *inPort = sess.currentRoute.inputs.firstObject.portType ?: @"none";
            NSString *outPort = sess.currentRoute.outputs.firstObject.portType ?: @"none";
            agLiveDiagLog(@"AUDIO_ROUTE", @"Route changed reason=%lu | in=%@ | out=%@ | engineRunning=%d",
                          (unsigned long)reason, inPort, outPort, self.liveAudioEngine.isRunning);
            if ([inPort isEqualToString:@"none"]) {
                return; // 当 in=none 时跳过无效脉冲与重启，彻底根除 5s+5s=10s 看门狗死锁注销
            }
            if ([outPort isEqualToString:AVAudioSessionPortBuiltInReceiver]) {
                [sess overrideOutputAudioPort:AVAudioSessionPortOverrideSpeaker error:nil];
            }
            if (!self.liveAudioEngine.isRunning) {
                [sess setActive:YES withOptions:0 error:nil];
                NSError *err = nil;
                [self.liveAudioEngine startAndReturnError:&err];
                agLiveDiagLog(@"AUDIO_ROUTE_HEAL", @"Restarted engine after route change (running=%d, err=%@)", self.liveAudioEngine.isRunning, err.localizedDescription ?: @"none");
            }
            if (self.pcmPlayerNode && !self.pcmPlayerNode.isPlaying) {
                [self.pcmPlayerNode play];
            }
            [self primeFullDuplexAudioEngineWithSilentPulse:@"ROUTE_CHANGE"];
        });
    }
}"""
assert audio_route_target in code, "未找到 handleAudioSessionInterruption / RouteChange 函数块"
code = code.replace(audio_route_target, audio_route_replacement, 1)

# ==============================================================================
# 4. 【修复 3 & 4】避免每 2 秒向 locationd 发送重复清理 XPC + 静态缓存 SpringBoard infoDictionary
# ==============================================================================
push_sim_target = """- (void)pushSimulatedLocationToDaemonWithCourse:(double)course speed:(double)speed forceRestart:(BOOL)forceRestart {
    (void)course; (void)speed; (void)forceRestart;
    if (![[[NSBundle mainBundle] bundleIdentifier] isEqualToString:@"com.apple.springboard"]) {
        return;
    }"""
push_sim_replacement = """- (void)pushSimulatedLocationToDaemonWithCourse:(double)course speed:(double)speed forceRestart:(BOOL)forceRestart {
    (void)course; (void)speed;
    if (!forceRestart) return; // 仅在开关定位时执行 1 次清理，避免每 2 秒向 locationd 发送 6 条 XPC 造成主线程阻塞
    if (![[[NSBundle mainBundle] bundleIdentifier] isEqualToString:@"com.apple.springboard"]) {
        return;
    }"""
assert push_sim_target in code, "未找到 pushSimulatedLocationToDaemonWithCourse 入口"
code = code.replace(push_sim_target, push_sim_replacement, 1)

bundle_hook_target = """%hook NSBundle
- (NSDictionary *)infoDictionary {
    NSDictionary *orig = %orig;
    if (agIsSpringBoardProcess() && self == [NSBundle mainBundle]) {
        NSMutableDictionary *dict = orig ? [orig mutableCopy] : [NSMutableDictionary dictionary];
        dict[@"NSCameraUsageDescription"] = @"天使守护需要使用相机进行独立实景视频 AI 对话";
        dict[@"NSMicrophoneUsageDescription"] = @"天使守护需要使用麦克风进行实时语音对话";
        dict[@"NSSpeechRecognitionUsageDescription"] = @"天使守护需要使用语音识别实现原厂级打断检测";
        return dict;
    }
    return orig;
}
%end"""

bundle_hook_replacement = """%hook NSBundle
- (NSDictionary *)infoDictionary {
    NSDictionary *orig = %orig;
    if (agIsSpringBoardProcess() && self == [NSBundle mainBundle]) {
        static NSDictionary *sCachedSBInfoDict = nil;
        static dispatch_once_t onceToken;
        dispatch_once(&onceToken, ^{
            NSMutableDictionary *dict = orig ? [orig mutableCopy] : [NSMutableDictionary dictionary];
            dict[@"NSCameraUsageDescription"] = @"天使守护需要使用相机进行独立实景视频 AI 对话";
            dict[@"NSMicrophoneUsageDescription"] = @"天使守护需要使用麦克风进行实时语音对话";
            dict[@"NSSpeechRecognitionUsageDescription"] = @"天使守护需要使用语音识别实现原厂级打断检测";
            sCachedSBInfoDict = [dict copy];
        });
        return sCachedSBInfoDict ?: orig;
    }
    return orig;
}
%end"""
assert bundle_hook_target in code, "未找到 %hook NSBundle infoDictionary"
code = code.replace(bundle_hook_target, bundle_hook_replacement, 1)

# ==============================================================================
# 5. 在插件《设置》页面最底部加上：反馈BUG联系：571322016@qq.com
# ==============================================================================
settings_footer_target = """        AGConfigTab tabs[4] = { AGConfigTabNativeLive, AGConfigTabVideoAI, AGConfigTabPureText, AGConfigTabTranslation };
        for (int i = 0; i < 4; i++) {
            UIView *card = [self buildIndependentModuleAccordionCardForTab:tabs[i] originY:currentY width:cardW];
            [self.accordionContainerView addSubview:card];
            currentY += card.frame.size.height + 12.0;
        }

        CGRect f = self.accordionContainerView.frame;"""

settings_footer_replacement = """        AGConfigTab tabs[4] = { AGConfigTabNativeLive, AGConfigTabVideoAI, AGConfigTabPureText, AGConfigTabTranslation };
        for (int i = 0; i < 4; i++) {
            UIView *card = [self buildIndependentModuleAccordionCardForTab:tabs[i] originY:currentY width:cardW];
            [self.accordionContainerView addSubview:card];
            currentY += card.frame.size.height + 12.0;
        }

        UILabel *bugContactLabel = [[UILabel alloc] initWithFrame:CGRectMake(16.0, currentY + 6.0, cardW, 26.0)];
        bugContactLabel.text = @"反馈BUG联系：571322016@qq.com";
        bugContactLabel.textAlignment = NSTextAlignmentCenter;
        bugContactLabel.font = [UIFont systemFontOfSize:12.5 weight:UIFontWeightMedium];
        bugContactLabel.textColor = [UIColor colorWithWhite:0.68 alpha:1.0];
        [self.accordionContainerView addSubview:bugContactLabel];
        currentY += 38.0;

        CGRect f = self.accordionContainerView.frame;"""
assert settings_footer_target in code, "未找到 reloadSettingsLayoutAnimated 底部布局块"
code = code.replace(settings_footer_target, settings_footer_replacement, 1)

# ==============================================================================
# 6. 精简 agJBRootPath 为纯正 Dopamine 2.x (/var/jb) 无根路径解析器
# ==============================================================================
old_jbroot_block_start = "// ==================== Dopamine (无根/隐根) & Relaxin (RootHide) 零依赖内存自定位解析器 ===================="
old_jbroot_block_end = "// 高速缓存判断当前是否为 SpringBoard 主桌面进程"
start_idx = code.find(old_jbroot_block_start)
end_idx = code.find(old_jbroot_block_end)
assert start_idx != -1 and end_idx != -1, "未找到 agJBRootPath 函数块"

dopamine_jbroot_func = """// ==================== Dopamine 2.x (/var/jb) 纯净无根路径解析器 ====================
static NSString *agJBRootPath(NSString *relativePath) {
    if (!relativePath || relativePath.length == 0) return @"/var/jb";
    if ([relativePath hasPrefix:@"/var/jb/"]) return relativePath;
    NSString *cleanRel = [relativePath hasPrefix:@"/"] ? relativePath : [@"/" stringByAppendingString:relativePath];
    return [@"/var/jb" stringByAppendingPathComponent:cleanRel];
}

"""
code = code[:start_idx] + dopamine_jbroot_func + code[end_idx:]

# ==============================================================================
# 7. 将核心算法常量与全部 XOR 敏感选择器数组迁移至 __DATA,__ag_core 段 (ANGEL_CORE_DATA)
# ==============================================================================
code = code.replace(
    """static inline uint64_t agKernelObfuscateMask(uint32_t channelSalt) {
    uint64_t z = 0x9E3779B97F4A7C15ULL ^ ((uint64_t)channelSalt * 0xBF58476D1CE4E5B9ULL);
    z = (z ^ (z >> 30)) * 0xBF58476D1CE4E5B9ULL;
    z = (z ^ (z >> 27)) * 0x94D049BB133111EBULL;
    return z ^ (z >> 31);
}""",
    """static inline uint64_t agKernelObfuscateMask(uint32_t channelSalt) {
    uint64_t z = kAGProtectedMixPrime1 ^ ((uint64_t)channelSalt * kAGProtectedMixPrime2);
    z = (z ^ (z >> 30)) * kAGProtectedMixPrime2;
    z = (z ^ (z >> 27)) * kAGProtectedMixPrime3;
    return z ^ (z >> 31);
}"""
)
code = code.replace("uint64_t stateBits = 0xA5A5000000000000ULL;", "uint64_t stateBits = kAGProtectedKernelMagic;")
code = code.replace("!= 0xA5A5000000000000ULL", "!= kAGProtectedKernelMagic")
code = code.replace(
    """    const double a = 6378245.0;
    const double ee = 0.00669342162296594323;""",
    """    const double a = kAGProtectedEarthAxisA;
    const double ee = kAGProtectedEarthEE;"""
)

xor_arrays = [
    "kEncCLSimCls", "kEncSetRep", "kEncSetDel", "kEncClear", "kEncStop", "kEncFlush",
    "kEncSrcCls", "kEncInitSrc", "kEncIsSimSoft", "kEncIsProdAcc",
    "kEncIsSim1", "kEncIsSim2", "kEncSrcInfo", "kEncType", "kEncClientLoc"
]
for arr_name in xor_arrays:
    code = code.replace(f"static const uint8_t {arr_name}[]", f"ANGEL_CORE_DATA static uint8_t {arr_name}[]")

code = code.replace('objc_getClass("AMapLocationManager")', 'objc_getClass(kAGProtectedHookAMapCls)')

# ==============================================================================
# 8. 安装插件后默认全部开关设为关闭状态 (NO)
# ==============================================================================
code = code.replace(
    'BOOL autoRestore = (p[@"AG_LocSpoof_AutoRestore"] == nil) ? YES : [p[@"AG_LocSpoof_AutoRestore"] boolValue];',
    'BOOL autoRestore = (p[@"AG_LocSpoof_AutoRestore"] == nil) ? NO : [p[@"AG_LocSpoof_AutoRestore"] boolValue];'
)
code = code.replace(
    """static BOOL agIsMarqueeEnabled(void) {
    if (!gCachedPrefs) {
        reloadGeminiPrefsCache();
    }
    NSNumber *val = gCachedPrefs[@"AG_Marquee_Enabled"];
    return val ? [val boolValue] : YES;
}""",
    """static BOOL agIsMarqueeEnabled(void) {
    if (!gCachedPrefs) {
        reloadGeminiPrefsCache();
    }
    NSNumber *val = gCachedPrefs[@"AG_Marquee_Enabled"];
    return val ? [val boolValue] : NO;
}"""
)
code = code.replace(
    """static BOOL agIsTabEnabled(AGConfigTab tab) {
    NSDictionary *p = agReadOnlyPrefs();
    NSString *key = (tab == AGConfigTabTranslation) ? @"AG_Trans_Enabled" : [NSString stringWithFormat:@"%@_Enabled", agPrefixForTab(tab)];
    NSNumber *val = p[key];
    return val ? [val boolValue] : YES;
}""",
    """static BOOL agIsTabEnabled(AGConfigTab tab) {
    if (!gAngelIsAuthorized) return NO;
    NSDictionary *p = agReadOnlyPrefs();
    NSString *key = (tab == AGConfigTabTranslation) ? @"AG_Trans_Enabled" : [NSString stringWithFormat:@"%@_Enabled", agPrefixForTab(tab)];
    NSNumber *val = p[key];
    return val ? [val boolValue] : NO;
}"""
)
code = code.replace(
    """- (BOOL)isTweakEnabled {
    NSNumber *val = loadGeminiPrefs()[@"Gemini_Enabled"];
    return val ? [val boolValue] : YES;
}""",
    """- (BOOL)isTweakEnabled {
    if (!gAngelIsAuthorized) return NO;
    NSNumber *val = loadGeminiPrefs()[@"Gemini_Enabled"];
    return val ? [val boolValue] : NO;
}"""
)
code = code.replace(
    'self.masterEnableSwitch.on = prefs[@"Gemini_Enabled"] ? [prefs[@"Gemini_Enabled"] boolValue] : YES;',
    'self.masterEnableSwitch.on = prefs[@"Gemini_Enabled"] ? [prefs[@"Gemini_Enabled"] boolValue] : NO;'
)
code = code.replace(
    """static BOOL agTransIsEnabled(void) {
    NSNumber *v = agReadOnlyPrefs()[@"AG_Trans_Enabled"];
    return v ? [v boolValue] : YES;
}""",
    """static BOOL agTransIsEnabled(void) {
    if (!gAngelIsAuthorized) return NO;
    NSNumber *v = agReadOnlyPrefs()[@"AG_Trans_Enabled"];
    return v ? [v boolValue] : NO;
}"""
)
code = code.replace(
    """static BOOL agTransIsHighAccuracyOCR(void) {
    NSDictionary *p = loadGeminiPrefs();
    NSNumber *val = p[@"AG_Trans_HighAccuracyOCR"];
    return val ? [val boolValue] : YES;
}""",
    """static BOOL agTransIsHighAccuracyOCR(void) {
    NSDictionary *p = loadGeminiPrefs();
    NSNumber *val = p[@"AG_Trans_HighAccuracyOCR"];
    return val ? [val boolValue] : NO;
}"""
)

# ==============================================================================
# 9. 在 %ctor 注入 Dopamine 跨进程 UDID 绑定预解密与云脑自动握手
# ==============================================================================
ctor_sb_target = """        if (isSpringBoard) {
            agRecordSpringBoardBootEventAndWarmupCache();"""
ctor_sb_replacement = """        if (isSpringBoard) {
            Angel_TryFastDecryptFromCachedSession();
            Angel_PerformCloudHandshake(nil, ^{
                [[AGGlobalActionPillWindow sharedPill] showActionPill:
                    [NSString stringWithFormat:@"🛡️ 云脑授权已激活 (剩余 %ld 天)", (long)gAngelRemainingDays]];
            });
            agRecordSpringBoardBootEventAndWarmupCache();"""
assert ctor_sb_target in code, "未找到 SpringBoard %ctor 入口"
code = code.replace(ctor_sb_target, ctor_sb_replacement, 1)

ctor_pref_target = """        if (isPreferences) {
            reloadGeminiPrefsCache();"""
ctor_pref_replacement = """        if (isPreferences) {
            Angel_TryFastDecryptFromCachedSession();
            Angel_PerformCloudHandshake(nil, nil);
            reloadGeminiPrefsCache();"""
assert ctor_pref_target in code, "未找到 Preferences %ctor 入口"
code = code.replace(ctor_pref_target, ctor_pref_replacement, 1)

ctor_app_target = """        gCurrentClientStepID = AGDiagStep_CtorEnter;
        agSyncClientHooksWithCurrentState();"""
ctor_app_replacement = """        gCurrentClientStepID = AGDiagStep_CtorEnter;
        if (!Angel_TryFastDecryptFromCachedSession()) {
            Angel_PerformCloudHandshake(nil, ^{
                agSyncClientHooksWithCurrentState();
            });
        }
        agSyncClientHooksWithCurrentState();"""
assert ctor_app_target in code, "未找到 Client App %ctor 入口"
code = code.replace(ctor_app_target, ctor_app_replacement, 1)

with open(OUTPUT_FILE, "w", encoding="utf-8") as f:
    f.write(code)

print(f"[+] 殿堂级加固 + 防卡死修复 + 底部BUG联系邮箱植入完成！已生成 {OUTPUT_FILE}（共 {len(code.splitlines())} 行）")
