//Draw a frame and repeat until frame time is over
void Display_one_Frame (byte buffer_r[], byte buffer_g[], byte buffer_b[], unsigned long end_time)
{
   byte SR[] = {0,0,0}; //Buffer for the 5 shift registers
   
   byte back = 0; // Return value of the SPI
    
   do{                   
        for (byte column=0; column < NUM_ROWS; column++) // For all 6 rows
        {
          //The RGB line 3 x 12 = 36 bits is divided into the 5 8-bit shift registers buffer (5x8 = 40 bits). The remaining 4 bits in the 5th SR buffers are filled with zeros.
          for (byte k=0; k<8 ; k++) {bitWrite(SR[0], k   , buffer_r[k  ] & (1 << column));} //SR1 contains 8x red        
          //for (byte k=0; k<4 ; k++) {bitWrite(SR[1], 3-k , buffer_r[k+8] & (1 << column));} //SR2 contains 4x red
          //for (byte k=0; k<4 ; k++) {bitWrite(SR[1], 7-k , buffer_b[k  ] & (1 << column));} //and 4x blue
          for (byte k=0; k<8 ; k++) {bitWrite(SR[1], k   , buffer_g[k] & (1 << column));} //SR3 contains 8x blue        
          for (byte k=0; k<8 ; k++) {bitWrite(SR[2], k   , buffer_b[k  ] & (1 << column));} //SR4 contains 8x green
          //for (byte k=0; k<4 ; k++) {bitWrite(SR[4], 3-k , buffer_g[k+8] & (1 << column));} //SR5 contains 4x green
          //for (byte k=0; k<4 ; k++) {bitWrite(SR[4], 7-k ,               0              );} //and 4x 0

          byte row = B0000000;
          bitSet(row, column);//Create byte for SR6 (current row)
          
          //Draw shift registers
          EnableSPI();          
            digitalWrite(latchPin, LOW);                          // LatchPin to ground, so LEDs are not blinking when "pushing through"
            back = SendRecSPI(row);                               // Byte for SR6 slide out
            for (byte j=3; j>0; j--){back = SendRecSPI(SR[j-1]);} // 5 bytes for SR5 slide out to SR1
            digitalWrite(latchPin, HIGH);                         // LatchPin to HIGH, thus activating all SR
          DisableSPI();
                                   
          //Warten
          delayMicroseconds(300); // wait         
        }
        
      } while (millis() <= end_time); // So lange den aktuellen Frame wiederholen bis FrameTime vorüber ist      
}
