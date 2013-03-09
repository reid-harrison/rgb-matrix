//Procedure for 3 balls of different colors that jump around randomly
void Jumping_RGB_Balls (unsigned long duration, int pause)
{
  byte coordinates_r[] = {0,0};
  byte coordinates_g[] = {0,0};
  byte coordinates_b[] = {0,0};
  
  unsigned long end_time  = 0;
  duration = millis() + (duration * 1000);
      
  do{          
      //Delete old frame
      for (byte column=0; column < 8; column++)
      {
        buffer_r[column] = 0;
        buffer_g[column] = 0;
        buffer_b[column] = 0;
      }
      
      //Calculate new frame
      coordinates_r[0]=random(8);
      coordinates_r[1]=random( 6);
      coordinates_g[0]=random(8);
      coordinates_g[1]=random( 6);
      coordinates_b[0]=random(8);
      coordinates_b[1]=random( 6);
      
      end_time = millis() + ((unsigned long) pause);
      
      bitSet(buffer_r[coordinates_r[0]],coordinates_r[1]); //r
      bitSet(buffer_g[coordinates_g[0]],coordinates_g[1]); //g
      bitSet(buffer_b[coordinates_b[0]],coordinates_b[1]); //b
   
      //Draw new frame
      Display_one_Frame(buffer_r, buffer_g, buffer_b, end_time);
      
    } while (millis() <= duration);
}

