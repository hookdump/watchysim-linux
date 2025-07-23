# Agent 2: Core Window/Drawing Port Task

## Objective
Port the core window management and drawing system from Windows GDI+ to the selected Linux framework.

## Key Files to Modify
- `WatchySim.cpp` - Main window and event loop
- `Watchy.cpp` - Drawing implementation
- `arduino/Common.cpp` - Any platform-specific utilities

## Core Functions to Port

### Window Management
1. **InitInstance()** - Create main window
2. **WndProc()** - Event handling to framework events
3. **Timer handling** - Update watch face every second
4. **Window sizing** - Maintain proper display area

### Drawing Primitives (DisplaySim class)
1. **setPixel(x, y, color)**
   - Current: GDI SetPixel
   - Port to: Framework pixel drawing

2. **drawLine(x0, y0, x1, y1, color)**
   - Current: GDI+ DrawLine
   - Port to: Framework line drawing

3. **fillRect(x, y, w, h, color)**
   - Current: GDI+ FillRectangle
   - Port to: Framework rectangle fill

4. **drawBitmap(bitmap, x, y, w, h)**
   - Current: GDI+ DrawImage
   - Port to: Framework bitmap rendering

### Display Management
1. **Display buffer** - 200x200 pixel array
2. **Refresh mechanism** - Efficient redraw
3. **Coordinate system** - Maintain same offsets
4. **Color format** - Ensure compatibility

## Implementation Strategy
1. Create abstraction layer for drawing operations
2. Maintain same public API for Watchy.cpp
3. Use preprocessor directives for platform code
4. Test with simplest watch face first (Basic)

## Critical Details
- Display is 200x200 pixels
- Current offsets: X=50, Y=100 (may need adjustment)
- Color is 16-bit (GxEPD library expects this)
- Font rendering uses bitmap operations

## Testing Checklist
- [ ] Window appears with correct size
- [ ] Background image loads and displays
- [ ] Basic watch face renders
- [ ] Time updates every second
- [ ] All drawing primitives work
- [ ] Colors render correctly
- [ ] No flicker or artifacts

## Potential Challenges
- Color format differences between platforms
- Font rendering accuracy
- Performance with pixel-by-pixel operations
- Timer precision for updates