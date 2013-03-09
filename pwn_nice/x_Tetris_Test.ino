
static byte shift = 2;
int shapeWidth = 3;
int currentRow = 5;

void Tetris_Test(int pause)
{
  unsigned long end_time = millis() + pause;
  
  if (shift == 2)
  {
     shift = 0; 
  }
  //delete old picture
  for (byte column=0; column < NUM_COLS; column++)
  {
    buffer_r[column] = 0;
    buffer_g[column] = 0;
    buffer_b[column] = 0;
  }

  
    
  for (byte j=0; j<shapeWidth; j++)
  {
 
          buffer_g[j] = pgm_read_byte_near(piece_Z + j + (shapeWidth * shift));
          buffer_b[j] = pgm_read_byte_near(piece_Z + j + (shapeWidth * shift));
      

  }


  Display_one_Frame(buffer_r, buffer_g, buffer_b, end_time);
  shift++;
  currentRow++;
}

