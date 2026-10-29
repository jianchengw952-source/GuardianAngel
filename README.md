# 守护天使 (GuardianAngel)

基于 iOS 15.0 ~ 16.5+ 的全功能越狱助手与系统级智能交互插件。

## 核心特性
- **📍 虚拟定位与防检测**：支持步频/车速拟人位移、真实卫星微抖动、基站/Wi-Fi 硬件伪装与 App 单独生效隔离。
- **🎙️ 实时语音与全双工对讲**：毫秒级响应、超自然语音对话、低延迟音频缓冲。
- **📹 语音视觉与屏幕看图**：实时摄像头镜头画面理解与屏幕多模态解析。
- **💬 纯文本对话**：全兼容中转与本地大模型交互。
- **🌐 屏幕翻译与听译**：逐框原位跟随翻译与零音损实时听译。
- **🕵️‍♂️ 四级深度崩溃取证**：底层内核开机时间直读、白苹果重启与桌面崩溃多维归因、准确真凶 `.dylib` 与 Sileo 插件名定位。

## 架构兼容
- **Dopamine (Rootless)**: `iphoneos-arm64` (安装于 `/var/jb`)
- **Relaxin (RootHide)**: `iphoneos-arm64e` (安装于 `/var/containers/Bundle/Application/.jbroot-*`)
- 严格遵循 Sileo 标准 pure `.xz` 格式封装（`control.tar.xz` + `data.tar.xz`）。
