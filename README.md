# ConnectPanel

基于 Qt6 的跨平台网络通信调试面板，集成 TCP Socket 与串口通信功能。

A cross-platform network communication panel built with Qt6, featuring TCP Socket and Serial Port communication.

[![Qt](https://img.shields.io/badge/Qt-6.5%2B-brightgreen?logo=qt)](https://www.qt.io/)
[![License](https://img.shields.io/badge/license-Educational-blue)](./LICENSE)

## 目录 | Table of Contents

- [功能特性 | Features](#功能特性--features)
- [项目结构 | Project Structure](#项目结构--project-structure)
- [技术栈 | Tech Stack](#技术栈--tech-stack)
- [构建运行 | Build & Run](#构建运行--build--run)
- [使用说明 | Usage](#使用说明--usage)
- [架构设计 | Architecture](#架构设计--architecture)
- [License](#license)

---

## 功能特性 | Features

- **TCP 客户端** — 支持自定义 IP/端口连接，发送文本消息，实时接收并显示服务器响应
- **串口通信** — 自动扫描系统可用串口，支持波特率、数据位、停止位、校验位等参数配置
- **日志系统** — 带时间戳的通信日志，支持清空与导出为 `.txt` 文件
- **设置持久化** — 使用 `QSettings` 自动保存/恢复用户配置（IP、端口、串口参数等）
- **后台线程** — 通过 `QThread` + Worker 模式处理耗时任务，保持 UI 流畅
- **Fusion 风格** — 跨平台一致的界面外观

---

## 项目结构 | Project Structure

```
ConnectPanel/
├── main.cpp                    # 应用入口，设置 Fusion 风格
├── mainwindow.h                # 主窗口声明
├── mainwindow.cpp              # 主窗口逻辑（TCP/串口交互、UI状态管理）
├── mainwindow.ui               # Qt Designer 界面布局文件
├── worker.h / worker.cpp       # 后台 Worker 线程
├── logmanager.h / .cpp         # 日志管理器（添加、清空、导出）
├── tcpclientmanager.h / .cpp   # TCP 客户端管理器（连接、收发、断线处理）
├── serialportmanager.h / .cpp  # 串口管理器（扫描、打开、关闭、收发）
├── settingsmanager.h / .cpp    # 设置持久化管理器
├── CMakeLists.txt              # CMake 构建配置
├── .gitignore
└── README.md
```

---

## 技术栈 | Tech Stack

| 模块 | 说明 |
|------|------|
| **Qt 6.5+** | Core / Widgets / Network / SerialPort |
| **构建系统** | CMake 3.19+ |
| **编译器** | MinGW 64-bit (GCC) / MSVC / Clang |
| **界面风格** | Fusion Style |
| **平台** | Windows / Linux / macOS |

---

## 构建运行 | Build & Run

### 前置要求 | Prerequisites

- Qt 6.5 或更高版本（确保包含 `Network` 和 `SerialPort` 模块）
- CMake 3.19+
- 支持 C++17 的编译器

### 命令行构建 | Command Line

```bash
# 配置（请将路径替换为你的 Qt 安装路径）
cmake -B build -DCMAKE_PREFIX_PATH=/path/to/Qt/6.x.x/mingw_64 -G "MinGW Makefiles"

# 编译
cmake --build build

# 运行
./build/ConnectPanel
```

### Qt Creator

1. 打开 Qt Creator
2. **File → Open File or Project…** → 选择 `CMakeLists.txt`
3. 选择对应的 Kit（如 Qt 6.5 MinGW 64-bit）
4. 点击 **Run** (Ctrl+R)

---

## 使用说明 | Usage

### TCP 客户端

1. 在 **TCP Connection** 面板输入目标 IP 地址和端口号
2. 点击 **Connect** 建立连接
3. 在消息输入框输入文本，点击 **Send** 发送
4. 通信日志实时显示在右侧日志区域
5. 点击 **Disconnect** 断开连接

### 串口通信

1. 切换到 **Serial Port** 选项卡
2. 点击 **Scan** 扫描可用串口
3. 在下拉菜单中选择串口，配置波特率、数据位、停止位、校验位
4. 点击 **Open** 打开串口
5. 发送/接收数据
6. 点击 **Close** 关闭串口

### 日志管理

- **Clear Log** — 清空当前日志
- **Save Log** — 将日志导出为 `.txt` 文件

---

## 架构设计 | Architecture

```
┌─────────────────────────────────────────┐
│               MainWindow                 │
│  ┌──────────┐  ┌─────────────────────┐  │
│  │   UI     │  │     Managers        │  │
│  │ (.ui)    │  │                     │  │
│  │          │  │  TcpClientManager   │  │
│  │  TCP    │  │  SerialPortManager  │  │
│  │  Serial │  │  LogManager         │  │
│  │  Log    │  │  SettingsManager    │  │
│  └──────────┘  └─────────────────────┘  │
│                     │                    │
│              ┌──────▼──────┐             │
│              │ Worker Thread│             │
│              └──────────────┘             │
└─────────────────────────────────────────┘
```

- **MainWindow** — 负责 UI 交互与信号/槽调度
- **Managers** — 各自封装独立的通信/日志/设置逻辑，遵循单一职责
- **Worker Thread** — 后台耗时操作，通过信号槽与主线程通信

---

## License

本项目仅供学习参考使用。

This project is for educational purposes only.
