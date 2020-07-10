# InfinityTable

![alt text](https://github.com/tylerpleiter/InfinityTable/blob/master/BuildImages/03_completed.jpg "Completed Build")

## Description:

Partially finished project to create a custom infinity table. Currently works as a typical infinity table using LED's and mirror tinting to create an infinity effect. However bottom layer glass is also mirror tinted rather than typical mirror. This will allow for an LED array underneath to function as low resolution display for simple games. Table pattern is controllable via WiFi, with the ssid and password hardcoded in espMain. After configuring the router to give the ESP a static IP, the table is easily addressable. 

## Attributes:

- 4 layer LED
- WiFi controllable

## Materials:

- Oak (Bunnings)
- - 65x12mm
- - 30x8mm
- - 65x12mm
- Pine (Bunnings)
- - 776x476x18mm
- Corner Brackets and Screws (Bunnings)
- Glass (Local Glazier)
- - 6mm 772x485mm
- - 3mm 766x479mm
- Mirror Tinting (Bunnings)
- 60 leds/m -non-waterproof -ws2812B -black -5m (Amazon)
- Switching LED Power Supply 220VAC-5VDC 40A (Ebay)
- Arduino ATMega with WiFi (ESP8266) (Jaycar)

## Roadmap:

- Add 4x4 pixel alphabet to enable message display
- Install LED array underneath bottom layer along with acrylic sheet for diffusion.
- Install two distance sensors either side of table to function as simple control interface.
- Program MCU to run simple games (ping pong, game of life, etc.).

## Issues:

- Due to amount of LEDs to write, communication suffers between ATmega and ESP8266 causing function changes to fail

## Credit:

- FastLED Library - Daniel Garcia, Mark Kriegsman
- ESP32 HTML Sketch (modified) - Rui Santos
