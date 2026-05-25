# Styra

Video wallpaper application for Wayland. Written in C++ using Qt and libmpv.

## Overview

- Video wallpaper application written in C++
- Built using Qt and libmpv
- Designed for Wayland compositors
- System tray integration
- Minimal desktop-friendly UI

## A Quick Look


## Build Instructions

### Linux

You’ll need the following dependencies installed:

* Qt 6 (Widgets and OpenGL module)
* CMake
* A C++17 compatible compiler
* Make / Ninja (or equivalent build tool)

Install Qt and build tools using your package manager (`pacman`, `apt`, `dnf`, etc).

> Package names may vary by distro. On some systems, Qt is split across multiple packages.

```bash
sudo pacman -S qt6-base qt6-tools cmake make
```

Then clone, build, and run:

```bash

git clone https://github.com/rrajvardhan/styra
cd styra
mkdir build && cd build
cmake ..
make
./main

```

> If CMake can’t find a dependency, you may need to manually specify include/library paths or adjust your environment variables.
