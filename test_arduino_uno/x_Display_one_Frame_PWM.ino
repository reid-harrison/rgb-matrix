//Procedure for an image (frame )in mixed colors (PWM)
void Display_one_Frame_PWM (int frame[6][8][3], unsigned long end_time)
{  
  byte SR[] = {0,0,0}; // Buffer for the 3 shift registers

  byte back =  0; // Return values ​​of the SPI

  do {      
    //Frame display       
    for (int j = 0; j < 256; j++) {  
   
       byte led;
       byte row = B00000000;    // row: current source. on when (1)
    byte red;    // current sinker when on (0)
    byte green;  // current sinker when on (0)
    byte blue;   // current sinker when on (0)   
      
      
      
      for (int row = 0; row < 6; row++) {           
        
        red = B11111111;    // off
      green = B11111111;  // off
      blue = B11111111;   // off
      
      for(led = 0; led < 8; led++) {
        if(j < frame[row][led][0]) {
          red &= ~(1<<led);
        }
        if(j < frame[row][led][1]) {
          green &= ~(1<<led);
        }
        if(j < frame[row][led][2]) {
          blue &= ~(1<<led);
        }
      }
        /*for (int col = 0; col < 8; col++) {
          
          if (frame[row][col][0] > 0 ) {
            frame[row][col][0]--;
            bitWrite(SR[0], col, 1);
          } else {
            bitWrite(SR[0], col, 0);
          }
          
          //delay(50);
          
          if (frame[row][col][1] > 0 ) {
            frame[row][col][1]--;
            bitWrite(SR[1], col, 1);
          } else {
            bitWrite(SR[1], col, 0);
          }
          
          if (frame[row][col][2] > 0 ) {
            frame[row][col][2]--;
            bitWrite(SR[2], col, 1);
          } else {
            bitWrite(SR[2], col, 0);
          }
          
        }*/
      
        byte rrrrrr = B0000000;
        bitSet(rrrrrr, row); // create byte for SR6 (current row)
        
        
     /*   digitalWrite(__spi_latch,LOW);
      spi_transfer(blue);
      spi_transfer(green);
      spi_transfer(red);
      spi_transfer(B00000001<<row);
      digitalWrite(__spi_latch,HIGH);
      digitalWrite(__spi_latch,LOW);*/

        //describe shift registers   
        EnableSPI();       
        digitalWrite(latchPin, LOW);                           // LatchPin to ground, so LEDs are not blinking when "pushing through"
        back = SendRecSPI(rrrrrr);                                // Byte for SR6 slide out
        
        back = SendRecSPI(blue);
        back = SendRecSPI(green);
        back = SendRecSPI(red);
       /* for (int l = 3; l > 0; l--){
          back = SendRecSPI(SR[l-1]);
        }  // 5 bytes for SR5 slide out to SR1*/
        digitalWrite(latchPin, HIGH);                          // LatchPin to HIGH, thus activating all SR
        DisableSPI();

        //Warten
        //delayMicroseconds(100); // just wait         
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



