//The procedure can go through a text as scrolling text
void Display_String (int pause)
{  

  //Text to be displayed
  byte string[] = "Fuck off!";
  
  byte color = 0;
    
  const byte string_length = sizeof(string);
  
  //A virtual GRB matrix is created which the text fits into (each letter is 5 columns wide)
  byte textfield_r[(string_length*6)+24];
  byte textfield_g[(string_length*6)+24];
  byte textfield_b[(string_length*6)+24];

  byte current_letter = 0;
  unsigned long end_time  = 0;

  for (int i=0; i < ((string_length*6)+24); i++)
  {
    textfield_r[i] = 0;
    textfield_g[i] = 0;
    textfield_b[i] = 0;
  }

  //delete old picture
  for (byte column=0; column < NUM_COLS; column++)
  {
    buffer_r[column] = 0;
    buffer_g[column] = 0;
    buffer_b[column] = 0;
  }    
    
  //create a text field
  for (int i=0; i<(string_length-1); i++)
  {
    //Every character of current
    current_letter = string[i];
    color = 0;//random(6);
   
    for (byte j=0; j<5; j++)
    {
      switch (color)
      {
      case  0:
        //The current letter is red
        textfield_r[NUM_COLS+(i*6)+j] = pgm_read_byte_near(character_set + (((current_letter-32)*5)+j));
        textfield_g[NUM_COLS+(i*6)+j] = 0;
        textfield_b[NUM_COLS+(i*6)+j] = 0;
      break;

      case  1:
        //The current letter is green
        textfield_r[NUM_COLS+(i*6)+j] = 0;
        textfield_g[NUM_COLS+(i*6)+j] = pgm_read_byte_near(character_set + (((current_letter-32)*5)+j));
        textfield_b[NUM_COLS+(i*6)+j] = 0;
      break;
      
      case  2:
        //The current letter is blue
        textfield_r[NUM_COLS+(i*6)+j] = 0;
        textfield_g[NUM_COLS+(i*6)+j] = 0;
        textfield_b[NUM_COLS+(i*6)+j] = pgm_read_byte_near(character_set + (((current_letter-32)*5)+j));
      break;
      
      case  3:
        //The current letter is red & green
        textfield_r[NUM_COLS+(i*6)+j] = pgm_read_byte_near(character_set + (((current_letter-32)*5)+j));
        textfield_g[NUM_COLS+(i*6)+j] = pgm_read_byte_near(character_set + (((current_letter-32)*5)+j));
        textfield_b[NUM_COLS+(i*6)+j] = 0;
      break;
      
      case  4:
        //The current letter is red & blue
        textfield_r[NUM_COLS+(i*6)+j] = pgm_read_byte_near(character_set + (((current_letter-32)*5)+j));
        textfield_g[NUM_COLS+(i*6)+j] = 0;
        textfield_b[NUM_COLS+(i*6)+j] = pgm_read_byte_near(character_set + (((current_letter-32)*5)+j));
      break;
      
      case  5:
        //The current letter is green & blue
        textfield_r[NUM_COLS+(i*6)+j] = 0;
        textfield_g[NUM_COLS+(i*6)+j] = pgm_read_byte_near(character_set + (((current_letter-32)*5)+j));
        textfield_b[NUM_COLS+(i*6)+j] = pgm_read_byte_near(character_set + (((current_letter-32)*5)+j));
      break;
      }      
    }
    //textfield_r[(12+(i*6)+5)] = B00000000;
  }
  
  //Signature field run by LED matrix
  for (int counter=0; counter<((string_length*5)+string_length-1+NUM_COLS); counter++)
  {
    //create new picture
    for (byte column=0; column<NUM_COLS; column++)
    {
      buffer_r[column] = textfield_r[counter+column];
      buffer_g[column] = textfield_g[counter+column];
      buffer_b[column] = textfield_b[counter+column];
    }
    end_time = millis() + ((unsigned long) pause);
    
    //Draw current picture
    Display_one_Frame(buffer_r, buffer_g, buffer_b, end_time); 
  }
}

