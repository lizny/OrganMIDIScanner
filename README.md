# OrganMIDIScanner
Arduino code to scan organ keyboard for press/release events and generate MIDI commands.  

MIDI encoding is heavily borrowed from https://github.com/tttapa/Control-Surface - if you are using shift registers you can just use that directly with no changes, so go check it out.

Uses MCP23017 port expanders for parallel keyboard inputs using IC2 bus.  

I am using Adafruit's Metro Mini for my controllers, but any arduino capable of serial USB will do.

Requires the Adafruit MCP23017 library:

https://github.com/adafruit/Adafruit-MCP23017-Arduino-Library


Edit the file Organ_Hardware.h and uncomment the section for the keyboard you are currently building. The default is Pedalboard,
comment that out and uncomment what you need. If you need more manuals, just copy the Swell section and update the channel number.

I am using the Hairless MIDI Serial Bridge https://projectgus.github.io/hairless-midiserial/  to receive the MIDI commands from USB and connect to your MIDI software on your computer.
If you are using a Mac, be sure to enable the IAC internal Midi driver. Open Spotlight and search for Audio MIDI setup to find the utility.
