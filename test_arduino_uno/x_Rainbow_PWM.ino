//Creates a rainbow out of mixed colors PWM
void Rainbow_PWM (unsigned long duration, int pause)
{
  byte LUT[40];
  byte Frame[96];
  byte counter = 0;
  unsigned long end_time  = 0;
  
  duration = millis() + (duration * 1000);
     
  //Create LUT 
  for (byte k=0; k<8; k++) 
  {
              LUT[k     ] = color(7  ,k  ,0  );  // R: 7     ;  G: 0-->7 ; B: 0 
              LUT[8  + k] = color(7-k,7  ,0  );  // R: 7-->0 ;  G: 7     ; B: 0
    if (k<4) {LUT[16 + k] = color(0  ,7  ,k  );} // R: 0     ;  G: 7     ; B: 0-->3
              LUT[20 + k] = color(0  ,7-k,3  );  // R: 0     ;  G: 7-->0 ; B: 3
              LUT[28 + k] = color(k  ,0  ,3  );  // R: 0-->7 ;  G: 0     ; B: 3
    if (k<4) {LUT[36 + k] = color(7  ,0  ,3-k);} // R: 7     ;  G: 0     ; B: 3-->0
  } 
    
  do
  {
    if (counter == 40) {counter = 0;}
  
    //Frame erstellen
    for (byte i=0; i<8; i++)
    {
      for (byte j=0; j<8; j++)
      {
        if ((counter+i) < 40) {Frame[j*12+i] =  LUT[counter+i];} else {Frame[j*12+i] =  LUT[counter+i-40];}
      } 
    }
        
    //Draw Frame
    end_time = millis() + ((unsigned long) pause);
    Display_one_Frame_PWM(Frame, end_time);
    counter++;
  
  } while (millis() <= duration);
}

