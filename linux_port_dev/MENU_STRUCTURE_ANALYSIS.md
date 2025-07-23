# WatchySim Menu Structure Analysis

## Overview
WatchySim has a comprehensive menu system for simulating various Watchy states. The menu is defined in `WatchySim.rc` and callbacks are implemented in `WatchySim.cpp`.

## Complete Menu Hierarchy

### 1. Time Menu (`&Time`)
- **Current** (`ID_TIME_CURRENT` = 32774)
  - Resets time to current system time
  - Callback: `watchy.resetTime()`
  
- **Short** (`ID_TIME_SHORT` = 32772)
  - Sets time to: May 1, 2011 01:01:01
  - Tests minimal date/time display
  
- **Long** (`ID_TIME_LONG` = 32771)
  - Sets time to: September 31, 2099 18:33:01
  - Tests maximum date/time display
  
- **Custom** (`ID_TIME_CUSTOM` = 32805)
  - Opens input dialog for custom date/time entry
  - Dialog ID: `IDD_INPUT_BOX`
  - Format: MM/DD/YYYY HH:MM
  - Calculates day of week automatically

### 2. Battery Menu (`&Battery`)
- **Dead** (`ID_BATTERY_DEAD` = 32797)
  - Sets voltage: 0.0V
  
- **Low** (`ID_BATTERY_LOW` = 32776)
  - Sets voltage: 3.7V
  
- **Medium** (`ID_BATTERY_MEDIUM` = 32777)
  - Sets voltage: 3.81V
  
- **High** (`ID_BATTERY_HIGH` = 32778)
  - Sets voltage: 3.96V
  
- **Max** (`ID_BATTERY_MAX` = 32779)
  - Sets voltage: 4.2V

### 3. Bluetooth Menu (`B&luetooth`)
- **On** (`ID_BLUETOOTH_ON` = 32780)
  - Enables Bluetooth state
  
- **Off** (`ID_BLUETOOTH_OFF` = 32781)
  - Disables Bluetooth state

### 4. WiFi Menu (`&Wifi`)
- **On** (`ID_WIFI_ON` = 32782)
  - Enables WiFi state
  - Sets weather to external source
  - Resets temperature from internal (27°C) to default (20°C)
  
- **Off** (`ID_WIFI_OFF` = 32783)
  - Disables WiFi state
  - Sets weather code to 800 (clear)
  - Sets weather to internal source
  - Sets temperature to internal sensor value (27°C)

### 5. Steps Menu (`&Steps`)
- **None** (`ID_STEPS_NONE` = 32784)
  - Sets steps: 0
  
- **Lazy** (`ID_STEPS_LAZY` = 32785)
  - Sets steps: 12
  
- **Regular** (`ID_STEPS_REGULAR` = 32786)
  - Sets steps: 5,280
  
- **Athlete** (`ID_STEPS_ATHLETE` = 32787)
  - Sets steps: 52,769

### 6. Weather Menu (`W&eather`)
All weather options set external weather flag to true.

- **Cloudy** (`ID_WEATHER_CLOUDY` = 32788)
  - Weather code: 802
  
- **Few Clouds** (`ID_WEATHER_FEWCLOUDS` = 32789)
  - Weather code: 801
  
- **Clear** (`ID_WEATHER_CLEAR` = 32790)
  - Weather code: 800
  
- **Atmosphere** (`ID_WEATHER_ATMOSPHERE` = 32791)
  - Weather code: 750
  
- **Snow** (`ID_WEATHER_SNOW` = 32792)
  - Weather code: 650
  
- **Rain** (`ID_WEATHER_RAIN` = 32793)
  - Weather code: 550
  
- **Drizzle** (`ID_WEATHER_DRIZZLE` = 32794)
  - Weather code: 350
  
- **Thunderstorm** (`ID_WEATHER_THUNDERSTORM` = 32795)
  - Weather code: 250
  
- **Other** (`ID_WEATHER_OTHER` = 32796)
  - Weather code: 150

### 7. Temperature Menu (`Te&mperature`)
Temperature unit options:
- **Celsius** (`ID_TEMPERATURE_CELSIUS` = 32798)
  - Sets metric units
  
- **Fahrenheit** (`ID_TEMPERATURE_FAHRENHEIT` = 32799)
  - Sets imperial units

Temperature value options (all set external weather flag):
- **Canada** (`ID_TEMPERATURE_CANADA` = 32804)
  - Sets temperature: -45°C
  
- **Chilly** (`ID_TEMPERATURE_CHILLY` = 32801)
  - Sets temperature: 7°C
  
- **Warm** (`ID_TEMPERATURE_WARM` = 32802)
  - Sets temperature: 15°C
  
- **Inferno** (`ID_TEMPERATURE_INFERNO` = 32803)
  - Sets temperature: 40°C

### 8. Tools Menu (`T&ools`)
- **Screenshot** (`ID_TOOLS_SCREENSHOT` = 32773)
  - Opens save dialog for GIF file
  - Captures 200x200 pixel display area
  - Uses GDI+ to save as GIF

## Dialog Specifications

### Input Box Dialog (`IDD_INPUT_BOX` = 129)
- Title: "Enter Value"
- Components:
  - Static text: "Enter your value:"
  - Edit control: `IDC_EDIT_USERTEXT` (ID 1000)
  - OK button: Standard OK
  - Cancel button: Standard Cancel
- Used for custom time input
- Modal dialog

### About Box Dialog (`IDD_ABOUTBOX` = 103)
- Title: "About WatchySim"
- Components:
  - Icon: Watchy icon
  - Static text: "WatchySim, Version 1.0"
  - Static text: "Copyright (c) 2021"
  - OK button: Standard OK

## Callback Pattern

All menu callbacks follow this pattern:
1. Perform the action (set value)
2. Call `InvalidateRect(hWnd, NULL, false)`
3. Call `PostMessage(hWnd, WM_PAINT, 0, 0)`
4. Return 0

## Special Behaviors

1. **Timer Updates**: A 1-second timer updates the watch face, but only repaints when seconds = 0 (every minute)

2. **WiFi Temperature Logic**: 
   - When WiFi is ON and temperature is internal (27°C), it resets to default (20°C)
   - When WiFi is OFF, temperature is always set to internal (27°C)

3. **Weather External Flag**:
   - All weather menu items set external flag to true
   - WiFi OFF sets external flag to false

4. **Screenshot Function**:
   - Captures only the watch display area (200x200 pixels)
   - Display offset: X=241, Y=198 from window client area
   - Saves as GIF format using GDI+

5. **Custom Date Entry**:
   - Uses `getDayOfWeek()` function to calculate day of week
   - Year is adjusted by -1900 for tm structure
   - Month is incremented by 1 after parsing

## Display Constants
- `DISPLAY_WIDTH`: 200 pixels
- `DISPLAY_HEIGHT`: 200 pixels
- `DISPLAY_OFFSET_X`: 241 pixels
- `DISPLAY_OFFSET_Y`: 198 pixels
- `WATCHY_DEFAULT_TEMP`: 20°C
- `WATCHY_INTERNAL_TEMP`: 27°C

## Window Properties
- Window style: `WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU`
- Client area: 676x676 pixels (adjusted for window frame)
- Icon: Custom WatchySim icon
- Background: White brush
- Menu: Loaded from resources

## Implementation Notes for Linux Port

1. **Menu System**: Need to replicate exact menu structure with same IDs for compatibility
2. **Dialogs**: Custom time input dialog is essential; about dialog is optional
3. **Screenshot**: Need equivalent functionality to capture display area as image
4. **Timer**: Need 1-second timer that triggers repaint every minute
5. **GDI+ Replacement**: Need graphics library for drawing operations
6. **Resource Loading**: Background image loaded from embedded resource