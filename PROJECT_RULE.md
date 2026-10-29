# 守护天使 (GuardianAngel) 独立开发约束准则

## 1. 独立目录与独立仓库隔离规范
- **本项目的唯一 GitHub 远程仓库**：`https://github.com/jianchengw952-source/GuardianAngel.git`
- **本项目的唯一本地工作区目录**：`D:\双工具版本\`
- **工作区绝对隔离红线**：
  - 严禁触碰或修改 `TimeGuardianSiri123` 仓库！
  - 严禁触碰或覆盖 `D:\天使siri测试版\` 目录！
  - 严禁触碰或覆盖 `D:\主修实时翻译.txt`、`D:\主修实时翻译_2.txt`、`E:\主修实时翻译.txt`（这些为 TimeGuardianSiri123 的历史专用文件）！
  - 后续所有对【守护天使】(GuardianAngel) 的代码修改、编译、TXT 备份与 deb 安装包更新，**只更新到 `D:\双工具版本\`** 及远程 `GuardianAngel` 仓库！

## 2. 当前版本状态 (2026-10-29 Run #4)
- **核心特性**：
  - 升级 `agJBRootPath` 使用 `dladdr` 内存级精准自定位，提取 `.jbroot-XXXX` 或 `/var/jb`，0 依赖外部库，100% 穿透微信/钉钉沙盒；
  - 引入内部配置读取线程白名单标记 `sIsInternalPrefsFileRead`，放行 `%hook NSFileManager`；
  - `%ctor` 构造函数启动即预先锁定真实越狱路径。
- **最新 deb 产物（保存在 `D:\双工具版本\`）**：
  - Rootless (arm64): `com.custom.timeguardiansiri123_6.0.0_iphoneos-arm64.deb` (`A6672F326D792665CBCFF3D7FC6C6B770D55DB58225260197A8668043D44AE7B`)
  - RootHide (arm64e): `com.custom.timeguardiansiri123_6.0.0_iphoneos-arm64e.deb` (`E23C8909D05CBD5853C5C7DCECDDFB0962B452B44C0C2D51ABC95BF48B61795C`)
