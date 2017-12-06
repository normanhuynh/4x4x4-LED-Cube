/*
Code written by Norman Huynh
Date Written: January 25, 2015
File name: LED_cube_PCB.ino
An all-in-one file that controls the prototype LED cube created on the breadboard */

/*---------------------------------------
Variable Declaration
---------------------------------------*/
// Cube anode pin mapping
int layer[4] = {10, 11, 12, 13}; //10 11 12 13 ] 8, 9, 10, 11

// 595 pin mapping
int dataPin = 4; // 4 5
int clockPin = 3; // 3 6
int latchPin = 2;  // 2 7                  
                    
// Animation constants
int layerAllOn = 0b00000000;                                 // Use twice in shiftOut16 to make all 16 LEDS in a layer on
int layerAllOff = 0b11111111;                                // same as above, but makes all 16 LEDs off instead

int allOnStates[] = {LOW, layerAllOn};                       // Lists used as input for 1 parameter for entire cube layer control for 1 ON/OFF state
int allOffStates[] = {HIGH, layerAllOff};


//-----------------------------//
// Animation Frames
//-----------------------------//
// bounce frames
int bounceAnodes[13][4] = { {LOW, HIGH, HIGH, HIGH}, {LOW, LOW, HIGH, HIGH}, {LOW, LOW, LOW, HIGH}, {LOW, LOW, LOW, LOW},
                            {HIGH, LOW, LOW, LOW}, {HIGH, HIGH, LOW, LOW}, {HIGH, HIGH, HIGH, LOW},{HIGH, HIGH, HIGH, LOW},
                            {HIGH, HIGH, LOW, LOW}, {HIGH, LOW, LOW, LOW}, {LOW, LOW, LOW, LOW}, {LOW, LOW, LOW, HIGH}, 
                            {LOW, LOW, HIGH, HIGH} };
int bounceCathodes[13][4][2] = { 
                              { {layerAllOn, layerAllOn}, {layerAllOn, layerAllOn}, {layerAllOn, layerAllOn}, {layerAllOn, layerAllOn} }, // [frame][layer][LEDs]
                              { {layerAllOn, layerAllOn}, {layerAllOn, layerAllOn}, {layerAllOn, layerAllOn}, {layerAllOn, layerAllOn} },
                              { {layerAllOn, layerAllOn}, {layerAllOn, layerAllOn}, {layerAllOn, layerAllOn}, {layerAllOn, layerAllOn} },
                              { {layerAllOn, layerAllOn}, {layerAllOn, layerAllOn}, {layerAllOn, layerAllOn}, {layerAllOn, layerAllOn} },
                              { {layerAllOn, layerAllOn}, {layerAllOn, layerAllOn}, {layerAllOn, layerAllOn}, {layerAllOn, layerAllOn} },
                              { {layerAllOn, layerAllOn}, {layerAllOn, layerAllOn}, {layerAllOn, layerAllOn}, {layerAllOn, layerAllOn} },
                              { {layerAllOn, layerAllOn}, {layerAllOn, layerAllOn}, {layerAllOn, layerAllOn}, {layerAllOn, layerAllOn} },
                              { {layerAllOn, layerAllOn}, {layerAllOn, layerAllOn}, {layerAllOn, layerAllOn}, {layerAllOn, layerAllOn} },
                              { {layerAllOn, layerAllOn}, {layerAllOn, layerAllOn}, {layerAllOn, layerAllOn}, {layerAllOn, layerAllOn} },
                              { {layerAllOn, layerAllOn}, {layerAllOn, layerAllOn}, {layerAllOn, layerAllOn}, {layerAllOn, layerAllOn} },
                              { {layerAllOn, layerAllOn}, {layerAllOn, layerAllOn}, {layerAllOn, layerAllOn}, {layerAllOn, layerAllOn} },
                              { {layerAllOn, layerAllOn}, {layerAllOn, layerAllOn}, {layerAllOn, layerAllOn}, {layerAllOn, layerAllOn} },
                              { {layerAllOn, layerAllOn}, {layerAllOn, layerAllOn}, {layerAllOn, layerAllOn}, {layerAllOn, layerAllOn} }
                              };
                              
// explode frames
int explosionAnodes[7][4] = { {HIGH, HIGH, HIGH, LOW}, {HIGH, HIGH, LOW, LOW}, {HIGH, LOW, LOW, LOW}, {LOW, LOW, LOW, LOW}, {HIGH, LOW, LOW, LOW}, 
                             {HIGH, HIGH, LOW, LOW}, {HIGH, HIGH, HIGH, LOW} };
     
int explosionCathodes[7][4][2] = { 
                                { {0b11111001, 0b10011111}, {0b11111111, 0b11111111}, {0b11111111, 0b11111111}, {0b11111111, 0b11111111} },
                                { {0b11111001, 0b10011111}, {0b11111001, 0b10011111}, {0b11111111, 0b11111111}, {0b11111111, 0b11111111} },
                                { {0b11111001, 0b10011111}, {0b11111001, 0b10011111}, {0b11111001, 0b10011111}, {0b11111111, 0b11111111} },
                                { {0b11111001, 0b10011111}, {0b11111001, 0b10011111}, {0b11111001, 0b10011111}, {0b00000110, 0b01100000} },
                                { {0b11111111, 0b11111111}, {0b10011111, 0b10011111}, {0b00000110, 0b01100000}, {0b00000110, 0b01100000} },
                                { {0b11111111, 0b11111111}, {0b11111111, 0b11111111}, {0b00000110, 0b01100000}, {0b00000110, 0b01100000} },
                                { {0b11111111, 0b11111111}, {0b11111111, 0b11111111}, {0b11111111, 0b11111111}, {0b00000110, 0b01100000} },
                                };
                                                               
//----------------------------------------------------//
// SETUP FUNCTION
//----------------------------------------------------//
void setup() {
  for (int anode = 0; anode <= 3; anode++) {
    pinMode(layer[anode], OUTPUT);
  }
  pinMode(dataPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(latchPin, OUTPUT);  
}

//----------------------------------------------------//
// Utilities - Functions that do useful stuff
//----------------------------------------------------//
void shiftOut16(int byte1, int byte2) {
  /* Calls x2 shiftOut functions to output 16 bits from a list with 2 elements,
  each representing a byte */
  digitalWrite(latchPin, LOW);
  
  shiftOut(dataPin, clockPin, MSBFIRST, byte1);
  shiftOut(dataPin, clockPin, MSBFIRST, byte2);
  
  digitalWrite(latchPin, HIGH);
}  

void allState(int layerState[]) {
  /* accepts list with 3 parameters, with state of anode, and 2 bytes of all ON or OFF state */
  for (int anode = 0; anode <= 3; anode++) {
    digitalWrite(layer[anode], layerState[0]);
    shiftOut16(layerState[1], layerState[1]);
  }
}
  
void layerWrite(int timeDelay, int anodesList[][4], int cathodesList[][4][2], int amtFrames) {  
  /* takes a frame delay rate, list of anode states, list of cathode states, and number of frames to repeat */  
  allState(allOffStates);
  for (int frame = 0; frame < amtFrames; frame++) {
      for (int anode = 0; anode <= 3; anode++) {
        for (int flat = 0; flat <= 3; flat++) {
          digitalWrite(layer[anode], HIGH);        
          shiftOut16(cathodesList[frame][flat][0], cathodesList[frame][flat][1]);        
          digitalWrite(layer[anode], anodesList[frame][anode]);
          delay(timeDelay);
       }
     }
  }
}
//----------------------------------------------------//
// Working Cube Animations
//----------------------------------------------------//                            
void upDown(int timeDelay, int anodesList[][4], int cathodesList[][4][2], int amtFrames) {
  layerWrite(timeDelay, anodesList, cathodesList, amtFrames);
}

void explosion(int timeDelay, int anodesList[][4], int cathodesList[][4][2], int amtFrames) {
  layerWrite(timeDelay, anodesList, cathodesList, amtFrames);
}

//----------------------------------------------------//
// MAIN LOOP
//----------------------------------------------------//
void loop() {
  explosion(5, explosionAnodes, explosionCathodes, 7);
  delay(3);
  upDown(5, bounceAnodes, bounceCathodes, 13);

}
  
