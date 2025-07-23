# Linux UI Framework Evaluation for WatchySim Port

## Executive Summary

After evaluating SDL2, GTK+, FLTK, and SFML for the WatchySim Linux port, **SDL2** emerges as the clear winner for our needs. It provides excellent pixel-level control, minimal dependencies, simple API, and has proven track record for similar applications.

## Framework Comparison Table

| Feature | SDL2 | GTK+ (gtkmm) | FLTK | SFML |
|---------|------|--------------|------|------|
| **Pixel Control** | ✅ Excellent | ✅ Good (via Cairo) | ✅ Good | ✅ Excellent |
| **API Simplicity** | ✅ Very Simple | ❌ Complex | ✅ Simple | ✅ Simple |
| **Menu Support** | ⚠️ Need Dear ImGui | ✅ Native | ✅ Native | ⚠️ Need Dear ImGui |
| **Dependencies** | ✅ Minimal | ❌ Heavy | ✅ Minimal | ✅ Moderate |
| **Documentation** | ✅ Excellent | ✅ Good | ⚠️ Adequate | ✅ Good |
| **Community** | ✅ Very Active | ✅ Active | ⚠️ Small but stable | ✅ Active |
| **Build Complexity** | ✅ Simple | ❌ Complex | ✅ Simple | ✅ Simple |
| **Binary Size** | ✅ ~2MB | ❌ ~20MB+ | ✅ ~3MB | ✅ ~5MB |
| **Performance** | ✅ Excellent | ⚠️ Good | ✅ Good | ✅ Excellent |

## Detailed Framework Analysis

### 1. SDL2 (Simple DirectMedia Layer)

**Pros:**
- Industry standard for game/multimedia applications
- Extremely lightweight and fast
- Direct pixel manipulation via SDL_Surface
- Excellent cross-platform support
- Minimal dependencies (just libsdl2-dev)
- Large community and extensive documentation
- Battle-tested in thousands of projects

**Cons:**
- No native menu support (but Dear ImGui integration is trivial)
- More low-level than widget toolkits

**Dependencies:**
```bash
sudo apt-get install libsdl2-dev
# Optional for menus:
# Dear ImGui (header-only, no deps)
```

**Proof of Concept:**
```cpp
#include <SDL2/SDL.h>
#include <cstdint>

class SDLWatchySim {
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* texture;
    uint32_t* pixels;
    const int DISPLAY_WIDTH = 200;
    const int DISPLAY_HEIGHT = 200;
    const int SCALE = 3; // 3x scaling for better visibility

public:
    bool init() {
        if (SDL_Init(SDL_INIT_VIDEO) < 0) return false;
        
        window = SDL_CreateWindow("WatchySim",
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            DISPLAY_WIDTH * SCALE, DISPLAY_HEIGHT * SCALE,
            SDL_WINDOW_SHOWN);
        if (!window) return false;
        
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if (!renderer) return false;
        
        texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
            SDL_TEXTUREACCESS_STREAMING, DISPLAY_WIDTH, DISPLAY_HEIGHT);
        if (!texture) return false;
        
        pixels = new uint32_t[DISPLAY_WIDTH * DISPLAY_HEIGHT];
        memset(pixels, 0xFF, DISPLAY_WIDTH * DISPLAY_HEIGHT * sizeof(uint32_t));
        
        return true;
    }
    
    void setPixel(int x, int y, bool black) {
        if (x >= 0 && x < DISPLAY_WIDTH && y >= 0 && y < DISPLAY_HEIGHT) {
            pixels[y * DISPLAY_WIDTH + x] = black ? 0xFF000000 : 0xFFFFFFFF;
        }
    }
    
    void drawLine(int x0, int y0, int x1, int y1, bool black) {
        // Bresenham's line algorithm
        int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
        int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
        int err = dx + dy, e2;
        
        while (true) {
            setPixel(x0, y0, black);
            if (x0 == x1 && y0 == y1) break;
            e2 = 2 * err;
            if (e2 >= dy) { err += dy; x0 += sx; }
            if (e2 <= dx) { err += dx; y0 += sy; }
        }
    }
    
    void fillRect(int x, int y, int w, int h, bool black) {
        for (int j = y; j < y + h; j++) {
            for (int i = x; i < x + w; i++) {
                setPixel(i, j, black);
            }
        }
    }
    
    void update() {
        SDL_UpdateTexture(texture, NULL, pixels, DISPLAY_WIDTH * sizeof(uint32_t));
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
    }
    
    bool handleEvents() {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) return false;
            // Handle other events (mouse, keyboard)
        }
        return true;
    }
    
    ~SDLWatchySim() {
        delete[] pixels;
        SDL_DestroyTexture(texture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }
};

// Usage example
int main(int argc, char* argv[]) {
    SDLWatchySim sim;
    if (!sim.init()) return 1;
    
    // Draw test pattern
    sim.fillRect(0, 0, 200, 200, false);  // White background
    sim.drawLine(10, 10, 190, 190, true); // Black diagonal line
    sim.fillRect(50, 50, 100, 100, true); // Black square
    
    while (sim.handleEvents()) {
        sim.update();
        SDL_Delay(16); // ~60 FPS
    }
    
    return 0;
}
```

### 2. GTK+ with gtkmm

**Pros:**
- Native Linux look and feel
- Built-in menu and dialog support
- Cairo graphics excellent for 2D drawing
- Mature and stable

**Cons:**
- Heavy dependencies (GTK3/4, Cairo, Pango, etc.)
- Complex API for simple tasks
- Overkill for a 200x200 pixel display
- Significant learning curve

**Dependencies:**
```bash
sudo apt-get install libgtkmm-3.0-dev
```

**Proof of Concept:**
```cpp
#include <gtkmm.h>
#include <cairomm/context.h>

class WatchySimWindow : public Gtk::Window {
private:
    Gtk::DrawingArea drawing_area;
    Glib::RefPtr<Gdk::Pixbuf> pixbuf;
    const int DISPLAY_SIZE = 200;
    const int SCALE = 3;

public:
    WatchySimWindow() {
        set_title("WatchySim");
        set_default_size(DISPLAY_SIZE * SCALE, DISPLAY_SIZE * SCALE);
        
        pixbuf = Gdk::Pixbuf::create(Gdk::COLORSPACE_RGB, false, 8,
                                     DISPLAY_SIZE, DISPLAY_SIZE);
        pixbuf->fill(0xFFFFFFFF); // White background
        
        drawing_area.signal_draw().connect(
            sigc::mem_fun(*this, &WatchySimWindow::on_draw));
        
        add(drawing_area);
        show_all_children();
    }
    
    void setPixel(int x, int y, bool black) {
        if (x < 0 || x >= DISPLAY_SIZE || y < 0 || y >= DISPLAY_SIZE) return;
        
        guchar* pixels = pixbuf->get_pixels();
        int rowstride = pixbuf->get_rowstride();
        int n_channels = pixbuf->get_n_channels();
        
        guchar* p = pixels + y * rowstride + x * n_channels;
        p[0] = p[1] = p[2] = black ? 0 : 255;
    }
    
    bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) {
        Gdk::Cairo::set_source_pixbuf(cr, pixbuf, 0, 0);
        cr->scale(SCALE, SCALE);
        cr->paint();
        return true;
    }
};

int main(int argc, char* argv[]) {
    auto app = Gtk::Application::create(argc, argv, "org.watchysim");
    WatchySimWindow window;
    return app->run(window);
}
```

### 3. FLTK (Fast Light Toolkit)

**Pros:**
- Very lightweight
- Simple API
- Built-in widget support
- Small binary size
- Good for simple GUIs

**Cons:**
- Dated look and feel
- Smaller community
- Less modern documentation
- Limited advanced graphics features

**Dependencies:**
```bash
sudo apt-get install libfltk1.3-dev
```

**Proof of Concept:**
```cpp
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/fl_draw.H>

class WatchyDisplay : public Fl_Box {
private:
    unsigned char* buffer;
    const int DISPLAY_SIZE = 200;
    
public:
    WatchyDisplay(int x, int y, int w, int h) : Fl_Box(x, y, w, h) {
        buffer = new unsigned char[DISPLAY_SIZE * DISPLAY_SIZE * 3];
        memset(buffer, 255, DISPLAY_SIZE * DISPLAY_SIZE * 3);
    }
    
    void setPixel(int x, int y, bool black) {
        if (x < 0 || x >= DISPLAY_SIZE || y < 0 || y >= DISPLAY_SIZE) return;
        int idx = (y * DISPLAY_SIZE + x) * 3;
        buffer[idx] = buffer[idx+1] = buffer[idx+2] = black ? 0 : 255;
    }
    
    void draw() override {
        fl_draw_image(buffer, x(), y(), DISPLAY_SIZE, DISPLAY_SIZE);
    }
    
    ~WatchyDisplay() {
        delete[] buffer;
    }
};

int main(int argc, char** argv) {
    Fl_Window window(600, 600, "WatchySim");
    WatchyDisplay display(0, 0, 600, 600);
    
    window.end();
    window.show(argc, argv);
    
    return Fl::run();
}
```

### 4. SFML (Simple and Fast Multimedia Library)

**Pros:**
- Modern C++ design
- Good 2D graphics support
- Active development
- Good performance

**Cons:**
- More game-oriented
- Larger than SDL2
- No native menu support
- Less widespread than SDL

**Dependencies:**
```bash
sudo apt-get install libsfml-dev
```

**Proof of Concept:**
```cpp
#include <SFML/Graphics.hpp>
#include <vector>

class SFMLWatchySim {
private:
    sf::RenderWindow window;
    sf::Image image;
    sf::Texture texture;
    sf::Sprite sprite;
    const int DISPLAY_SIZE = 200;
    const int SCALE = 3;
    
public:
    SFMLWatchySim() : window(sf::VideoMode(DISPLAY_SIZE * SCALE, 
                                           DISPLAY_SIZE * SCALE), "WatchySim") {
        image.create(DISPLAY_SIZE, DISPLAY_SIZE, sf::Color::White);
        texture.loadFromImage(image);
        sprite.setTexture(texture);
        sprite.setScale(SCALE, SCALE);
    }
    
    void setPixel(int x, int y, bool black) {
        if (x >= 0 && x < DISPLAY_SIZE && y >= 0 && y < DISPLAY_SIZE) {
            image.setPixel(x, y, black ? sf::Color::Black : sf::Color::White);
        }
    }
    
    void update() {
        texture.loadFromImage(image);
    }
    
    void run() {
        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed)
                    window.close();
            }
            
            window.clear();
            window.draw(sprite);
            window.display();
        }
    }
};
```

## Menu Implementation Strategy

For SDL2 and SFML, we'll use **Dear ImGui** for menus:

```cpp
// Dear ImGui integration example with SDL2
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"

void setupImGui(SDL_Window* window, SDL_Renderer* renderer) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    
    ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer2_Init(renderer);
}

void renderMenu() {
    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
    
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Save Screenshot")) { /* ... */ }
            if (ImGui::MenuItem("Exit")) { /* ... */ }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Settings")) {
            if (ImGui::MenuItem("Set Time...")) { /* ... */ }
            if (ImGui::MenuItem("Battery Level...")) { /* ... */ }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
    
    ImGui::Render();
    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());
}
```

## Recommendation: SDL2

**SDL2 is the recommended framework** for the following reasons:

1. **Perfect fit for requirements**: Excellent pixel manipulation, minimal overhead
2. **Proven track record**: Used in countless emulators and similar projects
3. **Simple integration**: The existing GDI calls map almost 1:1 to SDL2
4. **Minimal dependencies**: Just libsdl2-dev (vs GTK's 20+ packages)
5. **Cross-platform bonus**: If needed, could maintain Windows/Linux in same codebase
6. **Dear ImGui integration**: Provides professional menus with minimal effort
7. **Performance**: Hardware accelerated rendering for smooth updates
8. **Documentation**: Extensive tutorials and examples available

## Implementation Plan with SDL2

1. **Phase 1**: Basic window and pixel drawing
   - Replace GDI window creation with SDL2
   - Implement setPixel, drawLine, fillRect
   - Test with simple patterns

2. **Phase 2**: Complete drawing API
   - Port all drawing functions from Watchy.cpp
   - Implement font rendering
   - Add image loading support

3. **Phase 3**: Menu system
   - Integrate Dear ImGui
   - Implement time setting dialog
   - Add battery/wifi/bt status controls

4. **Phase 4**: Polish
   - Add screenshot capability
   - Implement proper scaling
   - Add keyboard shortcuts

## Build Configuration

Simple CMakeLists.txt for SDL2:
```cmake
cmake_minimum_required(VERSION 3.10)
project(WatchySim)

set(CMAKE_CXX_STANDARD 11)

find_package(SDL2 REQUIRED)

add_executable(WatchySim
    main.cpp
    Watchy.cpp
    DisplaySim.cpp
    # ... other source files
)

target_include_directories(WatchySim PRIVATE ${SDL2_INCLUDE_DIRS})
target_link_libraries(WatchySim ${SDL2_LIBRARIES})
```

## Challenges and Solutions

| Challenge | SDL2 Solution |
|-----------|---------------|
| Menu system | Dear ImGui integration (proven, lightweight) |
| Font rendering | SDL_ttf or port existing bitmap font system |
| Image loading | SDL_image or use existing GIF resources |
| Event handling | SDL event system maps well to button presses |
| Screenshot | SDL_RenderReadPixels + SDL_image save |

## Conclusion

SDL2 provides the best balance of simplicity, performance, and features for the WatchySim Linux port. Its minimal dependencies and straightforward API will allow for a clean port with minimal changes to the existing codebase structure.