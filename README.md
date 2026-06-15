# Majin

![Language](https://img.shields.io/badge/Language-C%2B%2B23-00599C?logo=cplusplus)
![Graphics](https://img.shields.io/badge/Graphics-Vulkan-AC162C?logo=vulkan)
![Build](https://img.shields.io/badge/Build-CMake-064F8C?logo=cmake)
![Status](https://img.shields.io/badge/Status-In%20Development-yellow)

<br />
<div align="center">
  <p align="center">
    A modern 3D game engine built with Vulkan for high-performance rendering.
    <br />
    Designed for indie game development with a focus on multiplayer social deduction experiences.
  </p>
</div>

<br/>
<div align="center">
  <img src="https://github.com/Eduardo79Silva/Majin/blob/master/images/hello-triangle.png?raw=true" alt="Game Engine Demo" width="70%">
</div>

---

**A lightweight, performance-focused game engine built on Vulkan and modern C++.**

Majin provides the foundational systems needed for 3D game development including rendering pipeline management, model loading, and real-time graphics. The architecture separates engine logic from game code, enabling rapid iteration and experimentation.

## Quick Start

```bash
git clone --recursive https://github.com/Eduardo79Silva/Majin.git
cd Majin
mkdir build && cd build
cmake ..
cmake --build .
./app
```

## Why Majin

Building a custom engine provides complete control over the rendering pipeline and game systems. Majin focuses on performance and flexibility, using Vulkan for low-level GPU access and modern C++ for clean, maintainable code. The modular architecture allows adding custom systems without framework constraints.

## Features

**Rendering Pipeline**
- Vulkan-based rendering with command buffer management
- Dynamic pipeline configuration and shader loading
- Swap chain with double buffering for smooth presentation
- Window resize handling and frame synchronization

**Core Systems**
- Vertex buffer management with custom vertex layouts
- Device abstraction layer for Vulkan operations
- GLFW integration for cross-platform window management
- Shader compilation pipeline with GLSL support

**Architecture**
- Separation between engine and application code
- Resource management with RAII patterns
- Modular component design for extensibility

## Getting Started

### Prerequisites

- CMake 3.20 or higher
- C++23 compatible compiler (GCC 11+, Clang 14+, MSVC 2022+)
- [Vulkan SDK](https://vulkan.lunarg.com/sdk/home)
- Git with submodule support

### Building

Clone the repository with submodules:

```bash
git clone --recursive https://github.com/yourusername/Majin.git
cd Majin
```

Generate build files:

```bash
mkdir build
cd build
cmake ..
```

Build the project:

```bash
cmake --build .
```

Run the application:

```bash
./app
```

## Technology Stack

- C++23 with modern STL features
- Vulkan API for graphics
- GLFW for windowing and input
- GLM for vector and matrix math
- CMake for build configuration
- GLSL for shader development

## Contributing

Contributions are welcome. When submitting pull requests:

- Follow the existing code style
- Test changes with the sample application
- Update documentation for new features
- Keep commits focused and descriptive

## Acknowledgments

Built with:
- [GLFW](https://github.com/glfw/glfw) - Window and input management
- [GLM](https://github.com/g-truc/glm) - Mathematics library
- Vulkan SDK - Graphics API

## Connect

<div align="center">

[![Portfolio](https://img.shields.io/badge/Portfolio-000000?style=for-the-badge&logo=About.me&logoColor=white)](https://eduardo79silva.github.io/)
&nbsp;&nbsp;
[![YouTube](https://img.shields.io/badge/YouTube-FF0000?style=for-the-badge&logo=youtube&logoColor=white)](https://www.youtube.com/@eduardo-silva-79)
&nbsp;&nbsp;
[![LinkedIn](https://img.shields.io/badge/LinkedIn-0077B5?style=for-the-badge&logo=linkedin&logoColor=white)](https://www.linkedin.com/in/eduardosilva)

</div>
