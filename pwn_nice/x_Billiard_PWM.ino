//Billiard ball with a tail generated (PWM)
void Billiard_PWM (unsigned long duration, int pause)
{

  byte Frame[48];
  unsigned long end_time  = 0;
  
  byte Pos_X0;
  byte Pos_Y0;
  byte Pos_X1;
  byte Pos_Y1;
  byte Pos_X2;
  byte Pos_Y2;
  byte Dir_X ;
  byte Dir_Y ;
  byte Col = random(3);
  
  duration = millis() + (duration * 1000);
  
  Pos_X0 = random(8);
  Pos_Y0 = random(6);
  Pos_X1 = random(8);
  Pos_Y1 = random(6);
  Pos_X2 = random(8);
  Pos_Y2 = random(6);
  Dir_X  = random(2);
  Dir_Y  = random(2);
    
  do
  {
    //old frame deletion
    for (byte i=0; i<48; i++){Frame[i] = 36;}
    
    //Create frame
    Pos_X2 = Pos_X1;
    Pos_Y2 = Pos_Y1;
      
    Pos_X1 = Pos_X0;
    Pos_Y1 = Pos_Y0;
      
    if (Pos_X0 == 10) {Dir_X = 0;}
    if (Pos_X0 ==  0) {Dir_X = 1;}
    if (Pos_Y0 ==  5) {Dir_Y = 0;}
    if (Pos_Y0 ==  0) {Dir_Y = 1;}
    if (Dir_X == 0) {Pos_X0--;} else {Pos_X0++;}
    if (Dir_Y == 0) {Pos_Y0--;} else {Pos_Y0++;}
   
    switch (Col)
    {
      case 0: Frame[(Pos_Y0 * 8) + Pos_X0] = 5*32; Frame[(Pos_Y1 * 8) + Pos_X1] = 4*32; Frame[(Pos_Y2 * 8) + Pos_X2] = 3*32; break;
      case 1: Frame[(Pos_Y0 * 8) + Pos_X0] = 5*4 ; Frame[(Pos_Y1 * 8) + Pos_X1] = 4*4 ; Frame[(Pos_Y2 * 8) + Pos_X2] = 3*4 ; break;
      case 2: Frame[(Pos_Y0 * 8) + Pos_X0] =   3 ; Frame[(Pos_Y1 * 8) + Pos_X1] =   2 ; Frame[(Pos_Y2 * 8) + Pos_X2] =   1 ; break; 
    }   
    
    //Draw frame
    end_time = millis() + ((unsigned long) pause);
    Display_one_Frame_PWM(Frame, end_time);
    
  } while (millis() <= duration);
}
