//Generates random colors (PWM) on the entire matrix and can change color cyclically
void Random_PWM (unsigned long duration, int pause)
{

  int Frame[6][8][3];
  unsigned long end_time  = 0;
  
  duration = millis() + (duration * 1000);
  
  for (int row = 0; row < 6; row++) {
    for (int col = 0; col < 8; col++) {
      for (int color = 0; color < 3; color++) {
        Frame[row][col][color] = random(156) + 100;
      }
    }
  }
    
  do
  {
    //Create Frame
    //for (byte i=0; i<48; i++) {if (Frame[i] < 255) {Frame[i]++;} else {Frame[i] = 0;}}
    
    //Draw frame
    end_time = millis() + ((unsigned long) pause);
    Display_one_Frame_PWM(Frame, end_time);
    
  } while (millis() <= duration);
}

