#ifndef ORGAN_HARDWARE_H_
#define ORGAN_HARDWARE_H_

#define LIGHTS false

// Pedalboard
/*
KEYBOARD = true;
uint16_t invertState = 0; 
const uint8_t NUM_MCP = 2;
const uint8_t NUM_PINS = 16;
const uint8_t CHANNEL = 0;  // shows up as 1
const uint8_t NUM_SHOES = 0;
*/
// Great
/*
KEYBOARD = true;
uint16_t invertState = 65535; 
const uint8_t NUM_MCP = 4;
const uint8_t NUM_PINS = 16;
const uint8_t CHANNEL = 1;
const uint8_t NUM_SHOES = 3;
*/

// Swell

uint16_t invertState = 65535;
//const uint8_t NUM_MCP = 4;
const uint8_t NUM_PINS = 16;
const uint8_t CHANNEL = 2;
const uint8_t NUM_SHOES = 0;


// Controls
// Expression Pedals
/*
const uint8_t CHANNEL = 15;
const uint8_t NUM_SHOES = 3;
uint16_t invertState = 65535;
const uint8_t NUM_MCP = 0;
const uint8_t NUM_PINS = 16;
*/

// Pistons
const uint8_t PCHANNEL = 15;
//uint16_t invertState = 65535;
//const uint8_t NUM_MCP = 2;
//const uint8_t NUM_PINS = 16;

// DATA

const uint8_t AnalogPins[] = { // used for expression shoes 
    A0,A1,A2                  // correspond to left, center, right shoes
  };
const uint8_t ShoeCodes[] = { 61,62,63};


const uint8_t noteAddresses[8][16] = { 
  
  // map port expanders and pins to MIDI notes
  // keyboards use chips 0-2 or 0-4  
  // pistons use chips 6-7
  
	{  // ic2 address 0
		36,37,38,39, 40,41,42,43, 44,45,46,47, 48,49,50,51 
	}
	,  
	{  // ic2 address 1
	  52,53,54,55, 56,57,58,59, 60,61,62,63, 64,65,66,67 
	}
  ,
  {  // ic2 address 2
    68,69,70,71, 72,73,74,75, 76,77,78,79, 80,81,82,83 
  }
  ,
  {  // ic2 address 3
    84,85,86,87, 88,89,90,91, 92,93,94,95, 96,97,98,99
  }
  ,
  {  // ic2 address 4
    36,37,38,39, 40,41,42,43, 44,45,46,47, 48,49,50,51 
  }
  ,  
  {  // ic2 address 5
    52,53,54,55, 56,57,58,59, 60,61,62,63, 64,65,66,67 
  }
  ,
  {  // ic2 address 6
    1,2,3,4, 5,6,7,8, 9,10,11,12, 13,14,15,16 
  }
  ,
  {  // ic2 address 7
    17,18,19,20, 21,22,23,24, 25,26,27,28, 29,30,31,32
  }
};

#endif
