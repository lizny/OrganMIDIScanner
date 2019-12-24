#ifndef ORGAN_HARDWARE_H_
#define ORGAN_HARDWARE_H_

// Pedalboard

const uint8_t NUM_MCP = 2;
const uint8_t NUM_PINS = 16;
const uint8_t CHANNEL = 1;

// Great
/*
const uint8_t NUM_MCP = 4;
const uint8_t NUM_PINS = 16;
const uint8_t CHANNEL = 1;
*/
// Swell 
/*
const uint8_t NUM_MCP = 4;
const uint8_t NUM_PINS = 16;
const uint8_t CHANNEL = 2;
*/

const uint8_t pedalAddresses[2][16] = { // map port expanders and pins to MIDI notes
	{// ic2 address 0
		36,37,38,39, 40,41,42,43, 44,45,46,47, 48,49,50,51 
	}
	,  
	{  // ic2 address 1
	  52,53,54,55, 56,57,58,59, 60,61,62,63, 64,65,66,67 
	}
};




#endif
