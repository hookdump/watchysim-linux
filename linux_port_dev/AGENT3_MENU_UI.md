# Agent 3: Menu & UI Elements Task

## Objective
Port the menu system and UI dialogs from Windows resources to Linux framework.

## Current Windows Menu Structure
```
WatchySim
├── Set Time (Custom Dialog)
├── Set Weather
│   ├── Clear
│   ├── Cloudy  
│   ├── Rain
│   └── Snow
├── Set Temperature (-20°F to 120°F submenu)
├── Set Battery Level (0% to 100% submenu)
├── Charging Status
├── Take Screenshot
└── Exit
```

## UI Elements to Port

### 1. Menu Bar
- Convert from Windows resource menu (WatchySim.rc)
- Implement all menu items with same IDs
- Maintain keyboard shortcuts

### 2. Custom Time Dialog
- Current: Windows DialogBox with 6 input fields
- Need: Hour, Minute, Second, Month, Day, Year inputs
- Validation: Ensure valid date/time values

### 3. Menu Callbacks
Map these Windows commands to Linux actions:
- `ID_WATCHYSIM_SETTIME` - Open time dialog
- `ID_WATCHYSIM_CLEAR/CLOUDY/RAIN/SNOW` - Set weather
- `ID_WATCHYSIM_TEMP_*` - Set temperature
- `ID_WATCHYSIM_BATTERY_*` - Set battery level
- `ID_WATCHYSIM_CHARGINGSTATUS` - Toggle charging
- `ID_WATCHYSIM_TAKESCREENSHOT` - Save screenshot
- `IDM_EXIT` - Close application

### 4. Screenshot Functionality
- Current: Uses GetSaveFileName dialog
- Need: Native file save dialog
- Format: PNG or BMP
- Default filename: WatchySim_YYYYMMDD_HHMMSS.png

## Implementation Notes
1. Maintain same menu IDs for compatibility
2. Use framework's native menu system if possible
3. Keep menu structure identical to Windows version
4. Ensure all callbacks update the watch face properly

## Framework-Specific Considerations

### If GTK+:
- Use GtkMenuBar and GtkMenuItem
- GtkDialog for custom time input
- GtkFileChooserDialog for screenshots

### If SDL2:
- May need Dear ImGui or similar for menus
- Custom dialog implementation
- Native file dialog library needed

### If FLTK:
- Fl_Menu_Bar for menus
- Fl_Window with Fl_Input for time dialog
- Fl_File_Chooser for save dialog

## Testing Checklist
- [ ] All menu items appear correctly
- [ ] Set Time dialog accepts valid input
- [ ] Weather changes update display
- [ ] Temperature changes update display
- [ ] Battery level changes update display
- [ ] Charging indicator toggles
- [ ] Screenshot saves to selected location
- [ ] Exit closes application cleanly

## Success Criteria
- User can access all functions via menu
- Dialog inputs are validated
- All settings affect watch face immediately
- Screenshot captures exact display content