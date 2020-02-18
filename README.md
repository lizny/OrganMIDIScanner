# OrganMIDIScanner
Arduino code to scan organ keyboard for press/release events and generate MIDI commands.  


Uses MCP23017 port expanders for parallel keyboard inputs using IC2 bus.  

I am using Adafruit's itsybitsy controller, which is capable of native MIDI
over USB.   This means I don't need any extra software, just plug it in and
Hauptwerk, GrandOrgue, Organteq, or whatever will detect it.  Each keyboard
will show up as a separate MIDI device, that works fine.

Requires the Adafruit MCP23017 library:

https://github.com/adafruit/Adafruit-MCP23017-Arduino-Library

Also the Wire library for i2c communication and MIDIUSB library available Arduino.cc

Edit the file Organ_Hardware.h and uncomment the section for the keyboard you are currently building. The default is Pedalboard,
comment that out and uncomment what you need. If you need more manuals, just copy the Swell section and update the channel number.

I have mine built so the manuals sense the magnet movingaway but the
pedals have the magnets moving into range when a key is pressed. To flip
the logic change the value of invert for the keyboard you are building in
the Organ_hardware.h file

