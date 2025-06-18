# SCP Classified Site 作弊框架 / SCP Classified Site Cheat Framework

## 项目描述 / Project Description
基于 C++23 的 Unity 游戏作弊框架，通过逆向工程实现游戏功能修改。使用 IL2CPPResolver 进行 Unity 运行时分析，结合 ImGui 实现实时配置界面。  
A Unity game cheat framework based on C++23, enabling game feature modifications through reverse engineering. Utilizes IL2CPPResolver for Unity runtime analysis and ImGui for real-time configuration interface.

## 功能特性 / Features
### 战斗模块 / Combat Modules
- **Aimbot**: 自瞄功能，支持距离/血量/准心距离优先级  
  Auto-aim functionality with distance/health/crosshair distance priorities
- **BulletTrace**: 子弹轨迹可视化  
  Bullet trajectory visualization
- **Weapon**: 武器属性修改  
  Weapon attribute modification

### 移动模块 / Movement Modules
- **AutoJump**: 自动跳跃障碍物  
  Automatic obstacle jumping
- **Speed**: 玩家移动速度修改  
  Player movement speed modification

### 渲染模块 / Render Modules
- **ESP**: 玩家位置透视  
  Player position ESP (Extra Sensory Perception)
- **Chams**: 玩家模型材质替换  
  Player model material replacement (Chams)

## 技术栈 / Technology Stack
- **核心框架**: IL2CPPResolver (Unity 逆向)  
  **Core Framework**: IL2CPPResolver (Unity reverse engineering)
- **GUI 系统**: ImGui 1.91.8  
  **GUI System**: ImGui 1.91.8
- **钩子引擎**: SafetyHook 0.6.5  
  **Hook Engine**: SafetyHook 0.6.5
- **日志系统**: spdlog 1.15.2  
  **Logging System**: spdlog 1.15.2
- **构建系统**: xmake  
  **Build System**: xmake

## 构建指南 / Build Guide
### 本地构建 / Local Build
```bash
# 安装 xmake / Install xmake
scoop install xmake

# 克隆项目 / Clone project
git clone https://github.com/yourname/scp-classified-site-cheat.git
cd scp-classified-site-cheat

# 配置项目 (Windows 64位 调试模式)
# Configure project (Windows x64 Debug mode)
xmake f -p windows -a x64 -m debug -y

# 运行测试 / Run tests
xmake test

# 构建项目 / Build project
xmake
```

### CI/CD 集成 / CI/CD Integration
项目已配置 GitHub Actions CI：  
Project configured with GitHub Actions CI:
```yaml
name: CI
on: [push, pull_request]
jobs:
  build:
    runs-on: Windows-latest
    steps:
      - uses: actions/checkout@v4
      - uses: xmake-io/github-action-setup-xmake@v1
        with:
          xmake-version: '2.9.9'
      - run: xmake f -p windows -a x64 -m debug -y
      - run: xmake test
      - run: xmake
```

## 项目结构 / Project Structure
```
scp-classified-site-cheat/
├── src/                 # 核心源码 / Core source
│   ├── draw/            # GUI 渲染系统 / GUI rendering system
│   ├── module/          # 作弊模块 / Cheat modules
│   │   ├── Combat/      # 战斗功能 / Combat features
│   │   ├── Movement/    # 移动功能 / Movement features
│   │   └── Render/      # 渲染功能 / Render features
│   ├── u3d/             # Unity 游戏对象封装 / Unity game object wrappers
│   └── utils/           # 工具库 / Utility library
├── src-test/            # 单元测试 / Unit tests
├── xmake.lua            # 构建配置 / Build configuration
└── LICENSE              # 项目许可证 / Project license
```

## 免责声明 / Disclaimer
⚠️ **重要提示** ⚠️  
本项目仅用于教育目的，展示游戏逆向工程和安全研究技术。实际在游戏中使用可能违反《SCP Classified Site》用户协议，并导致账号封禁。开发者不对滥用此项目造成的任何后果负责。详细信息请参阅 [LICENSE](LICENSE) 文件。  
This project is for educational purposes only, demonstrating game reverse engineering and security research techniques. Actual use in the game may violate the SCP Classified Site User Agreement and result in account bans. The developers are not responsible for any consequences of misuse. See [LICENSE](LICENSE) for details.