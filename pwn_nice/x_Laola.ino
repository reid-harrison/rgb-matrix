//Procedure for a Laola wave
void Laola (byte counter[6], byte Direction[6], unsigned long duration, int pause)
{        
  unsigned long end_time  = 0;
  
  duration = millis() + (duration * 1000);
      
  do{          
      //Delete old picture
      for (byte column=0; column < 8; column++)
      {
        buffer_r[column] = 0;
        buffer_g[column] = 0;
        buffer_b[column] = 0;
      }
      
      //Calculate new image       
      for (int i=0; i<6; i++)
      {
        if (Direction[i] == 1) {counter[i] = counter[i] + 1;} else {counter[i] = counter[i] - 1;}
        if (counter[i]  >= 6) {Direction[i] = 0;}
        if (counter[i]  <= 0) {Direction[i] = 1;}
      }
                             
      bitSet(buffer_b[0],counter[0]  );
      bitSet(buffer_b[1],counter[0]  );
      bitSet(buffer_b[0],counter[0]+1);
      bitSet(buffer_b[1],counter[0]+1);

      bitSet(buffer_b[2],counter[1]  );
      bitSet(buffer_b[3],counter[1]  );
      bitSet(buffer_b[2],counter[1]+1);
      bitSet(buffer_b[3],counter[1]+1);
      bitSet(buffer_g[2],counter[1]  );
      bitSet(buffer_g[3],counter[1]  );
      bitSet(buffer_g[2],counter[1]+1);
      bitSet(buffer_g[3],counter[1]+1);     
     
      bitSet(buffer_g[4],counter[2]  );
      bitSet(buffer_g[5],counter[2]  );
      bitSet(buffer_g[4],counter[2]+1);
      bitSet(buffer_g[5],counter[2]+1);
    
      bitSet(buffer_r[6],counter[3]  );
      bitSet(buffer_r[7],counter[3]  );
      bitSet(buffer_r[6],counter[3]+1);
      bitSet(buffer_r[7],counter[3]+1);
      bitSet(buffer_g[6],counter[3]  );
      bitSet(buffer_g[7],counter[3]  );
      bitSet(buffer_g[6],counter[3]+1);
      bitSet(buffer_g[7],counter[3]+1);    

      
      end_time = millis() + ((unsigned long) pause);           
    
      //Display new image
      Display_one_Frame(buffer_r, buffer_g, buffer_b, end_time);
      
    } while (millis() <= duration);
}

