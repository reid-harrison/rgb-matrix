#define RED  color(7, 0, 0)
#define BLUE  color(0, 0, 7)
#define GREEN  color(0, 7, 0)

#define PURPLE  color(7, 0, 7)
#define CYAN  color(0, 7, 7)
#define YELLOW  color(7, 7, 0)

const byte COLORS[] = { RED, BLUE, GREEN, PURPLE, CYAN, YELLOW };

//Procedure for an image (frame )in mixed colors (PWM)
void Display_one_Frame_PWM(byte frame[6][8], unsigned long end_time) {  
  byte SR[] = {0,0,0}; // Buffer for the 5 shift registers

  byte back =  0; // Return values ​​of the SPI
  byte index = 0; // LED-index

  const byte LUT_rg[] = {
    B11111100, B10000000, B11000000, B10100000, B11010000, B10001010, B11100100, B10000000, B11010000, B10100000, B11001000, B10000000, B11110100, B10000000, B11000000, B10101010, B11010000, B10000000, B11100000, B10000000    };
  const byte LUT_bl[] = {
    B00001110, B00001000, B00001100, B00001000, B00001110, B00001000, B00001100, B00001000, B00001110, B00001000, B00001100, B00001000, B00001110, B00001000, B00001100, B00001000, B00001110, B00001000, B00001100, B00001000    };

  do {      
    //Frame display       
    for (byte j = 0; j < 20; j++) {             
      for (byte row = 0; row < 6; row++) {          
        //The RGB line 3 x 12 = 36 bits is divided into the 5 8-bit shift registers buffer (5x8 = 40 bits). The remaining 4 bits in the 5th SR buffers are filled with zeros.
        for (byte col = 0; col < 8; col++) {
          bitWrite(SR[0], col, bitRead(LUT_rg[j], (byte)((frame[row][col] >> 5))));
          
          bitWrite(SR[1], col, bitRead(LUT_rg[j], (byte)(((byte)(frame[row][col] << 3)) >> 5) ));
          
          bitWrite(SR[2], col, bitRead(LUT_bl[j], (byte)(((byte)(frame[row][col] << 6)) >> 6) ));
        } //SR0 contains 8 x red  
      
        byte ground = B0000000;
        bitSet(ground, row); // create byte for SR6 (current row)

          //describe shift registers   
        EnableSPI();       
        digitalWrite(latchPin, LOW);                           // LatchPin to ground, so LEDs are not blinking when "pushing through"
        back = SendRecSPI(ground);                                // Byte for SR6 slide out
        
        for (byte l = 3; l > 0; l--) {
          back = SendRecSPI(SR[l-1]);
        }  // 5 bytes for SR5 slide out to SR1
        
        digitalWrite(latchPin, HIGH);                          // LatchPin to HIGH, thus activating all SR
        DisableSPI();

        //Warten
       // delayMicroseconds(1); // just wait         
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

