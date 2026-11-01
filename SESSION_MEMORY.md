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
