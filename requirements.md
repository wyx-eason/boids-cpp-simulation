# 项目需求文档：C++ Boids 集群模拟器 (工程化终极版)

## 1. 项目概述
本项目旨在开发一个基于 Boids 算法（鸟群模拟）的 2D 交互式图形程序。
**核心目标**：不依赖庞大的游戏引擎，从零搭建现代 C++ 工程，重点练习 RAII 内存管理、多文件架构解耦，以及面向 CPU 缓存友好的数据布局设计。

## 2. 技术栈与环境限制
* **语言标准**：C++17。
* **开发环境**：Windows + VS Code。
* **构建工具**：CMake。
* **图形库**：Raylib（通过 CMake 的 `FetchContent` 自动下载并静态链接）。
* **编码规范**：
    * 绝对禁止使用原始指针（raw pointers）进行所有权管理。
    * 绝对禁止手动调用 `new` 和 `delete`。
    * 资源管理必须遵循 RAII 原则。
    * 头文件必须使用 `#pragma once` 防止重复包含。

---

## 3. 开发与学习计划

### Day 1 (Phase 1)：环境搭建与基础渲染 (✅ 已完成)
跑通 CMake 环境，弹出一个可交互的 Raylib 窗口，实现基础的 Game Loop。

### Day 2 (Phase 2)：Boids 核心算法实现 (✅ 已完成)
实现 Boids 的三条核心规则（分离、对齐、凝聚），已完成视觉参数调优（防震颤、平方反比距离、150只鸟、1280x720窗口分辨率）。

### Day 3 (Phase 3)：工程架构重构与内存优化 (🚀 当前目标)
**目标**：将所有代码从单文件拆分为多文件模块，优化内存布局。
* **AI 任务**：
    1.  建立清晰的目录结构：`src/math`, `src/core`, `src/main.cpp`。
    2.  确保 `Flock` 内部使用 `std::vector<Boid>`（扁平化数据结构）以提高 CPU Cache 命中率，而不是 `vector<unique_ptr<Boid>>`。
    3.  解耦物理更新逻辑（Update）与图形渲染逻辑（Draw）。
    4.  更新 `CMakeLists.txt` 以正确编译和链接所有拆分后的源文件。
* **Day 3 学习与校验资源推荐**（防止 AI 拆分文件时引入编译错误）：
    * *C++ 头文件保护*：[LearnCpp - Header guards](https://www.learncpp.com/cpp-tutorial/header-guards/)。
    * *数据局部性*：[Game Programming Patterns - Data Locality](https://gameprogrammingpatterns.com/data-locality.html)。理解为什么要用扁平数组代替指针数组。