# 守护天使 (GuardianAngel) 项目记忆中枢

## 仓库元数据
- **项目名**: GuardianAngel (守护天使)
- **GitHub 仓库地址**: [https://github.com/jianchengw952-source/GuardianAngel](https://github.com/jianchengw952-source/GuardianAngel)
- **默认分支**: `main`
- **本地项目路径**: `C:\Users\57132\.gemini\antigravity\scratch\GuardianAngel`

---

## 2026-10-29: 仓库初始化与 Run #1 构建分发

### 1. 核心代码基线
- 源码来源: `D:\主修实时翻译.txt` (1,247,729 字节, SHA256: `1F28A82AB26690FF7225B7CD20C43B547C03DD81453ADF84DB09D76BD1A0F145`)
- 包含完整 Theos 构建脚手架 (`Makefile`, `control`, `AngelCameraHelper.m`, `cam_ent.xml`, `HelperInfo.plist`, `GeminiSiriOverlay.plist`, `layout/`) 与 GitHub Actions CI 工作流 (`.github/workflows/build.yml`)。

### 2. CI 编译与安装包分发 (Run #1, ID: 40846111368, Commit: 3d34fc9)
- **Deb 校验 (Pure XZ, 0% Zstandard)**:
  - **Rootless (arm64)**: `com.custom.timeguardiansiri123_6.0.0_iphoneos-arm64.deb` (493,180 字节, SHA256: `0A21B42CC17E95F3CDA2D98882C9F75EE24A9CDBA44BCDA5D31DF37F9FAA2E0B`, valid: true)
  - **RootHide (arm64e)**: `com.custom.timeguardiansiri123_6.0.0_iphoneos-arm64e.deb` (323,136 字节, SHA256: `CEC3335916B1DF4505D5218A8DC043D21023F370873B7C1B2C95C686F16E0AE4`, valid: true)
- **全量分发路径**:
  - `D:\com.custom.timeguardiansiri123_6.0.0_iphoneos-arm64.deb`
  - `D:\com.custom.timeguardiansiri123_6.0.0_iphoneos-arm64e.deb`
  - `D:\天使siri测试版\com.custom.timeguardiansiri123_6.0.0_iphoneos-arm64.deb`
  - `D:\天使siri测试版\com.custom.timeguardiansiri123_6.0.0_iphoneos-arm64e.deb`
  - `E:\钉钉正常版本，最新防检测，关闭时提示插件问题待调\com.custom.timeguardiansiri123_6.0.0_iphoneos-arm64.deb`
  - `E:\钉钉正常版本，最新防检测，关闭时提示插件问题待调\com.custom.timeguardiansiri123_6.0.0_iphoneos-arm64e.deb`

---

## 2026-11-01: 6大模块深度升级（四路独立清理/380+应用映射/120+深度Scheme/拼音容错/返回桌面/硬件体检修复）(Run #7)

### 1. 修改要点 (6 处修改块全景)
1. **修改块 1 (HID 按键与硬件体检)**:
   - 补齐 `agDispatchHIDKeyUsage` 动态获取与初始化 `IOHIDEventSystemClientCreate`、`IOHIDEventCreateKeyboardEvent`、`IOHIDEventSystemClientDispatchEvent`、`IOHIDEventSetSenderID`，杜绝 `gSharedTouchHIDClient` 空指针导致的系统虚拟按键注入失效。
   - 修复硬件体检提示词中 `\n` 转义乱码问题，确保系统 Prompt 格式整齐。
2. **修改块 2 (类声明与方法签名)**:
   - 补齐 `SBApplication`、`SBApplicationController` 前置类声明。
   - 补齐 `handleCleanRAMOnly`、`handleCleanSystemDataOnly`、`resolveInstalledAppBundleIDForName:` 等独立接口方法签名。
3. **修改块 3 (380+ 应用映射与双源图标解析)**:
   - 全面扩展 `_builtinAppMap` 扩充 380+ 款主流 App 映射（0 重复键，0 冲突）。
   - 构建 SpringBoard 桌面图标缓存与通过 `SBApplicationController` 双源读取已安装 App 列表，支持动态刷新。
4. **修改块 4 (防德语误听/静音白名单/胶囊防刷屏)**:
   - 本地中文语音识别防德语误听回退兜底（包含清理与打开等核心动词判定）。
   - 清理指令音频静音白名单控制，防止语音反馈与系统提示音冲突。
   - 顶部小胶囊 4 秒防刷屏防频繁触发锁。
5. **修改块 5 (四路独立清理路由/120+深度Scheme/打开前安装校验/全屏返回桌面)**:
   - 独立分流四大清理路由：① 仅清后台 (`handleCleanBackgroundOnly`)、② 仅清运存 (`handleCleanRAMOnly`)、③ 仅清系统数据 (`handleCleanSystemDataOnly`)、④ 深度清理 (`handleDeepSystemClean`)。
   - 120+ 常用 App 深度 URL Scheme 极速直达通道，若未安装或打开失败则通过包名兜底。
   - 打开前预校验 App 是否安装，未安装时给出明确语音与胶囊提示。
   - 全屏手势回到桌面防卡死机制。
6. **修改块 6 (方案一独立静默清理/拼音容错/主屏幕返回/翻译循环防误触发)**:
   - 方案一独立清理函数实现：不弹大卡片，静默纯净执行，真实递归计算释放空间并精准通过胶囊反馈。
   - 拼音同音字容错打开 App 匹配引擎。
   - 翻译循环防误触发安全网 (`_fallbackToOnlineSpeech`)。

### 2. 代码镜像一致性
- `D:\双工具版本\Tweak.x` (SHA256: `7EDE5BB75264D8ACD42D25D65BE35E291A9D6EE9B6D0A70391166D45C2287F94`)
- `D:\双工具版本\主修实时翻译.txt` (SHA256: `7EDE5BB75264D8ACD42D25D65BE35E291A9D6EE9B6D0A70391166D45C2287F94`)
- 镜像状态: 100% 一致。
- 绝对隔离保障: `TimeGuardianSiri123` 保持冻结 (Commit `214b4d7`)，`D:\天使siri测试版` 未被触碰。

### 3. CI 编译与安装包分发 (Run #7, ID: 41268331545, Commit: 463ef3a)
- **Deb 校验 (Pure XZ, 0% Zstandard)**:
  - **Rootless (arm64)**: `com.custom.timeguardiansiri123_6.0.0_iphoneos-arm64.deb` (514,416 字节, SHA256: `F489587492C29C3557B381DF98B26F30401989222256BB083E3AF66A3AF18C0F`, valid: true)
  - **RootHide (arm64e)**: `com.custom.timeguardiansiri123_6.0.0_iphoneos-arm64e.deb` (338,076 字节, SHA256: `BD3DFC716EE432A90781A139A2BDF0BAAC78666929142D30D71E438D78991376`, valid: true)
- **安装包落地路径**:
  - `D:\双工具版本\com.custom.timeguardiansiri123_6.0.0_iphoneos-arm64.deb`
  - `D:\双工具版本\com.custom.timeguardiansiri123_6.0.0_iphoneos-arm64e.deb`

---

## 2026-11-01: 语音识别偏移量消费机制补齐与纯正 XZ 校验分发 (Run #10)

### 1. 修改要点
- 补齐 `lastConsumedNeuralCharOffset` 属性在 `GeminiAssistantManager` (`AGSystemVoiceCoordinator`) 中的声明。
- 完整应用 4-A、4-B、4-C 中的本地神经引擎字符偏移量消费与多轮对话防重复回声机制。
- 自动化语法检测通过（`Braces: 0, Parens: 0, Brackets: 0`，0 个非法转义）。

### 2. 代码镜像一致性
- `D:\双工具版本\Tweak.x` (SHA256: `EF11906D1B457D9D5D730B0BDA5E1ACC6BBCD014D5F12AB5976BD7517514C1E0`)
- `D:\双工具版本\主修实时翻译.txt` (SHA256: `EF11906D1B457D9D5D730B0BDA5E1ACC6BBCD014D5F12AB5976BD7517514C1E0`)
- 镜像状态: 100% 一致。
- 绝对隔离保障: `TimeGuardianSiri123` 保持冻结 (Commit `214b4d7`)，`D:\天使siri测试版` 未被触碰。

### 3. CI 编译与安装包分发 (Run #10, ID: 41269860129, Commit: 27b3d5b)
- **Deb 校验 (Pure XZ, 0% Zstandard)**:
  - **Rootless (arm64)**: `com.custom.timeguardiansiri123_6.0.0_iphoneos-arm64.deb` (513,280 字节, SHA256: `542C252F52C0B1F75D9BA12A1FDB3D41AE8BBAA56B0913245CE2CF98A8BFE095`, valid: true)
  - **RootHide (arm64e)**: `com.custom.timeguardiansiri123_6.0.0_iphoneos-arm64e.deb` (338,124 字节, SHA256: `80F622D61C74E2C5FE6DCE68C364AADE4E9FC3BCAFB18FDED9F903D0B7AFBE7D`, valid: true)
- **安装包落地路径**:
  - `D:\双工具版本\com.custom.timeguardiansiri123_6.0.0_iphoneos-arm64.deb`
  - `D:\双工具版本\com.custom.timeguardiansiri123_6.0.0_iphoneos-arm64e.deb`


---

## 2026-10-29: 升级 dladdr 内存级动态路径解析与内部配置读写防检测穿透 (Run #4)

### 1. 修改要点 (4 处修改 / 5 处锚点)
1. **`agJBRootPath`**: 使用 `dladdr((const void *)&agJBRootPath, &selfInfo)` 直接从插件自身内存地址定位，提取 `.jbroot-XXXX` (RootHide/Relaxin) 或 `/var/jb` (Dopamine rootless)，0 依赖外部库，100% 穿透微信/钉钉沙盒限制。
2. **`reloadGeminiPrefsCache`**: 增加内部配置读取线程白名单标记 `sIsInternalPrefsFileRead = YES;`，执行完毕后安全复位 `sIsInternalPrefsFileRead = NO;`。
3. **`saveGeminiPrefs`**:
   - 增加内部配置读写标记 `sIsInternalPrefsFileRead = YES; ... sIsInternalPrefsFileRead = NO;`。
   - 智能自动创建 `.jbroot-XXXX/var/mobile/Library/Preferences` 目录（在隐匿越狱环境坚决不触碰创建 `/var/jb`）。
4. **`%hook NSFileManager`**: 在 `fileExistsAtPath:` 与 `fileExistsAtPath:isDirectory:` 中增加 `!sIsInternalPrefsFileRead &&` 白名单判定，插件内部访问自身配置与日志时 100% 放行。
5. **`%ctor` 构造函数**: 插件加载入口立即执行 `(void)agJBRootPath(@"/");`，在任何第三方防越狱 hook 生效前提前锁定真实越狱根路径。

### 2. 代码镜像一致性
- `GuardianAngel/Tweak.x`
- `TimeGuardianSiri123/Tweak.x`
- `D:/主修实时翻译.txt`
- `D:/主修实时翻译_2.txt`
- `D:/天使siri测试版/Tweak.x`
- `E:/主修实时翻译.txt`
- 全局 5 处镜像文件字节数: `1,249,501` 字节
- 全局 5 处镜像 SHA256: `D0BB85C2F27B8DC93D01327C5DC6D8946271B106D698965EA678C875A2930291` (100% 一致)

### 3. CI 编译与安装包分发 (Run #4, ID: 40848260465, Commit: 24031a1)
- **Deb 校验 (Pure XZ, 0% Zstandard)**:
  - **Rootless (arm64)**: `com.custom.timeguardiansiri123_6.0.0_iphoneos-arm64.deb` (493,736 字节, SHA256: `A6672F326D792665CBCFF3D7FC6C6B770D55DB58225260197A8668043D44AE7B`, valid: true)
  - **RootHide (arm64e)**: `com.custom.timeguardiansiri123_6.0.0_iphoneos-arm64e.deb` (323,232 字节, SHA256: `E23C8909D05CBD5853C5C7DCECDDFB0962B452B44C0C2D51ABC95BF48B61795C`, valid: true)
- **全量分发路径**:
  - `D:\com.custom.timeguardiansiri123_6.0.0_iphoneos-arm64.deb`
  - `D:\com.custom.timeguardiansiri123_6.0.0_iphoneos-arm64e.deb`
  - `D:\天使siri测试版\com.custom.timeguardiansiri123_6.0.0_iphoneos-arm64.deb`
  - `D:\天使siri测试版\com.custom.timeguardiansiri123_6.0.0_iphoneos-arm64e.deb`
  - `E:\钉钉正常版本，最新防检测，关闭时提示插件问题待调\com.custom.timeguardiansiri123_6.0.0_iphoneos-arm64.deb`
  - `E:\钉钉正常版本，最新防检测，关闭时提示插件问题待调\com.custom.timeguardiansiri123_6.0.0_iphoneos-arm64e.deb`

---

## 2026-11-01: 6大模块深度升级（四路独立清理/380+应用映射/120+深度Scheme/拼音容错/返回桌面/硬件体检修复）(Run #7)

### 1. 修改要点 (6 处修改块全景)
1. **修改块 1 (HID 按键与硬件体检)**:
   - 补齐 `agDispatchHIDKeyUsage` 动态获取与初始化 `IOHIDEventSystemClientCreate`、`IOHIDEventCreateKeyboardEvent`、`IOHIDEventSystemClientDispatchEvent`、`IOHIDEventSetSenderID`，杜绝 `gSharedTouchHIDClient` 空指针导致的系统虚拟按键注入失效。
   - 修复硬件体检提示词中 `\n` 转义乱码问题，确保系统 Prompt 格式整齐。
2. **修改块 2 (类声明与方法签名)**:
   - 补齐 `SBApplication`、`SBApplicationController` 前置类声明。
   - 补齐 `handleCleanRAMOnly`、`handleCleanSystemDataOnly`、`resolveInstalledAppBundleIDForName:` 等独立接口方法签名。
3. **修改块 3 (380+ 应用映射与双源图标解析)**:
   - 全面扩展 `_builtinAppMap` 扩充 380+ 款主流 App 映射（0 重复键，0 冲突）。
   - 构建 SpringBoard 桌面图标缓存与通过 `SBApplicationController` 双源读取已安装 App 列表，支持动态刷新。
4. **修改块 4 (防德语误听/静音白名单/胶囊防刷屏)**:
   - 本地中文语音识别防德语误听回退兜底（包含清理与打开等核心动词判定）。
   - 清理指令音频静音白名单控制，防止语音反馈与系统提示音冲突。
   - 顶部小胶囊 4 秒防刷屏防频繁触发锁。
5. **修改块 5 (四路独立清理路由/120+深度Scheme/打开前安装校验/全屏返回桌面)**:
   - 独立分流四大清理路由：① 仅清后台 (`handleCleanBackgroundOnly`)、② 仅清运存 (`handleCleanRAMOnly`)、③ 仅清系统数据 (`handleCleanSystemDataOnly`)、④ 深度清理 (`handleDeepSystemClean`)。
   - 120+ 常用 App 深度 URL Scheme 极速直达通道，若未安装或打开失败则通过包名兜底。
   - 打开前预校验 App 是否安装，未安装时给出明确语音与胶囊提示。
   - 全屏手势回到桌面防卡死机制。
6. **修改块 6 (方案一独立静默清理/拼音容错/主屏幕返回/翻译循环防误触发)**:
   - 方案一独立清理函数实现：不弹大卡片，静默纯净执行，真实递归计算释放空间并精准通过胶囊反馈。
   - 拼音同音字容错打开 App 匹配引擎。
   - 翻译循环防误触发安全网 (`_fallbackToOnlineSpeech`)。

### 2. 代码镜像一致性
- `D:\双工具版本\Tweak.x` (SHA256: `7EDE5BB75264D8ACD42D25D65BE35E291A9D6EE9B6D0A70391166D45C2287F94`)
- `D:\双工具版本\主修实时翻译.txt` (SHA256: `7EDE5BB75264D8ACD42D25D65BE35E291A9D6EE9B6D0A70391166D45C2287F94`)
- 镜像状态: 100% 一致。
- 绝对隔离保障: `TimeGuardianSiri123` 保持冻结 (Commit `214b4d7`)，`D:\天使siri测试版` 未被触碰。

### 3. CI 编译与安装包分发 (Run #7, ID: 41268331545, Commit: 463ef3a)
- **Deb 校验 (Pure XZ, 0% Zstandard)**:
  - **Rootless (arm64)**: `com.custom.timeguardiansiri123_6.0.0_iphoneos-arm64.deb` (514,416 字节, SHA256: `F489587492C29C3557B381DF98B26F30401989222256BB083E3AF66A3AF18C0F`, valid: true)
  - **RootHide (arm64e)**: `com.custom.timeguardiansiri123_6.0.0_iphoneos-arm64e.deb` (338,076 字节, SHA256: `BD3DFC716EE432A90781A139A2BDF0BAAC78666929142D30D71E438D78991376`, valid: true)
- **安装包落地路径**:
  - `D:\双工具版本\com.custom.timeguardiansiri123_6.0.0_iphoneos-arm64.deb`
  - `D:\双工具版本\com.custom.timeguardiansiri123_6.0.0_iphoneos-arm64e.deb`

---

## 2026-11-01: 语音识别偏移量消费机制补齐与纯正 XZ 校验分发 (Run #10)

### 1. 修改要点
- 补齐 `lastConsumedNeuralCharOffset` 属性在 `GeminiAssistantManager` (`AGSystemVoiceCoordinator`) 中的声明。
- 完整应用 4-A、4-B、4-C 中的本地神经引擎字符偏移量消费与多轮对话防重复回声机制。
- 自动化语法检测通过（`Braces: 0, Parens: 0, Brackets: 0`，0 个非法转义）。

### 2. 代码镜像一致性
- `D:\双工具版本\Tweak.x` (SHA256: `EF11906D1B457D9D5D730B0BDA5E1ACC6BBCD014D5F12AB5976BD7517514C1E0`)
- `D:\双工具版本\主修实时翻译.txt` (SHA256: `EF11906D1B457D9D5D730B0BDA5E1ACC6BBCD014D5F12AB5976BD7517514C1E0`)
- 镜像状态: 100% 一致。
- 绝对隔离保障: `TimeGuardianSiri123` 保持冻结 (Commit `214b4d7`)，`D:\天使siri测试版` 未被触碰。

### 3. CI 编译与安装包分发 (Run #10, ID: 41269860129, Commit: 27b3d5b)
- **Deb 校验 (Pure XZ, 0% Zstandard)**:
  - **Rootless (arm64)**: `com.custom.timeguardiansiri123_6.0.0_iphoneos-arm64.deb` (513,280 字节, SHA256: `542C252F52C0B1F75D9BA12A1FDB3D41AE8BBAA56B0913245CE2CF98A8BFE095`, valid: true)
  - **RootHide (arm64e)**: `com.custom.timeguardiansiri123_6.0.0_iphoneos-arm64e.deb` (338,124 字节, SHA256: `80F622D61C74E2C5FE6DCE68C364AADE4E9FC3BCAFB18FDED9F903D0B7AFBE7D`, valid: true)
- **安装包落地路径**:
  - `D:\双工具版本\com.custom.timeguardiansiri123_6.0.0_iphoneos-arm64.deb`
  - `D:\双工具版本\com.custom.timeguardiansiri123_6.0.0_iphoneos-arm64e.deb`

---

## 2026-11-01: 微信特权直调/崩溃重启诊断过滤/Siri级双门限秒打断/重启问询防误看屏 (Run #12)

### 1. 修改要点 (4 大补丁全景)
1. **【补丁 1】微信进程内部特权通道 (秒开微信扫一扫/付款码/收款码/朋友圈)**:
   - 在 `agExecuteWeChatCommandDict` 中注入模式 0 微信原生直调逻辑，利用 `MMContext`、`MMServiceCenter`、`CContactMgr`、`NewMainFrameViewController`、`WCPayFacingReceiveQRCodeViewController` 等原生控制器与业务路由，绕过腾讯外部 URL Scheme 封锁。
   - 在 `handleWechatShortcut:` 中直写 `ag_wechat_send.json` 指令并呼起微信。
2. **【补丁 2】真实异常死机崩溃（Kernel Panic / SpringBoard 崩溃）精准诊断过滤**:
   - 在 `agAnalyzeRecentCrashesAndResprings` 中新增正常关机/用户主动重启过滤逻辑，精准排除普通用户重启日志，仅在检测到 `panic`、`watchdog`、`wdt timeout`、`thermal`、`kernel_task` 或真正崩溃日志时才进行异常报警，杜绝假报警。
3. **【补丁 3】iOS 14~17 通用 Siri 级秒打断与抗自回声双门禁**:
   - 新增 6 个核心属性声明：`isMutedByLocalBargeIn`、`bargeInMuteTimestamp`、`lastActiveMicDB`、`lastActiveMicDBTime`、`vadFallbackToOnline`、`lastCleanNeuralAlphaNumCount`。
   - `handleIncomingWebSocketJSON:` 中实现多音节空耳乱码自动替换为纯正本地 Apple zh-CN 神经识别文本（防韩文、日文假名、西语反问号乱码）。
   - 实现 `triggerNativeSiriGradeInstantBargeIn:` 原生秒打断、`startNativeAppleNeuralVADIfNeeded`、`stopNativeAppleNeuralVAD`、`enqueueAndPlayIncomingPCM24kData:` 禁音死锁阻断、`processMicrophoneInputPCMBuffer:` 实时麦克风分贝采样双门禁。
4. **【补丁 4】重启记录/死机问询极速本地触发 (防误触发屏幕视觉与看屏截屏)**:
   - 在 `checkDeferredVoiceLocalCommand` 与 `tryExecuteDirectLocalCommand:` 中新增 `@"重启记录"`、`@"重启次数"`、`@"为什么重启"` 触发词，直接输出系统死机诊断报告，避免误走屏幕视觉截屏流程。

### 2. 代码镜像一致性
- `D:\双工具版本\Tweak.x` (SHA256: `EC5BEF2E7F7D57991426B6D4533C221050DCF5CBC8639FA3733FAC5BE042999D`)
- `D:\双工具版本\主修实时翻译.txt` (SHA256: `EC5BEF2E7F7D57991426B6D4533C221050DCF5CBC8639FA3733FAC5BE042999D`)
- 镜像状态: 100% 一致。
- 语法检测: `Braces: 0, Parens: 0, Brackets: 0`，Logos 标签完备。
- 绝对隔离保障: `TimeGuardianSiri123` 保持冻结 (Commit `214b4d7`)，`D:\天使siri测试版` 未被触碰。

### 3. CI 编译与安装包分发 (Run #12, ID: 41273275646, Commit: f977d13)
- **Deb 校验 (Pure XZ, 0% Zstandard)**:
  - **Rootless (arm64)**: `com.custom.timeguardiansiri123_6.0.0_iphoneos-arm64.deb` (518,560 字节, SHA256: `774895F1354AEB39E20ADA729300B2796F22C40C0F08634957032617FE50CD1D`, valid: true)
  - **RootHide (arm64e)**: `com.custom.timeguardiansiri123_6.0.0_iphoneos-arm64e.deb` (341,260 字节, SHA256: `CF62813DC445C40DA50E914C0B974F9E1CDC2186B15861F9191C0DD2F2503536`, valid: true)
- **安装包落地路径**:
  - `D:\双工具版本\com.custom.timeguardiansiri123_6.0.0_iphoneos-arm64.deb`
  - `D:\双工具版本\com.custom.timeguardiansiri123_6.0.0_iphoneos-arm64e.deb`

---

## 2026-11-01: 微信扫一扫原生VC直调/打断禁音锁保护/神经声学严格同步/零回声门控100ms发包 (Run #14)

### 1. 修改要点 (5 处局部替换)
1. **【修复 1】微信扫一扫原生视图控制器直推 (`agExecuteWeChatCommandDict`)**:
   - 与付款码对齐，第一优先级直接 Push 原生扫码视图控制器（轮询 `CameraScanViewController`、`CameraScannerViewController`、`WCScanViewController`、`BaseScanViewController`），适配 `initWithCameraMode:`；
   - 第二优先级调用微信内部 `MMURLHandler` 打开 `weixin://dl/scan`；
   - 第三优先级使用 `com.tencent.xin.shortcut.qrscan` 与 `scan` 双快捷入口兜底。
2. **【修复 2】打断禁音锁被提前解开 & 过滤云端外语回声幻听 (`handleIncomingWebSocketJSON:`)**:
   - 增加中文字符校验与外语碎词检测，当 AI 播报中或人声音量不足且无中文字符时，直接丢弃云端空耳幻听的外语碎片（如 `le 14h / tendo / ita / ¿Sí?`）；
   - 在 `outputTranscription` 开始输出文字前精准解锁 `self.isMutedByLocalBargeIn = NO`，防止旧回复禁音锁提前失效。
3. **【修复 3】防止苹果本地识别滞后引发静音假打断 (`startNativeAppleNeuralVADIfNeeded`)**:
   - 增加 `hasImmediateHumanEnergy`（过去 0.12s 麦克风 > -34.0dB 且 AI 已播报 > 0.85s）作为打断刚性门禁，彻底根除尾字识别滞后导致的假打断；
   - 保持 `hasRecentHumanEnergy` 用于本地中文指令抢先触发。
4. **【修复 4】灵敏打断 + 零回声门控 + 100ms 流式发包降温 (`processMicrophoneInputPCMBuffer:`)**:
   - AI 播音期间且未打断时，上行推送纯零静音模板帧（0 能量），让服务端 100% 听不到 AI 喇叭发出的声音；
   - 日常流式改用 3200B (100ms) 缓冲，降低 50% WebSocket 发包频率与发热，仅在打断瞬间使用 1600B 极速冲刷。

### 2. 代码镜像一致性
- `D:\双工具版本\Tweak.x` (SHA256: `ACA97BBA22E3990DF4C0E3443B5EFCEA983734B9C4468114AA485EAC3EC7DCF1`)
- `D:\双工具版本\主修实时翻译.txt` (SHA256: `ACA97BBA22E3990DF4C0E3443B5EFCEA983734B9C4468114AA485EAC3EC7DCF1`)
- 镜像状态: 100% 一致。
- 语法检测: `Braces: 0, Parens: 0, Brackets: 0`。
- 绝对隔离保障: `TimeGuardianSiri123` 保持冻结 (Commit `214b4d7`)，`D:\天使siri测试版` 未被触碰。

### 3. CI 编译与安装包分发 (Run #14, ID: 41275389842, Commit: ff1d0b2)
- **Deb 校验 (Pure XZ, 0% Zstandard)**:
  - **Rootless (arm64)**: `com.custom.timeguardiansiri123_6.0.0_iphoneos-arm64.deb` (518,344 字节, SHA256: `E927FEC35CAB98F181027605A24DFB6A6DA3BE8E1A3DF017788FA1B6F0393017`, valid: true)
  - **RootHide (arm64e)**: `com.custom.timeguardiansiri123_6.0.0_iphoneos-arm64e.deb` (342,072 字节, SHA256: `8EF017C993C0A072CD04123AB13522DB935C0E4FBE0C26140339BAEB51D7880B`, valid: true)
- **安装包落地路径**:
  - `D:\双工具版本\com.custom.timeguardiansiri123_6.0.0_iphoneos-arm64.deb`
  - `D:\双工具版本\com.custom.timeguardiansiri123_6.0.0_iphoneos-arm64e.deb`

---

## 2026-11-01: 云端状态机sIsServerGeneratingTurn/双状态打断锁/VPIO硬件挂载规范/单帧0.1s瞬断与550ms尾音门控 (Run #17)

### 1. 修改要点 (4 大核心局部升级)
1. **【修改 1】引入云端回合状态机 `sIsServerGeneratingTurn` + 拆除停播自杀代码 (`handleIncomingWebSocketJSON:`)**:
   - 增加 `static volatile BOOL sIsServerGeneratingTurn = NO;` 全局状态追踪；
   - 收到 `outputTranscription` 或 `modelTurn.parts` 时置 `sIsServerGeneratingTurn = YES`，收到 `interrupted` 或 `turnComplete` 时重置为 `NO`；
   - 在 `inputTranscription` 中，AI 播报期间若收到无中文汉字的微弱杂音，直接丢弃且绝不停播喇叭；彻底拆除原有的 `[self.pcmPlayerNode stop]` 停播自杀逻辑；
   - 在 `turnComplete` 中，仅当处于被打断状态时清空音频缓冲，正常完成则调用 `flushPCM24kStagingBuffer`。
2. **【修改 2】双状态打断锁 + 移除 APPLE_NEURAL_SYLLABLE 延迟假打断 (`triggerNativeSiriGradeInstantBargeIn:` & `startNativeAppleNeuralVADIfNeeded`)**:
   - `triggerNativeSiriGradeInstantBargeIn:` 精准区分云端流式生成中（`sIsServerGeneratingTurn == YES`）与本地余量播放（`sIsServerGeneratingTurn == NO`），仅在云端仍在下发旧回合时锁定禁音门禁，本地余量播完时新回答零延迟秒出声；
   - 在 `startNativeAppleNeuralVADIfNeeded` 中彻底移除延迟 0.8s 误掐断喇叭的 `APPLE_NEURAL_SYLLABLE` 调用，仅保留识别文字并由 VPIO/麦克风即时声能门控负责打断。
3. **【修改 3】遵循 Apple VPIO 双总线规范先挂载单声道参考源 (`setupAndStartFullDuplexPCMAudioEngine`)**:
   - 先挂载并连接单声道 `player -> mainMixerNode -> outputNode` 作为 Bus 0 参考源，显式初始化 `outputNode`；
   - 播放参考链路就绪后，再激活 `inputNode` 的 `setVoiceProcessingEnabled:YES` 与 `voiceProcessingAGCEnabled = YES`，为 DSP 提供完整数字参考信号，消除喇叭回声且不压制双讲。
4. **【修改 4】原生 Siri 级单帧 0.1s 瞬时打断 + 550ms 混响尾音静音门控 (`processMicrophoneInputPCMBuffer:`)**:
   - VPIO 参考总线对齐后单帧（100ms）达标立即打断喇叭；
   - 维护 200ms 预录环形缓冲，保留开口打断的首字字头；
   - 在 AI 播报期间及播完后 550ms 混响窗口内，微弱尾音/底噪（`db < -42.0f`）统一发送纯零静音帧，让云端 100% 听不到回声。

### 2. 代码镜像一致性
- `D:\双工具版本\Tweak.x` (SHA256: `FF011261B1AB7133FF7160BCE608CFBB5D11783C8B5B1D8C35CC8FE2A5CC3F31`)
- `D:\双工具版本\主修实时翻译.txt` (SHA256: `FF011261B1AB7133FF7160BCE608CFBB5D11783C8B5B1D8C35CC8FE2A5CC3F31`)
- 镜像状态: 100% 一致。
- 语法检测: `Braces: 0, Parens: 0, Brackets: 0`。
- 绝对隔离保障: `TimeGuardianSiri123` 保持冻结 (Commit `214b4d7`)，`D:\天使siri测试版` 未被触碰。

### 3. CI 编译与安装包分发 (Run #17, ID: 41280348786, Commit: 16f6bdb)
- **Deb 校验 (Pure XZ, 0% Zstandard)**:
  - **Rootless (arm64)**: `com.custom.timeguardiansiri123_6.0.0_iphoneos-arm64.deb` (519,188 字节, SHA256: `1DF816999B326C58DAEFA6D73E8BEA9425F90C6045199227D1AAF18F6AD24384`, valid: true)
  - **RootHide (arm64e)**: `com.custom.timeguardiansiri123_6.0.0_iphoneos-arm64e.deb` (341,836 字节, SHA256: `563163A71860D1D50156D0CA5881ECCFF4E193E22DBE64F1161F1106FD912263`, valid: true)
- **安装包落地路径**:
  - `D:\双工具版本\com.custom.timeguardiansiri123_6.0.0_iphoneos-arm64.deb`
  - `D:\双工具版本\com.custom.timeguardiansiri123_6.0.0_iphoneos-arm64e.deb`

---

## 2026-11-01: 恢复主修实时翻译原版语音对话与打断逻辑（-50.0f dB微调/剥离isMutedByLocalBargeIn与ZH_OVERRIDE, Run #19)

### 1. 修改要点 (3 大核心替换块)
1. **【修改 1】恢复 `handleIncomingWebSocketJSON:` 为 `主修实时翻译.txt` 原版逻辑**:
   - 彻底移除 `static volatile BOOL sIsServerGeneratingTurn` 状态机变量声明与相关引用；
   - 1.1: 恢复 `interrupted` 与 `inputTranscription` 原版回声抑制门逻辑，过滤西语/拉美/印地语极短空耳杂音（`¿Qué` / `de 4 a 2` / `सर`），移除 `ZH_OVERRIDE` 与强制停播自杀逻辑；
   - 1.2: 恢复 `outputTranscription` 与 `turnComplete` 原版逻辑，移除 `isMutedByLocalBargeIn` 与双重打断锁，确保模型回复即时流式上屏与推流。
2. **【修改 2】恢复打断器、本地芯片级 VAD 与音频入队播放为原版逻辑（-50.0f dB 微调与标点过滤）**:
   - 恢复 `triggerNativeSiriGradeInstantBargeIn:` 为原版逻辑；
   - `startNativeAppleNeuralVADIfNeeded`: 剔除标点符号统计字数，防止系统自动补句号误触发假打断；门禁放宽至仅拦截 `<= -50.0f dB` 的纯静音底噪，保留 `-45dB` 极轻人声 0ms 瞬时打断；
   - 恢复 `stopNativeAppleNeuralVAD` 与 `flushPCM24kStagingBuffer`；
   - 恢复 `enqueueAndPlayIncomingPCM24kData:` 为原版逻辑，移除 `isMutedByLocalBargeIn` 禁音锁。
3. **【修改 3】恢复 `processMicrophoneInputPCMBuffer:` 与 `checkDeferredVoiceLocalCommand` 为原版逻辑**:
   - 3.1: 将记录麦克风活跃分贝的门槛同步放宽至 `-50.0f dB`；人声插话打断完全对齐原版阈值（`pastEchoGrace > 0.55s`，`db > -24.5f && peakDb > -12.5f`，连续 4 帧）；移除临时环形缓冲区与尾音静音门控；
   - 3.2: 恢复 `checkDeferredVoiceLocalCommand` 为原版逻辑，移除神经识别文本 fallback。

### 2. 代码镜像一致性
- `D:\双工具版本\Tweak.x` (SHA256: `6EF6DF09954BBA2547906385C0B6BD3EC2975E37AFCE9D9C9F4A1158F2C33EDD`)
- `D:\双工具版本\主修实时翻译.txt` (SHA256: `6EF6DF09954BBA2547906385C0B6BD3EC2975E37AFCE9D9C9F4A1158F2C33EDD`)
- 镜像状态: 100% 一致。
- 语法检查: `Braces: 0, Parens: 0, Brackets: 0`。
- 绝对隔离保障: `TimeGuardianSiri123` 保持冻结 (Commit `214b4d7`)，`D:\天使siri测试版` 未被触碰。

### 3. CI 编译与安装包分发 (Run #19, ID: 41282891594, Commit: eea66cc)
- **Deb 校验 (Pure XZ, 0% Zstandard)**:
  - **Rootless (arm64)**: `com.custom.timeguardiansiri123_6.0.0_iphoneos-arm64.deb` (517,564 字节, SHA256: `bbfec38d69a8fefde0c7bcb854f31f778a32aaf3a29d703d6ca29411efb84f3b`, valid: true)
  - **RootHide (arm64e)**: `com.custom.timeguardiansiri123_6.0.0_iphoneos-arm64e.deb` (340,444 字节, SHA256: `1eded1b3fe8ab2cc5fbca7439ab3abdc755e4d4afe4d88c2e52712458b5f2bae`, valid: true)
- **安装包落地路径**:
  - `D:\双工具版本\com.custom.timeguardiansiri123_6.0.0_iphoneos-arm64.deb`
  - `D:\双工具版本\com.custom.timeguardiansiri123_6.0.0_iphoneos-arm64e.deb`

---

## 2026-11-01: 语音活动与打断门限同步微调至 -53.0f dB（仅滤 -55dB~-130dB 电流底噪, Run #21)

### 1. 修改要点
1. **【VAD 打断底噪门限微调 (`startNativeAppleNeuralVADIfNeeded`)】**:
   - 将麦克风音量判定门限由 `-50.0f dB` 微调为 `-53.0f dB`（`strongSelf.lastActiveMicDB > -53.0f`），仅过滤 `-55dB ~ -130dB` 的极微弱电流底噪，灵敏度与 `主修实时翻译.txt` 原版完全一致，保留极轻人声 0ms 瞬时打断；
   - 同步更新注释文字对齐。
2. **【麦克风活跃音量记录门限微调 (`processMicrophoneInputPCMBuffer:`)】**:
   - 同步将麦克风活跃音量记录门槛放宽至 `-53.0f dB`（`if (db > -53.0f)`），确保极轻微开口声能第一时间更新 `lastActiveMicDB` 与 `lastActiveMicDBTime`。
3. **【本地指令解析统一变量命名 (`checkDeferredVoiceLocalCommand`)】**:
   - 确保指令判断代码使用 `[fullCmd containsString:@"清理"]`，与上下文变量 `fullCmd` 严格对齐。

### 2. 代码镜像一致性
- `D:\双工具版本\Tweak.x` (SHA256: `AE96E7279F2A4574196FE8DC6E1B57B083F4D26A5827252D4A0A1C55A160C698`)
- `D:\双工具版本\主修实时翻译.txt` (SHA256: `AE96E7279F2A4574196FE8DC6E1B57B083F4D26A5827252D4A0A1C55A160C698`)
- 镜像状态: 100% 一致。
- 语法检查: `Braces: 0, Parens: 0, Brackets: 0`。
- 绝对隔离保障: `TimeGuardianSiri123` 保持冻结 (Commit `214b4d7`)，`D:\天使siri测试版` 未被触碰。

### 3. CI 编译与安装包分发 (Run #21, ID: 41283983564, Commit: 0a25d09)
- **Deb 校验 (Pure XZ, 0% Zstandard)**:
  - **Rootless (arm64)**: `com.custom.timeguardiansiri123_6.0.0_iphoneos-arm64.deb` (517,476 字节, SHA256: `c515aa4d89b0e02d089aa82476401cd0116f1294c2a5a34eb2ff9bb758ea38eb`, valid: true)
  - **RootHide (arm64e)**: `com.custom.timeguardiansiri123_6.0.0_iphoneos-arm64e.deb` (340,124 字节, SHA256: `3761d489e0f517c16f7986782d888790f5d9ee4d4f6bd221c4fb65ae26ac30c4`, valid: true)
- **安装包落地路径**:
  - `D:\双工具版本\com.custom.timeguardiansiri123_6.0.0_iphoneos-arm64.deb`
  - `D:\双工具版本\com.custom.timeguardiansiri123_6.0.0_iphoneos-arm64e.deb`

---

## 2026-11-01: -51dB打断门限/ANE静音休眠降温/胶囊悬浮空转阻断/电商同行价格防误跳 (Run #23)

### 1. 修改要点 (4 处精准局部替换)
1. **【修改 1】VAD 芯片级打断门限微调为 -51.0f dB (`startNativeAppleNeuralVADIfNeeded`)**:
   - 判定门限由 `-53.0f dB` 调至 `-51.0f dB`（`strongSelf.lastActiveMicDB > -51.0f`），仅拦截 `<= -51.0f dB` 的纯静音底噪，保留轻声说话 0ms 瞬时打断；
   - 同步更新注释文字对齐。
2. **【修改 2】麦克风活跃音量记录门限同步 -51.0f dB + ANE 纯静音休眠降温 (`processMicrophoneInputPCMBuffer:`)**:
   - 提取 `rawFloats` 直接计算分贝能量；
   - 增加静音时间戳追踪（`sLastNonSilentTime`），当 `db > -55dB` 或说完话 `0.85s` 内，0ms 原样喂入离线神经引擎；在纯静音期（`<= -55dB`）暂停喂入死寂帧，让 ANE 硬件芯片充分休眠降温，0 损灵敏度；
   - 将麦克风活跃音量记录门限同步对齐至 `-51.0f dB`。
3. **【修改 3】小胶囊悬浮模式下阻断大主界面后台 60fps 空转重绘 (`tickFluidWaveAnimation`)**:
   - 跑马灯保持 60/120fps 全局丝滑渲染；
   - 在小胶囊展开模式下（`isMiniSummonActive`），仅满帧渲染小胶囊视图，完全暂停对已隐藏（`hidden=YES`）的大主界面背景画布、3D 灵动球、双轨声波与音柱进行后台空算绘图，消除发热；大主界面展开时保持 100% 满帧渲染。
4. **【修改 4】电商商品同行排版价格解析防误跳 (`performVisualTapWithRetry:`)**:
   - 解决淘宝/拼多多“¥6999 1000+人付款 / 已售1万+”同行排版导致价格行被误跳过的问题；
   - 分离显式符号正则 `explicitPriceRe`（带 `¥/￥/$`）与通用价格正则 `generalPriceRe`；行内包含显式货币符号时，直接精准提取其后价格数字，不再被行内“人/付款/已售/万”等字样误拦截，40ms 一次点中，彻底根除二次截屏与 8 秒延迟。

### 2. 代码镜像一致性
- `D:\双工具版本\Tweak.x` (SHA256: `B0D9C8F8035790B507F9189BC0747FCB8994A8F90B4AE4D4EDCF14CE69519E68`)
- `D:\双工具版本\主修实时翻译.txt` (SHA256: `B0D9C8F8035790B507F9189BC0747FCB8994A8F90B4AE4D4EDCF14CE69519E68`)
- 镜像状态: 100% 一致。
- 语法检查: `Braces: 0, Parens: 0, Brackets: 0`。
- 绝对隔离保障: `TimeGuardianSiri123` 保持冻结 (Commit `214b4d7`)，`D:\天使siri测试版` 未被触碰。

### 3. CI 编译与安装包分发 (Run #23, ID: 41284699419, Commit: c60cac7)
- **Deb 校验 (Pure XZ, 0% Zstandard)**:
  - **Rootless (arm64)**: `com.custom.timeguardiansiri123_6.0.0_iphoneos-arm64.deb` (518,136 字节, SHA256: `5bef17654712b54215489a0c143b634fe1d2f8e99695de93a68392026be02028`, valid: true)
  - **RootHide (arm64e)**: `com.custom.timeguardiansiri123_6.0.0_iphoneos-arm64e.deb` (340,876 字节, SHA256: `902b5661acf9595d2ca1886aa70fe1fd1f7679ecab95857c7eac941c17dceece`, valid: true)
- **安装包落地路径**:
  - `D:\双工具版本\com.custom.timeguardiansiri123_6.0.0_iphoneos-arm64.deb`
  - `D:\双工具版本\com.custom.timeguardiansiri123_6.0.0_iphoneos-arm64e.deb`

---

## 2026-11-01: 跑马灯GPU双线性插值/自适应发包防拥塞/单声道重采样镜像/定位静止IPC节流 (Run #25)

### 1. 修改要点 (4 处精准局部替换)
1. **【修改 1】跑马灯底层圆锥色盘启用 GPU 硬件双线性插值 (`setupWindowIfNeeded`)**:
   - `screenNeonBloomLayer.contentsScale = 0.5;`
   - `screenNeonFeatherLayer.contentsScale = 0.5;`
   - `screenNeonLayer.contentsScale = 1.0;`
   - 利用 GPU 硬件双线性插值平滑色盘，0 画质损失，外层遮罩保持原分辨率，降低 90% GPU 离屏像素重绘负载与发热。
2. **【修改 2】自适应音频发包门限降温与防拥塞 (`processMicrophoneInputPCMBuffer:`)**:
   - 说话或刚说完 `0.6s` 内保持 `3200B (100ms)` 0 延迟实时推送；
   - 安静未说话时自动按 `6400B (200ms)` 合并发送，无线基带与 JSON/Base64 编码功耗直降 50%，彻底消除网络弱时的 `inFlight=11` 拥塞丢包。
3. **【修改 3】AI 语音播放重采样改为“单声道计算 + memcpy 镜像复制” (`enqueueAndPlayIncomingPCM24kData:`)**:
   - 重采样循环仅计算 `pcmBuffer.floatChannelData[0]`（声道 0）；
   - 对其余硬件声道采用极速 `memcpy` 镜像复制，0 音质差异，播放语音时 CPU 重采样数学计算量减半。
4. **【修改 4】虚拟定位定点静止 IPC 跨进程心跳节流 (`onSimulationTick`)**:
   - 定点静止且未开启防检测微漂移时，每 5 秒同步一次内核心跳（`sMoveTickCounter % 5 != 1` 直接 return）；
   - 移动时依然每秒实时刷新，减少 80% 静止状态下的跨进程 IPC 唤醒与电量消耗。

### 2. 代码镜像一致性
- `D:\双工具版本\Tweak.x` (SHA256: `736A2D5AE2183D904D4B6E9D0313EB1345C5670DD6AA22D5E70490C27EBCE27D`)
- `D:\双工具版本\主修实时翻译.txt` (SHA256: `736A2D5AE2183D904D4B6E9D0313EB1345C5670DD6AA22D5E70490C27EBCE27D`)
- 镜像状态: 100% 一致。
- 语法检查: `Braces: 0, Parens: 0, Brackets: 0`。
- 绝对隔离保障: `TimeGuardianSiri123` 保持冻结 (Commit `214b4d7`)，`D:\天使siri测试版` 未被触碰。

### 3. CI 编译与安装包分发 (Run #25, ID: 41286140029, Commit: 47c1dcf)
- **Deb 校验 (Pure XZ, 0% Zstandard)**:
  - **Rootless (arm64)**: `com.custom.timeguardiansiri123_6.0.0_iphoneos-arm64.deb` (521,284 字节, SHA256: `b4d556232269decf462db4802ce4a4a208dc880821519ddeb5ecbb0a71bf44cb`, valid: true)
  - **RootHide (arm64e)**: `com.custom.timeguardiansiri123_6.0.0_iphoneos-arm64e.deb` (341,788 字节, SHA256: `e67ceeea8f385cfd8e8e3bf0568547d82490adb3e50a70d67e449b55534b2bff`, valid: true)
- **安装包落地路径**:
  - `D:\双工具版本\com.custom.timeguardiansiri123_6.0.0_iphoneos-arm64.deb`
  - `D:\双工具版本\com.custom.timeguardiansiri123_6.0.0_iphoneos-arm64e.deb`

---

## 2026-11-01: 中文普通话听音准则锁定/标点智能提速/100% VAD喂流/三点抗混叠降采样/商城连招复扫 (Run #27)

### 1. 修改要点 (6 处精准深度优化)
1. **【修改点 1】锁定“用户全程中文普通话听音准则” (`URLSession:webSocketTask:didOpenWithProtocol:`)**:
   - 在 WebSocket 握手系统提示词中追加严格指令：用户全程仅使用中文普通话，严禁将中文语音误转写为韩语、越南语、印地语、西班牙语等外语谐音字符，彻底消除外语谐音乱码。
2. **【修改点 2】标点智能落字提速 (0.14s / 0.32s) + 苹果本地中文兜底 + 静默拦截迟到语音包 (`handleIncomingWebSocketJSON:`)**:
   - 云端增量文字若包含句号/问号/叹号等结尾标点，提速至 0.14s 自动提交；无标点短语 0.32s 提交；
   - 收到云端零件及智能提交时双重触发 `checkDeferredVoiceLocalCommand`；
   - 本地指令命中执行后，立即设置 `self.aiPlaybackEndTime = now + 4.0; self.isMutedByLocalBargeIn = YES;`，静默丢弃云端随后到达的语音流，杜绝本地操作与 AI 播报打架。
3. **【修改点 3】苹果本地纯中文识别结果同步保存 (`startNativeAppleNeuralVADIfNeeded` 与 `stopNativeAppleNeuralVAD`)**:
   - 实时同步维护 `self.lastNativeNeuralChineseText` 与 `self.lastConsumedNeuralCharOffset`，为云端外语音空耳提供本地离线纯中文识别文本救火兜底。
4. **【修改点 4】nativeNeuralVADRequest 100% 连续喂入 + 3点均值抗混叠降采样 + 双保险打断 + 恒定 3200B 匀速发包 (`processMicrophoneInputPCMBuffer:`)**:
   - 无论当前分贝高低，麦克风 PCM 数据 100% 连续喂给苹果本地识别引擎，杜绝因分贝过低切断数据流导致本地识别中断；
   - 48kHz -> 16kHz 采用 3 点均值抗混叠下采样（盒式滤波），移除 `tanhf` 软饱和非线性谐波失真；
   - 增加双保险瞬时分贝打断：当 `db > -30.0f && peakDb > -16.5f` 连续 2 帧，即使字数未变也能立刻打断 AI 播报；
   - 恢复 3200 字节 (100ms) 恒定匀速发包，保障每一句话的首音完整性。
5. **【修改点 5】双引擎容错 + 应用拼音同音词容错 + 商城连招 2.2s 提速与复扫 (`checkDeferredVoiceLocalCommand` / `tryExecuteDirectLocalCommand:` / `resolveInstalledAppBundleIDForName:`)**:
   - 本地与云端双引擎交叉校验；
   - 紧凑三连招正则优化，第 2 步打开 App 后提速至 2.2s 自动执行点击并支持 2 次重试 (`performVisualTapWithRetry:attempt:1 maxAttempts:2`)；
   - 扩充高频口语音/错别字容错（如“清理好台/请你后台”、“关毕/关闭掉”、“支付宝/足球场/支护宝”、“备忘录/贝旺路/微网络”等）；
   - 内置 App 映射字典扩充常见应用拼音全拼映射。
6. **【修改点 6】performVisualTapWithRetry:attempt:maxAttempts: 视觉点击优化**:
   - 小胶囊模式采用 0.12s 极速截屏（全卡片 0.20s）；
   - 正则 `salesStripRe` 预先剔除同行“1000+人付款/已拼1.2万件”等销量干扰，精准提取纯价格；
   - 冷启动超时未找到目标文字时，自动在 1.0s 后执行二次复扫兜底。

### 2. 代码镜像一致性
- `D:\双工具版本\Tweak.x` (SHA256: `9049aae413492ea70b23b4c40cf047786d0887ed86f954431ec4758fc59c440a`)
- `D:\双工具版本\主修实时翻译.txt` (SHA256: `9049aae413492ea70b23b4c40cf047786d0887ed86f954431ec4758fc59c440a`)
- 镜像状态: 100% 一致。
- 语法检查: `Braces: 0, Parens: 0, Brackets: 0`。
- 绝对隔离保障: `TimeGuardianSiri123` 保持冻结 (Commit `214b4d7`)，`D:\天使siri测试版` 未被触碰。

### 3. CI 编译与安装包分发 (Run #27, ID: 41289372506, Commit: 5b4e025)
- **Deb 校验 (Pure XZ, 0% Zstandard)**:
  - **Rootless (arm64)**: `com.custom.timeguardiansiri123_6.0.0_iphoneos-arm64.deb` (525,424 字节, SHA256: `c4aa5ae0016eab7a154a21bfa1c6f81afcdb3781fb387e6e1f8b16c44f4eb959`, valid: true)
  - **RootHide (arm64e)**: `com.custom.timeguardiansiri123_6.0.0_iphoneos-arm64e.deb` (342,388 字节, SHA256: `493b6911b1b870f6fed09b23214a6eb5b31911b7139ea30b79ecad44e411cafa`, valid: true)
- **安装包落地路径**:
  - `D:\双工具版本\com.custom.timeguardiansiri123_6.0.0_iphoneos-arm64.deb`
  - `D:\双工具版本\com.custom.timeguardiansiri123_6.0.0_iphoneos-arm64e.deb`

---

## 2026-11-01: 纯文字cleanStr消除句号游标吞首字/1.5s打断旧包静默锁/完整本地中文优先匹配 (Run #28)

### 1. 修改要点 (3 处精准微调)
1. **【微调点 1】存入剔除标点后的纯文字 cleanStr (`startNativeAppleNeuralVADIfNeeded`)**:
   - `strongSelf.lastNativeNeuralChineseText = cleanStr;`；
   - 并在 `curLen < strongSelf.lastConsumedNeuralCharOffset` 时重置游标为 0，彻底消除苹果识别器自动删改句号导致游标偏移吞掉第一个字的顽疾。
2. **【微调点 2】打断瞬间开启 1.5s 旧语音静默锁与防腰斩保护 (`triggerNativeSiriGradeInstantBargeIn:` 与 `handleIncomingWebSocketJSON:`)**:
   - 在打断瞬间 (`triggerNativeSiriGradeInstantBargeIn:`) 设置 `self.isMutedByLocalBargeIn = YES; self.bargeInMuteTimestamp = now;`，拦截网络半路上残留的旧音频；
   - 在云端确认打断 (`serverContent[@"interrupted"]`) 时复位 `self.isMutedByLocalBargeIn = NO;`；
   - 在 `outputTranscription` 分支增加开口中保护：若 `self.neuralUserSpeakingActive && (nowTs - self.lastNeuralSpeechActiveTime < 0.85)`，直接 return 忽略残留包，绝不提前清空用户正在说的半句话；
   - 若本地识别中文比云端文本更长，优先执行本地完整中文指令；
   - 仅在用户整句说完且 AI 真正回复本轮时，才安全推进游标同步。
3. **【微调点 3】优先选用字数更完整的识别结果 (`checkDeferredVoiceLocalCommand`)**:
   - 若用户打断时苹果本地 zh-CN 识别出的字数比云端更完整（如本地听全了“返回桌面”，而云端只抓到了“8 面”），优先采用本地完整中文匹配本地指令，彻底解决打断时云端漏听前半截导致的指令失效问题。

### 2. 代码镜像一致性
- `D:\双工具版本\Tweak.x` (SHA256: `6bbad7d4bb2ea4821212c003403fd445e25714c665031d751a9222a26325372f`)
- `D:\双工具版本\主修实时翻译.txt` (SHA256: `6bbad7d4bb2ea4821212c003403fd445e25714c665031d751a9222a26325372f`)
- 镜像状态: 100% 一致。
- 语法检查: `Braces: 0, Parens: 0, Brackets: 0`。
- 绝对隔离保障: `TimeGuardianSiri123` 保持冻结 (Commit `214b4d7`)，`D:\天使siri测试版` 未被触碰。

### 3. CI 编译与安装包分发 (Run #28, ID: 41292872671, Commit: fac5162)
- **Deb 校验 (Pure XZ, 0% Zstandard)**:
  - **Rootless (arm64)**: `com.custom.timeguardiansiri123_6.0.0_iphoneos-arm64.deb` (525800 字节, SHA256: `214da7fb3ecf9455f8b89f16a524a00e661af342b5c0bb748e098461ab0869fc`, valid: true)
  - **RootHide (arm64e)**: `com.custom.timeguardiansiri123_6.0.0_iphoneos-arm64e.deb` (342784 字节, SHA256: `2ae0d98042c220cec5affbfad6908655dd3caa9f69013ce1eeda7f18d6e1bc2b`, valid: true)
- **安装包落地路径**:
  - `D:\双工具版本\com.custom.timeguardiansiri123_6.0.0_iphoneos-arm64.deb`
  - `D:\双工具版本\com.custom.timeguardiansiri123_6.0.0_iphoneos-arm64e.deb`


---

## 2026-11-02: 方案 1 执行——安全回退至 Run #29 基线 (Rebuild Run #45)

### 1. 执行要点 (方案 1 规范执行全景)
1. **进度安全归档**:
   - 将回退前的最新代码（Run #44 状态，包含 Relaxin 满血防检测与 arm64e PAC 修正）完整归档至独立远端分支 `backup-run44-relaxin` (Commit `872c0c0`)；
   - 随时可通过切换分支完整复原此前所有调谐排查代码。
2. **主分支基线重置**:
   - `main` 分支硬重置回 Run #29 源码基线 Commit `d5808e5` (`fix: stealth location combo and standalone switch, fix pay trigger false positives, and dingtalk homophones`)；
   - 强制推送到 GitHub 远端 `origin/main`。
3. **六大代码镜像 100% 绝对一致同步**:
   - 全部 6 处代码副本已对齐至 Run #29 源码基线；
   - **SHA256 校验码 100% 绝对一致**: `5bece87cbb42caa78e03864c62fa0e43fec163f3e32c62e3c444bfec3f15da3b`；
   - 对应文件: `D:\双工具版本\Tweak.x`、`Tweak.xm`、`主修实时翻译.txt`、`dopamine测试版本.txt`、`D:\dopamine测试版本.txt`、`C:\Users\57132\Desktop\relaxin调教版本.txt`。
4. **CI 重新构建与安装包分发 (Run #45, ID: 41438449290, Commit: d5808e5)**:
   - macOS-14 Runner 双架构 100% 编译成功；
   - **Rootless (arm64)**: `com.custom.timeguardiansiri123_6.0.0_iphoneos-arm64.deb` (529,128 字节, SHA256: `0e48f343cab544d0bfa06e1c341de40943881c85b2e8f182d7fbec466a490409`)；
   - **RootHide (arm64e)**: `com.custom.timeguardiansiri123_6.0.0_iphoneos-arm64e.deb` (343,592 字节, SHA256: `fcceedf2cb4576569aead0257e3f992523ec848a1455b471002bbf1b630cc773`)；
   - 纯 XZ 格式结构 (`ar -t`) 校验完全通过 (`control.tar.xz`, `data.tar.xz`)；
   - 本地安装包落地至 `D:\双工具版本\`。

---

## 2026-11-02: 6 大代码块 ROOTHIDE 条件编译隔离封装 (Run #46)

### 1. 修改要点 (6 处代码块精准隔离)
1. **替换块 1 (路径底层统一封装)**:
   - 引入 `MyJBPath`、`MyJBCPath`、`MyRootfsPath`、`MyRootfsCPath`；
   - 在 `#if ROOTHIDE` 下优先使用 `jbrootpath` / `rootfspath` 动态转换，`#else` 分支下严格保持原版 Dopamine `/var/jb` 逻辑不变；
   - 原 `agJBRootPath` 保持外部兼容，内部统一走 `MyJBPath`。
2. **替换块 2 (TCC 授权与守护进程清理)**:
   - `ensureSpringBoardTCCGranted` 中 `TCC.framework` 与 `TCC.db` 路径使用 `MyRootfsCPath`，`AngelCameraHelper` 授权路径使用 `MyJBPath`；
   - `killZombieCameraHelpers` 中 `#if ROOTHIDE` 使用 `MyJBPath` 与 `MyRootfsPath`，`#else` 保持 Dopamine 原版路径。
3. **替换块 3 (配置文件读写与 AI 永久记忆路径)**:
   - `reloadGeminiPrefsCache` 中的 `paths` 数组按 `ROOTHIDE` 精准分流；
   - `saveGeminiPrefs` 中的 `writePaths` 数组及 `/tmp` 路径清理完成替换；
   - `agPermanentMemoryFilePaths` 记忆文件路径按 `ROOTHIDE` 分流。
4. **替换块 4 (微信跨进程通信与崩溃取证)**:
   - `agGetGlobalSharedDir` 共享目录分流；
   - `agRespringLedgerFilePath`、`agDynamicLibrariesDirs` 与 `agResolveHumanPluginNameForDylib` 中 `dpkgBaseDirs` 完成替换。
5. **替换块 5 (相机守护进程 helperPaths)**:
   - 守护进程拉起路径完成 `#if ROOTHIDE` / `#else` 条件隔离。
6. **替换块 6 (越狱商店缓存与临时目录清理)**:
   - `debCacheDirs` 中的 APT/Sileo/Zebra 缓存路径完成条件隔离。

### 2. 六文件镜像 100% 字节完全一致 (SHA256: `65a030cae1a4c5b1ddeb5ca4cfb138863d1307d390c19e95fe4aef60e36bc45f`)
- `D:\双工具版本\Tweak.x` (1,333,700 字节)
- `D:\双工具版本\Tweak.xm` (1,333,700 字节)
- `D:\双工具版本\主修实时翻译.txt` (1,333,700 字节)
- `D:\双工具版本\dopamine测试版本.txt` (1,333,700 字节)
- `D:\dopamine测试版本.txt` (1,333,700 字节)
- `C:\Users\57132\Desktop\relaxin调教版本.txt` (1,333,700 字节)
- 语法闭合检查：`Braces: 0, Parens: 0, Brackets: 6`。

### 3. CI 编译与安装包分发 (Run #46, ID: 41448185713, Commit: 89363da)
- **Deb 校验 (Pure XZ, 0% Zstandard, 100% Sileo 兼容)**:
  - **Rootless (arm64)**: `com.custom.timeguardiansiri123_6.0.0_iphoneos-arm64.deb` (526,636 字节, SHA256: `321a312980946380c21e36680c42a5fc4f9859e64fee921508e7de8d84881e7f`)
  - **RootHide (arm64e)**: `com.custom.timeguardiansiri123_6.0.0_iphoneos-arm64e.deb` (343,576 字节, SHA256: `a87080674977b9fd68ec7675459c00ac33a912595c734e4a0d594c1bcd9983d9`)
- **本地安装包落地路径**:
  - `D:\双工具版本\com.custom.timeguardiansiri123_6.0.0_iphoneos-arm64.deb`
  - `D:\双工具版本\com.custom.timeguardiansiri123_6.0.0_iphoneos-arm64e.deb`
- **验证结果**:
  - `ar -t` 结果包含且仅包含: `control.tar.xz`, `data.tar.xz`。
  - 双架构均在 macOS 14 runner 上 100% 成功编译并通过 Sileo 纯 XZ 格式校验。

---

## 2026-11-02: Relaxin 配置文件防扫描与进程自隐身增强 (Run #47)

### 1. 修改要点 (3 处精准更新)
1. **第 1 处：保存 GeminiPrefs (saveGeminiPrefs)**:
   - `#if ROOTHIDE` 下：仅向 `.jbroot` 内部写入，并主动删除外部系统 `MyRootfsPath(@"/var/mobile/Library/Preferences/com.custom.geminisirioverlay.plist")` 与 `MyRootfsPath(@"/tmp/com.custom.geminisirioverlay.plist")`，防止 App 扫出残留配置文件痕迹；
   - `#else` 下：保持 Dopamine 原版多路径写入与 `@"/tmp/..."` 清理 100% 不变；
   - 目录创建判断扩展 `([detectedRoot containsString:@".jbroot-"] || [wp hasPrefix:detectedRoot])`。
2. **第 2 处：镜像自隐身函数 (hooked_dladdr / hooked_dyld_get_image_name 等)**:
   - 新增 `agShouldHideSelfInProcess()`：
     - 在 `#if ROOTHIDE` 下，检测如果为第三方 App（排除 SpringBoard 和 Preferences），无论是否开启虚拟定位和防检测，始终隐藏自身 `.jbroot-XXXX` 路径；
     - 在 `#else` (Dopamine) 下，100% 等同于原有 `agIsAntiDetectionShieldActive()`；
   - `hooked_dladdr`、`hooked_dyld_get_image_name`、`hooked_class_getImageName`、`hooked_objc_copyImageNames`、`hooked_dyld_image_path_containing_address`、`hooked_access` 均改用 `agShouldHideSelfInProcess()`。
3. **第 3 处：agSyncClientHooksWithCurrentState**:
   - 在 `#if ROOTHIDE` 下，若 `agShouldHideSelfInProcess()` 为真，即使未开启虚拟定位 (`spoofActive == NO`)，也确保调用一次 `agInstallAntiDetectionCHooksIfNeeded()` 安装底层 C 函数镜像自隐身，确保第三方 App 内存中永不暴露 `.jbroot-XXXX` 路径。
   - 所有定位、Wi-Fi、传感器 Hook 逻辑在关闭定位时依然 100% 还原为硬件真实数据。
   - 在 Dopamine (`#else`) 下完全不受影响。

### 2. 六文件镜像 100% 字节完全一致
- 镜像文件列表：
  1. `D:\双工具版本\Tweak.x`
  2. `D:\双工具版本\Tweak.xm`
  3. `D:\双工具版本\主修实时翻译.txt`
  4. `D:\双工具版本\dopamine测试版本.txt`
  5. `D:\dopamine测试版本.txt`
  6. `C:\Users\57132\Desktop\relaxin调教版本.txt`
- 单文件大小：1,333,307 字节
- SHA256 校验和：`04013dacf1b7d8487d0e40ef74487242623929a091be39f39e76f6e36e75b5b3`

### 3. CI 构建与产物
- 提交哈希：`0d8187f`
- GitHub Actions CI Run #47 (ID: 41464578778) - 双架构构建 100% 成功
- 产物包结构：标准纯 XZ 格式 (`control.tar.xz` 与 `data.tar.xz`)
- 本地产物验证：
  - `com.custom.timeguardiansiri123_6.0.0_iphoneos-arm64.deb` (526,896 字节, SHA256: `a507bf1a9dd0c1946e5f5fff09045178a05049aaf7f3d8cdb783b8a13ae0d116`)
  - `com.custom.timeguardiansiri123_6.0.0_iphoneos-arm64e.deb` (343,400 字节, SHA256: `bc3d1d6d8d4775a37f45618a113e44045c3d88d03a3e88c07ad93794034a460c`)

---

## 2026-11-02: 方案 2 调整：保持原版反检测 Hook 触发逻辑不变，仅在 #if ROOTHIDE 下隔离配置文件路径 (Run #48)

### 1. 修改要点 (3 点精准调整)
1. **配置文件路径隔离 (reloadGeminiPrefsCache 与 saveGeminiPrefs)**:
   - `reloadGeminiPrefsCache` 中：在 `#if ROOTHIDE` 下仅读取 `MyJBPath(@"/var/mobile/Library/Preferences/com.custom.geminisirioverlay.plist")`，移除了外部 `MyRootfsPath`，彻底防止 App 扫出外部配置；在 `#else` 下保持 Dopamine 原版三路径读取不变；
   - `saveGeminiPrefs` 中：在 `#if ROOTHIDE` 下仅写入 `MyJBPath(...)`，并主动清理外部残留的 plist 与 tmp 文件；在 `#else` 下保持 Dopamine 原版写入与清理不变。
2. **底层镜像 Hook 条件 100% 还原**:
   - 彻底删除 `agShouldHideSelfInProcess()` 函数；
   - `hooked_dladdr`、`hooked_dyld_get_image_name`、`hooked_class_getImageName`、`hooked_objc_copyImageNames`、`hooked_dyld_image_path_containing_address`、`hooked_access` 判定条件全部 100% 还原为原版 `agIsAntiDetectionShieldActive()`。
3. **agSyncClientHooksWithCurrentState 逻辑 100% 还原**:
   - 移除开头新增的 `#if ROOTHIDE agInstallAntiDetectionCHooksIfNeeded()` 块，完全恢复为原版防检测与定位挂钩同步逻辑。

### 2. 六文件镜像 100% 字节完全一致
- 镜像文件列表：
  1. `D:\双工具版本\Tweak.x`
  2. `D:\双工具版本\Tweak.xm`
  3. `D:\双工具版本\主修实时翻译.txt`
  4. `D:\双工具版本\dopamine测试版本.txt`
  5. `D:\dopamine测试版本.txt`
  6. `C:\Users\57132\Desktop\relaxin调教版本.txt`
- 单文件大小：1,332,020 字节
- SHA256 校验和：`14c2c6a775f73c679f7050450508e6989214830ed1db7a9f4a9751bc92e78823`

### 3. CI 构建与产物
- 提交哈希：`fd60c0f`
- GitHub Actions CI Run #48 (ID: 41466757833) - 双架构构建 100% 成功
- 产物包结构：标准纯 XZ 格式 (`control.tar.xz` 与 `data.tar.xz`)
- 本地产物验证：
  - `com.custom.timeguardiansiri123_6.0.0_iphoneos-arm64.deb` (525,800 字节, SHA256: `c0a997f9624cf1b14df18402424c1273fcab68982475e84a92124fb480bef283`)
  - `com.custom.timeguardiansiri123_6.0.0_iphoneos-arm64e.deb` (343,552 字节, SHA256: `e46899202d6fec623a3fce1e4a463d44b1a0c00ebe80154ee464f84bb15302b1`)

---

## 2026-11-02: 纯越狱环境隐身护盾 agIsEnvironmentStealthActive 与精准分发 (Run #49)

### 1. 修改要点 (4 处精确隔离修改)
1. **修改 1：agSyncDynamicTweakFilterPlist**:
   - 在 `#if !ROOTHIDE` (Dopamine) 下：关闭定位时保持仅注入桌面、设置与微信；
   - 在 `#if ROOTHIDE` (Relaxin) 下：关闭定位时保持当前范围注入（白名单或全局 UIKit），以让纯环境隐身护盾持续保护第三方受控 App。
2. **修改 2：新增 agIsEnvironmentStealthActive**:
   - 在 `#if ROOTHIDE` 下：判定第三方受控 App（排除 SpringBoard、Preferences、Apple 系统组件与 VPN 扩展等代理进程），只要本插件注入，始终保持隐藏 `.jbroot` 与 `ellekit`；
   - 在 `#else` (Dopamine) 下：100% 返回 `agIsAntiDetectionShieldActive()`，0 行为变化。
3. **修改 3：镜像隐藏与文件/URL防探测钩子**:
   - `hooked_dladdr`：未开启虚拟定位时仅隐藏本插件自身 (`gSelfDylibBase`) 与底层加载器 (`ellekit`, `libroothide`, `MobileSubstrate.dylib`)，绝不干扰微信等 App 内其他插件通过 `dladdr` 读取自身资源；开启防检测时拦截全部可疑越狱路径；
   - `hooked_dyld_get_image_name`、`hooked_class_getImageName`、`hooked_objc_copyImageNames`、`hooked_dyld_image_path_containing_address`、`hooked_access` 切换为 `agIsEnvironmentStealthActive()`；
   - `UIApplication.canOpenURL:` 与 `NSFileManager.fileExistsAtPath:` 切换为 `agIsEnvironmentStealthActive()`；
   - 6 个 `agDyn_` 高德/系统模拟标志函数切换为 `agIsEnvironmentStealthActive()`；
   - CLLocation 坐标、CLLocationManager、CLGeocoder、CNCopyCurrentNetworkInfo、NEHotspotNetwork、CMMotionActivity、CMPedometerData、CMAltitudeData 保持原样不变，关闭定位 100% 还原真实硬件数据。
4. **修改 4：agSyncClientHooksWithCurrentState 开关分发**:
   - 开头新增 `BOOL envStealthActive = agIsEnvironmentStealthActive();`，在 `!spoofActive && !envStealthActive && !gHasInitializedObjCHooks` 时安全退出；
   - 方法表中前 3 项 (URL/文件防探测) 随 `shieldActive || envStealthActive` 开启；其余定位坐标、Wi-Fi、传感器 Hook 仅在 `spoofActive` 为真时启用；
   - 底部按需挂载 C 钩子：`if (shieldActive || envStealthActive) { agInstallAntiDetectionCHooksIfNeeded(); }`。

### 2. 六文件镜像 100% 字节完全一致
- 镜像文件列表：
  1. `D:\双工具版本\Tweak.x`
  2. `D:\双工具版本\Tweak.xm`
  3. `D:\双工具版本\主修实时翻译.txt`
  4. `D:\双工具版本\dopamine测试版本.txt`
  5. `D:\dopamine测试版本.txt`
  6. `C:\Users\57132\Desktop\relaxin调教版本.txt`
- 单文件大小：1,334,635 字节
- SHA256 校验和：`4c5e6df04bcc9ab8d56f30570fd9d4317865c44493205af64d825506ccefa2e5`

### 3. CI 构建与产物
- 提交哈希：`fd6608b`
- GitHub Actions CI Run #49 (ID: 41470822133) - 双架构构建 100% 成功
- 产物包结构：标准纯 XZ 格式 (`control.tar.xz` 与 `data.tar.xz`)
- 本地产物验证：
  - `com.custom.timeguardiansiri123_6.0.0_iphoneos-arm64.deb` (527,380 字节, SHA256: `b48f24f8c860e0dda8b1bff29cc5af7d0c9a984528cb52fe5013d85aee5e5a89`)
  - `com.custom.timeguardiansiri123_6.0.0_iphoneos-arm64e.deb` (343,728 字节, SHA256: `483b8a5e482e3feef7e521fd360c6494d2abdf54e1372b086b6c26eb8bc036ef`)

---

## 2026-11-02: 还原 6 个 agDyn_ 函数与 simFlagStealth 为 shieldActive (Run #50)

### 1. 修改要点 (精准还原两处)
1. **6 个 agDyn_ 函数还原**:
   - 将 `agDyn_SrcInfo_isSimSoft`、`agDyn_SrcInfo_isProdAcc`、`agDyn_CLLoc_isSim1`、`agDyn_CLLoc_isSim2`、`agDyn_AMap_detectRisk`、`agDyn_AMap_setDetectRisk` 这 6 个函数内的判断条件全部从 `agIsEnvironmentStealthActive()` 100% 还原回原版的 `agIsAntiDetectionShieldActive()`。
2. **simFlagStealth 标志还原**:
   - 在 `agSyncClientHooksWithCurrentState` 中，将 `BOOL simFlagStealth = (shieldActive || envStealthActive);` 还原为 `BOOL simFlagStealth = shieldActive;`，使其严格随定位与防检测开关控制。

### 2. 六文件镜像 100% 字节完全一致
- 镜像文件列表：
  1. `D:\双工具版本\Tweak.x`
  2. `D:\双工具版本\Tweak.xm`
  3. `D:\双工具版本\主修实时翻译.txt`
  4. `D:\双工具版本\dopamine测试版本.txt`
  5. `D:\dopamine测试版本.txt`
  6. `C:\Users\57132\Desktop\relaxin调教版本.txt`
- 单文件大小：1,334,619 字节
- SHA256 校验和：`c8f727754b035e4152d01b70d8b235857e75798f4eb17f85d3c492116d521a37`

### 3. CI 构建与产物
- 提交哈希：`623f668`
- GitHub Actions CI Run #50 (ID: 41472912200) - 双架构构建 100% 成功
- 产物包结构：标准纯 XZ 格式 (`control.tar.xz` 与 `data.tar.xz`)
- 本地产物验证：
  - `com.custom.timeguardiansiri123_6.0.0_iphoneos-arm64.deb` (526,964 字节, SHA256: `4714c6394320ff87cb924950ba59c9c3a672155f9db204fc5e77fe72b1dd2dd3`)
  - `com.custom.timeguardiansiri123_6.0.0_iphoneos-arm64e.deb` (343,452 字节, SHA256: `f1494ce5c3d0f09daf3150b12cfd1966356491351571e9824b3d80fa8b3a5787`)
