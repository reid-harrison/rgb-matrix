// this code is obsolete, please use the code provided on
// the project page if you use one of my 8x8 rgb matrix diy kits.

/*
2008 - robert:aT:spitzenpfeil_d*t:org
*/


#define __spi_clock 13   // SCK - hardware SPI
#define __spi_latch 10
#define __spi_data 11    // MOSI - hardware SPI
#define __spi_data_in 12 // MISO - hardware SPI (unused)
#define __display_enable 9
#define __rows 6
#define __max_row __rows-1
#define __leds_per_row 8
#define __max_led __leds_per_row-1
#define __brightness_levels 32 // 0...15 above 28 is bad for ISR ( move to timer1, lower irq freq ! )
#define __max_brightness __brightness_levels-1
#define __fade_delay 4

#define __TIMER1_MAX 0xFFFF // 16 bit CTR
#define __TIMER1_CNT 0x0130 // 32 levels --> 0x0130; 38 --> 0x0157 (flicker)
#define __TIMER2_MAX 0xFF // 8 bit CTR
#define __TIMER2_CNT 0xFF // max 28 levels !
#include <avr/interrupt.h>   
#include <avr/io.h>
#include <stdint.h>

byte brightness_red[__leds_per_row][__rows]; 
byte brightness_green[__leds_per_row][__rows];
byte brightness_blue[__leds_per_row][__rows]; 

ISR(TIMER1_OVF_vect) {
  //TCNT2 = __TIMER2_MAX - __TIMER2_CNT; // precharge TIMER2 to maximize ISR time --> max led brightness
  TCNT1 = __TIMER1_MAX - __TIMER1_CNT;
  byte cycle;
  
  digitalWrite(__display_enable,LOW); // enable display inside ISR
  
  for(cycle = 0; cycle < __max_brightness; cycle++) {
    byte led;
    byte row = B00000000;    // row: current source. on when (1)
    byte red;    // current sinker when on (0)
    byte green;  // current sinker when on (0)
    byte blue;   // current sinker when on (0)

    for(row = 0; row <= __max_row; row++) {
      
      red = B11111111;    // off
      green = B11111111;  // off
      blue = B11111111;   // off
      
      for(led = 0; led <= __max_led; led++) {
        if(cycle < brightness_red[row][led]) {
          red &= ~(1<<led);
        }
        if(cycle < brightness_green[row][led]) {
          green &= ~(1<<led);
        }
        if(cycle < brightness_blue[row][led]) {
          blue &= ~(1<<led);
        }
      }

      digitalWrite(__spi_latch,LOW);
      spi_transfer(blue);
      spi_transfer(green);
      spi_transfer(red);
      spi_transfer(B00000001<<row);
      digitalWrite(__spi_latch,HIGH);
      digitalWrite(__spi_latch,LOW);
    }
  }

  /*
  // turn off all leds when ISR is not running
  // otherwise leds will flash to full brightness when 1111 is set, which
  // stays on outside the ISR !
  digitalWrite(__spi_latch,LOW);
  spi_transfer(B11111111); // blue off
  spi_transfer(B11111111); // green off
  spi_transfer(B11111111); // red off
  spi_transfer(B00000000); // rows off
  digitalWrite(__spi_latch,HIGH);
  digitalWrite(__spi_latch,LOW);
  */
  digitalWrite(__display_enable,HIGH);    // disable display outside ISR
}


void setup(void) {
  //Serial.begin(9600);
  randomSeed(555);
  byte ctr1;
  byte ctr2;
  
  pinMode(__spi_clock,OUTPUT);
  pinMode(__spi_latch,OUTPUT);
  pinMode(__spi_data,OUTPUT);
  pinMode(__spi_data_in,INPUT);
  pinMode(__display_enable,OUTPUT);
  digitalWrite(__spi_latch,LOW);
  digitalWrite(__spi_data,LOW);
  digitalWrite(__spi_clock,LOW);
    
  setup_hardware_spi();
  delay(10);
  set_matrix_rgb(0,0,0);
  //setup_timer2_ovf();  
  setup_timer1_ovf();
  // display enable/disable is done inside the ISR !
}


void loop(void) {
  
  int ctr;
  for(ctr=0; ctr < 4; ctr++) { 
    fader();
  }
  for(ctr=0; ctr < 2; ctr++) { 
    fader_hue();
  }
  for(ctr=0; ctr < 1000; ctr++) { 
    color_wave(30);
  }
  for(ctr=0; ctr < 100; ctr++) { 
    rainbow();
  }
  for(ctr=0; ctr < 10; ctr++) { 
    set_matrix_hue(80);
  }
  for(ctr=0; ctr < 1; ctr++) { 
    matrix_test();
  }
  set_matrix_rgb(0,0,0);
  for(ctr=0; ctr < 250; ctr++) { 
    matrix_heart(0);
  }
  for(ctr=0; ctr < 4; ctr++) { 
    matrix_heart_2();
  }
  for(ctr=0; ctr < 10000; ctr++) { 
    random_leds();
  }
  smile_blink(200,8,100);
  delay(2500);
  explode(300,150);
}


byte spi_transfer(byte data)
{
  SPDR = data;                    // Start the transmission
  while (!(SPSR & (1<<SPIF)))     // Wait the end of the transmission
  {
  };
  return SPDR;                    // return the received byte, we don't need that
}


void set_led_red(byte row, byte led, byte red) {
  brightness_red[row][led] = red;
}


void set_led_green(byte row, byte led, byte green) {
  brightness_green[row][led] = green;
}


void set_led_blue(byte row, byte led, byte blue) {
  brightness_blue[row][led] = blue;
}


void set_led_rgb(byte row, byte led, byte red, byte green, byte blue) {
  set_led_red(row,led,red);
  set_led_green(row,led,green);
  set_led_blue(row,led,blue);
}


void set_matrix_rgb(byte red, byte green, byte blue) {
  byte ctr1;
  byte ctr2;
  for(ctr2 = 0; ctr2 <= __max_row; ctr2++) {
    for(ctr1 = 0; ctr1 <= __max_led; ctr1++) {
      set_led_rgb(ctr2,ctr1,red,green,blue);
    }
  }
}


void set_row_rgb(byte row, byte red, byte green, byte blue) {
  byte ctr1;
  for(ctr1 = 0; ctr1 <= __max_led; ctr1++) {
      set_led_rgb(row,ctr1,red,green,blue);
  }
}


void set_column_rgb(byte column, byte red, byte green, byte blue) {
  byte ctr1;
  for(ctr1 = 0; ctr1 <= __max_row; ctr1++) {
      set_led_rgb(ctr1,column,red,green,blue);
  }
}


void set_row_hue(byte row, int hue) {
  byte ctr1;
  for(ctr1 = 0; ctr1 <= __max_led; ctr1++) {
      set_led_hue(row,ctr1,hue);
  }
}


void set_column_hue(byte column, int hue) {
  byte ctr1;
  for(ctr1 = 0; ctr1 <= __max_row; ctr1++) {
      set_led_hue(ctr1,column,hue);
  }
}


void set_matrix_hue(int hue) {
  byte ctr1;
  byte ctr2;
  for(ctr2 = 0; ctr2 <= __max_row; ctr2++) {
    for(ctr1 = 0; ctr1 <= __max_led; ctr1++) {
      set_led_hue(ctr2,ctr1,hue);
    }
  }
}



void fader(void) {
  byte ctr1;
  byte row;
  byte led;

  for(ctr1 = 0; ctr1 <= __max_brightness; ctr1++) {
    for(row = 0; row <= __max_row; row++) {
      for(led = 0; led <= __max_led; led++) {
        set_led_rgb(row,led,ctr1,ctr1,ctr1);
      }
    }
    delay(__fade_delay);
  }
  
  for(ctr1 = __max_brightness; (ctr1 >= 0) & (ctr1 != 255); ctr1--) {
    for(row = 0; row <= __max_row; row++) {
      for(led = 0; led <= __max_led; led++) {
        set_led_rgb(row,led,ctr1,ctr1,ctr1);
      }
    }
    delay(__fade_delay);
  }
}


void fader_hue(void) {
  int ctr1;
  byte row;
  byte led;

  for(ctr1 = 0; ctr1 < 360; ctr1=ctr1+3) {
    set_matrix_hue((float)(ctr1));
    delay(__fade_delay);
  }
}


void no_irq_fader(void) {
  byte ctr1;
  byte row;
  byte led;
  byte ctr2;
  
  for(ctr1 = 0; ctr1 <= __max_brightness; ctr1++) {
    for(row = 0; row <= __max_row; row++) {
      for(led = 0; led <= __max_led; led++) {
        set_led_rgb(row,led,ctr1,ctr1,ctr1);
      }
    }
    for(ctr2 = 0; ctr2 <= __fade_delay; ctr2++) {
      no_irq_pwm();
    }
  }
    
  for(ctr1 = __max_brightness; (ctr1 >= 0) & (ctr1 != 255); ctr1--) {
    for(row = 0; row <= __max_row; row++) {
      for(led = 0; led <= __max_led; led++) {
        set_led_rgb(row,led,ctr1,ctr1,ctr1);
      }
    }  
    for(ctr2 = 0; ctr2 <= __fade_delay; ctr2++) {
      no_irq_pwm();
    }
  }
}


void no_irq_pwm(void) {
  
  byte cycle;

  for(cycle = 0; cycle < __max_brightness; cycle++) {
    byte led;
    byte row = B00000000;    // row: current source. on when (1)
    byte red;    // current sinker when on (0)
    byte green;  // current sinker when on (0)
    byte blue;   // current sinker when on (0)

    for(row = 0; row <= __max_row; row++) {

      red = B11111111;    // off
      green = B11111111;  // off
      blue = B11111111;   // off
     
      for(led = 0; led <= __max_led; led++) {
        if(cycle < brightness_red[row][led]) {
          red &= ~(1<<led);
        }
        if(cycle < brightness_green[row][led]) {
          green &= ~(1<<led);
        }
        if(cycle < brightness_blue[row][led]) {
          blue &= ~(1<<led);
        }
      }

      digitalWrite(__spi_latch,LOW);
      spi_transfer(blue);
      spi_transfer(green);
      spi_transfer(red);
      spi_transfer(B00000001<<row);
      digitalWrite(__spi_latch,HIGH);
      digitalWrite(__spi_latch,LOW);
      
      /*
      Serial.print(B00000001<<row,BIN);
      Serial.print(" - ");
      Serial.print(red,BIN);
      Serial.print(" - ");
      Serial.print(green,BIN);
      Serial.print(" - ");
      Serial.println(blue,BIN);
      */
    }
    
  }
 
}


void set_led_hue(byte row, byte led, int hue) {

  // see wikipeda: HSV
  float S=100.0,V=100.0,s=S/100.0,v=V/100.0,h_i,f,p,q,t,R,G,B;
    
    hue = hue%360;
    h_i = hue/60;            
    f = (float)(hue)/60.0 - h_i;
    p = v*(1-s);
    q = v*(1-s*f);
    t = v*(1-s*(1-f));
    
    if      ( h_i == 0 ) { 
      R = v; 
      G = t; 
      B = p;
    }
    else if ( h_i == 1 ) { 
      R = q; 
      G = v; 
      B = p;
    }
    else if ( h_i == 2 ) { 
      R = p; 
      G = v; 
      B = t;
    }
    else if ( h_i == 3 ) { 
      R = p; 
      G = q; 
      B = v;
    }
    else if ( h_i == 4 ) { 
      R = t; 
      G = p; 
      B = v;
    }
    else                   { 
      R = v; 
      G = p; 
      B = q;
    }

    set_led_rgb(row,led,byte(R*(float)(__max_brightness)),byte(G*(float)(__max_brightness)),byte(B*(float)(__max_brightness)));
    
    /*
    Serial.println(byte(R*(float)(__max_brightness)),DEC);
    Serial.println(byte(G*(float)(__max_brightness)),DEC);
    Serial.println(byte(B*(float)(__max_brightness)),DEC);
    Serial.println("---");
    */    
}


void matrix_heart(int hue) {
  set_row_byte_hue(1,B00110110,hue);
  set_row_byte_hue(2,B01111111,hue);
  set_row_byte_hue(3,B01111111,hue);
  set_row_byte_hue(4,B00111110,hue);
  set_row_byte_hue(5,B00011100,hue);
  set_row_byte_hue(6,B00001000,hue);
}


void matrix_test(void) {
  byte ctr1;
  byte ctr2;
  int hue;
  
  for(hue = 0; hue < 360; hue=hue+32) {
    for(ctr2 = 0; ctr2 <= __max_row; ctr2++) {
      for(ctr1 = 0; ctr1 <= __max_led; ctr1++) {
        set_led_hue(ctr2,ctr1,hue);
        delay(5);
      }
    }
  }  
}


void matrix_heart_2(void) {
  int hue;
  for(hue = 0; hue < 360; hue=hue+16) {
    set_row_byte_hue(1,B00110110,hue);
    set_row_byte_hue(2,B01111111,hue);
    set_row_byte_hue(3,B01111111,hue);
    set_row_byte_hue(4,B00111110,hue);
    set_row_byte_hue(5,B00011100,hue);
    set_row_byte_hue(6,B00001000,hue);
    delay(3*__fade_delay);  
  }
}


void rainbow(void) {
  byte column;
  for(column = 0; column <= __max_led; column++) {
    set_column_hue(column,column*50);
  } 
}


void color_wave(uint8_t width) {
  uint8_t column;
  static uint16_t shift = 0;
  for(column = 0; column <= __max_led; column++) {
    set_column_hue(column,column*width+shift);
  } 
  shift++;
}


void random_leds(void) {
  set_led_hue((byte)(random(__rows)),(byte)(random(__leds_per_row)),(int)(random(360)));
}


void smile_on(int hue) {
  set_row_byte_hue(0,B00000000,hue);
  set_row_byte_hue(1,B01100110,hue);
  set_row_byte_hue(2,B01100110,hue);
  set_row_byte_hue(3,B00000000,hue);
  set_row_byte_hue(4,B00011000,hue);
  set_row_byte_hue(5,B10011001,hue);
  set_row_byte_hue(6,B01000010,hue);
  set_row_byte_hue(7,B00111100,hue);
}


void smile_off(int hue) {
  set_row_byte_hue(0,B00000000,hue);
  set_row_byte_hue(1,B00000000,hue);
  set_row_byte_hue(2,B01100110,hue);
  set_row_byte_hue(3,B00000000,hue);
  set_row_byte_hue(4,B00011000,hue);
  set_row_byte_hue(5,B10011001,hue);
  set_row_byte_hue(6,B01000010,hue);
  set_row_byte_hue(7,B00111100,hue);
}


void smile_blink(int hue, byte times, int pause) {
 byte ctr;
 for(ctr = 0; ctr < times; ctr++) {
   delay(pause);
   smile_on(hue);
   delay(pause);
   smile_off(hue);
   delay(pause);
   smile_on(hue);
 }
}


void explode(int hue, byte pause) {
  set_row_byte_hue(0,B00000000,hue);
  set_row_byte_hue(1,B00000000,hue);
  set_row_byte_hue(2,B00000000,hue);
  set_row_byte_hue(3,B00011000,hue);
  set_row_byte_hue(4,B00011000,hue);
  set_row_byte_hue(5,B00000000,hue);
  set_row_byte_hue(6,B00000000,hue);
  set_row_byte_hue(7,B00000000,hue);
  delay(pause);
  set_row_byte_hue(0,B00000000,hue);
  set_row_byte_hue(1,B00000000,hue);
  set_row_byte_hue(2,B00111100,hue);
  set_row_byte_hue(3,B00100100,hue);
  set_row_byte_hue(4,B00100100,hue);
  set_row_byte_hue(5,B00111100,hue);
  set_row_byte_hue(6,B00000000,hue);
  set_row_byte_hue(7,B00000000,hue);
  delay(pause);
  set_row_byte_hue(0,B00000000,hue);
  set_row_byte_hue(1,B01111110,hue);
  set_row_byte_hue(2,B01000010,hue);
  set_row_byte_hue(3,B01000010,hue);
  set_row_byte_hue(4,B01000010,hue);
  set_row_byte_hue(5,B01000010,hue);
  set_row_byte_hue(6,B01111110,hue);
  set_row_byte_hue(7,B00000000,hue);
  delay(pause);
  set_row_byte_hue(0,B11111111,hue);
  set_row_byte_hue(1,B10000001,hue);
  set_row_byte_hue(2,B10000001,hue);
  set_row_byte_hue(3,B10000001,hue);
  set_row_byte_hue(4,B10000001,hue);
  set_row_byte_hue(5,B10000001,hue);
  set_row_byte_hue(6,B10000001,hue);
  set_row_byte_hue(7,B11111111,hue);
  delay(pause);
  set_matrix_rgb(0,0,0);
}


void set_row_byte_hue(byte row, byte data_byte, int hue) {
  byte led;
  for(led = 0; led <= __max_led; led++) {
    if( (data_byte>>led)&(B00000001) ) {
      set_led_hue(row,led,hue);
    }
    else {
      set_led_rgb(row,led,0,0,0);
    }
  }
}


void setup_hardware_spi(void) {
  byte clr;
  // spi prescaler: 
  // SPI2X SPR1 SPR0
  //   0     0     0    fosc/4
  //   0     0     1    fosc/16
  //   0     1     0    fosc/64
  //   0     1     1    fosc/128
  //   1     0     0    fosc/2
  //   1     0     1    fosc/8
  //   1     1     0    fosc/32
  //   1     1     1    fosc/64
  SPCR |= ( (1<<SPE) | (1<<MSTR) ); // enable SPI as master
  //SPCR |= ( (1<<SPR1) ); // set prescaler bits
  SPCR &= ~ ( (1<<SPR1) | (1<<SPR0) ); // clear prescaler bits
  clr=SPSR; // clear SPI status reg
  clr=SPDR; // clear SPI data reg
  SPSR |= (1<<SPI2X); // set prescaler bits
  //SPSR &= ~(1<<SPI2X); // clear prescaler bits
}


void setup_timer2_ovf(void) {
  // Arduino runs at 16 Mhz...
  // Timer Settings, for the Timer Control Register etc. , thank you internets. ATmega168 !
  // Timer2 (8bit) Settings:
  // Timer2 affects delay() !
  // prescaler (frequency divider) values:   CS22    CS21   CS20
  //                                           0       0      0    stopped
  //                                           0       0      1      /1      62500 Hz
  //                                           0       1      0      /8       7813 Hz
  //                                           0       1      1      /32      1953 Hz
  //                                           1       0      0      /64       977 Hz
  //                                           1       0      1      /128      488 Hz
  //                                           1       1      0      /256      244 Hz
  //                                           1       1      1      /1024      61 Hz
  // irq_freq = 16MHz / ( 256 * prescaler )
  //
  // set irq to 61 Hz: CS22-bit = 1, CS21-bit = 1, CS20-bit = 1
  TCCR2B |= ( (1<<CS22) | (1<<CS21) | (1<<CS20));      
  //TCCR2B &= ~( (1<<CS20) );       
  // Use normal mode  
  TCCR2A &= ~( (1<<WGM21) | (1<<WGM20) );
  TCCR2B &= ~( (1<<WGM22) );  
  //Timer2 Overflow Interrupt Enable  
  TIMSK2 |= (1<<TOIE2);
  TCNT2 = __TIMER2_MAX - __TIMER2_CNT;                 
  // enable all interrupts
  sei(); 
}


void setup_timer1_ovf(void) {
  // Arduino runs at 16 Mhz...
  // Timer1 (16bit) Settings:
  // prescaler (frequency divider) values:   CS12    CS11   CS10
  //                                           0       0      0    stopped
  //                                           0       0      1      /1  
  //                                           0       1      0      /8  
  //                                           0       1      1      /64
  //                                           1       0      0      /256 
  //                                           1       0      1      /1024
  //                                           1       1      0      external clock on T1 pin, falling edge
  //                                           1       1      1      external clock on T1 pin, rising edge
  //
  TCCR1B &= ~ ( (1<<CS11) );
  TCCR1B |= ( (1<<CS12) | (1<<CS10) );      
  //normal mode
  TCCR1B &= ~ ( (1<<WGM13) | (1<<WGM12) );
  TCCR1A &= ~ ( (1<<WGM11) | (1<<WGM10) );
  //Timer1 Overflow Interrupt Enable  
  TIMSK1 |= (1<<TOIE1);
  TCNT1 = __TIMER1_MAX - __TIMER1_CNT;
  // enable all interrupts
  sei(); 
}
