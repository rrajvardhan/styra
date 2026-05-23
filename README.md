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
cd tafel
mkdir build && cd build
cmake ..
make
./main

```

> If CMake can’t find a dependency, you may need to manually specify include/library paths or adjust your environment variables.

### Windows (Untested, but expected to work.)

You’ll need the following dependencies installed:

* Qt 6 (Widgets and Qt Creator)
* A C++17 compatible compiler
* CMake
* MinGW
* Git

> Follow the **standard Qt installation process** (Qt Creator + MinGW toolchain).

```bash
git clone https://github.com/rrajvardhan/styra

```

1. Open **Qt Creator**
2. Go to **File → Open File or Project**
3. Select `CMakeLists.txt` in the `styra/` directory
4. Choose **Desktop Qt 6.x MinGW 64-bit**
5. Click **Configure Project**
6. Press **Ctrl + B** to build
7. Press **Ctrl + R** to run

> To bundle the application, use Qt’s standard deployment tool `windeployqt`.
