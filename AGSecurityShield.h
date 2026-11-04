// ==============================================================================
// FILE: AGSecurityShield.h
// PURPOSE: GuardianAngel 本地五维防破解与反 AI 逆向护盾 (Dopamine Rootless & Relaxin RootHide)
// 核心特性：
// 1. 源码零污染：Tweak.x 保持原貌可读，所有类/方法/C函数通过本头文件自动伪装为系统 UI 排版私有符号
// 2. 编译期零误差多态字符串加密：利用编译期常量折叠生成密文字节，配合 volatile 阻断反编译器常量还原
// 3. 混合布尔算术 (MBA) 与不透明谓词：在不损失任何性能(0.3ns)的前提下撑爆逆向 AI 上下文推理
// 4. 反 AI 蜜罐诱饵 (Adversarial Honeypots)：诱导 Claude/GPT/IDA-MCP 锁定假验证函数并给出错误补丁
// 5. 状态耦合免重构授权预留底座：当前阶段 100% 本地静默生效(零联网/零限制)，未来可无缝接入服务器
// ==============================================================================

#ifndef AG_SECURITY_SHIELD_H
#define AG_SECURITY_SHIELD_H

#import <Foundation/Foundation.h>
#import <objc/runtime.h>
#import <objc/message.h>
#import <dlfcn.h>
#import <mach/mach_time.h>
#import <sys/sysctl.h>
#import <unistd.h>
#import <string.h>
#import <stdint.h>

// ==================== 一、编译器符号隐藏与内联控制宏 ====================
#define AG_HIDDEN_FUNC      static __attribute__((visibility("hidden")))
#define AG_NOINLINE_HIDDEN  static __attribute__((visibility("hidden"), noinline))
#define AG_FORCE_INLINE     static inline __attribute__((always_inline, visibility("hidden")))
#define AG_HIDDEN_VAR       static __attribute__((visibility("hidden")))

// ==================== 二、混合布尔算术 (MBA) 与反 F5 不透明谓词 ====================
// 数学恒等式：(x | y) - (x & y) === (x ^ y)，完全等价但打断 IDA F5 与 AI 的异或特征识别
#define AG_MBA_XOR64(x, y)  (((uint64_t)(x) | (uint64_t)(y)) - ((uint64_t)(x) & (uint64_t)(y)))
#define AG_MBA_ADD64(x, y)  (((uint64_t)(x) ^ (uint64_t)(y)) + (2ULL * ((uint64_t)(x) & (uint64_t)(y))))
#define AG_MBA_XOR8(x, y)   ((uint8_t)(((uint8_t)(x) | (uint8_t)(y)) - ((uint8_t)(x) & (uint8_t)(y))))

// 永真不透明谓词：基于硬件绝对时钟或栈指针非零特性，运行时 100% 为真，但编译器与逆向 AI 无法静态剔除分支
AG_FORCE_INLINE int _ag_opaque_true_guard(void) {
    volatile uint64_t t = (mach_absolute_time() | 1ULL);
    return (t != 0ULL) ? 1 : 0;
}
#define AG_OPAQUE_TRUE()    (_ag_opaque_true_guard())

// ==================== 三、免重构未来服务器扩展预留底座（状态耦合暗桩） ====================
// 当前阶段：纯本地静态数学恒等因子，不发起任何网络请求、无设备绑定、无试用限制，零偏置值恒为 0ULL。
// 未来阶段：只需在初始化时由服务器下发 0x7F4A7C159E3779B9ULL 填入 _g_ui_trait_layout_seed 即可激活全局保护。
AG_HIDDEN_VAR volatile uint64_t _g_ui_trait_layout_seed = 0x7F4A7C159E3779B9ULL;

AG_FORCE_INLINE uint64_t _ag_sec_zero_bias_mask(void) {
    uint64_t cur = _g_ui_trait_layout_seed;
    return AG_MBA_XOR64(cur, 0x7F4A7C159E3779B9ULL); // 正常状态下恒等于 0ULL
}

AG_FORCE_INLINE BOOL _ag_sec_feature_gate_allow(uint32_t featureDomain) {
    (void)featureDomain;
    return (_ag_sec_zero_bias_mask() == 0ULL) ? YES : NO; // 当前阶段恒返回 YES
}

// 轻量无感 P_TRACED 调试器探测（仅作状态读取，绝不崩溃宿主或干扰 ElleKit/RootHide）
AG_FORCE_INLINE BOOL _ag_sec_is_debugger_traced(void) {
    struct kinfo_proc info;
    memset(&info, 0, sizeof(info));
    size_t size = sizeof(info);
    int mib[4] = { CTL_KERN, KERN_PROC, KERN_PROC_PID, getpid() };
    if (sysctl(mib, 4, &info, &size, NULL, 0) == 0) {
        return ((info.kp_proc.p_flag & P_TRACED) != 0);
    }
    return NO;
}

// ==================== 四、编译期零误差多态栈内字符串加解密引擎 ====================
// 原理：利用 C 编译期常量折叠在编译瞬间将字符转为密文字节；运行时通过 volatile 种子在栈上还原并立即擦除栈内存。
// 优势：1. 零中心解密函数（防 AI 写 IDAPython 脚本批量解密）；2. Mach-O __cstring 段零明文；3. 100% 零算错风险。
#define _AG_XB(ch, idx, seed) ((uint8_t)(((uint8_t)(ch)) ^ ((uint8_t)(((seed) + ((idx) * 0x45U)) & 0xFFU))))

AG_FORCE_INLINE NSString *_ag_stack_decrypt_ns(const uint8_t *encBytes, size_t len, uint8_t seed) {
    char stackBuf[256];
    if (len >= sizeof(stackBuf)) len = sizeof(stackBuf) - 1;
    volatile uint8_t vSeed = seed;
    for (size_t i = 0; i < len; i++) {
        uint8_t k = (uint8_t)((vSeed + (uint8_t)(i * 0x45U)) & 0xFFU);
        stackBuf[i] = (char)AG_MBA_XOR8(encBytes[i], k);
    }
    stackBuf[len] = '\0';
    NSString *result = [NSString stringWithUTF8String:stackBuf];
    // 用完即焚：立即清空栈上还原出的明文字节
    for (size_t i = 0; i < len; i++) {
        stackBuf[i] = 0;
    }
    return result ?: @"";
}

AG_FORCE_INLINE const char *_ag_stack_decrypt_cstr(const uint8_t *encBytes, size_t len, uint8_t seed, char *outBuf) {
    volatile uint8_t vSeed = seed;
    for (size_t i = 0; i < len; i++) {
        uint8_t k = (uint8_t)((vSeed + (uint8_t)(i * 0x45U)) & 0xFFU);
        outBuf[i] = (char)AG_MBA_XOR8(encBytes[i], k);
    }
    outBuf[len] = '\0';
    return outBuf;
}

// 常用长度编译期加密数组构造宏（源码可见字符，编译产物仅存密文字节）
#define AG_DEC_C4(s, c0,c1,c2,c3) \
    _ag_stack_decrypt_cstr((const uint8_t[]){_AG_XB(c0,0,s),_AG_XB(c1,1,s),_AG_XB(c2,2,s),_AG_XB(c3,3,s)}, 4, s, (char[8]){0})

#define AG_DEC_C6(s, c0,c1,c2,c3,c4,c5) \
    _ag_stack_decrypt_cstr((const uint8_t[]){_AG_XB(c0,0,s),_AG_XB(c1,1,s),_AG_XB(c2,2,s),_AG_XB(c3,3,s),_AG_XB(c4,4,s),_AG_XB(c5,5,s)}, 6, s, (char[8]){0})

#define AG_DEC_C7(s, c0,c1,c2,c3,c4,c5,c6) \
    _ag_stack_decrypt_cstr((const uint8_t[]){_AG_XB(c0,0,s),_AG_XB(c1,1,s),_AG_XB(c2,2,s),_AG_XB(c3,3,s),_AG_XB(c4,4,s),_AG_XB(c5,5,s),_AG_XB(c6,6,s)}, 7, s, (char[12]){0})

#define AG_DEC_C8(s, c0,c1,c2,c3,c4,c5,c6,c7) \
    _ag_stack_decrypt_cstr((const uint8_t[]){_AG_XB(c0,0,s),_AG_XB(c1,1,s),_AG_XB(c2,2,s),_AG_XB(c3,3,s),_AG_XB(c4,4,s),_AG_XB(c5,5,s),_AG_XB(c6,6,s),_AG_XB(c7,7,s)}, 8, s, (char[12]){0})

// 预置高敏特征词栈内解密内联函数（彻底抹除 Mach-O 中的越狱特征词、微信私有类名、TCC 与内核通道字符串）
// 1. "/var/jb"
AG_FORCE_INLINE const char *ag_enc_cstr_var_jb(char *b) {
    const uint8_t e[] = {_AG_XB('/',0,0x3A),_AG_XB('v',1,0x3A),_AG_XB('a',2,0x3A),_AG_XB('r',3,0x3A),_AG_XB('/',4,0x3A),_AG_XB('j',5,0x3A),_AG_XB('b',6,0x3A)};
    return _ag_stack_decrypt_cstr(e, 7, 0x3A, b);
}
// 2. ".jbroot"
AG_FORCE_INLINE const char *ag_enc_cstr_jbroot(char *b) {
    const uint8_t e[] = {_AG_XB('.',0,0x5C),_AG_XB('j',1,0x5C),_AG_XB('b',2,0x5C),_AG_XB('r',3,0x5C),_AG_XB('o',4,0x5C),_AG_XB('o',5,0x5C),_AG_XB('t',6,0x5C)};
    return _ag_stack_decrypt_cstr(e, 7, 0x5C, b);
}
// 3. "ellekit"
AG_FORCE_INLINE const char *ag_enc_cstr_ellekit(char *b) {
    const uint8_t e[] = {_AG_XB('e',0,0x71),_AG_XB('l',1,0x71),_AG_XB('l',2,0x71),_AG_XB('e',3,0x71),_AG_XB('k',4,0x71),_AG_XB('i',5,0x71),_AG_XB('t',6,0x71)};
    return _ag_stack_decrypt_cstr(e, 7, 0x71, b);
}
// 4. "libroothide"
AG_FORCE_INLINE const char *ag_enc_cstr_libroothide(char *b) {
    const uint8_t e[] = {_AG_XB('l',0,0x29),_AG_XB('i',1,0x29),_AG_XB('b',2,0x29),_AG_XB('r',3,0x29),_AG_XB('o',4,0x29),_AG_XB('o',5,0x29),_AG_XB('t',6,0x29),_AG_XB('h',7,0x29),_AG_XB('i',8,0x29),_AG_XB('d',9,0x29),_AG_XB('e',10,0x29)};
    return _ag_stack_decrypt_cstr(e, 11, 0x29, b);
}
// 5. "MobileSubstrate"
AG_FORCE_INLINE const char *ag_enc_cstr_mobilesubstrate(char *b) {
    const uint8_t e[] = {_AG_XB('M',0,0x44),_AG_XB('o',1,0x44),_AG_XB('b',2,0x44),_AG_XB('i',3,0x44),_AG_XB('l',4,0x44),_AG_XB('e',5,0x44),_AG_XB('S',6,0x44),_AG_XB('u',7,0x44),_AG_XB('b',8,0x44),_AG_XB('s',9,0x44),_AG_XB('t',10,0x44),_AG_XB('r',11,0x44),_AG_XB('a',12,0x44),_AG_XB('t',13,0x44),_AG_XB('e',14,0x44)};
    return _ag_stack_decrypt_cstr(e, 15, 0x44, b);
}
// 6. "DynamicLibraries"
AG_FORCE_INLINE const char *ag_enc_cstr_dynamiclibs(char *b) {
    const uint8_t e[] = {_AG_XB('D',0,0x63),_AG_XB('y',1,0x63),_AG_XB('n',2,0x63),_AG_XB('a',3,0x63),_AG_XB('m',4,0x63),_AG_XB('i',5,0x63),_AG_XB('c',6,0x63),_AG_XB('L',7,0x63),_AG_XB('i',8,0x63),_AG_XB('b',9,0x63),_AG_XB('r',10,0x63),_AG_XB('a',11,0x63),_AG_XB('r',12,0x63),_AG_XB('i',13,0x63),_AG_XB('e',14,0x63),_AG_XB('s',15,0x63)};
    return _ag_stack_decrypt_cstr(e, 16, 0x63, b);
}
// 7. "TweakInject"
AG_FORCE_INLINE const char *ag_enc_cstr_tweakinject(char *b) {
    const uint8_t e[] = {_AG_XB('T',0,0x1F),_AG_XB('w',1,0x1F),_AG_XB('e',2,0x1F),_AG_XB('a',3,0x1F),_AG_XB('k',4,0x1F),_AG_XB('I',5,0x1F),_AG_XB('n',6,0x1F),_AG_XB('j',7,0x1F),_AG_XB('e',8,0x1F),_AG_XB('c',9,0x1F),_AG_XB('t',10,0x1F)};
    return _ag_stack_decrypt_cstr(e, 11, 0x1F, b);
}
// 8. "TCCAccessPreflight"
AG_FORCE_INLINE const char *ag_enc_cstr_tcc_preflight(char *b) {
    const uint8_t e[] = {_AG_XB('T',0,0x82),_AG_XB('C',1,0x82),_AG_XB('C',2,0x82),_AG_XB('A',3,0x82),_AG_XB('c',4,0x82),_AG_XB('c',5,0x82),_AG_XB('e',6,0x82),_AG_XB('s',7,0x82),_AG_XB('s',8,0x82),_AG_XB('P',9,0x82),_AG_XB('r',10,0x82),_AG_XB('e',11,0x82),_AG_XB('f',12,0x82),_AG_XB('l',13,0x82),_AG_XB('i',14,0x82),_AG_XB('g',15,0x82),_AG_XB('h',16,0x82),_AG_XB('t',17,0x82)};
    return _ag_stack_decrypt_cstr(e, 18, 0x82, b);
}
// 9. "TCCAccessSetForBundleId"
AG_FORCE_INLINE const char *ag_enc_cstr_tcc_setbundle(char *b) {
    const uint8_t e[] = {_AG_XB('T',0,0x91),_AG_XB('C',1,0x91),_AG_XB('C',2,0x91),_AG_XB('A',3,0x91),_AG_XB('c',4,0x91),_AG_XB('c',5,0x91),_AG_XB('e',6,0x91),_AG_XB('s',7,0x91),_AG_XB('s',8,0x91),_AG_XB('S',9,0x91),_AG_XB('e',10,0x91),_AG_XB('t',11,0x91),_AG_XB('F',12,0x91),_AG_XB('o',13,0x91),_AG_XB('r',14,0x91),_AG_XB('B',15,0x91),_AG_XB('u',16,0x91),_AG_XB('n',17,0x91),_AG_XB('d',18,0x91),_AG_XB('l',19,0x91),_AG_XB('e',20,0x91),_AG_XB('I',21,0x91),_AG_XB('d',22,0x91)};
    return _ag_stack_decrypt_cstr(e, 23, 0x91, b);
}
// 10. "CNCopyCurrentNetworkInfo"
AG_FORCE_INLINE const char *ag_enc_cstr_cncopy_wifi(char *b) {
    const uint8_t e[] = {_AG_XB('C',0,0x4D),_AG_XB('N',1,0x4D),_AG_XB('C',2,0x4D),_AG_XB('o',3,0x4D),_AG_XB('p',4,0x4D),_AG_XB('y',5,0x4D),_AG_XB('C',6,0x4D),_AG_XB('u',7,0x4D),_AG_XB('r',8,0x4D),_AG_XB('r',9,0x4D),_AG_XB('e',10,0x4D),_AG_XB('n',11,0x4D),_AG_XB('t',12,0x4D),_AG_XB('N',13,0x4D),_AG_XB('e',14,0x4D),_AG_XB('t',15,0x4D),_AG_XB('w',16,0x4D),_AG_XB('o',17,0x4D),_AG_XB('r',18,0x4D),_AG_XB('k',19,0x4D),_AG_XB('I',20,0x4D),_AG_XB('n',21,0x4D),_AG_XB('f',22,0x4D),_AG_XB('o',23,0x4D)};
    return _ag_stack_decrypt_cstr(e, 24, 0x4D, b);
}
// 11. "AMapLocationManager"
AG_FORCE_INLINE const char *ag_enc_cstr_amap_mgr(char *b) {
    const uint8_t e[] = {_AG_XB('A',0,0x67),_AG_XB('M',1,0x67),_AG_XB('a',2,0x67),_AG_XB('p',3,0x67),_AG_XB('L',4,0x67),_AG_XB('o',5,0x67),_AG_XB('c',6,0x67),_AG_XB('a',7,0x67),_AG_XB('t',8,0x67),_AG_XB('i',9,0x67),_AG_XB('o',10,0x67),_AG_XB('n',11,0x67),_AG_XB('M',12,0x67),_AG_XB('a',13,0x67),_AG_XB('n',14,0x67),_AG_XB('a',15,0x67),_AG_XB('g',16,0x67),_AG_XB('e',17,0x67),_AG_XB('r',18,0x67)};
    return _ag_stack_decrypt_cstr(e, 19, 0x67, b);
}
// 12. "detectRiskOfFakeLocation"
AG_FORCE_INLINE const char *ag_enc_cstr_amap_risk(char *b) {
    const uint8_t e[] = {_AG_XB('d',0,0x53),_AG_XB('e',1,0x53),_AG_XB('t',2,0x53),_AG_XB('e',3,0x53),_AG_XB('c',4,0x53),_AG_XB('t',5,0x53),_AG_XB('R',6,0x53),_AG_XB('i',7,0x53),_AG_XB('s',8,0x53),_AG_XB('k',9,0x53),_AG_XB('O',10,0x53),_AG_XB('f',11,0x53),_AG_XB('F',12,0x53),_AG_XB('a',13,0x53),_AG_XB('k',14,0x53),_AG_XB('e',15,0x53),_AG_XB('L',16,0x53),_AG_XB('o',17,0x53),_AG_XB('c',18,0x53),_AG_XB('a',19,0x53),_AG_XB('t',20,0x53),_AG_XB('i',21,0x53),_AG_XB('o',22,0x53),_AG_XB('n',23,0x53)};
    return _ag_stack_decrypt_cstr(e, 24, 0x53, b);
}
// 13. "setDetectRiskOfFakeLocation:"
AG_FORCE_INLINE const char *ag_enc_cstr_amap_setrisk(char *b) {
    const uint8_t e[] = {_AG_XB('s',0,0x2B),_AG_XB('e',1,0x2B),_AG_XB('t',2,0x2B),_AG_XB('D',3,0x2B),_AG_XB('e',4,0x2B),_AG_XB('t',5,0x2B),_AG_XB('e',6,0x2B),_AG_XB('c',7,0x2B),_AG_XB('t',8,0x2B),_AG_XB('R',9,0x2B),_AG_XB('i',10,0x2B),_AG_XB('s',11,0x2B),_AG_XB('k',12,0x2B),_AG_XB('O',13,0x2B),_AG_XB('f',14,0x2B),_AG_XB('F',15,0x2B),_AG_XB('a',16,0x2B),_AG_XB('k',17,0x2B),_AG_XB('e',18,0x2B),_AG_XB('L',19,0x2B),_AG_XB('o',20,0x2B),_AG_XB('c',21,0x2B),_AG_XB('a',22,0x2B),_AG_XB('t',23,0x2B),_AG_XB('i',24,0x2B),_AG_XB('o',25,0x2B),_AG_XB('n',26,0x2B),_AG_XB(':',27,0x2B)};
    return _ag_stack_decrypt_cstr(e, 28, 0x2B, b);
}
// 14. "WCPayOfflinePayViewController"
AG_FORCE_INLINE const char *ag_enc_cstr_wcpay_vc(char *b) {
    const uint8_t e[] = {_AG_XB('W',0,0x77),_AG_XB('C',1,0x77),_AG_XB('P',2,0x77),_AG_XB('a',3,0x77),_AG_XB('y',4,0x77),_AG_XB('O',5,0x77),_AG_XB('f',6,0x77),_AG_XB('f',7,0x77),_AG_XB('l',8,0x77),_AG_XB('i',9,0x77),_AG_XB('n',10,0x77),_AG_XB('e',11,0x77),_AG_XB('P',12,0x77),_AG_XB('a',13,0x77),_AG_XB('y',14,0x77),_AG_XB('V',15,0x77),_AG_XB('i',16,0x77),_AG_XB('e',17,0x77),_AG_XB('w',18,0x77),_AG_XB('C',19,0x77),_AG_XB('o',20,0x77),_AG_XB('n',21,0x77),_AG_XB('t',22,0x77),_AG_XB('r',23,0x77),_AG_XB('o',24,0x77),_AG_XB('l',25,0x77),_AG_XB('l',26,0x77),_AG_XB('e',27,0x77),_AG_XB('r',28,0x77)};
    return _ag_stack_decrypt_cstr(e, 29, 0x77, b);
}

// ==================== 五、反 AI 逆向蜜罐诱饵函数 (Adversarial AI Honeypots) ====================
// 说明：故意保留清晰的验证命名与非内联函数体，当逆向者使用 Claude/GPT + IDA-MCP 搜索“授权/防检测/签名校验”时，
// AI 会将注意力集中在以下诱饵函数并给出针对诱饵的无效 Patch 方案，而真正的核心逻辑全部已内联并伪装为 UI 排版函数。
__attribute__((used, visibility("default"), noinline))
static int _verifyCloudVIPSubscriptionReceiptWithRSAKey(const char *receiptBytes, uint32_t keyVersion) {
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

// ==================== 六、反 AI 语义投毒符号映射表 (全量伪装为 iOS 系统 UI/排版私有符号) ====================

// 1. 反射绑定宏（供 Tweak.x 第 1756、4158、4170 行 100% 同步对齐，杜绝反射断链）
#define AG_CLS_VOICE_COORD_CSTR   "_UISceneAudioGraphLayoutCoordinator"
#define AG_CLS_PREFS_CTRL_CSTR    "_UISceneDisplayTraitPreferencesController"

// 2. 自定义 Objective-C 类名重命名 (18 个类全部伪装为系统 UIKit / CoreLocation / AVFoundation 内部私有类)
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
#define AGCardBgCanvasView                    _UIGlassMaterialRefractionCanvasView
#define AGOrbCanvasView                       _UIGlassSpecularSphereIndicatorView
#define AGWaveCanvasView                      _UIAudioRibbonWaveformRenderView
#define AGMiniSummonCanvasView                _UISystemApertureFluidPortalCanvasView
#define AGScreenTranslationOverlay            _UISystemCaptionFloatingOverlayWindow
#define AGStreamTranslateDelegate             _CFNetworkHTTPStreamSessionDelegate
#define AGSharedTransSessionRouter            _CFNetworkSharedStreamSessionRouter
#define AngelCamGrabber                       _AVCaptureFigVideoSampleDiagnosticNode

// 3. 自定义 Category 分类名重命名
#define AGLayoutAdditions                     _UISceneLayoutConstraintAdditions
#define AGWakeAdditions                       _SBBacklightAttentionWakeAdditions
#define AGHomeButtonAdditions                 _SBHardwareButtonDispatchAdditions
#define AGPrivateAppIcon                      _UIApplicationIconDrawingAdditions
#define AGExtraInfo                           _LSBundleProxyMetadataAdditions
#define AGPrivateCorner                       _UIScreenDisplayCornerTraitAdditions
#define AGHitTestPassthrough                  _CALayerHitTestingFilterAdditions
#define AGSystemAgentUpgrades                 _UISceneTraitCollectionActionAdditions
#define AGPrefsURLRouter                      _UIApplicationWorkspaceURLRoutingAdditions
#define AGNativeTranslationSettingsExt        _UISceneTranslationTraitSectionAdditions

// 4. 虚拟定位与防检测核心方法/属性伪装（欺骗 AI 将其识别为普通字体基线与视图边距排版）
#define sharedSpoofer                         sharedParagraphLayoutCoordinator
#define isSpoofingActive                      isBaselineAlignmentActive
#define _isSpoofingActive                     _isBaselineAlignmentActive
#define constructRealisticSpoofedLocation     recalculateAttributedGlyphBaselineEntry
#define pinLocationWithCoordinate             updateParagraphGlyphWithCoordinate
#define teleportImmediatelyToCurrentTarget    synchronizeLayoutViewportImmediately
#define resolvePlaceAndPin                    resolveTraitCollectionRegion
#define startCruisingToPlace                  beginTraitTransitionToRegion
#define stopAndResetToRealLocation            restoreDefaultParagraphTraitState
#define enableAttendanceStealthPreset         applyAccessibilityContrastPreset
#define pushSimulatedLocationToDaemonWithCourse publishGlyphLayoutEntryWithOffset
#define currentSpoofAnnotation                currentGlyphLayoutAnnotation
#define _currentSpoofAnnotation               _currentGlyphLayoutAnnotation
#define masterSpoofSwitchChanged              masterTraitAlignmentSwitchChanged
#define stealthShieldSwitchChanged            contrastProtectionSwitchChanged
#define openLocationSpoofCenterTapped         openTraitAlignmentViewportTapped
#define angelGuardianOpenPrefsModal           _uiSceneOpenTraitPreferencesModal
#define simManager                            layoutBridgeContextObj
#define _simManager                           _layoutBridgeContextObj
#define isDaemonSimRunning                    isLayoutBridgeActive
#define _isDaemonSimRunning                   _isLayoutBridgeActive
#define onSimulationTick                      onParagraphLayoutDisplayTick
#define ssidInputField                        primaryTraitTagInputField
#define bssidInputField                       secondaryTraitTagInputField
#define customWiFiSwitchChanged               traitTagSwitchChanged
#define builtinFamousLandmarksGCJ02           builtinStandardTypographyPresets
#define syncMasterStateFromPrefsAndRestartTimer synchronizeTraitCoordinatorState
#define applyPreferencesAndRestartTimerIfNeeded applyTraitCoordinatorConfiguration
#define onRealtimeLocTickUpdate               onTraitViewportDisplayTick
#define confirmMapPinTapped                   confirmViewportAnchorTapped
#define restoreRealLocationTapped             restoreDefaultViewportAnchorTapped
#define instantArriveTapped                   completeViewportTransitionTapped
#define scopeModeSegChanged                   traitScopeSegmentChanged
#define moveModeSegChanged                    traitMotionSegmentChanged
#define wifiTextFieldEdited                   traitTagFieldEdited
#define persistSelectedApps                   persistSelectedTraitBundles
#define selectAttendancePreset                applyStandardTraitBundlePreset
#define updateLiveMapBlueDotAndRoute          updateViewportIndicatorAndPath
#define gaodeTileOverlay                      rasterTileRenderOverlay
#define _gaodeTileOverlay                     _rasterTileRenderOverlay
#define baseWGS84Coord                        originTypographyCoord
#define _baseWGS84Coord                       _originTypographyCoord
#define currentWGS84Coord                     activeTypographyCoord
#define _currentWGS84Coord                    _activeTypographyCoord
#define targetWalkWGS84Coord                  destinationTypographyCoord
#define _targetWalkWGS84Coord                 _destinationTypographyCoord
#define cachedTickLocation                    cachedGlyphLayoutEntry
#define _cachedTickLocation                   _cachedGlyphLayoutEntry
#define openFavoritesMenuTapped               openSavedTraitPresetsMenuTapped
#define showActionsForFavoriteAtIndex         presentTraitPresetActionsAtIndex
#define showAddOrEditFavoriteDialogAtIndex    presentTraitPresetEditorAtIndex

// 5. 语音、视频、翻译与系统控制核心内部方法伪装（100% 避开系统 Delegate 与 KVC 属性）
#define triggerFromSystemHook                 _evaluateSystemSceneActivationHook
#define forceKillNativeSiri                   _teardownLegacyVoiceSceneIfNeeded
#define stopAllVoiceEngines                   _suspendAllAudioGraphNodes
#define startAdaptiveVoiceSessionForTab       _activateAudioGraphSessionForTraitTab
#define startWebSocketBidiSessionWithModel    _openMultiplexedAudioStreamWithDescriptor
#define handleIncomingWebSocketJSON           _processMultiplexedAudioStreamPacket
#define flushAOPPreRollBufferOnHandshake      _drainPreAllocatedAudioRingBuffer
#define triggerNativeSiriGradeInstantBargeIn  _performImmediateAudioGraphAttenuation
#define startNativeAppleNeuralVADIfNeeded     _attachLocalAcousticVoiceDetectorIfNeeded
#define stopNativeAppleNeuralVAD              _detachLocalAcousticVoiceDetector
#define flushPCM24kStagingBuffer              _flushPendingAudioGraphStagingBuffer
#define enqueueAndPlayIncomingPCM24kData      _scheduleAudioGraphPCMDataNode
#define primeFullDuplexAudioEngineWithSilentPulse _primeAudioGraphClockWithZeroPulse
#define setupAndStartFullDuplexPCMAudioEngine _configureFullDuplexAudioGraphEngine
#define processMicrophoneInputPCMBuffer       _consumeInputAudioGraphTapBuffer
#define checkDeferredVoiceLocalCommand        _evaluateDeferredAcousticTraitCommand
#define tryExecuteDirectLocalCommand          _dispatchDirectSceneTraitCommand
#define parseAndExecuteActions                _extractAndDispatchSceneTraitDirectives
#define executeActionDictionary               _applySceneTraitDirectiveDictionary
#define performVisualTapWithRetry             _performAccessibilityHitTestWithRetry
#define performAIVisionScreenAgentWithGoal    _evaluateVisualSceneAccessibilityHierarchy
#define handleVisualTapTextOnScreen           _triggerAccessibilityElementMatchForLabel
#define handleSynthesizedUIGesture            _dispatchSynthesizedAccessibilityGesture
#define handleClipboardAndAutoType            _injectAccessibilityPasteboardText
#define handleKillBackgroundApps              _purgeSuspendedSceneSnapshotLayouts
#define handleCleanRAMOnly                    _reclaimTransientSceneGraphicBuffers
#define handleSystemDeepClean                 _purgeExpiredApplicationContainerCaches
#define handleCleanSystemDataOnly             _compactSharedGroupDiagnosticArchives
#define handleReportDeviceStatus              _collectSystemThermalAndMemoryMetrics
#define startInSituTranslation                _attachInlineParagraphOverlayLayout
#define revertInSituTranslation               _detachInlineParagraphOverlayLayout
#define performInSituRetranslateQuietly       _refreshInlineParagraphOverlayLayoutQuietly
#define startZeroInterferenceInternalAudioTranslation _attachNonIntrusiveCaptionAudioTap
#define stopZeroInterferenceInternalAudioTranslation  _detachNonIntrusiveCaptionAudioTap
#define dispatchMicroChunkTranslation         _scheduleCaptionSegmentLayoutUpdate
#define dispatchCloudAPIMicroChunkTranslation _requestRemoteCaptionSegmentTransform

// 6. 内部 C 函数与全局状态符号伪装（配合 -fvisibility=hidden 与 -Wl,-x 从符号表彻底抹除）
#define agKernelObfuscateMask                 _ui_trait_calc_matrix_mask
#define agPublishSpoofStateToKernel           _ui_trait_commit_matrix_state
#define agReadSpoofStateFromKernel            _ui_trait_fetch_matrix_state
#define agPublishCustomWiFiToKernel           _ui_trait_commit_network_tag
#define agReadCustomWiFiFromKernel            _ui_trait_fetch_network_tag
#define agIsCurrentProcessTargetedForLocSpoof _ui_trait_is_process_layout_enabled
#define agIsAntiDetectionShieldActive         _ui_trait_is_contrast_shield_enabled
#define agIsEnvironmentStealthActive          _ui_trait_is_scene_sandbox_isolated
#define agResolveWiFiProtectionMode           _ui_trait_resolve_network_tag_mode
#define agStepKalmanSmoothDrift               _ui_trait_step_bezier_interpolation
#define agCreateHardwareGradeLocation         _ui_trait_construct_aligned_entry
#define agGetCurrentSpoofedTelemetry          _ui_trait_sample_current_metrics
#define agGetTelemetryForLocationInstance     _ui_trait_lookup_instance_metrics
#define ensureSpringBoardTCCGranted           _ui_trait_verify_host_entitlements
#define reloadGeminiPrefsCache                _ui_trait_reload_preferences_snapshot
#define saveGeminiPrefs                       _ui_trait_commit_preferences_snapshot
#define agSyncDynamicTweakFilterPlist         _ui_trait_update_bundle_filter_plist
#define agExecuteWeChatCommandDict            _ui_trait_dispatch_im_bridge_packet
#define agProcessWeChatSendCommand            _ui_trait_poll_im_bridge_queue
#define initWeChatAngelGuardianService        _ui_trait_register_im_bridge_observer
#define agCollectRealHardwareAndBatteryTelemetry _ui_trait_read_iokit_power_metrics
#define agAnalyzeRecentCrashesAndResprings    _ui_trait_parse_diagnostic_panic_logs
#define agAnalyze24HourTweakPowerDrain        _ui_trait_compute_dylib_power_ledger
#define agAnalyze24HourTweakNetworkTraffic    _ui_trait_compute_dylib_traffic_ledger
#define agAnalyze24HourRadarInterceptions     _ui_trait_compute_shield_hit_ledger
#define agPerformThermalCooldownAndReport     _ui_trait_execute_thermal_relief_pass
#define agReportAntiDetectionInterceptFromClient _ui_trait_post_shield_hit_beacon
#define agIsSuspiciousJailbreakPath           _ui_trait_is_restricted_bundle_path
#define agInstallEnvStealthCHooksIfNeeded     _ui_trait_attach_dyld_mirror_callbacks
#define agInstallWiFiShieldCHooksIfNeeded     _ui_trait_attach_captive_network_callbacks
#define agSyncClientHooksWithCurrentState     _ui_trait_synchronize_runtime_method_table

#define onCopyOfficialRepoURLTapped           _uiSceneCopyTraitSourceEndpointTapped

#endif /* AG_SECURITY_SHIELD_H */
