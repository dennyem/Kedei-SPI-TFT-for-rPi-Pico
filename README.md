# Kedei-SPI-TFT-for-rPi-Pico
This project uses a Raspberry Pi SPI TFT with touchscreen from Kedei, init code is specific to version 6.2, other versions may require changes.

These displays have always been troublesome according to many reviews I have read, available code and examples seem to be the main issue.  This project provides driver code for the display and touchscreen for the Raspberry Pi Pico / Pico W on the Arduino (IDE 2.2) platform.  The example includes a modified version of the the Rinky-Dink UTFT library however it is not in library form but simply C++ files in the sketch folder. Standard SPI functions are used so the project could be easily adapted to othe MCU's.
