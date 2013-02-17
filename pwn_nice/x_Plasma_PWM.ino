//Billiard ball with a tail generated (PWM)
void Plasma_PWM (unsigned long duration, int pause)
{

  byte  Frame[48];
  int  Plasma[48];
  
  unsigned long end_time  = 0; 
  duration = millis() + (duration * 1000);
    
  byte LUT[40];

  int  pixel_color = 0;
  int paletteShift = 0;

  byte pixel_size_x = random(2,2);
  byte pixel_size_y = random(2,2);
   
  //Calculate LUT 
  for (byte k=0; k<8; k++) 
  {
              LUT[k     ] = color(7  ,k  ,0  );  // R: 7     ;  G: 0-->7 ; B: 0 
              LUT[8  + k] = color(7-k,7  ,0  );  // R: 7-->0 ;  G: 7     ; B: 0
    if (k<4) {LUT[16 + k] = color(0  ,7  ,k  );} // R: 0     ;  G: 7     ; B: 0-->3
              LUT[20 + k] = color(0  ,7-k,3  );  // R: 0     ;  G: 7-->0 ; B: 3
              LUT[28 + k] = color(k  ,0  ,3  );  // R: 0-->7 ;  G: 0     ; B: 3
    if (k<4) {LUT[36 + k] = color(7  ,0  ,3-k);} // R: 7     ;  G: 0     ; B: 3-->0
  }
    
  //Plasma geometry calculated once
  byte choise = random(2);
  
  for(byte x = 0; x < 8;  x++)
  {
    for(byte y = 0; y < 6; y++)
    {
      if(choise == 0) {pixel_color = (int) ((20+20*(sin(((float)x)/pixel_size_x)) + 20+20*(sin(((float)y)/pixel_size_y))) / 2);}
      if(choise == 1) {pixel_color = (int) ((20+20*(sin(((float)x)/pixel_size_x)) + 20+20*(sin(((float)y)/pixel_size_y))) / 2 + 20 + 20*sin(((float)x + (float)y)/ pixel_size_y));}
      
      Plasma[y*8+x] = pixel_color;        
    }
  }
 
 
  //main program   
  do
  {
    
    //Calculate frame
    paletteShift = (int) (millis() / 30);

    for (byte x = 0; x < 8; x++)
    {
      for (byte y = 0; y < 6; y++)
      {
        Frame[y*8+x] = LUT[(Plasma[y*8+x] + paletteShift) % 40];
      }
    }   
    
    //Draw new frame
    end_time = millis() + ((unsigned long) pause);
    //Display_one_Frame_PWM(Frame, end_time);
    
  } while (millis() <= duration);
}
