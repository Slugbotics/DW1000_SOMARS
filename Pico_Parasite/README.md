To use this program, you will need to ensure the correct pins are connected to the DW1000 (based on the pin selection in the pins.h file).

Then you need to build, first install the pico SDK
https://github.com/pimoroni/pimoroni-pico/blob/main/setting-up-the-pico-sdk.md
(make sure the pico sdk path is exported on your current terminal)

then you will need to build the executable
1. mkdir build
2. cd build
3. cmake ..
4. make

This will generate a .uf2 file which you will install onto the pico by holding down the button on the pico when you plug it into your computer.