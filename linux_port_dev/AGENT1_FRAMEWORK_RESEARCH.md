# Agent 1: Framework Research & Selection Task

## Objective
Research and recommend the best C++ UI framework for porting WatchySim to Linux.

## Requirements
- Must be C/C++ (not Python, etc.)
- Lightweight (avoid Qt)
- Good 2D drawing support with pixel-level control
- Easy menu/dialog creation
- Active maintenance
- Reasonable documentation

## Frameworks to Evaluate

### 1. SDL2 (Simple DirectMedia Layer)
**Research Areas:**
- Pixel manipulation performance
- Menu/dialog capabilities (SDL2 + Dear ImGui?)
- Event handling system
- Image loading support
- Build complexity

### 2. GTK+ (with Cairo for drawing)
**Research Areas:**
- GTK3 vs GTK4 for our needs
- Cairo 2D graphics capabilities
- Native menu support
- GtkDrawingArea for custom rendering
- gtkmm (C++ bindings) evaluation

### 3. FLTK (Fast Light Toolkit)
**Research Areas:**
- Drawing API capabilities
- Built-in widget support
- Simplicity of API
- Community activity
- Performance characteristics

### 4. SFML (Simple and Fast Multimedia Library)
**Research Areas:**
- 2D graphics capabilities
- Event system
- UI elements availability
- C++ design
- Linux support quality

## Evaluation Criteria
1. **Drawing Performance**: Can it handle 200x200 pixel manipulation efficiently?
2. **API Simplicity**: How much code to create window and draw pixels?
3. **Menu Support**: Native menus or need additional library?
4. **Dependencies**: What needs to be installed?
5. **Documentation**: Quality and availability
6. **Community**: Active development and support?

## Deliverables
1. Summary comparison table
2. Code snippet showing basic window + pixel drawing for each
3. Recommendation with justification
4. Potential challenges for each option

## Notes
- Remember: The display is only 200x200 pixels
- We need precise pixel control for watch face rendering
- Menus are simple but necessary (set time, battery, etc.)
- Screenshot capability is required