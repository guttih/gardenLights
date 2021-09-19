# gardenLights - lightsCommander
This project will take commands from VoffCon and send them to the 
lightController(s).

This project is one of two applications the researched on this [web page].


# Connections
Description on how to wire the esp32 to the 

| PIN | NRF24L01 | Esp32 Development module |
| --- | -------: | :----------------------- |
| 1   | GND      | GND                      |
| 2   | VCC      | 3V3                      |
| 3   | CE       | D12                      |
| 4   | CSN      | D14                      |
| 5   | SCK      | D18                      |
| 6   | MOSI     | D23                      |
| 7   | MISO     | D19                      |
| 8   | IRQ      | -                        |

[web page]: https://guttih.com/list/project-garden-lights