#ifndef LIGHTCOMMANDER_H
#define LIGHTCOMMANDER_H

/*
    SpotlightCommander.ino

    Gets commands from serial and sends them wireless 

	Connecting the Esp32 to the NRF24L01
	PIN	NRF24L01  Esp32 Dev module	
	1	GND			GND	               
	2	VCC			3V3	               
	3	CE			D12	               
	4	CSN			D14	               
	5	SCK			D18	               
	6	MOSI		D23	               
	7	MISO		D19	               
	8	IRQ			 - not connectd
*/

#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>

//RF24 radio(7,8);                    // nRF24L01(+) radio attached using Getting Started board 
RF24 radio(12,14);

RF24Network gardenNetwork(radio);          // Network uses that radio

const uint16_t this_node = 00;        // Address of our node in Octal format

const unsigned long interval = 2000; //ms  // How often to send 'hello world to the other unit

unsigned long last_sent;             // When did we last send?
unsigned long packets_sent;          // How many have we sent already


struct payload_t {                  // Structure of our payload
unsigned long command;
unsigned long extra;
};

#define COMMAND_COUNT 25

#define INVALID_COMMAND 100
char *commands[COMMAND_COUNT] = {
    "UP"          ,"DOWN"       ,"OFF"        ,"ON"    ,
    "RED"         ,"GREEN"      ,"BLUE"       ,"WHITE" ,
    "RED_LIGHT"   ,"GREEN_LIGHT","PURPLE"     ,"FLASH" ,
    "ORANGE"      ,"GREEN_CYAN" ,"PURPLE_PINK","STROBE",
    "ORANGE_LIGHT","CYAN"       ,"PINK"       ,"FADE"  ,
    "YELLOW"      ,"CYAN_GREEN" ,"PINK_PURPLE","SMOOTH", 
    "FFF"
};


void gardenNetworkSetup(void)
{
//Serial.begin(115200);
Serial.println("RF24Network/examples/helloworld_tx/");

SPI.begin();
radio.begin();
gardenNetwork.begin(/*channel*/ 90, /*node address*/ this_node);
}

//returns INVALID_COMMAND if command is not a valid command
unsigned long getCommand(const char *strCommand) {
    for (int i = 0; i < COMMAND_COUNT; i++) {
    if (strcmp(strCommand, commands[i]) == 0)
        return i;
    }
    
    return INVALID_COMMAND;
}

bool sendCommand(uint16_t octalAddress, unsigned long command){
    payload_t payload = {command, 0 };
    RF24NetworkHeader header(octalAddress);
    Serial.print("Sending command "); Serial.print(commands[payload.command]);Serial.print(" to address "); Serial.print(octalAddress, OCT);
    bool ok = gardenNetwork.write(header,&payload,sizeof(payload));
    if (ok)
    Serial.println(", sent ok.");
    else
    Serial.println(", failed sending.");
    return ok;
}

void getCommandsFromSerial(){
    gardenNetwork.update();       

    while(Serial.available() > 0 ) {
        String str = Serial.readString();
        str.replace("\n","");
        str.toUpperCase();
        payload_t payload = {getCommand(str.c_str()), 0 };
        if (payload.command != INVALID_COMMAND) 
            sendCommand(01, payload.command);
        else
            Serial.println("unknown command");
    }
}


class GardenLightCommander {

};

#endif