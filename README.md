# qt_day06_connect_panel

基于 Qt6 的网络/串口连接面板程序，支持 TCP Socket 与串口通信。

## 功能特性

- **TCP 客户端** — 连接/断开 TCP 服务器，发送自定义消息
- **串口通信** — 扫描可用串口，配置波特率、数据位、停止位等参数
- **日志系统** — 实时显示通信日志，支持清空与保存到文件
- **设置持久化** — 自动保存/加载用户配置（IP、端口、串口参数等）
- **后台任务** — 通过 Worker 线程处理耗时操作，避免阻塞 UI

## 技术栈

| 模块 | 说明 |
|------|------|
| Qt 6.5+ | Core / Widgets / Network / SerialPort |
| 构建系统 | CMake 3.19+ |
| 编译器 | MinGW 64-bit (GCC) |
| 界面风格 | Fusion |

## 项目结构

```
├── main.cpp          # 应用入口
├── mainwindow.h/cpp  # 主窗口逻辑（TCP、串口、UI交互）
├── mainwindow.ui     # 主窗口界面布局
├── worker.h/cpp      # 后台 Worker 线程
├── logmanager.h/cpp  # 日志管理器
├── CMakeLists.txt    # CMake 构建配置
└── build/            # 构建输出目录
```

## 构建运行

```bash
# 配置
cmake -B build -DCMAKE_PREFIX_PATH=/path/to/Qt/6.x.x/mingw_64

# 编译
cmake --build build

# 运行
./build/qt_day06_connect_panel
```

或在 Qt Creator 中直接打开 `CMakeLists.txt` 构建运行。

## 使用说明

1. **TCP 连接** — 输入 IP 地址和端口，点击"连接"按钮
2. **串口连接** — 在串口选项卡中选择端口和参数，点击"打开串口"
3. **发送数据** — 在输入框中输入内容，点击发送
4. **日志管理** — 使用"清空日志"和"保存日志"按钮管理通信记录

## License

仅供学习使用。
