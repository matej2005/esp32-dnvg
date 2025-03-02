

# ESP32-DNVG

Code for digital night vision based on esp32-cam dev board and GC9A01A TFT display

Base code used from [Displaying Live Video from ESP32-CAM](https://hjwwalters.com/esp32-cam-gc9a01/) 

Main feature from other codes for esp32 based nvg´s, is onscreen menu, controled by pushbutton

## Use
- single click scrooling
- double click enter
- hold exit


## Tested with

|          | OV2640  | OV7725  |
|:--------:|:-------:|:-------:|
| ESP-32s  |    ✅   |    ?    |



## Parts i used

| part    |   link    |
|:-------:|:---------:|
| display | [ali](https://www.aliexpress.com/item/1005004786844308.html)|
| esp32   | [from local e-shop](https://www.laskakit.cz/esp32-cam-2-4ghz-wifi-bluetooth-modul_-ipex-antena/)|
| lens    | [ali](https://www.aliexpress.com/item/32242183276.html)|


## Schematic

![schematic](pics/schematic.png)


## Features

 1. Onscreen menu
    - Changing filter
        - No Effect, Negative, Grayscale, Red Tint, Green Tint, Blue Tint, Sepia
    - Rotation of display
        - 0, 90, 180, 270
    - White balance
    - Restoring setings back
    - Setings are saved, when exiting from menu
    - FPS counter
    
  2. Some debug info on serial













