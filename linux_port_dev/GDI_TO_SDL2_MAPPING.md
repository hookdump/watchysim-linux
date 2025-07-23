# GDI to SDL2 API Mapping Guide

## Overview
This document shows how to map the current Windows GDI calls in WatchySim to SDL2 equivalents for the Linux port.

## Core Drawing Functions

### 1. Pixel Drawing
```cpp
// Windows GDI (current)
SetPixel(*hdc, x + DISPLAY_OFFSET_X, y + DISPLAY_OFFSET_Y, color | color << 8);

// SDL2 (replacement)
pixels[y * DISPLAY_WIDTH + x] = black ? 0xFF000000 : 0xFFFFFFFF;
SDL_UpdateTexture(texture, NULL, pixels, DISPLAY_WIDTH * sizeof(uint32_t));
```

### 2. Line Drawing
```cpp
// Windows GDI (current)
Graphics graphics(hdc);
Pen pen(color == GxEPD_BLACK ? Color(255,0,0,0) : Color(255,255,255,255));
graphics.DrawLine(&pen, x0, y0, x1, y1);

// SDL2 (replacement)
// Use Bresenham's algorithm (see sdl2_poc.cpp for implementation)
drawLine(x0, y0, x1, y1, color == GxEPD_BLACK);
```

### 3. Rectangle Filling
```cpp
// Windows GDI (current)
SolidBrush brush(color == GxEPD_BLACK ? Color(255,0,0,0) : Color(255,255,255,255));
graphics.FillRectangle(&brush, x, y, w, h);

// SDL2 (replacement)
for (int j = y; j < y + h; j++) {
    for (int i = x; i < x + w; i++) {
        setPixel(i, j, color == GxEPD_BLACK);
    }
}
```

### 4. Window Creation
```cpp
// Windows GDI (current)
WNDCLASS wndClass;
wndClass.style = CS_HREDRAW | CS_VREDRAW;
wndClass.lpfnWndProc = WndProc;
RegisterClass(&wndClass);
hWnd = CreateWindow(...);

// SDL2 (replacement)
SDL_Init(SDL_INIT_VIDEO);
window = SDL_CreateWindow("WatchySim", 
    SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
    600, 600, SDL_WINDOW_SHOWN);
renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
```

### 5. Event Handling
```cpp
// Windows GDI (current)
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
        case WM_COMMAND: // Menu events
        case WM_PAINT:   // Redraw
        case WM_DESTROY: // Close
    }
}

// SDL2 (replacement)
SDL_Event event;
while (SDL_PollEvent(&event)) {
    switch (event.type) {
        case SDL_QUIT:
        case SDL_KEYDOWN:
        case SDL_MOUSEBUTTONDOWN:
    }
}
```

### 6. Menu Implementation
```cpp
// Windows GDI (current)
case IDM_BATTERYLOW:
    Serial.batteryLevel = 25;
    InvalidateRect(hWnd, NULL, TRUE);
    break;

// SDL2 + Dear ImGui (replacement)
if (ImGui::BeginMenu("Battery")) {
    if (ImGui::MenuItem("Low (25%)")) {
        Serial.batteryLevel = 25;
        needsRedraw = true;
    }
}
```

## Font Rendering

### Current GDI Approach
```cpp
// Uses GDI+ DrawImage for bitmap fonts
graphics.DrawImage(&fontBitmap, x, y);
```

### SDL2 Approach
```cpp
// Option 1: Port existing bitmap font system
void drawFontBitmap(int16_t x, int16_t y, const uint8_t *bitmap, 
                   uint16_t w, uint16_t h, bool black) {
    for (int j = 0; j < h; j++) {
        for (int i = 0; i < w; i++) {
            if (getBitmapPixel(bitmap, i, j)) {
                setPixel(x + i, y + j, black);
            }
        }
    }
}

// Option 2: Use SDL_ttf for TrueType fonts
TTF_Font* font = TTF_OpenFont("font.ttf", 24);
SDL_Surface* text = TTF_RenderText_Solid(font, "12:34", {0,0,0});
```

## Image Loading

### Current GDI Approach
```cpp
myResource = ::FindResource(NULL, MAKEINTRESOURCE(IDR_BACKGROUND), RT_RCDATA);
pStream = SHCreateMemStream((LPBYTE)LockResource(myResourceData), myResourceSize);
image = new Image(pStream);
graphics.DrawImage(image, 0, 0);
```

### SDL2 Approach
```cpp
// Load from file
SDL_Surface* bg = IMG_Load("background.png");
SDL_Texture* bgTexture = SDL_CreateTextureFromSurface(renderer, bg);

// Or embed in binary and load from memory
SDL_RWops* rw = SDL_RWFromMem(embedded_data, embedded_size);
SDL_Surface* bg = IMG_Load_RW(rw, 1);
```

## Complete Function Mapping Table

| WatchySim Function | GDI Implementation | SDL2 Implementation |
|-------------------|-------------------|-------------------|
| `drawPixel()` | `SetPixel()` | Direct pixel array access |
| `drawLine()` | `Graphics::DrawLine()` | Bresenham's algorithm |
| `fillRect()` | `Graphics::FillRectangle()` | Nested loops |
| `drawRect()` | `Graphics::DrawRectangle()` | 4 `drawLine()` calls |
| `drawCircle()` | `Graphics::DrawEllipse()` | Midpoint circle algorithm |
| `drawBitmap()` | `Graphics::DrawImage()` | Pixel-by-pixel copy |
| `print()` | Font rendering | Bitmap font system |
| `showWatchFace()` | `OnPaint()` | Main render function |

## Build System Changes

### Current (Windows)
- Visual Studio project files
- Windows SDK dependencies
- GDI+ libraries

### SDL2 (Linux)
```cmake
cmake_minimum_required(VERSION 3.10)
project(WatchySim)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

add_executable(WatchySim
    main.cpp
    Watchy.cpp
    DisplaySim.cpp
    ${WATCHFACE_SOURCES}
)

target_link_libraries(WatchySim 
    ${SDL2_LIBRARIES}
    ${SDL2_IMAGE_LIBRARIES}
)
```

## Resource Handling

### Current (Windows)
- Resources compiled into .rc file
- Loaded via Windows resource API

### SDL2 (Linux)
- Option 1: External files in resources/ directory
- Option 2: Use xxd or similar to embed in source
- Option 3: Use CMake to generate resource headers

## Key Advantages of SDL2 Port

1. **Simpler API**: SDL2's pixel buffer approach is more straightforward
2. **Better performance**: Direct pixel access vs GDI+ overhead
3. **Cross-platform**: Same code could work on Windows/Mac/Linux
4. **Modern tooling**: CMake, pkg-config, standard Linux development
5. **Smaller binary**: ~2MB vs ~10MB+ with GDI+

## Migration Priority

1. **Phase 1**: Core display functions (pixel, line, rect)
2. **Phase 2**: Font system (reuse existing bitmap fonts)
3. **Phase 3**: Menu system (Dear ImGui integration)
4. **Phase 4**: Resource loading (images, icons)
5. **Phase 5**: Dialog boxes (time setting, etc.)