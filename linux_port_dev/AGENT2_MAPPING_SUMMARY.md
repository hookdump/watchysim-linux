# Agent 2 GDI Mapping Summary

## Task Completed
I have analyzed all GDI/GDI+ drawing functions in the WatchySim codebase and created a comprehensive mapping to Linux graphics frameworks.

## Key Findings

### GDI/GDI+ Functions Used
The codebase uses a relatively small subset of GDI/GDI+ functionality:

1. **Basic Drawing Operations**:
   - `SetPixel()` - Individual pixel drawing
   - `Graphics::DrawLine()` - Line drawing  
   - `Graphics::FillRectangle()` - Rectangle filling
   - Circle/triangle drawing (implemented manually with pixels)

2. **Graphics Context**:
   - `Graphics` class for GDI+ operations
   - `HDC` (device context) for raw GDI
   - Color management with `Color`, `Pen`, `SolidBrush`

3. **Window Management**:
   - Standard Win32 window creation and message loop
   - Timer-based updates (1 second timer, minute-based repaints)
   - Menu-driven interaction

4. **Special Features**:
   - Screenshot saving to GIF
   - Background image loading from resources
   - Bitmap font rendering (pixel-by-pixel)

### Framework Recommendations

Based on my analysis, here's my ranking of Linux frameworks for this port:

#### 1. **SDL2** (RECOMMENDED)
**Pros:**
- Most direct mapping from GDI functions
- `SetPixel()` -> `SDL_RenderDrawPoint()` (1:1 mapping)
- Simple API that matches current architecture
- Built-in timer support
- Good image loading capabilities
- Cross-platform if needed later

**Cons:**
- Need SDL_image for PNG support
- No built-in circle drawing (but current code draws manually anyway)

#### 2. **Cairo/GTK+**
**Pros:**
- More sophisticated graphics capabilities
- Native Linux look and feel
- Built-in circle/arc drawing

**Cons:**
- No direct pixel-setting function (must use 1x1 rectangles)
- More complex API
- Heavier dependency

#### 3. **FLTK**
**Pros:**
- Very simple API
- Lightweight
- Built-in menu system

**Cons:**
- Less common, smaller community
- Limited image format support

### Critical Implementation Notes

1. **Color Format**: Current code uses 16-bit color values (`0xFFFF` for white, `0x0000` for black). These will need conversion for any framework.

2. **Display Offsets**: The display is offset by (241, 198) pixels from window origin. This must be preserved.

3. **Bitmap Font Rendering**: The current implementation draws fonts pixel-by-pixel from bitmap data. This will work identically in any framework.

4. **Timer Logic**: Updates every second but only repaints when seconds == 0 (every minute). This optimization should be maintained.

### Porting Effort Estimate

- **SDL2**: ~2-3 days (straightforward mapping)
- **Cairo/GTK+**: ~3-4 days (more API differences)
- **FLTK**: ~2 days (simple but less documented)

## Files for Reference

- Full mapping document: `/home/ig/Projects/watchysim/linux_port_dev/GDI_FUNCTION_MAPPING.md`
- Key files to modify:
  - `WatchySim.cpp` - Main window and event handling
  - `Watchy.cpp` - All drawing operations
  - `Watchy.h` - Class definitions

## Next Steps for Main Agent

1. Choose SDL2 as the graphics framework
2. Set up Linux build environment with SDL2 development libraries
3. Start with window creation and basic drawing functions
4. Port the display drawing functions in order of complexity
5. Handle resource loading (background image)
6. Implement menu system
7. Test with actual watch faces