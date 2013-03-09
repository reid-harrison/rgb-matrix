//Procedure for 2 moving lines of different color
//Line 1 runs back and forth horizontally and is red
//Line 2 runs vertically up and down and is green
void Moving_RGB_Lines (unsigned long duration, int pause)
{
  byte counter_r  = 0;
  byte counter_g  = 0;
  byte direction_r = 1;
  byte direction_g = 1;
        
  unsigned long end_time  = 0;
  
  duration = millis() + (duration * 1000);
      
  do{          
      //Clear old frame
      for (byte column=0; column < 8; column++)
      {
        buffer_r[column] = 0;
        buffer_g[column] = 0;
        buffer_b[column] = 0;
      }
      
      //Create new frame
      
      //red line running horizontally  
      if (direction_r ==   1) {counter_r = counter_r + 1;} else {counter_r = counter_r - 1;}
      if (counter_r  ==  7) {direction_r = 0;}
      if (counter_r  ==   0) {direction_r = 1;}
     
      //green line running vertically 
      if (direction_g ==   1) {counter_g = counter_g + 1;} else {counter_g = counter_g - 1;}
      if (counter_g  ==   5) {direction_g = 0;}
      if (counter_g  ==   0) {direction_g = 1;}
            
      end_time = millis() + ((unsigned long) pause);      
      
      buffer_r[counter_r] = B11111111; //Red   
      for (byte column=0; column < 8; column++){bitSet(buffer_g[column],counter_g);} //Green    
    
      //Draw new frame
      Display_one_Frame(buffer_r, buffer_g, buffer_b, end_time);
      
    } while (millis() <= duration);
}


