# 4x4x4 LED Cube

This is a hand-soldered, reprogrammable 4x4x4 LED cube that uses Arduino - an open source IDE used to control user-made projects. This compact cube lights up with different patterns and is used to impress friends and family or as an amazing decoration for your desk. Of course, the hardware is not supplied in this repo - you will have to source that yourself.

# Main Components

  - ATmega328 microcontroller 
  - 2 x 74HC595N shift registers 
  - 4x4x4 cube matrix
  - 4 x BC327 transistors that control each layer of the cube.

Datasheets for the ATmega328 and for the 74HC595N are enclosed in the enclosed .zip file.

# How it Works - Quick Rundown
The LEDs are controlled by 2 shift registers that receive 16 bits from the programmed microcontroller. The shift registers then send out ON/OFF states to each layer of the cube’s cathodes before each corresponding layer anode is written to a LOW state in order to light the LEDs that were written to a LOW state. This process is rapidly repeated to give the impression that all LEDs in each layer are lit up simultaneously.

# Applications Used

  - **Arduino** - to create the LED cube sketch code
  - **Eagle** - to create the PCB and schematic
