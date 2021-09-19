#include "FloodLightRemote.h"

irCommand FloodLightRemote::getCommand(BUTTON irButton) {

    switch(irButton){
        case UP           : return commands[0][0];        case DOWN        : return commands[0][1];        case OFF         : return commands[0][2];        case ON     : return commands[0][3];        
        case RED          : return commands[1][0];        case GREEN       : return commands[1][1];        case BLUE        : return commands[1][2];        case WHITE  : return commands[1][3];        
        case RED_LIGHT    : return commands[2][0];        case GREEN_LIGHT : return commands[2][1];        case PURPLE      : return commands[2][2];        case FLASH  : return commands[2][3];        
        case ORANGE       : return commands[3][0];        case GREEN_CYAN  : return commands[3][1];        case PURPLE_PINK : return commands[3][2];        case STROBE : return commands[3][3];        
        case ORANGE_LIGHT : return commands[4][0];        case CYAN        : return commands[4][1];        case PINK        : return commands[4][2];        case FADE   : return commands[4][3];        
        case YELLOW       : return commands[5][0];        case CYAN_GREEN  : return commands[5][1];        case PINK_PURPLE : return commands[5][2];        case SMOOTH : return commands[5][3]; 
        case FF           : return {0xffffff, "FF"};
    }
    //will not happen
    
    return commands[0][2];
}

void FloodLightRemote::printCommandNumbers(){
    Serial.println(commandNumbers());
}

String FloodLightRemote::commandNumbers() {
    
    String ret = "\n";
    unsigned int commandCount = ((unsigned int)BUTTON::FF ) + 1;

    ret+=String("Number of commands : ") + String(commandCount) + "\n";

    for(unsigned int btnCommand = 0; btnCommand < commandCount; btnCommand++ ) {
        ret+=btnCommand;
        ret+=" : ";
        ret+=getCommand((BUTTON)btnCommand).name;
        ret+= "\n";
    }
    return ret;
}

//returns invalidCommand if strCommand does not exist in commands
irCommand FloodLightRemote::getCommand(const char *strCommand) {
    String findMe(strCommand);
    findMe.toUpperCase();

    if (strcmp(findMe.c_str(), "FF") == 0){
        return getCommand(FF);
    }
    for(int r = 0; r < REMOTE_ROWS; r++) {
        for(int c = 0; c < REMOTE_COLS; c++) {
            if (strcmp(findMe.c_str(), commands[r][c].name) == 0)
                return commands[r][c];
        }
    }

    if (strcmp(findMe.c_str(), testAllCommand.name) == 0)
        return testAllCommand;
    else if (strcmp(findMe.c_str(), commandNumbersCommand.name) == 0)
        return commandNumbersCommand;
    else
        return invalidCommand;
}

int FloodLightRemote::getMaxLen() {
    int len, maxLen = 0;
    for(int r = 0; r < REMOTE_ROWS; r++) {
        for(int c = 0; c < REMOTE_COLS; c++) {
            len = strlen(commands[r][c].name);
            if (len > maxLen)
                maxLen = len;
        }
    }
    return maxLen;
}

char * FloodLightRemote::formatCommandForDisplay(char *result, irCommand com, bool useName, int length) {
    if (useName){
    char format[20];
        sprintf (format, "|%%-%ds|", length); 
        sprintf (result, format,  com.name);
        return result;
    }
    sprintf (result, "%04X ", com.code); 
    return result;
}

void FloodLightRemote::printCommands(bool printNames) {
    char buffer[100];
    Serial.print("Send pin: ");
    Serial.println(IR_SEND_PIN);
    
    int length = getMaxLen()+1;
    for(int r = 0; r < REMOTE_ROWS; r++) {
        for(int c = 0; c < REMOTE_COLS; c++) {
            Serial.print(formatCommandForDisplay(buffer, commands[r][c], printNames, length));
          }
          Serial.println();    
    }

    Serial.print(formatCommandForDisplay(buffer, testAllCommand, printNames, length));
    Serial.print(formatCommandForDisplay(buffer, commandNumbersCommand, printNames, length));
}

void FloodLightRemote::sendVal(unsigned long val) {
  sendVal(val, 32);
  delay(remoteSendDelay);

  
  if (val != DOWN && val != UP)
    sendVal(val, 32);
}

void FloodLightRemote::sendVal(unsigned long val, int bits) {
  ir.sendNEC(val, bits);
  delay(remoteSendDelay);
}

void  FloodLightRemote::sendAndPrintCommand(BUTTON irButton, char *result, bool useName, int length){
    irCommand cmd = getCommand(irButton); 
    Serial.print(formatCommandForDisplay(result, cmd, true, length));
    sendVal(cmd.code);
}

void FloodLightRemote::testAllButtons() {
    char buffer[100];
    int length = getMaxLen()+1;

    Serial.println();
    irCommand cmd;

    sendAndPrintCommand(ON, buffer, true, length);
    sendAndPrintCommand(OFF, buffer, true, length);
    sendAndPrintCommand(ON, buffer, true, length);
    Serial.println();

    // Listing cols cols then rows because FLASH, STROBE, FADE AND SMOOTH do not play well with other commands
    for(int c = 0; c < REMOTE_COLS; c++) {
        for(int r = 0; r < REMOTE_ROWS; r++) {
            cmd = commands[r][c];
            Serial.print(formatCommandForDisplay(buffer, cmd, true, length));
            if (!(r == 0 && (c == 2 || c == 3))) {
                sendVal(cmd.code); //skipping OFF, done above
            }
        }
            Serial.println();    
    }
    sendAndPrintCommand(WHITE, buffer, true, length);
}

bool FloodLightRemote::sendButtonCommand(BUTTON irButton) {
    irCommand cmd = getCommand(irButton);
    sendVal(cmd.code);
    delay(remoteSendDelay);
}
