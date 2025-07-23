# WatchySim Linux Port Development Plan

## Overview
Porting WatchySim from Windows (GDI+) to Linux (SDL2) while maintaining compatibility with existing watch faces.

## Current Status
- ✅ Created basic SDL2 framework (WatchySim_SDL2.cpp)
- ✅ Window creation with proper size (676x676)
- ✅ Display area with correct offsets (241, 198)
- ✅ Timer system (1-second updates)
- ⏳ Graphics context adapter needed
- ⏳ Menu system (Dear ImGui)
- ⏳ Background image loading
- ⏳ Font rendering system
- ⏳ Build system (CMake/Makefile)

## Architecture Changes

### Windows Version
- Uses GDI+ for graphics
- Windows message loop
- Resource files for background
- WinMain entry point
- Native Windows menus

### Linux Version  
- SDL2 for graphics and windowing
- SDL2 event loop
- File-based resources
- Standard main() entry point
- Dear ImGui for menus (planned)

## Major Tasks

### 1. Graphics Adapter Layer (CRITICAL)
Create adapter classes to translate GDI+ calls to SDL2:
- [ ] Graphics class adapter (maps GDI+ Graphics to SDL2)
- [ ] HDC adapter (device context mapping)
- [ ] Color conversion (GDI+ to SDL2 pixel format)
- [ ] Coordinate system alignment

### 2. DisplaySim Implementation
Port all DisplaySim methods to use SDL2:
- [ ] fillScreen()
- [ ] drawBitmap()
- [ ] setTextColor()
- [ ] setFont() and font rendering
- [ ] print/println methods
- [ ] getTextBounds()
- [ ] Drawing primitives (pixel, line, rect, etc.)

### 3. Font System
- [ ] Port GFX font format support
- [ ] Implement font rendering with SDL2_ttf or custom renderer
- [ ] Support for built-in fonts (FreeMonoBold9pt7b, DSEG7_Classic_Bold_53)

### 4. Resource Loading
- [ ] Load background.png from file instead of Windows resources
- [ ] Create resource directory structure
- [ ] Implement cross-platform resource loading

### 5. Menu System with Dear ImGui
- [ ] Integrate Dear ImGui with SDL2
- [ ] Recreate menu structure:
  - Time menu (Current, Short, Long, Custom)
  - Battery menu (Dead, Low, Medium, High, Max)
  - Bluetooth (On/Off)
  - WiFi (On/Off)
  - Steps (None, Lazy, Regular, Athlete)
  - Weather conditions
  - Temperature settings
  - Tools (Screenshot)

### 6. Build System
- [ ] Create CMakeLists.txt
- [ ] Handle dependencies (SDL2, SDL2_image, SDL2_ttf, Dear ImGui)
- [ ] Support for different watch faces
- [ ] Debug/Release configurations

### 7. Platform Abstraction
- [ ] Create platform-specific headers
- [ ] Use #ifdef for platform-specific code
- [ ] Maintain single codebase for both platforms (stretch goal)

## Implementation Order
1. Graphics adapter layer (blocking everything else)
2. Basic DisplaySim implementation
3. Font rendering
4. Test with 7_SEG watch face
5. Background image loading
6. Menu system with Dear ImGui
7. Full DisplaySim implementation
8. Test with other watch faces
9. Build system
10. Platform abstraction for dual-platform support

## Testing Checkpoints
- [ ] Window appears with correct size
- [ ] Display area renders at correct position
- [ ] Timer updates work
- [ ] Simple shapes can be drawn
- [ ] Text can be rendered
- [ ] 7_SEG watch face displays time
- [ ] Menu system works
- [ ] All watch faces work correctly

## Known Challenges
1. **Font Rendering**: GDI+ has built-in font support, SDL2 requires SDL2_ttf or custom implementation
2. **Bitmap Format**: Watch faces use specific bitmap formats that need conversion
3. **Color Model**: GDI+ uses different color representation than SDL2
4. **Coordinate System**: Ensure pixel-perfect alignment with Windows version
5. **Menu Integration**: Dear ImGui has different paradigm than Windows native menus

## Resources Needed
- background.png (extract from Windows resources)
- SDL2 development libraries
- SDL2_image for PNG loading
- SDL2_ttf for font rendering (optional)
- Dear ImGui source

## Notes
- Keep Windows #ifdef guards to potentially maintain cross-platform code
- Test each component thoroughly before moving to the next
- Document any deviations from Windows behavior
- Consider creating unit tests for critical components