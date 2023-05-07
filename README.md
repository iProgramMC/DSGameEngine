# DSGameEngine
A DS game engine based off libnds with frame buffer capabilities for the top screen.

It includes basic image drawing functions with transparency and flipping, and basic rectangle drawing.

The graphics work is all done in software, only that the CPU will copy the framebuffer data to the GPU of the Nintendo DS.

It's quite inefficient, but who needs backgrounds and sprites when you can have this?


This repo will also have examples of programs written based off it.

The platform game contains sprites from New Super Mario Bros. for the Nintendo DS. I won't provide them here for legal reasons.
The 3D engine doesn't contain any other things.

You need devkitPro (namely devkitARM) to build.
