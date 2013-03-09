//Generates random colors (PWM) on the entire matrix and can change color cyclically
void Random_PWM (unsigned long duration, int pause)
{

  byte Frame[48];
  unsigned long end_time  = 0;
  
  duration = millis() + (duration * 1000);
  
  for (byte i=0; i<48; i++){Frame[i] = random(50,255);}
    
  do
  {
    //Create Frame
    for (byte i=0; i<48; i++) {if (Frame[i] < 255) {Frame[i]++;} else {Frame[i] = 0;}}
    
    //Draw frame
    end_time = millis() + ((unsigned long) pause);
    //Display_one_Frame_PWM(Frame, end_time);
    
  } while (millis() <= duration);
}


