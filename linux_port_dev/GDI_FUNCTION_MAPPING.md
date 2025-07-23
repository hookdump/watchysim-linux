# GDI to Linux Graphics Framework Function Mapping

This document provides a detailed mapping of all GDI/GDI+ functions used in WatchySim to their equivalents in various Linux graphics frameworks.

## Summary of GDI/GDI+ Usage in WatchySim

The WatchySim application uses GDI+ for:
1. Window creation and management (Win32 API)
2. Graphics context management
3. Basic drawing operations (pixels, lines, rectangles, circles, triangles)
4. Bitmap/image handling
5. Text rendering (through bitmap fonts)

## GDI/GDI+ Functions Used

### Core GDI+ Classes and Functions

| GDI/GDI+ Function | Purpose | File Location |
|-------------------|---------|---------------|
| `Graphics` class | Main drawing context | WatchySim.cpp, Watchy.h |
| `Graphics::DrawImage()` | Draw background image | WatchySim.cpp:69 |
| `Graphics::FillRectangle()` | Fill rectangle with color | Watchy.cpp:206 |
| `Graphics::DrawLine()` | Draw line | Watchy.cpp:420 |
| `Image` class | Image loading from stream | WatchySim.cpp |
| `Bitmap` class | Bitmap creation for screenshots | WatchySim.cpp:550 |
| `Color` class | Color representation | Watchy.cpp |
| `Pen` class | Line drawing pen | Watchy.cpp:418 |
| `SolidBrush` class | Solid color brush | Watchy.cpp:204 |

### Raw GDI Functions

| GDI Function | Purpose | File Location |
|--------------|---------|---------------|
| `SetPixel()` | Draw individual pixel | Watchy.cpp:193, 407 |
| `HDC` | Device context handle | Throughout |
| `BeginPaint()/EndPaint()` | Paint handling | WatchySim.cpp |
| `BitBlt()` | Bitmap block transfer | WatchySim.cpp:545 |
| `CreateCompatibleDC()` | Create memory DC | WatchySim.cpp:542 |
| `CreateCompatibleBitmap()` | Create bitmap | WatchySim.cpp:543 |
| `SelectObject()` | Select GDI object | WatchySim.cpp:544 |
| `GetDC()/ReleaseDC()` | Get/release DC | WatchySim.cpp |

### Win32 Window Management

| Win32 Function | Purpose |
|----------------|---------|
| `CreateWindow()` | Create main window |
| `RegisterClass()` | Register window class |
| `ShowWindow()` | Show window |
| `UpdateWindow()` | Update window |
| `InvalidateRect()` | Request repaint |
| `SetTimer()` | Set update timer |
| `PostMessage()` | Post window message |
| `GetMessage()/DispatchMessage()` | Message loop |

## Function Mapping to Linux Frameworks

### SDL2 Equivalents

| GDI/GDI+ Function | SDL2 Equivalent | Notes |
|-------------------|-----------------|-------|
| `Graphics` context | `SDL_Renderer*` | Main rendering context |
| `SetPixel()` | `SDL_RenderDrawPoint()` | Single pixel drawing |
| `Graphics::DrawLine()` | `SDL_RenderDrawLine()` | Line drawing |
| `Graphics::FillRectangle()` | `SDL_RenderFillRect()` | Filled rectangle |
| `Graphics::DrawImage()` | `SDL_RenderCopy()` | Image/texture rendering |
| `Pen/Color` | `SDL_SetRenderDrawColor()` | Set drawing color |
| `Image` loading | `SDL_LoadBMP()/IMG_Load()` | Load images (SDL_image) |
| `CreateWindow()` | `SDL_CreateWindow()` | Window creation |
| `BeginPaint()/EndPaint()` | `SDL_RenderPresent()` | Present rendered frame |
| `SetTimer()` | `SDL_AddTimer()` | Timer functionality |
| Message loop | `SDL_PollEvent()` | Event handling |
| `BitBlt()` | `SDL_RenderCopy()` | Copy texture regions |

### GTK+/Cairo Equivalents

| GDI/GDI+ Function | GTK+/Cairo Equivalent | Notes |
|-------------------|----------------------|-------|
| `Graphics` context | `cairo_t*` | Cairo drawing context |
| `SetPixel()` | `cairo_rectangle()` + `cairo_fill()` | 1x1 rectangle |
| `Graphics::DrawLine()` | `cairo_move_to()` + `cairo_line_to()` + `cairo_stroke()` | Line drawing |
| `Graphics::FillRectangle()` | `cairo_rectangle()` + `cairo_fill()` | Filled rectangle |
| `Graphics::DrawImage()` | `cairo_set_source_surface()` + `cairo_paint()` | Image drawing |
| `Pen/Color` | `cairo_set_source_rgb()` | Set drawing color |
| `SolidBrush` | `cairo_set_source_rgb()` + `cairo_fill()` | Solid fill |
| `Image` loading | `gdk_pixbuf_new_from_file()` | Load images |
| `CreateWindow()` | `gtk_window_new()` | Window creation |
| Paint handling | `draw` signal callback | Handle drawing |
| `SetTimer()` | `g_timeout_add()` | Timer functionality |
| Message loop | `gtk_main()` | Main event loop |

### FLTK Equivalents

| GDI/GDI+ Function | FLTK Equivalent | Notes |
|-------------------|-----------------|-------|
| `Graphics` context | Drawing functions in `draw()` | Override draw() method |
| `SetPixel()` | `fl_point()` | Single pixel |
| `Graphics::DrawLine()` | `fl_line()` | Line drawing |
| `Graphics::FillRectangle()` | `fl_rectf()` | Filled rectangle |
| `Graphics::DrawImage()` | `Fl_Image::draw()` | Image drawing |
| `Pen/Color` | `fl_color()` | Set drawing color |
| `Image` loading | `Fl_PNG_Image`, `Fl_JPEG_Image` | Load images |
| `CreateWindow()` | `Fl_Window` constructor | Window creation |
| Paint handling | Override `draw()` method | Handle drawing |
| `SetTimer()` | `Fl::add_timeout()` | Timer functionality |
| Message loop | `Fl::run()` | Main event loop |

## Drawing Primitives Implementation Details

### Pixel Drawing
- **GDI**: `SetPixel(HDC, x, y, color)`
- **SDL2**: `SDL_RenderDrawPoint(renderer, x, y)`
- **Cairo**: Small rectangle: `cairo_rectangle(cr, x, y, 1, 1); cairo_fill(cr)`
- **FLTK**: `fl_point(x, y)`

### Line Drawing
- **GDI+**: `Graphics::DrawLine(pen, x1, y1, x2, y2)`
- **SDL2**: `SDL_RenderDrawLine(renderer, x1, y1, x2, y2)`
- **Cairo**: `cairo_move_to(cr, x1, y1); cairo_line_to(cr, x2, y2); cairo_stroke(cr)`
- **FLTK**: `fl_line(x1, y1, x2, y2)`

### Rectangle Drawing
- **GDI+**: `Graphics::FillRectangle(brush, x, y, w, h)`
- **SDL2**: `SDL_Rect rect = {x, y, w, h}; SDL_RenderFillRect(renderer, &rect)`
- **Cairo**: `cairo_rectangle(cr, x, y, w, h); cairo_fill(cr)`
- **FLTK**: `fl_rectf(x, y, w, h)`

### Circle Drawing
- **GDI**: Manual pixel plotting (as seen in Watchy.cpp)
- **SDL2**: No built-in, use SDL_gfx or manual implementation
- **Cairo**: `cairo_arc(cr, x, y, radius, 0, 2*M_PI); cairo_stroke/fill(cr)`
- **FLTK**: `fl_circle(x, y, radius)`

## Color Format Considerations

### GDI/GDI+ Colors
- Uses COLORREF (0x00BBGGRR) for GDI
- GDI+ uses Color class with ARGB format
- WatchySim uses simple color constants: `GxEPD_WHITE = 0xFFFF`, `GxEPD_BLACK = 0x0000`

### Linux Framework Colors
- **SDL2**: Uses RGBA format, need conversion
- **Cairo**: Uses normalized floats (0.0-1.0) for RGB
- **FLTK**: Uses Fl_Color (32-bit RGBA)

## Coordinate System Notes

- GDI/GDI+ uses top-left origin (0,0) with Y increasing downward
- All Linux frameworks use the same coordinate system
- Display offset constants (`DISPLAY_OFFSET_X = 241`, `DISPLAY_OFFSET_Y = 198`) need to be maintained

## Memory Management Differences

### GDI/GDI+
- Manual cleanup required for GDI objects
- GDI+ uses RAII with proper destructors

### Linux Frameworks
- **SDL2**: Manual cleanup with SDL_Destroy* functions
- **Cairo**: Reference counting with cairo_destroy()
- **FLTK**: Automatic memory management for widgets

## Recommended Framework Selection

Based on the analysis:

1. **SDL2** - Best match for current architecture
   - Direct pixel manipulation support
   - Simple API similar to GDI
   - Good performance for 2D graphics
   - Cross-platform consistency

2. **Cairo/GTK+** - Good for more complex graphics
   - Better text rendering
   - More sophisticated drawing operations
   - Native Linux integration

3. **FLTK** - Simplest to implement
   - Built-in widget system
   - Easy event handling
   - Lightweight

## Special Considerations

1. **Bitmap Font Rendering**: Current implementation draws fonts as bitmaps pixel by pixel. This approach will work identically in all frameworks.

2. **Timer Updates**: The application updates every second but only repaints every minute. This logic needs to be preserved.

3. **Screenshot Functionality**: Each framework has different methods for capturing window contents.

4. **Resource Loading**: The background image is loaded from embedded resources. This needs to be adapted for Linux resource handling.