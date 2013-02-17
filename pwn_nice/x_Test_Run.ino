//Procedure to test each LED
void Test_Run ()
{
  unsigned long end_time  = 0;
  
  //"scour" all red LEDs                  
  for (int column=0; column<8; column++)
  {    
    for (int row=0; row<6; row++)
    {
      //Delete old frame
      for (byte column=0; column < 8; column++)
      {
        buffer_r[column] = 0;
        buffer_g[column] = 0;
        buffer_b[column] = 0;
      }

      //Calculate new frame
      bitSet(buffer_r[column],row);
      end_time = millis() + 300;
         
      //Draw new frame
      Display_one_Frame(buffer_r, buffer_g, buffer_b, end_time);

    }      
  }
  
  //"scour" all green LEDs
  for (int column=0; column<8; column++)
  {    
    for (int row=0; row<6; row++)
    {
      //Delete old frame
      for (byte column=0; column < 8; column++)
      {
        buffer_r[column] = 0;
        buffer_g[column] = 0;
        buffer_b[column] = 0;
      }

      //Calculate new frame
      bitSet(buffer_g[column],row);
      end_time = millis() + 300;
         
      //Draw new frame
      Display_one_Frame(buffer_r, buffer_g, buffer_b, end_time);
    }      
  }

  //"scour" all blue LEDs
  for (int column=0; column<8; column++)
  {    
    for (int row=0; row<6; row++)
    {
      //Delete old frame
      for (byte column=0; column < 8; column++)
      {
        buffer_r[column] = 0;
        buffer_g[column] = 0;
        buffer_b[column] = 0;
      }

      //Calculate new frame
      bitSet(buffer_b[column],row);
      end_time = millis() + 300;
         
      //Draw new frame
      Display_one_Frame(buffer_r, buffer_g, buffer_b, end_time);
    }      
  }  
  
}
