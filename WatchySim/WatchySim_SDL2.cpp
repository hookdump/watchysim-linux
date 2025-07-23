/*
 * WatchySim_SDL2.cpp - Linux port of WatchySim using SDL2
 * 
 * This is a port of the Windows WatchySim to Linux using SDL2.
 * Menu functionality is temporarily commented out (to be added with Dear ImGui later).
 * 
 * Compile with: g++ -o WatchySim_SDL2 WatchySim_SDL2.cpp `sdl2-config --cflags --libs`
 */

#include <SDL2/SDL.h>
#include <cstdint>
#include <cstring>
#include <ctime>
#include <iostream>

// Include Watchy headers
#ifndef _WIN32
#include "Watchy_SDL2.h"
#else
#include "Watchy.h"
#endif
#include "GraphicsAdapter.h"

// Watch faces
#include "WatchFaces/7_SEG/Watchy_7_SEG.h"
//#include "WatchFaces/AnalogGabel/Watchy_AnalogGabel.h"
//#include "WatchFaces/DOS/Watchy_DOS.h"
//#include "WatchFaces/MacPaint/Watchy_MacPaint.h"
//#include "WatchFaces/Mario/Watchy_Mario.h"
//#include "WatchFaces/Pokemon/Watchy_Pokemon.h"
//#include "WatchFaces/PowerShell/Watchy_PowerShell.h"
//#include "WatchFaces/Tetris/Watchy_Tetris.h"

// Initialize the watch face
Watchy7SEG watchy = Watchy7SEG();
//WatchyAnalogGabel watchy = WatchyAnalogGabel();
//WatchyDOS watchy = WatchyDOS();
//WatchyMacPaint watchy = WatchyMacPaint();
//WatchyMario watchy = WatchyMario();
//WatchyPowerShell watchy = WatchyPowerShell();
//WatchyPokemon watchy = WatchyPokemon();
//WatchyTetris watchy = WatchyTetris();

// SDL2 display class
class WatchySimSDL2 {
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* displayTexture;
    SDL_Texture* backgroundTexture;
    uint32_t* displayPixels;
    
    static const int SCALE = 3;
    static const int WINDOW_WIDTH = 676;
    static const int WINDOW_HEIGHT = 676;
    
    // E-ink colors
    static const uint32_t COLOR_WHITE = 0xFFFFFFFF;
    static const uint32_t COLOR_BLACK = 0xFF000000;
    
    // Timer ID
    SDL_TimerID updateTimer;
    
public:
    WatchySimSDL2() : window(nullptr), renderer(nullptr), displayTexture(nullptr), 
                      backgroundTexture(nullptr), displayPixels(nullptr), updateTimer(0) {}
    
    bool init() {
        
        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) {
            std::cerr << "SDL initialization failed: " << SDL_GetError() << std::endl;
            return false;
        }
        
        
        // Create window with fixed size (same as Windows version)
        window = SDL_CreateWindow("WatchySim",
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            WINDOW_WIDTH, WINDOW_HEIGHT,
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
        
        
        // Create display texture
        displayTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
            SDL_TEXTUREACCESS_STREAMING, DISPLAY_WIDTH, DISPLAY_HEIGHT);
            
        if (!displayTexture) {
            std::cerr << "Display texture creation failed: " << SDL_GetError() << std::endl;
            return false;
        }
        
        
        // TODO: Load background image (for now, just use a gray background)
        // In the Windows version, this loads from resources
        // For Linux, we'll need to load from a file
        
        displayPixels = new uint32_t[DISPLAY_WIDTH * DISPLAY_HEIGHT];
        clearDisplay();
        
        
        // Set up timer for updates (1 second interval)
        updateTimer = SDL_AddTimer(1000, timerCallback, this);
        
        
        return true;
    }
    
    void clearDisplay() {
        memset(displayPixels, 0xFF, DISPLAY_WIDTH * DISPLAY_HEIGHT * sizeof(uint32_t));
    }
    
    void drawBackground() {
        // For now, just draw a gray background
        SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255);
        SDL_RenderClear(renderer);
        
        // TODO: Draw the actual watch background image
        // This would involve loading the background.png and rendering it
    }
    
    void onPaint() {
        // Clear the display buffer
        clearDisplay();
        
        // Create SDL2 graphics context - use display dimensions, not window dimensions!
        SDL2GraphicsContext context(renderer, displayTexture, displayPixels, 
                                   DISPLAY_WIDTH, DISPLAY_HEIGHT);
        
        // Let Watchy draw the watch face
        watchy.showWatchFace(&context);
        
        // Note: context.present() is called which updates the texture
        // updateDisplay() will handle the actual rendering
    }
    
    void drawTestPattern() {
        // Clear to white first
        memset(displayPixels, 0xFF, DISPLAY_WIDTH * DISPLAY_HEIGHT * sizeof(uint32_t));
        
        // Draw a simple test pattern to verify display is working
        for (int y = 0; y < DISPLAY_HEIGHT; y++) {
            for (int x = 0; x < DISPLAY_WIDTH; x++) {
                // Draw a thick border
                if (x < 5 || x >= DISPLAY_WIDTH - 5 || y < 5 || y >= DISPLAY_HEIGHT - 5) {
                    displayPixels[y * DISPLAY_WIDTH + x] = COLOR_BLACK;
                }
                // Draw a grid
                else if (x % 20 == 0 || y % 20 == 0) {
                    displayPixels[y * DISPLAY_WIDTH + x] = COLOR_BLACK;
                }
            }
        }
        
        // Draw text "TEST" in center (rough approximation)
        int cx = DISPLAY_WIDTH / 2;
        int cy = DISPLAY_HEIGHT / 2;
        for (int i = -20; i < 20; i++) {
            for (int j = -5; j < 5; j++) {
                if (cx + i >= 0 && cx + i < DISPLAY_WIDTH && cy + j >= 0 && cy + j < DISPLAY_HEIGHT) {
                    displayPixels[(cy + j) * DISPLAY_WIDTH + (cx + i)] = COLOR_BLACK;
                }
            }
        }
    }
    
    void updateDisplay() {
        // Draw background
        drawBackground();
        
        // Note: The texture should already be updated by context.present() or manually
        // If we're calling this after drawTestPattern, we need to update the texture
        SDL_UpdateTexture(displayTexture, NULL, displayPixels, DISPLAY_WIDTH * sizeof(uint32_t));
        
        // Create destination rectangle centered in window
        SDL_Rect dstRect;
        dstRect.x = (WINDOW_WIDTH - DISPLAY_WIDTH * SCALE) / 2;
        dstRect.y = (WINDOW_HEIGHT - DISPLAY_HEIGHT * SCALE) / 2;
        dstRect.w = DISPLAY_WIDTH * SCALE;
        dstRect.h = DISPLAY_HEIGHT * SCALE;
        
        // Render the display texture
        SDL_RenderCopy(renderer, displayTexture, NULL, &dstRect);
        
        // Present
        SDL_RenderPresent(renderer);
    }
    
    static Uint32 timerCallback(Uint32 interval, void* param) {
        (void)param; // Unused parameter
        
        // Get current time
        time_t curr_time = time(NULL);
        struct tm* tm_local = localtime(&curr_time);
        
        // Convert to Watchy time structure
        struct tm watchy_tm = *tm_local;
        watchy.setTime(watchy_tm);
        
        // Only update display every minute (when seconds == 0)
        if (tm_local->tm_sec == 0) {
            // Push a user event to trigger display update
            SDL_Event event;
            SDL_memset(&event, 0, sizeof(event));
            event.type = SDL_USEREVENT;
            event.user.code = 1; // 1 = timer update
            SDL_PushEvent(&event);
        }
        
        return interval; // Continue timer
    }
    
    void run() {
        bool running = true;
        SDL_Event event;
        
        // Initial paint
        onPaint();
        updateDisplay();
        
        
        while (running) {
            while (SDL_PollEvent(&event)) {
                switch (event.type) {
                    case SDL_QUIT:
                        running = false;
                        break;
                        
                    case SDL_KEYDOWN:
                        handleKeyPress(event.key.keysym.sym);
                        break;
                        
                    case SDL_USEREVENT:
                        if (event.user.code == 1) {
                            // Timer update - repaint display
                            onPaint();
                            updateDisplay();
                        }
                        break;
                }
            }
            
            // Small delay to prevent CPU spinning
            SDL_Delay(10);
        }
    }
    
    void handleKeyPress(SDL_Keycode key) {
        switch (key) {
            case SDLK_ESCAPE:
                SDL_Event quit_event;
                quit_event.type = SDL_QUIT;
                SDL_PushEvent(&quit_event);
                break;
                
            // Menu shortcuts (commented out for now)
            /*
            case SDLK_t:
                // Time menu - reset to current time
                watchy.resetTime();
                onPaint();
                updateDisplay();
                break;
                
            case SDLK_b:
                // Battery menu - cycle through battery levels
                static float batteryLevels[] = {0.0f, 3.7f, 3.81f, 3.96f, 4.2f};
                static int currentBatteryIndex = 4;
                currentBatteryIndex = (currentBatteryIndex + 1) % 5;
                watchy.setBatteryVoltage(batteryLevels[currentBatteryIndex]);
                onPaint();
                updateDisplay();
                break;
                
            case SDLK_w:
                // WiFi toggle
                static bool wifiOn = false;
                wifiOn = !wifiOn;
                watchy.setWifi(wifiOn);
                if (wifiOn) {
                    watchy.setWeatherExternal(true);
                    if (watchy.getTemperature() == WATCHY_INTERNAL_TEMP) {
                        watchy.setTemperature(WATCHY_DEFAULT_TEMP);
                    }
                } else {
                    watchy.setWeatherCode(800);
                    watchy.setWeatherExternal(false);
                    watchy.setTemperature(WATCHY_INTERNAL_TEMP);
                }
                onPaint();
                updateDisplay();
                break;
            */
                
            case SDLK_r:
                // Refresh display
                onPaint();
                updateDisplay();
                break;
                
            case SDLK_t:
                // Test pattern
                drawTestPattern();
                updateDisplay();
                break;
                
            // Button simulations
            case SDLK_1:
            case SDLK_q:
                std::cout << "Button 1 (Menu/Back) pressed" << std::endl;
                // TODO: Call watchy button handler when implemented
                break;
                
            case SDLK_2:
            case SDLK_w:
                std::cout << "Button 2 (Up) pressed" << std::endl;
                // TODO: Call watchy button handler when implemented
                break;
                
            case SDLK_3:
            case SDLK_e:
                std::cout << "Button 3 (Down) pressed" << std::endl;
                // TODO: Call watchy button handler when implemented
                break;
                
            case SDLK_4:
                std::cout << "Button 4 (Select) pressed" << std::endl;
                // TODO: Call watchy button handler when implemented
                break;
        }
    }
    
    ~WatchySimSDL2() {
        if (updateTimer) {
            SDL_RemoveTimer(updateTimer);
        }
        if (displayPixels) delete[] displayPixels;
        if (displayTexture) SDL_DestroyTexture(displayTexture);
        if (backgroundTexture) SDL_DestroyTexture(backgroundTexture);
        if (renderer) SDL_DestroyRenderer(renderer);
        if (window) SDL_DestroyWindow(window);
        SDL_Quit();
    }
};

// Helper function for day of week calculation
int getDayOfWeek(int y, int m, int d) {
    static int t[] = { 0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4 };
    if (m < 3) {
        y -= 1;
    }
    return (y + y / 4 - y / 100 + y / 400 + t[m - 1] + d) % 7;
}

// Main function
int main(int argc, char* argv[]) {
    
    WatchySimSDL2 sim;
    
    
    if (!sim.init()) {
        std::cerr << "[ERROR] Failed to initialize!" << std::endl;
        return 1;
    }
    
    std::cout << "WatchySim SDL2 - Linux Port" << std::endl;
    std::cout << "Keys:" << std::endl;
    std::cout << "  ESC - Exit" << std::endl;
    std::cout << "  R   - Refresh display" << std::endl;
    std::cout << "  T   - Test pattern" << std::endl;
    std::cout << std::endl;
    std::cout << "Button Simulation:" << std::endl;
    std::cout << "  1 or Q - Button 1 (Menu/Back)" << std::endl;
    std::cout << "  2 or W - Button 2 (Up)" << std::endl;
    std::cout << "  3 or E - Button 3 (Down)" << std::endl;
    std::cout << "  4 or R - Button 4 (Select)" << std::endl;
    std::cout << std::endl;
    std::cout << "  (Full menu functionality coming soon with Dear ImGui)" << std::endl;
    
    
    sim.run();
    
    return 0;
}