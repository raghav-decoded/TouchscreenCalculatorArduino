#include <TouchScreen.h>

#include <SD.h>
#include <SPI.h>          // f.k. for Arduino-1.5.2
#include "Adafruit_GFX.h"// Hardware-specific library
#include <MCUFRIEND_kbv.h>
MCUFRIEND_kbv tft;


/*______Define LCD pins (I have asigned the default values)_______*/
#define YP A1  // must be an analog pin, use "An" notation!
#define XM A2  // must be an analog pin, use "An" notation!
#define YM 7   // can be a digital pin
#define XP 6   // can be a digital pin
#define LCD_CS A3
#define LCD_CD A2
#define LCD_WR A1
#define LCD_RD A0
#define LCD_RESET A4
/*_______End of defanitions______*/

/*______Assign names to colors and pressure_______*/
#define BLACK   0x0000 
#define BLUE   0x001F 
#define RED   0xF800 
#define GREEN  0x07E0 
#define CYAN   0x07FF 
#define PINK 0xF81F 
#define YELLOW  0xFFE0 
#define WHITE   0xFFFF 

#define MINPRESSURE 200
#define MAXPRESSURE 1000
/*_______Assigned______*/

/*____Calibrate TFT LCD_____*/
#define TS_MINX 125
#define TS_MINY 85
#define TS_MAXX 965
#define TS_MAXY 905
/*______End of Calibration______*/

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300); //300 is the sensitivity
//MCUFRIEND_kbv tft (LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET); //Start communication with LCD

String symbol[4][4] = {
  { "7", "8", "9", "/" },
  { "4", "5", "6", "*" },
  { "1", "2", "3", "-" },
  { "C", "0", "=", "+" }
};
 int X,Y;
double Num1,Num2,Number;// long Num1,Num2,Number;
 char action;
 boolean result = false;

 #if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)

#define SD_CS 53     // Set the chip select line to whatever you use

#else

// For Arduino Uno/Duemilanove, etc
//  connect the SD card with DI going to pin 11, DO going to pin 12 and SCK going to pin 13 (standard)
//  Then pin 10 goes to CS (or whatever you have set up)

#define SD_CS 10     // Set the chip select line to whatever you use (10 doesnt conflict with the library)

#endif

void setup() {
  Serial.begin(9600); //Use serial monitor for debugging
  tft.reset(); //Always reset at start
   uint32_t when = millis();
    //    while (!Serial) ;   //hangs a Leonardo until you connect a Serial
    if (!Serial) delay(5000);           //allow some time for Leonardo
    Serial.println("Serial took " + String((millis() - when)) + "ms to start");
    //    tft.reset();                 //hardware reset
    uint16_t ID = tft.readID(); //
    Serial.print("ID = 0x");
    Serial.println(ID, HEX);
    if (ID == 0xD3D3) ID = 0x9481; // write-only shield
//    ID = 0x9329;                             // force ID
    tft.begin(ID);

  tft.setRotation(2); // I just roated so that the power jack faces up - optional
  tft.fillScreen(BLACK);

  IntroScreen();
  
  draw_BoxNButtons(); 
}

void loop() {
TSPoint p = waitTouch();
X = p.y; Y = p.x;
//  Serial.print(X); Serial.print(','); Serial.println(Y);// + " " + Y);

DetectButtons();

if (result==true)
CalculateResult();

DisplayResult();   

  delay(300);
}

TSPoint waitTouch() {
  TSPoint p;
  do {
    p = ts.getPoint(); 
    pinMode(XM, OUTPUT);
    pinMode(YP, OUTPUT);
  } while((p.z < MINPRESSURE )|| (p.z > MAXPRESSURE));
  p.x = map(p.x, TS_MINX, TS_MAXX, 0, 320);
  p.y = map(p.y, TS_MINY, TS_MAXY, 0, 240);;
  return p;
}

void DetectButtons()
{
  
if(X<=60 && X >=0)  //Detecting Buttons on Column 1
  {
    if (Y>0 && Y<85) //If cancel Button is pressed
    {Serial.println ("Button Cancel"); Number=Num1=Num2=0; result=false;}
    
     if (Y>85 && Y<140) //If Button 1 is pressed
    {Serial.println ("Button 1"); 
    if (Number==0)
    Number=1.0;
    else
    Number = (Number*10.0) + 1.0; //Pressed twice
    }
    
     if (Y>140 && Y<192) //If Button 4 is pressed
    {Serial.println ("Button 4"); 
    if (Number==0)
    Number=4.0;
    else
    Number = (Number*10.0) + 4.0; //Pressed twice
    }
    
     if (Y>192 && Y<245) //If Button 7 is pressed
    {Serial.println ("Button 7");
    if (Number==0)
    Number=7.0;
    else
    Number = (Number*10.0) + 7.0; //Pressed twice
    } 
  }

 if(X>60 && X <=120) //Detecting Buttons on Column 2
  {
    if (Y>0 && Y<85)
    {Serial.println ("Button 0"); //Button 0 is Pressed
    if (Number==0)
    Number=0;
    else
    Number = (Number*10.0) + 0; //Pressed twice
    }
    
     if (Y>85 && Y<140)
    {Serial.println ("Button 2"); 
     if (Number==0)
    Number=2.0;
    else
    Number = (Number*10.0) + 2.0; //Pressed twice
    }
    
     if (Y>140 && Y<192)
    {Serial.println ("Button 5"); 
     if (Number==0)
    Number=5.0;
    else
    Number = (Number*10.0) + 5.0; //Pressed twic
    }
    
     if (Y>192 && Y<245)
    {Serial.println ("Button 8"); 
     if (Number==0)
    Number=8.0;
    else
    Number = (Number*10.0) + 8.0; //Pressed twic
    }   
  }

   if(X>120 && X < 180) //Detecting Buttons on Column 3
  {
    if (Y>0 && Y<85)
    {Serial.println ("Button Equal"); 
    Num2=Number;
    result = true;
    }
    
     if (Y>85 && Y<140)
    {Serial.println ("Button 3"); 
     if (Number==0)
    Number=3.0;
    else
    Number = (Number*10.0) + 3.0; //Pressed twice
    }
    
     if (Y>140 && Y<192)
    {Serial.println ("Button 6"); 
    if (Number==0)
    Number=6.0;
    else
    Number = (Number*10.0) + 6.0; //Pressed twice
    }
    
     if (Y>192 && Y<245)
    {Serial.println ("Button 9");
    if (Number==0)
    Number=9.0;
    else
    Number = (Number*10.0) + 9.0; //Pressed twice
    }   
  }
  /* if(X>180 && X <= 240 && Y > 245 && Y <=320){
    Serial.println("Scientific calculator");
    }*/

    if(X>180 && X <= 240)  //Detecting Buttons on Column 4
  {
    Num1 = Number;    
    Number =0;
    tft.setCursor(200, 20);
    tft.setTextColor(RED);
    if (Y>0 && Y<85)
    {Serial.println ("Addition"); action = 1; tft.println('+');}
     if (Y>85 && Y<140)
    {Serial.println ("Subtraction"); action = 2; tft.println('-');}
     if (Y>140 && Y<192)
    {Serial.println ("Multiplication"); action = 3; tft.println('*');}
     if (Y>192 && Y<245)
    {Serial.println ("Division"); action = 4; tft.println('/');}  
    delay(300);
  }  
}

void CalculateResult()
{
  if (action==1)
    Number = Num1+Num2;

  if (action==2)
    Number = Num1-Num2;

  if (action==3)
    Number = Num1*Num2;

  if (action==4)
    Number = Num1/Num2; 
}

void DisplayResult()
{
    tft.fillRect(0, 0, 240, 80, BLACK);  //clear result box
    tft.setCursor(10, 20);
    tft.setTextSize(4);
    tft.setTextColor(WHITE);
    tft.println(Number); //update new value
}

void IntroScreen()
{
  tft.setCursor (30, 120);
  tft.setTextSize (3);
  tft.setTextColor(RED);
  tft.println("ARITHMETIC");
  tft.setCursor (30, 160);
  tft.println("CALCULATOR");
  tft.setCursor (30, 220);
  tft.setTextSize (2);
  tft.setTextColor(BLUE);
  tft.println("...quardbaud");
  delay(1800);
}

void draw_BoxNButtons()
{
  //Draw the Result Box
  tft.fillRect(0, 0, 240, 80, BLACK);//Result

 //Draw First Column
  tft.fillRect  (0,260,60,60,RED); //C
  tft.fillRect  (0,200,60,60,BLUE);
  tft.fillRect  (0,140,60,60,BLUE);  
  tft.fillRect  (0,80,60,60,BLUE);

 //Draw Third Column  
  tft.fillRect  (120,260,60,60,YELLOW);//=
  tft.fillRect  (120,200,60,60,BLUE);
  tft.fillRect  (120,140,60,60,BLUE);
  tft.fillRect  (120,80,60,60,BLUE);

  //Draw Secound & Fourth Column  
  for (int b=260; b>=80; b-=60)
 { tft.fillRect  (180,b,60,60,GREEN); 
   tft.fillRect  (60,b,60,60,BLUE);}

  //Draw Horizontal Lines
  for (int h=80; h<=320; h+=60)
  tft.drawFastHLine(0, h, 240, PINK);

  //Draw Vertical Lines
  for (int v=0; v<=240; v+=60)
  tft.drawFastVLine(v, 80, 240, PINK);

  //Display keypad lables 
  for (int j=0;j<4;j++) {
    for (int i=0;i<4;i++) {
      tft.setCursor(22 + (60*i), 100 + (60*j));
      tft.setTextSize(3);
      tft.setTextColor(WHITE);
      tft.println(symbol[j][i]);
    }
  }
}
