You are a technical project manager in charge of porting the WatchySim app from Windows to Linux.

The current code works for Windows. Just create a new branch and fully modify it to work on Linux instead. It is fine if it breaks for Windows. Although it would be ideal to somehow keep a single codebase that works in both platform, thats likely too difficult.

You only have access to the terminal, so at some point you may come across roadbloacks when it comes to testing things regarding the UI. When you do, just ask for my help, i will test the application for you.

IMPORTANT Feel free to spawn multiple sub agents to accelrate this project.

ALSO IMPORTANT: Use files to coordinate, maybe create a new folder for it. like a DEV_PLAN.md, and potentially files for sub agents tasks, etc.

OK, lets get to work!

For extra context, feel free to consider the hints/pointers discussed in the following discussion: (althought yoiu dont have to follow this strictyly, they may be useful hints, feel free to use them or discard them based on your judgement).

DazDotOne
opened on Jun 8, 2024
Hi,

Firstly, as always, thanks for any and all work you've done on this. I'm a seasoned developer but very new to embedded systems and have recently acquired a watchy which I'm loving.

I'm looking at creating some faces (and possibly some drivers for different displays).

I'd love to be able to sim the watchy but I run Linux.

I was just wondering if you'd had any interactions with doing so and whether you had any pointers for where to start if not.

Thanks again

D.

Activity
LeeHolmes
LeeHolmes commented on Jun 8, 2024
LeeHolmes
on Jun 8, 2024 via email · edited by LeeHolmes
Owner
Hey Daniel - right now, it adapts the Watchy SDK (which is pretty fully the Arduino SDK with some additions) and maps it all into a GDI application for Windows. But if you're eager, I think you could port it to run on Linux without a ton of effort. The WatchySim is essentially just a bunch of mocks.

For example, if your Watchy watch face calls "print(<some string>)", there is a mock of that in Watchy.cpp (DisplaySim::print) that calls an mocked Arduino API (drawFontBitmap(...)). That does a bunch of stuff that finally calls into a few GDI-native functions like "setPixel". Then all of this is hosted inside a Windows GDI application.

If you wanted to port this to Linux, most of the work would just be cloning the very simple user interface, wiring the menu items (like setting the time) to code that already exists, and porting the handful of functions that do the actual drawing to a display area (like drawing a pixel, drawing a rectangle, drawing a line, measuring a string, and maybe a few others).

The vast majority of the code is mocking the Watchy SDK and Arduino SDKs into those primitive functions and wouldn't need to change.
DazDotOne
DazDotOne commented on Jun 9, 2024
DazDotOne
on Jun 9, 2024
Author
Amazing! Thanks for the response Lee.

I'll look at having a play then. Didn't want to go down a rabbit hole if it was going to be a no go.

I'll let you know if I make any progress.

LeeHolmes
LeeHolmes commented on Jun 9, 2024
LeeHolmes
on Jun 9, 2024 via email · edited by LeeHolmes
Owner
Would love to hear how it goes.

BTW, one thing that is easy to miss - it MUST be a C or C++-based UI framework that you use. There are lots of very capable UI frameworks on Linux for Python etc (i.e.: I made an oscilloscope app:
LeeHolmes/nanoscope: Front end to convert your Arduino Nano into a fully featured Oscilloscope (github.com)
<https://github.com/LeeHolmes/nanoscope>), but the point of WatchySim is to be able to accelerate Watchy development (which is Arduino C and C++).

I looked into Qt at one point, but it was a bloated mess. There are of course many others: C++ UI Libraries • memdump (philippegroarke.com) <https://philippegroarke.com/posts/2018/c++_ui_solutions/>
