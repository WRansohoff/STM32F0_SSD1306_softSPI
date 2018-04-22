# Overview

This project demonstrates example code for displaying an image on a 128x64-pixel SSD1306 monochrome display, using software SPI on an STM32F031K6 board.

On ST's "Nucleo" board for this chip, the pins used in this example are:

* PB0 / 'D3': "Chip Select" pin.

* PB1 / 'D6': "Data / Command" pin.

* PB6 / 'D5': "SPI Host Data Out" (MOSI) pin.

* PB7 / 'D4': "SPI Clock" (SCLK) pin.

The SSD1306 does not seem to provide a "device data out" (MISO) pin. I feel like the 'data 2' pin on their ribbon connectors might serve that function, but I don't see it on common existing designs. I haven't looked closely enough at the datasheet to say one way or the other though, and I left that pin floating in my board's design.

Anyways, these displays are ubiquitous enough that the world doesn't need *another* open-source C library for them, but hopefully this can demonstrate the basics of how to use them in an accessible way without the use of hardware peripherals or other abstractions.
