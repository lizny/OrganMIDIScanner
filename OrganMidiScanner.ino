#define DEBUG true

#include <Wire.h>
#include "MIDIUSB.h"

#include "Adafruit_MCP23017.h"
#include "Organ_Hardware.h"

// Connect pin #12 of the expander to Analog 5 (i2c clock)
// Connect pin #13 of the expander to Analog 4 (i2c data)
// Connect pins #15, 16 and 17 of the expander to ground (address selection)
// Connect pin #9 of the expander to 5V (power)
// Connect pin #10 of the expander to ground (common ground)
// Connect pin #18 through a ~10kohm resistor to 5V (reset pin, active low)

void noteOn(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOn = {0x09, 0x90 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOn);
  MidiUSB.flush();
}

void noteOff(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOff = {0x08, 0x80 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOff);
  MidiUSB.flush();
}

void controlChange(byte channel, byte control, byte value) {
  // First parameter is the event type (0x0B = control change).
  // Second parameter is the event type, combined with the channel.
  // Third parameter is the control number number (0-119).
  // Fourth parameter is the control value (0-127).
  midiEventPacket_t event = {0x0B, 0xB0 | channel, control, value};
  MidiUSB.sendMIDI(event);
}
 
Adafruit_MCP23017  mcpArray [8];  
uint16_t prevPinState [8];
uint16_t prevDelayPinState [8];
uint16_t pinState [8];
unsigned long lastDebounceTime = 0; 
unsigned long lastPedalDebounceTime = 0; 
unsigned long debounceDelay = 50;


int led = 13;

void setup() {  
    Serial.begin(115200);

     pinMode(led, OUTPUT);
    if (DEBUG) { 
      delay (1000);
      noteOn(CHANNEL, 64, 127);
      controlChange(15, 66, 0);
       digitalWrite(led, HIGH);
         delay (1000);
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
    if (DEBUG) { 
      noteOff(CHANNEL, 64, 127); 
      controlChange(15, 66, 127);
      digitalWrite(led, LOW);
    }
}

void loop() {
 read_keyboard(); 
 read_expression(); 
 
 delay(10);
}
  
const static int8_t falling = LOW - HIGH;
const static int8_t rising = HIGH - LOW;

const uint8_t velocity = 0b1111111; // Maximum velocity (0b1111111 = 0x7F = 127)
  
int oldExpressionValue[3] = {0,0,0};

void read_expression(void) {
  for ( int i = 0; i < NUM_SHOES; i++){
    int sensorValue = analogRead(AnalogPins[i]);
    int expressionValue = sensorValue / 8 ;  // convert 0-1023 to 0-127
    if (expressionValue != oldExpressionValue[i]){
      controlChange(CHANNEL, ShoeCodes[i], expressionValue); 
      oldExpressionValue[i] = expressionValue;
    }
  }
}


void read_keyboard(void) 
{
  bool changed = 0; 
  for (int a = 0; a < NUM_MCP; a++){                       // for each port expander...
    prevPinState[a] = pinState[a];                         // store previous pin state
    pinState[a] = mcpArray[a].readGPIOAB() ;              // read each pin and 
    changed = (prevPinState[a] != pinState[a]);            // true if any pin on the chip is different
    int8_t note;
    
    if (changed){                                          
       lastDebounceTime = millis();                         // reset the debouncing timer - note we don't care which pin(s) changed
    }
    
    if ((millis() - lastDebounceTime) > debounceDelay) {
       int8_t state, prev; 
       for (int i = 0; i < 16; i++) {                       // state of all pins is stable, so review each pin
           
        state = bitRead(pinState[a],i);                     // read a pin 
        prev  = bitRead(prevDelayPinState[a],i);            // read that pin's previous state 
        int8_t stateChange = state - prev;                  // compare
 
        note = noteAddresses[a][i];                        // pull the correct note for this expander and pin from the key mapping array
 
        if (stateChange == rising) {                        // Key is pressed
           if (DEBUG) {  digitalWrite(led, HIGH); }
           noteOn( CHANNEL, note, 64);
        }  
        
        if (stateChange == falling)  {                      // Key is released
           if (DEBUG) {  digitalWrite(led, LOW); }
           noteOff( CHANNEL, note, 64);                    
        }
      }                                                     // end for each pin
      prevDelayPinState[a] = pinState[a];                   // reset debounce state
    }                                                       // end loop when debounce delay passed
  }                                                         // end for each MCP
}                                                           // end key refresh function
