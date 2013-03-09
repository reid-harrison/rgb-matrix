//Procedure, the colors of the rainbow columns way through applying and leaves
void Moving_Rainbow (unsigned long duration, int pause)
{
  byte temp = 0;    
  unsigned long end_time  = 0;
  duration = millis() + (duration * 1000);
  
  //start frame (determined by hand)
  buffer_r[0] = 0;   buffer_r[1] = 0;   buffer_r[2] = 255; buffer_r[3] = 255; buffer_r[4] = 255; buffer_r[5] = 0;   buffer_r[6] = 0;   buffer_r[7] = 0;
  buffer_g[0] = 255; buffer_g[1] = 255; buffer_g[2] = 255; buffer_g[3] = 0;   buffer_g[4] = 0;   buffer_g[5] = 0;   buffer_g[6] = 255; buffer_g[7] = 255;
  buffer_b[0] = 255; buffer_b[1] = 0;   buffer_b[2] = 0;   buffer_b[3] = 0;   buffer_b[4] = 255; buffer_b[5] = 255; buffer_b[6] = 255; buffer_b[7] = 0;  
      
  do{          
      //Calculate new image (Columns exchanged cyclically 1<--2 2<--3 3<--4 ... 11<--12 12<--1)          
      //red
        temp = buffer_r[0];
      //  for (byte i=0; i<7; i++){buffer_r[i] = buffer_r[i+1];}
       buffer_r[7] = temp;
 
       //green
      temp = buffer_g[0];
       //for (byte i=0; i<7; i++){buffer_g[i] = buffer_g[i+1];}
      buffer_g[7] = temp;
        
      //blue
      temp = buffer_b[0];
      //for (byte i=0; i<7; i++){buffer_b[i] = buffer_b[i+1];}
     buffer_b[7] = temp;            
      
      end_time = millis() + ((unsigned long) pause);
         
      //Draw new frame
      Display_one_Frame(buffer_r, buffer_g, buffer_b, end_time);
      
    } while (millis() <= duration);
}

