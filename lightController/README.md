# gardenLights - lightRemote
This project will take commands from lightsCommander and send them to the 
spot light it's controlling using a IR transmitter.

This project is one of two applications the researched on this [web page].

# Connections
Description on how to wire the Arduino Nano 
to the NRF24L01 and the IR transmitter. 

| PIN | Arduino nano |  NRF24L01 | IR transmitter|
| --- | :----------- |  -------: | -------------:|
| 1   | GND          |  GND      | GND           |
| 2   | 3.3V         |  VCC      | VCC           |
| 3   | D7           |  CE       |               |
| 4   | D8           |  CSN      |               |
| 5   | D13          |  SCK      |               |
| 6   | D11          |  MOSI     |               |
| 7   | D12          |  MISO     |               |
| 8   | -            |  IRQ      |               |
| 9   | D3           |           | SIG           |


[web page]: https://guttih.com/list/project-garden-lights