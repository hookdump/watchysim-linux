# Agent 4: Build System & Testing Task

## Objective
Create a Linux build system and ensure all watch faces compile and run correctly.

## Current Windows Build
- Visual Studio solution (WatchySim.sln)
- Visual Studio project (WatchySim.vcxproj)
- Compiler: MSVC
- Dependencies: Windows SDK, GDI+

## Target Linux Build
- Build System: CMake (preferred) or Makefile
- Compiler: g++ or clang++
- Dependencies: Selected framework + libraries

## Build Configuration Requirements

### 1. Source Files
**Core Files:**
- WatchySim.cpp (modified for Linux)
- Watchy.cpp (modified drawing backend)
- arduino/*.cpp (String, Common, dtostrf)
- Selected watch face files

**Headers:**
- All .h files in main directory
- arduino/*.h files
- Framework headers

### 2. Compiler Flags
- C++11 or later (check Arduino compatibility)
- Optimization: -O2 for release
- Debugging: -g for development
- Warnings: -Wall -Wextra

### 3. Defines
- Platform detection macros
- Display constants
- Feature flags

### 4. Dependencies
Based on selected framework:
- SDL2: pkg-config sdl2
- GTK+: pkg-config gtk+-3.0
- FLTK: fltk-config
- Additional: image libraries, etc.

## CMakeLists.txt Structure
```cmake
cmake_minimum_required(VERSION 3.10)
project(WatchySim)

# Framework detection
# Source file lists
# Compiler options
# Target definition
# Dependency linking
# Install rules
```

## Testing Strategy

### 1. Build Test Matrix
- [ ] Ubuntu 20.04/22.04
- [ ] Debian 11/12
- [ ] Fedora latest
- [ ] Arch Linux

### 2. Watch Face Tests
Test each watch face for:
- Compilation without errors
- Proper rendering
- Time updates
- No memory leaks
- Performance (smooth updates)

### 3. Feature Tests
- [ ] All menu functions work
- [ ] Screenshot captures correctly
- [ ] Window resizing (if supported)
- [ ] Clean exit

### 4. Integration Tests
- [ ] Can switch between watch faces
- [ ] Settings persist during session
- [ ] No resource leaks

## Documentation Requirements
1. **README_LINUX.md** - Build instructions
2. **Dependencies list** - What to install
3. **Troubleshooting** - Common issues
4. **Development guide** - How to add watch faces

## Continuous Integration
Consider GitHub Actions for:
- Build validation
- Multiple distro testing
- Automated screenshots

## Deliverables
1. Working CMakeLists.txt
2. Build instructions
3. Test results for all watch faces
4. Performance comparison vs Windows
5. Known issues list