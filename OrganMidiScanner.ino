#define DEBUG false

#include <Wire.h>
#include "SerialMIDI_Interface.h"
#include "Adafruit_MCP23017.h"
#include "Organ_Hardware.h"

// Connect pin #12 of the expander to Analog 5 (i2c clock)
// Connect pin #13 of the expander to Analog 4 (i2c data)
// Connect pins #15, 16 and 17 of the expander to ground (address selection)
// Connect pin #9 of the expander to 5V (power)
// Connect pin #10 of the expander to ground (common ground)
// Connect pin #18 through a ~10kohm resistor to 5V (reset pin, active low)

Adafruit_MCP23017  mcpArray [NUM_MCP];  
uint16_t prevPinState [NUM_MCP];
uint16_t prevDelayPinState [NUM_MCP];
uint16_t pinState [NUM_MCP];
uint16_t invertState = 65535;
unsigned long lastDebounceTime = 0; 
unsigned long debounceDelay = 50;

HairlessMIDI_Interface hm;

void setup() {  
  
  if (DEBUG){
    Serial.begin(115200);
    Serial.println("Starting up");
  }else{
 
    hm.begin();
    hm.send(NOTE_ON, CHANNEL, 65, 127);
    delay(100);
    hm.send(NOTE_OFF, CHANNEL, 65, 127);
  }
  for (int a = 0; a < NUM_MCP; a++){
    mcpArray[a].begin(a);  
    for (int p = 0; p < NUM_PINS; p++){
      mcpArray[a].pinMode(p, INPUT); // for output to solenoids change this
      mcpArray[a].pullUp(p, HIGH);  // turn on a 100K pullup internally
      pinState[a] = mcpArray[a].readGPIOAB(); 
      prevPinState[a] = pinState[a];
      prevDelayPinState[a] = pinState[a];    
    }
  }
     hm.begin();
    hm.send(NOTE_ON, CHANNEL, 64, 127);
    delay(50);
    hm.send(NOTE_OFF, CHANNEL, 64, 127);
}



void loop() {
  // The LED will 'echo' the button
// digitalWrite(13, mcp.digitalRead(0));

 key_refresh(0); 
}

const static int8_t falling = LOW - HIGH;
const static int8_t rising = HIGH - LOW;

const uint8_t velocity = 0b1111111; // Maximum velocity (0b1111111 = 0x7F = 127)
 
void key_refresh(int pin) // Check if the key state changed, and send a MIDI Note On or Off accordingly
{
//  bool state = mcp.digitalRead(pin) ^ invertState; // read the button state and invert it if "invert" is true
  bool changed = 0; 
 // if(DEBUG) { Serial.println("reading array");}
  
  for (int a = 0; a < NUM_MCP; a++){
    prevPinState[a] = pinState[a];
    pinState[a] = mcpArray[a].readGPIOAB() ^ invertState;    
    changed = (prevPinState[a] != pinState[a]);
    if(DEBUG) { 
    Serial.print("addr "); Serial.print(a,DEC); Serial.print(" ");
    Serial.println(pinState[a],BIN);}
    int8_t note;
    
    if (changed){   // one or more pins has changed since last time  
        // reset the debouncing timer
       lastDebounceTime = millis();
    }
    
    if ((millis() - lastDebounceTime) > debounceDelay) {
       if (DEBUG){ Serial.println ("debounce delay passed"); }
       int8_t state, prev; 
       for (int i = 0; i < 16; i++) {  // review each pin
        
        // read a pin, check against it's previous state     
        state = bitRead(pinState[a],i);
        prev  = bitRead(prevDelayPinState[a],i);
        int8_t stateChange = state - prev;
 
        note = pedalAddresses[a][i];
 
        if (stateChange == falling)
        { // Key is released
           if (DEBUG){
          Serial.print ("pin: ");
          Serial.print (i,DEC);
          Serial.print ("  note: ");
          Serial.print (note,DEC);
          Serial.println (" off");
          }
          else{ 
         
           hm.send(NOTE_OFF, CHANNEL, note, velocity);
          }
        }

        if (stateChange == rising) { // Key is pressed
         if (DEBUG){ 
          Serial.print ("pin: ");
          Serial.print (i,DEC);
          Serial.print ("  note: ");
          Serial.print (note,DEC);
          Serial.println (" on"); 
          }else{
            hm.send(NOTE_ON, CHANNEL, note, velocity);
          }
        } // rising state   
      } // each pin
      prevDelayPinState[a] = pinState[a];
    }// debounce delay ok
   if (DEBUG){  Serial.flush(); }
  } // each MCP
}
