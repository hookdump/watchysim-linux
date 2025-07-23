/*
 * SDL2 Proof of Concept for WatchySim
 * 
 * Compile with: g++ -o sdl2_poc sdl2_poc.cpp `sdl2-config --cflags --libs`
 * 
 * This demonstrates:
 * - Creating a window with 200x200 display scaled 3x
 * - Pixel-perfect drawing operations
 * - Basic event handling
 * - Frame timing
 */

#include <SDL2/SDL.h>
#include <cstdint>
#include <cstring>
#include <cmath>
#include <iostream>

class WatchySimDisplay {
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* texture;
    uint32_t* pixels;
    
    static const int DISPLAY_WIDTH = 200;
    static const int DISPLAY_HEIGHT = 200;
    static const int SCALE = 3;
    
    // E-ink colors
    static const uint32_t COLOR_WHITE = 0xFFFFFFFF;
    static const uint32_t COLOR_BLACK = 0xFF000000;
    
public:
    WatchySimDisplay() : window(nullptr), renderer(nullptr), texture(nullptr), pixels(nullptr) {}
    
    bool init() {
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            std::cerr << "SDL initialization failed: " << SDL_GetError() << std::endl;
            return false;
        }
        
        window = SDL_CreateWindow("WatchySim - SDL2 Proof of Concept",
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            DISPLAY_WIDTH * SCALE, DISPLAY_HEIGHT * SCALE,
            SDL_WINDOW_SHOWN);
            
        if (!window) {
            std::cerr << "Window creation failed: " << SDL_GetError() << std::endl;
            return false;
        }
        
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        if (!renderer) {
            std::cerr << "Renderer creation failed: " << SDL_GetError() << std::endl;
            return false;
        }
        
        // Use nearest neighbor scaling for crisp pixels
        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
        
        texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
            SDL_TEXTUREACCESS_STREAMING, DISPLAY_WIDTH, DISPLAY_HEIGHT);
            
        if (!texture) {
            std::cerr << "Texture creation failed: " << SDL_GetError() << std::endl;
            return false;
        }
        
        pixels = new uint32_t[DISPLAY_WIDTH * DISPLAY_HEIGHT];
        clear();
        
        return true;
    }
    
    void clear() {
        memset(pixels, 0xFF, DISPLAY_WIDTH * DISPLAY_HEIGHT * sizeof(uint32_t));
    }
    
    void setPixel(int x, int y, bool black) {
        if (x >= 0 && x < DISPLAY_WIDTH && y >= 0 && y < DISPLAY_HEIGHT) {
            pixels[y * DISPLAY_WIDTH + x] = black ? COLOR_BLACK : COLOR_WHITE;
        }
    }
    
    void drawLine(int x0, int y0, int x1, int y1, bool black) {
        // Bresenham's line algorithm
        int dx = abs(x1 - x0);
        int sx = x0 < x1 ? 1 : -1;
        int dy = -abs(y1 - y0);
        int sy = y0 < y1 ? 1 : -1;
        int err = dx + dy;
        
        while (true) {
            setPixel(x0, y0, black);
            
            if (x0 == x1 && y0 == y1) break;
            
            int e2 = 2 * err;
            if (e2 >= dy) {
                err += dy;
                x0 += sx;
            }
            if (e2 <= dx) {
                err += dx;
                y0 += sy;
            }
        }
    }
    
    void fillRect(int x, int y, int w, int h, bool black) {
        for (int j = y; j < y + h && j < DISPLAY_HEIGHT; j++) {
            for (int i = x; i < x + w && i < DISPLAY_WIDTH; i++) {
                if (i >= 0 && j >= 0) {
                    setPixel(i, j, black);
                }
            }
        }
    }
    
    void drawRect(int x, int y, int w, int h, bool black) {
        drawLine(x, y, x + w - 1, y, black);           // Top
        drawLine(x, y + h - 1, x + w - 1, y + h - 1, black); // Bottom
        drawLine(x, y, x, y + h - 1, black);           // Left
        drawLine(x + w - 1, y, x + w - 1, y + h - 1, black); // Right
    }
    
    void drawCircle(int cx, int cy, int r, bool black) {
        // Midpoint circle algorithm
        int x = r;
        int y = 0;
        int err = 0;
        
        while (x >= y) {
            setPixel(cx + x, cy + y, black);
            setPixel(cx + y, cy + x, black);
            setPixel(cx - y, cy + x, black);
            setPixel(cx - x, cy + y, black);
            setPixel(cx - x, cy - y, black);
            setPixel(cx - y, cy - x, black);
            setPixel(cx + y, cy - x, black);
            setPixel(cx + x, cy - y, black);
            
            if (err <= 0) {
                y += 1;
                err += 2*y + 1;
            }
            if (err > 0) {
                x -= 1;
                err -= 2*x + 1;
            }
        }
    }
    
    void update() {
        SDL_UpdateTexture(texture, NULL, pixels, DISPLAY_WIDTH * sizeof(uint32_t));
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
    }
    
    void drawTestPattern() {
        // Clear to white
        clear();
        
        // Draw border
        drawRect(0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT, true);
        
        // Draw diagonal lines
        drawLine(0, 0, DISPLAY_WIDTH-1, DISPLAY_HEIGHT-1, true);
        drawLine(DISPLAY_WIDTH-1, 0, 0, DISPLAY_HEIGHT-1, true);
        
        // Draw center circle
        drawCircle(DISPLAY_WIDTH/2, DISPLAY_HEIGHT/2, 30, true);
        
        // Draw some rectangles
        fillRect(10, 10, 30, 30, true);
        fillRect(DISPLAY_WIDTH-40, 10, 30, 30, true);
        fillRect(10, DISPLAY_HEIGHT-40, 30, 30, true);
        fillRect(DISPLAY_WIDTH-40, DISPLAY_HEIGHT-40, 30, 30, true);
        
        // Draw text placeholder (would use actual font rendering in real implementation)
        fillRect(70, 90, 60, 20, true);  // Simulating "12:34"
    }
    
    ~WatchySimDisplay() {
        if (pixels) delete[] pixels;
        if (texture) SDL_DestroyTexture(texture);
        if (renderer) SDL_DestroyRenderer(renderer);
        if (window) SDL_DestroyWindow(window);
        SDL_Quit();
    }
};

int main(int argc, char* argv[]) {
    WatchySimDisplay display;
    
    if (!display.init()) {
        return 1;
    }
    
    // Draw test pattern
    display.drawTestPattern();
    
    // Main loop
    bool running = true;
    SDL_Event event;
    uint32_t frameStart, frameTime;
    const uint32_t targetFrameTime = 16; // ~60 FPS
    
    while (running) {
        frameStart = SDL_GetTicks();
        
        // Handle events
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    running = false;
                    break;
                    
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym) {
                        case SDLK_ESCAPE:
                            running = false;
                            break;
                        case SDLK_SPACE:
                            // Redraw test pattern on space
                            display.drawTestPattern();
                            break;
                        case SDLK_c:
                            // Clear on 'c'
                            display.clear();
                            break;
                    }
                    break;
                    
                case SDL_MOUSEBUTTONDOWN:
                    // Draw a pixel where clicked (accounting for scale)
                    int x = event.button.x / 3;
                    int y = event.button.y / 3;
                    display.fillRect(x-2, y-2, 5, 5, true);
                    break;
            }
        }
        
        // Update display
        display.update();
        
        // Frame timing
        frameTime = SDL_GetTicks() - frameStart;
        if (frameTime < targetFrameTime) {
            SDL_Delay(targetFrameTime - frameTime);
        }
    }
    
    std::cout << "WatchySim SDL2 POC terminated successfully" << std::endl;
    return 0;
}