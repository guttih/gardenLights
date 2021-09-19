/*
  SpotlightIRemoteController

  Receives commands from nRF24L01
  and relays them to the spotlight via IR signal

 */

#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>


#include "FloodLightRemote.h"

FloodLightRemote fRemote;


RF24 radio(7,8);                // nRF24L01(+) radio attached using Getting Started board 

RF24Network network(radio);      // Network uses that radio
const uint16_t this_node = 04;    // Address of our node in Octal format ( 04,031, etc)
const uint16_t other_node = 00;   // Address of the other node in Octal format

struct payload_t {                 // Structure of our payload
  unsigned long command;
  unsigned long extra;
};


void startNetwork(uint8_t _channel, uint16_t _node_address ){
    Serial.print("\n\nRemote: ");Serial.println(REMOTE_NAME);
    Serial.println("This node:");
    Serial.print("Address:");    Serial.println(this_node, OCT);
    Serial.print("Channel:");    Serial.println(_channel);
    network.begin(_channel, _node_address);
}

void setup(void)
{
  Serial.begin(115200);
  Serial.println("RF24Network SpotlightRemoteCommander");
  
  fRemote.printCommands(true);
  Serial.println();
  fRemote.printCommandNumbers();
  fRemote.testAllButtons();

  SPI.begin();
  radio.begin();
  startNetwork(90, this_node);
}

void loop(void){
  
  network.update();                  // Check the network regularly

  
  while ( network.available() ) {     // Is there anything ready for us?
    irCommand cmd;
    RF24NetworkHeader header;        // If so, grab it and print it out
    payload_t payload;
    network.read(header,&payload,sizeof(payload));
    cmd =  fRemote.getCommand((BUTTON)(unsigned long)payload.command);
    Serial.print("Received command: ");
    Serial.print(cmd.name);
    Serial.print(" extra data:  ");
    Serial.print(payload.extra);
    if (cmd.code != 0) {
      Serial.print(", sending to spotlight : ");
      Serial.println(cmd.code, HEX);
      fRemote.sendVal(cmd.code);
    } else {
      Serial.println(", not sending to spotlight.");
    }
  }
}
