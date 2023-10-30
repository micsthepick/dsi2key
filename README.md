DS(i)2Key is an application for Nintendo DS that allows you to send key signals to your PC thus emulating a PC game pad. Utilizing the touch screen you can also control the PC mouse.


## Building

make sure to clone all with `git clone --recurse-submodules`

otherwise, pull dsiwifi with `git submodule init && git pull --recurse-submodules`.

Requires the devkitPro toolchain for building the nds file - https://devkitpro.org/wiki/Getting_Started
If this is your first time building with DevkitPro, don't forget to carefully read the install instructions for the devkitPro pacman. You probably just need nds-dev, but reach out if that doesn't work.
You also will need nds-freetype from devkitpro's pacman.

### Windows
Simply run Visual studio community or greater (2022 is tested), and build. -- TODO - actually port to vigem?
-- also, in this branch, windows is probably broken and/or buggy at the moment

### Linux
also requires libxtst-dev
run ./compile.sh for server, and for client separately.