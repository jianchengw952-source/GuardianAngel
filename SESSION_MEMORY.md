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
