//Procedure for an image (frame )in mixed colors (PWM)
void Display_one_Frame_PWM (int frame[6][8][3], unsigned long end_time)
{  
  byte SR[] = {0,0,0}; // Buffer for the 3 shift registers

  byte back =  0; // Return values ​​of the SPI

  do {      
    //Frame display       
    for (int j = 0; j < 255; j++) {             
      for (int row = 0; row < 6; row++) {           
        for (int col = 0; col < 8; col++) {
          
          if (frame[row][col][0] > 0 ) {
            frame[row][col][0]--;
            bitWrite(SR[0], 0, 1);
          }
          
          if (frame[row][col][1] > 0 ) {
            frame[row][col][1]--;
            bitWrite(SR[1], 0, 1);
          }
          
          if (frame[row][col][2] > 0 ) {
            frame[row][col][2]--;
            bitWrite(SR[2], 0, 1);
          }
        }
      
        byte rrrrrr = B0000000;
        bitSet(rrrrrr, row); // create byte for SR6 (current row)

        //describe shift registers   
        EnableSPI();       
        digitalWrite(latchPin, LOW);                           // LatchPin to ground, so LEDs are not blinking when "pushing through"
        back = SendRecSPI(rrrrrr);                                // Byte for SR6 slide out
        for (int l = 3; l > 0; l--){
          back = SendRecSPI(SR[l-1]);
        }  // 5 bytes for SR5 slide out to SR1
        digitalWrite(latchPin, HIGH);                          // LatchPin to HIGH, thus activating all SR
        DisableSPI();

        //Warten
        delayMicroseconds(1); // just wait         
      }   
    }
  } 
  while (millis() <= end_time); //Repeat the current frame until end_time is reached

  EnableSPI();       
  digitalWrite(latchPin, LOW);                                                
  for (byte l=0; l<6; l++){
    back = SendRecSPI(0);
  } 
  digitalWrite(latchPin, HIGH);                         
  DisableSPI();  
}

// Definition of procedures for the SPI
void EnableSPI(void)  {
  SPCR |= 1 << SPE;
}
void DisableSPI(void) {
  SPCR &= ~(1 << SPE);
}
void WaitSPIF(void) {
  while (! (SPSR & (1 << SPIF))) {
    continue;
  }
}

byte SendRecSPI(byte Dbyte) 
{        
  SPDR = Dbyte;
  WaitSPIF();
  return SPDR;
}

//Procedure calculates the color
byte color(byte r, byte g, byte b){
  return 32*r+4*g+b;
}



