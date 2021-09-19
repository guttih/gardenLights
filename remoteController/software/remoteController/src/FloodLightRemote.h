#ifndef FLOODLIGHTRREMOTE_H
#define FLOODLIGHTRREMOTE_H

/*
Connections

Arduino                  IR
  Nano    NRF24L01    transmitter
  ---       ----         ---
  GND        GND	
  3.3V       VCC	
  D7         CE	
  D8         CSN	
  D13        SCK
  D11       MOSI
  D12       MISO	
  5V                     VCC
  GND                    GND
  D3                     SIG

*/

// Select one of these remotes

//#define BLACK_REMOTE_10W
//#define SILVER_REMOTE_10W
//#define SILVER_REMOTE_50W_1
#define SILVER_REMOTE_50W_2 
//#define SILVER_REMOTE_50W_3

#include <arduino.h>
#include <IRremote.h>

//Must connect send data to D3 on nano and pin 4 on esp32



struct irCommand {
    unsigned long code;
    const char *name;
};

enum BUTTON {
     UP          , DOWN       , OFF         , ON    ,
     RED         , GREEN      , BLUE        , WHITE , 
     RED_LIGHT   , GREEN_LIGHT, PURPLE      , FLASH , 
     ORANGE      , GREEN_CYAN , PURPLE_PINK , STROBE,
     ORANGE_LIGHT, CYAN       , PINK        , FADE  ,
     YELLOW      , CYAN_GREEN , PINK_PURPLE , SMOOTH, 
     FF
};

const int REMOTE_ROWS  = 6,
          REMOTE_COLS  = 4;

class FloodLightRemote {

    private: 
        IRsend    ir;
        const int remoteSendDelay = 30;
        irCommand invalidCommand {0, "INVALID COMMAND"};
        irCommand testAllCommand {1, "TEST"};
        irCommand commandNumbersCommand {2, "NUMBERS"};
        
        irCommand commands[REMOTE_ROWS][REMOTE_COLS] = 
#ifdef BLACK_REMOTE_10W      
            {   // BLACK_REMOTE_10W - BLACK Spotlight IR remote control codes
                //            0                         1                        2                        3             INDEX
                { {0xFFA05F,"UP"          },{0xFF20DF,"DOWN"       },{0xFF609F,"OFF"        },{0xFFE01F,"ON"    } }, //  0
                { {0xFF906F,"RED"         },{0xFF10EF,"GREEN"      },{0xFF50AF,"BLUE"       },{0xFFD02F,"WHITE" } }, //  1
                { {0xFFB04F,"RED_LIGHT"   },{0xFF30CF,"GREEN_LIGHT"},{0xFF708F,"PURPLE"     },{0xFFF00F,"FLASH" } }, //  2
                { {0xFFA857,"ORANGE"      },{0xFF28D7,"GREEN_CYAN" },{0xFF6897,"PURPLE_PINK"},{0xFFE817,"STROBE"} }, //  3
                { {0xFF9867,"ORANGE_LIGHT"},{0xFF18E7,"CYAN"       },{0xFF58A7,"PINK"       },{0xFFD827,"FADE"  } }, //  4
                { {0xFF8877,"YELLOW"      },{0xFF08F7,"CYAN_GREEN" },{0xFF48B7,"PINK_PURPLE"},{0xFFC837,"SMOOTH"} }  //  5
            //                                      |                       |      
            //                                      |                       Circular timing: can set the required numbers of hours and required period of time to
            //                                      |                       turn on and off  the light daily and accurately, especially fit for outdoor lighting.
            //                                      |                       
            //                                      Regular timing: can set the required numbers of our to turn off the light.  
            //                                      Very convenient to use, especially fit for sleeper who forget to switch off the light.           

            };
        #define REMOTE_NAME "BLACK_REMOTE_10W"
#elif defined (SILVER_REMOTE_10W)
            {   // SILVER_REMOTE_10W - SILVER Spotlight IR remote control codes
                //          0                         1                        2                        3           INDEX
                { {0xff906f,"UP"          },{0xffb847,"DOWN"       },{0xfff807,"OFF"        },{0xffb04f,"ON"    } }, //  0
                { {0xff9867,"RED"         },{0xffd827,"GREEN"      },{0xff8877,"BLUE"       },{0xffa857,"WHITE" } }, //  1
                { {0xffe817,"RED_LIGHT"   },{0xff48b7,"GREEN_LIGHT"},{0xff6897,"PURPLE"     },{0xffb24d,"FLASH" } }, //  2
                { {0xff02fd,"ORANGE"      },{0xff32cd,"GREEN_CYAN" },{0xff20df,"PURPLE_PINK"},{0xff00ff,"STROBE"} }, //  3
                { {0xff50af,"ORANGE_LIGHT"},{0xff7887,"CYAN"       },{0xff708f,"PINK"       },{0xff58a7,"FADE"  } }, //  4
                { {0xff38c7,"YELLOW"      },{0xff28d7,"CYAN_GREEN" },{0xfff00f,"PINK_PURPLE"},{0xff30cf,"SMOOTH"} }  //  5
            };
        #define REMOTE_NAME "SILVER_REMOTE_10W"
#elif defined (SILVER_REMOTE_50W_1)
            {   // SILVER_REMOTE_50W - SILVER Spotlight IR remote control codes for the big 50W flood lights
                //           0                         1                        2                        3           INDEX
                { {0xF700FF,"UP"          },{0xF7807F,"DOWN"       },{0xF740BF,"OFF"        },{0xF7C03F,"ON"    } }, //  0
                { {0xF720DF,"RED"         }, {0xF7A05F,"GREEN"     },{0xF7609F,"BLUE"       },{0xF7E01F,"WHITE" } }, //  1
                { {0xF710EF,"RED_LIGHT"   },{0xF750AF,"PURPLE"     },{0xF7906F,"GREEN_LIGHT"},{0xF7D02F,"FLASH" } }, //  2
                { {0xF730CF,"ORANGE"      },{0xF7708F,"PURPLE_PINK"},{0xF7B04F,"GREEN_CYAN" },{0xF7F00F,"STROBE"} }, //  3
                { {0xF708F7,"ORANGE_LIGHT"},{0xF748B7,"PINK"       },{0xF78877,"CYAN"       },{0xF7C837,"FADE"  } }, //  4
                { {0xF728D7,"YELLOW"      },{0xF76897,"PINK_PURPLE"},{0xF7A857,"CYAN_GREEN" },{0xF7E817,"SMOOTH"} }  //  5
            };
        #define REMOTE_NAME "SILVER_REMOTE_50W_1"
#elif defined (SILVER_REMOTE_50W_2)
            {   // SILVER_REMOTE_50W - SILVER Spotlight IR remote control codes for the big 50W flood lights
                //           0                         1                        2                        3           INDEX
                { {0xF700FF,"UP"          },{0xF7807F,"DOWN"       },{0xF740BF,"OFF"        },{0xF7C03F,"ON"    } }, //  0
                { {0xF720DF,"RED"         },{0xF7A05F,"GREEN"      },{0xF7609F,"BLUE"       },{0xF7E01F,"WHITE" } }, //  1
                { {0xF710EF,"RED_LIGHT"   },{0xF7906F,"GREEN_LIGHT"},{0xF750AF,"PURPLE"     },{0xF7D02F,"FLASH" } }, //  2
                { {0xF730CF,"ORANGE"      },{0xF7B04F,"GREEN_CYAN" },{0xF7708F,"PURPLE_PINK"},{0xF7F00F,"STROBE"} }, //  3
                { {0xF708F7,"ORANGE_LIGHT"},{0xF78877,"CYAN"       },{0xF748B7,"PINK"       },{0xF7C837,"FADE"  } }, //  4
                { {0xF728D7,"YELLOW"      },{0xF7A857,"CYAN_GREEN" },{0xF76897,"PINK_PURPLE"},{0xF7E817,"SMOOTH"} }  //  5
            };
        #define REMOTE_NAME "SILVER_REMOTE_50W_2"
#elif defined (SILVER_REMOTE_50W_3)
            {   // SILVER_REMOTE_50W - SILVER Spotlight IR remote control codes for the big 50W flood lights
                //           0                         1                        2                        3           INDEX
                { {0xF700FF,"UP"          },{0xF7807F,"DOWN"       },{0xF740BF,"OFF"        },{0xF7C03F,"ON"    } }, //  0
                { {0xF7A05F,"GREEN"       },{0xF720DF,"RED"        },{0xF7609F,"BLUE"       },{0xF7E01F,"WHITE" } }, //  1
                { {0xF710EF,"RED_LIGHT"   },{0xF750AF,"PURPLE"     },{0xF7906F,"GREEN_LIGHT"},{0xF7D02F,"FLASH" } }, //  2
                { {0xF730CF,"ORANGE"      },{0xF7708F,"PURPLE_PINK"},{0xF7B04F,"GREEN_CYAN" },{0xF7F00F,"STROBE"} }, //  3
                { {0xF708F7,"ORANGE_LIGHT"},{0xF748B7,"PINK"       },{0xF78877,"CYAN"       },{0xF7C837,"FADE"  } }, //  4
                { {0xF728D7,"YELLOW"      },{0xF76897,"PINK_PURPLE"},{0xF7A857,"CYAN_GREEN" },{0xF7E817,"SMOOTH"} }  //  5
            };
            #define REMOTE_NAME "SILVER_REMOTE_50W_3"
#endif
        
    
        int       getMaxLen();
        char *    formatCommandForDisplay(char *result, irCommand com, bool useName, int length);
        void      sendVal(unsigned long val, int bits);
        void      sendAndPrintCommand(BUTTON irButton, char *result, bool useName, int length);

    public:
        void printCommands(bool printNames);
        void testAllButtons();
        String commandNumbers();
        void printCommandNumbers();
        /*
        Sends a BUTTON command to the IR transmitter
        */
        bool sendButtonCommand(BUTTON irButton);
        irCommand getCommand(BUTTON irButton);
        irCommand getCommand(const char *strCommand);
        void      sendVal(unsigned long val);

};

#endif 