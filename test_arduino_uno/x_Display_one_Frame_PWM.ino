//Procedure for an image (frame )in mixed colors (PWM)
void Display_one_Frame_PWM (byte frame[48], unsigned long end_time)
{  
  byte SR[] = {0,0,0}; // Buffer for the 3 shift registers

  byte back =  0; // Return values ​​of the SPI
  byte index = 0; // LED-index

  const byte LUT_rg[] = {
    B11111100, B10000000, B11000000, B10100000, B11010000, B10001010, B11100100, B10000000, B11010000, B10100000, B11001000, B10000000, B11110100, B10000000, B11000000, B10101010, B11010000, B10000000, B11100000, B10000000    };
  const byte LUT_bl[] = {
    B00001110, B00001000, B00001100, B00001000, B00001110, B00001000, B00001100, B00001000, B00001110, B00001000, B00001100, B00001000, B00001110, B00001000, B00001100, B00001000, B00001110, B00001000, B00001100, B00001000    };

  do
  {      
    //Frame display       
    for (int j = 0; j < 20; j++)
    {             
      for (int row = 0; row < 6; row++) // For all 6 rows
      {           
        //The RGB line 3 x 12 = 36 bits is divided into the 5 8-bit shift registers buffer (5x8 = 40 bits). The remaining 4 bits in the 5th SR buffers are filled with zeros.
        index = 8*row;
        for (int k = 0; k < 8; k++)
        {
          bitWrite(SR[0], k, bitRead(LUT_rg[j], (byte)((frame[index + k] >> 5))));
          
          //Serial.println((bitRead(LUT_rg[j], (byte)((frame[index + k] >> 5)))));
          

        } //SR1 contains 8 x red
        
        for (int k = 0; k < 8; k++)
        {
            bitWrite(SR[1] , k, bitRead(LUT_bl[j], (byte)(((byte)(frame[index+k] << 6)) >> 6) ));
            
            //Serial.println("Next");
            //Serial.println(bitRead(LUT_bl[j], (byte)(((byte)(frame[index+k] << 6)) >> 6) ));
        } //SR3 contains 8x blue        
        for (int k = 0; k < 8 ; k++) {
          bitWrite(SR[2], k, bitRead(LUT_rg[j], (byte)(((byte)(frame[index + k] << 3)) >> 5) ));
          //Serial.println("nnnnnn");
          //Serial.println(bitRead(LUT_rg[j], (byte)(((byte)(frame[index + k] << 3)) >> 5) ));
        } //SR4 contains 8x green         
      
        byte rrrrrr = B0000000;
        bitSet(rrrrrr, row); // create byte for SR6 (current row)

          //describe shift registers   
        EnableSPI();       
        digitalWrite(latchPin, LOW);                           // LatchPin to ground, so LEDs are not blinking when "pushing through"
        back = SendRecSPI(rrrrrr);                                // Byte for SR6 slide out
        for (byte l=3; l>0; l--){
          back = SendRecSPI(SR[l-1]);
        }  // 5 bytes for SR5 slide out to SR1
        digitalWrite(latchPin, HIGH);                          // LatchPin to HIGH, thus activating all SR
        DisableSPI();

        //Warten
        delayMicroseconds(1); // just wait         
      }   
    }
    
    Serial.println(SR[0]);
    Serial.println(SR[1]);
    Serial.println(SR[2]);
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



