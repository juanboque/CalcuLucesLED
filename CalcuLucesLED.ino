/***********************************************************************************
*This program is a demo of how to use the touch function in a phone GUI
*This demo was made for LCD modules with 8bit or 16bit data port.
*This program requires the the LCDKIWI library.

* File                : display_phonecall.ino
* Hardware Environment: Arduino UNO&Mega2560
* Build Environment   : Arduino

*Set the pins to the correct ones for your development shield or breakout board.
*This demo use the BREAKOUT BOARD only and use these 8bit data lines to the LCD,
*pin usage as follow:
*                  LCD_CS  LCD_CD  LCD_WR  LCD_RD  LCD_RST  SD_SS  SD_DI  SD_DO  SD_SCK 
*     Arduino Uno    A3      A2      A1      A0      A4      10     11     12      13                            
*Arduino Mega2560    A3      A2      A1      A0      A4      10     11     12      13                           

*                  LCD_D0  LCD_D1  LCD_D2  LCD_D3  LCD_D4  LCD_D5  LCD_D6  LCD_D7  
*     Arduino Uno    8       9       2       3       4       5       6       7
*Arduino Mega2560    8       9       2       3       4       5       6       7 

*Remember to set the pins to suit your display module!
*
* @attention
*
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
* TIME. AS A RESULT, QD electronic SHALL NOT BE HELD LIABLE FOR ANY
* DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
* FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE 
* CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
**********************************************************************************/

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

#include <TouchScreen.h> //touch library
#include <LCDWIKI_GUI.h> //Core graphics library
#include <LCDWIKI_KBV.h> //Hardware-specific library

//if the IC model is known or the modules is unreadable,you can use this constructed function
LCDWIKI_KBV my_lcd(ILI9486,A3,A2,A1,A0,A4); //model,cs,cd,wr,rd,reset
//if the IC model is not known and the modules is readable,you can use this constructed function
//LCDWIKI_KBV my_lcd(320,480,A3,A2,A1,A0,A4);//width,height,cs,cd,wr,rd,reset

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1:
#define LED_PIN    23

// How many NeoPixels are attached to the Arduino?
#define LED_COUNT  17

// NeoPixel brightness, 0 (min) to 255 (max)
#define BRIGHTNESS 100

                             /*  r     g    b */
#define BLACK        0x0000  /*   0,   0,   0 */
#define BLUE         0x001F  /*   0,   0, 255 */
#define RED          0xF800  /* 255,   0,   0 */
#define GREEN        0x07E0  /*   0, 255,   0 */
#define CYAN         0x07FF  /*   0, 255, 255 */
#define MAGENTA      0xF81F  /* 255,   0, 255 */
#define YELLOW       0xFFE0  /* 255, 255,   0 */
#define WHITE        0xFFFF  /* 255, 255, 255 */
#define NAVY         0x000F  /*   0,   0, 128 */
#define DARKGREEN    0x03E0  /*   0, 128,   0 */
#define DARKCYAN     0x03EF  /*   0, 128, 128 */
#define MAROON       0x7800  /* 128,   0,   0 */
#define PURPLE       0x780F  /* 128,   0, 128 */
#define OLIVE        0x7BE0  /* 128, 128,   0 */
#define LIGHTGREY    0xC618  /* 192, 192, 192 */
#define DARKGREY     0x7BEF  /* 128, 128, 128 */
#define ORANGE       0xFD20  /* 255, 165,   0 */
#define GREENYELLOW  0xAFE5  /* 173, 255,  47 */
#define PINK         0xF81F  /* 255,   0, 255 */

/******************* UI details */
#define BUTTON_R 35 //the radius of button 
#define BUTTON_SPACING_X 35 //the horizontal distance between button
#define BUTTON_SPACING_Y 10  //the vertical distance between button
#define EDG_Y 10 //lower edge distance 
#define EDG_X 20 //left and right distance

#define YP A3  // must be an analog pin, use "An" notation!
#define XM A2  // must be an analog pin, use "An" notation!
#define YM 9   // can be a digital pin
#define XP 8   // can be a digital pin

//touch sensitivity for X
#define TS_MINX 906
#define TS_MAXX 116

//touch sensitivity for Y
#define TS_MINY 92
#define TS_MAXY 952

// We have a status line for like, is FONA working
#define STATUS_X 10
#define STATUS_Y 65

//touch sensitivity for press
#define MINPRESSURE 10
#define MAXPRESSURE 1000

#define DELAY_PRESS 175

#define PREMIO 5

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

int iResReal = 0, iResUser = 15;
bool bSumaNivel = false;
bool bRestas = false;
bool bSuma = true;

bool bUnidad = false;
bool bDecena = false;
bool bCentena = false;

unsigned int iTotelResforLED = 0;

//display string
void show_string(uint8_t *str,int16_t x,int16_t y,uint8_t csize,uint16_t fc, uint16_t bc,boolean mode)
{
    my_lcd.Set_Text_Mode(mode);
    my_lcd.Set_Text_Size(csize);
    my_lcd.Set_Text_colour(fc);
    my_lcd.Set_Text_Back_colour(bc);
    my_lcd.Print_String(str,x,y);
}


//display the main menu
void show_keyboard(void)
{
//    uint16_t i;
//   for(i = 0;i < sizeof(phone_button)/sizeof(button_info);i++)
//   {
//      my_lcd.Set_Draw_color(phone_button[i].button_colour);
//      my_lcd.Fill_Circle(phone_button[i].button_x, phone_button[i].button_y, BUTTON_R);
//      show_string(phone_button[i].button_name,phone_button[i].button_x-strlen(phone_button[i].button_name)*phone_button[i].button_name_size*6/2+phone_button[i].button_name_size/2+1,phone_button[i].button_y-phone_button[i].button_name_size*8/2+phone_button[i].button_name_size/2+1,phone_button[i].button_name_size,phone_button[i].button_name_colour,BLACK,1);
//   }

//    my_lcd.Set_Draw_color(BLUE);// Aquí se muestra los sumandos!!!
//    my_lcd.Fill_Round_Rectangle(270, 10, 320, 100, 5);  
    my_lcd.Set_Draw_color(BLACK);// Aquí se muestra los sumandos!!!
    my_lcd.Fill_Round_Rectangle(110, 10, 215, 160, 5);
  
    my_lcd.Set_Draw_color(WHITE);
    my_lcd.Fill_Circle(45, 265, 30);
    show_string("0",35,250,4,BLACK, WHITE,1);

    my_lcd.Set_Draw_color(WHITE);
    my_lcd.Fill_Circle(120, 265, 30);
    show_string("1",110,250,4,BLACK, WHITE,1);

    my_lcd.Set_Draw_color(WHITE);
    my_lcd.Fill_Circle(195, 265, 30);
    show_string("2",185,250,4,BLACK, WHITE,1);

    my_lcd.Set_Draw_color(WHITE);
    my_lcd.Fill_Circle(270, 265, 30);
    show_string("3",260,250,4,BLACK, WHITE,1);

    my_lcd.Set_Draw_color(WHITE);
    my_lcd.Fill_Circle(45, 335, 30);
    show_string("4",35,320,4,BLACK, WHITE,1);
    
    my_lcd.Set_Draw_color(WHITE);
    my_lcd.Fill_Circle(120, 335, 30);
    show_string("5",110,320,4,BLACK, WHITE,1);

    my_lcd.Set_Draw_color(WHITE);
    my_lcd.Fill_Circle(195, 335, 30);
    show_string("6",185,320,4,BLACK, WHITE,1);

    my_lcd.Set_Draw_color(WHITE);
    my_lcd.Fill_Circle(270, 335, 30);
    show_string("7",260,320,4,BLACK, WHITE,1);

    my_lcd.Set_Draw_color(WHITE);
    my_lcd.Fill_Circle(45, 405, 30);
    show_string("8",35,390,4,BLACK, WHITE,1);
    
    my_lcd.Set_Draw_color(WHITE);
    my_lcd.Fill_Circle(120, 405, 30);
    show_string("9",110,390,4,BLACK, WHITE,1);

    my_lcd.Set_Draw_color(WHITE);
    my_lcd.Fill_Circle(195, 405, 30);
    show_string("E",185,390,4,BLACK, WHITE,1);

    my_lcd.Set_Draw_color(WHITE);
    my_lcd.Fill_Circle(270, 405, 30);
    show_string("C",260,390,4,BLACK, WHITE,1);

    my_lcd.Set_Text_Mode(3);
    my_lcd.Set_Text_Size(2);
    my_lcd.Set_Text_colour(WHITE);    
    my_lcd.Print_String("INFO@XKEMATIC.COM",60,450);
}

void ShowSumatill_100(void)
{
  int iRand1, iRand2;
  String str1, str2;
 
  iRand1 = random(0,50);
  iRand2 = random(0,50);
  iResReal = iRand2 + iRand1;

  str1 = iRand1;
  str2 = iRand2;
  my_lcd.Set_Draw_color(BLACK);// Aquí se muestra los sumandos!!!
  my_lcd.Fill_Round_Rectangle(110, 10, 215, 100, 5);  
  my_lcd.Set_Text_Mode(3);
  my_lcd.Set_Text_Size(5);
  my_lcd.Set_Text_colour(WHITE);
  my_lcd.Set_Text_Back_colour(BLACK);
  if (iRand1 >= 10)
  {
      my_lcd.Print_String(str1,150,20); 
  } else
  {
      my_lcd.Print_String(str1,180,20); 
  } 
  my_lcd.Print_String("+",115,42); 
  if (iRand2 >= 10)
  {
      my_lcd.Print_String(str2,150,65); 
  } else
  {
      my_lcd.Print_String(str2,180,65); 
  } 
  my_lcd.Set_Draw_color(WHITE);
  my_lcd.Draw_Line(120, 105, 215, 105);
  my_lcd.Draw_Line(120, 106, 215, 106);
  my_lcd.Draw_Line(120, 107, 215, 107);
  my_lcd.Draw_Line(120, 108, 215, 108);  
}

void ShowSumatill_10(void)
{// como la suma sumará como mucho 10 los números a sumar tienen que cumplir este requisito
  int iRand1, iRand2;
  String str1, str2;
 
  iRand1 = random(0,10);
  iRand2 = random(0,(9-iRand1));
  iResReal = iRand2 + iRand1;

  str1 = iRand1;
  str2 = iRand2;
  my_lcd.Set_Draw_color(BLACK);// Aquí se muestra los sumandos!!!
  my_lcd.Fill_Round_Rectangle(110, 10, 215, 100, 5);  
  my_lcd.Set_Text_Mode(3);
  my_lcd.Set_Text_Size(5);
  my_lcd.Set_Text_colour(WHITE);
  my_lcd.Set_Text_Back_colour(BLACK);
  if (iRand1 >= 10)
  {
      my_lcd.Print_String(str1,150,20); 
  } else
  {
      my_lcd.Print_String(str1,180,20); 
  }
  my_lcd.Print_String("+",115,42); 
  if (iRand2 >= 10)
  {
      my_lcd.Print_String(str2,150,65); 
  } else
  {
      my_lcd.Print_String(str2,180,65); 
  }
  
  my_lcd.Set_Draw_color(WHITE);
  my_lcd.Draw_Line(120, 105, 215, 105);
  my_lcd.Draw_Line(120, 106, 215, 106);
  my_lcd.Draw_Line(120, 107, 215, 107);
  my_lcd.Draw_Line(120, 108, 215, 108);
}

void ShowRestasEasy_10(void)
{// como la suma sumará como mucho 10 los números a sumar tienen que cumplir este requisito
  int iRand1, iRand2;
  String str1, str2;
 
  iRand1 = random(0,10);
  iRand2 = random(0,iRand1);
  iResReal = iRand1 - iRand2;

  str1 = iRand1;
  str2 = iRand2;
  my_lcd.Set_Draw_color(BLACK);// Aquí se muestra los sumandos!!!
  my_lcd.Fill_Round_Rectangle(110, 10, 215, 100, 5);  
  my_lcd.Set_Text_Mode(3);
  my_lcd.Set_Text_Size(5);
  my_lcd.Set_Text_colour(WHITE);
  my_lcd.Set_Text_Back_colour(BLACK);
  if (str1 == "10")
  {
      my_lcd.Print_String(str1,150,20); 
  } else
  {
      my_lcd.Print_String(str1,180,20); 
  }
  my_lcd.Print_String("-",115,42); 
  if (str2 == "10")
  {
      my_lcd.Print_String(str2,150,65); 
  } else
  {
      my_lcd.Print_String(str2,180,65); 
  }
  
  my_lcd.Set_Draw_color(WHITE);
  my_lcd.Draw_Line(120, 105, 215, 105);
  my_lcd.Draw_Line(120, 106, 215, 106);
  my_lcd.Draw_Line(120, 107, 215, 107);
  my_lcd.Draw_Line(120, 108, 215, 108);
}

                            
void setup(void) 
{
  Serial.begin(9600);
   my_lcd.Init_LCD();
   Serial.println(my_lcd.Read_ID(), HEX);
   my_lcd.Fill_Screen(BLUE); 
   show_keyboard();
   Menuoperacion();
   ShowSumatill_10();
     // These lines are specifically to support the Adafruit Trinket 5V 16 MHz.
  // Any other board, you can remove this part (but no harm leaving it):
  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
    clock_prescale_set(clock_div_1);
  #endif
  // END of Trinket-specific code.

  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(BRIGHTNESS); // Set BRIGHTNESS to about 1/5 (max = 255)
}

void Menuoperacion()
{
  my_lcd.Set_Text_Mode(3);
  my_lcd.Set_Text_Size(2);
  my_lcd.Set_Text_colour(WHITE);
  my_lcd.Set_Draw_color(BLUE);// Aquí se muestra los suma!!!
  my_lcd.Fill_Round_Rectangle(230, 10, 315, 125, 5);
  if (bSuma)
  {
    my_lcd.Print_String("+Nivel",240,20);
    my_lcd.Print_String("Restas",240,60);
    my_lcd.Set_Draw_color(NAVY);
    my_lcd.Fill_Round_Rectangle(230, 90, 310, 125, 5);
    my_lcd.Print_String("Sumas",240,100); 
  } else if (bRestas)
  {
    my_lcd.Print_String("+Nivel",240,20);
    my_lcd.Set_Draw_color(NAVY);
    my_lcd.Fill_Round_Rectangle(230, 50, 315, 85, 5);
    my_lcd.Print_String("Restas",240,60);
    my_lcd.Print_String("Sumas",240,100); 
  } else if (bSumaNivel)
  {
    my_lcd.Set_Draw_color(NAVY);
    my_lcd.Fill_Round_Rectangle(230, 10, 315, 45, 5);
    my_lcd.Print_String("+Nivel",240,20);
    my_lcd.Print_String("Restas",240,60);
    my_lcd.Print_String("Sumas",240,100); 
  }
}

void ProcesaNumeros (unsigned int uiNum)
{
      String sNum = String(uiNum);
      delay(DELAY_PRESS);
      if (bUnidad == false)
      {
        iResUser = uiNum;
        my_lcd.Set_Draw_color(BLACK);// Aquí se muestra los sumandos!!!
        my_lcd.Fill_Round_Rectangle(110, 110, 215, 160, 5);
        my_lcd.Print_String(sNum,180,115);
        bUnidad = true;
      } else if (bDecena == false)
      {
        iResUser = iResUser + (uiNum*10);
        bDecena = true;
        my_lcd.Print_String(sNum,150,115);
      }else if (bCentena == false)
      {
        bCentena = true;
        iResUser = iResUser + (uiNum*100);
        my_lcd.Print_String(sNum,120,115);
      }
}

void loop(void)
{
  
  digitalWrite(13, HIGH);
  digitalWrite(52, HIGH);
  TSPoint p = ts.getPoint();
  digitalWrite(13, LOW);
  digitalWrite(52, LOW);
  
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
  
  if (p.z > MINPRESSURE && p.z < MAXPRESSURE)
  {
    p.x = map(p.x, TS_MINX, TS_MAXX, my_lcd.Get_Display_Width(),0);
    p.y = map(p.y, TS_MINY, TS_MAXY, my_lcd.Get_Display_Height(),0);

    if ((p.x>230) && (p.x<310) && (p.y>90) && (p.y<125))
    {
        //Si se pulsa aquí quiere hacer sumas
        bSuma = true;
        bRestas = false;
        bSumaNivel = false;
        Menuoperacion();
        ShowSumatill_10();
    }
    else if ((p.x>230) && (p.x<320) && (p.y>10) && (p.y<45))
    {
      // Más Nível de Suma, sumas hasta 200 llevándose
      bSumaNivel = true;
      bSuma = false;
      bRestas = false;
      Menuoperacion();
      ShowSumatill_100();
    } else if ((p.x>230) && (p.x<315) && (p.y>=50) && (p.y<85))
    {// si ha pulsado aquí se quieren hacer restas!
        bRestas = true;
        bSumaNivel = false;
        bSuma = false;
        Menuoperacion();
        ShowRestasEasy_10();
    }
    else if ((p.x>5) && (p.x<82) && (p.y>230) && (p.y<300))
    {
      //he pulsado 0
      ProcesaNumeros (0);
    } else if ((p.x>82) && (p.x<158) && (p.y>230) && (p.y<300))
    {
      //he pulsado 1
      ProcesaNumeros (1);
    }else if ((p.x>158) && (p.x<233) && (p.y>230) && (p.y<300))
    {
      //he pulsado 2
      ProcesaNumeros (2);
    }else if ((p.x>233) && (p.x<315) && (p.y>230) && (p.y<300))
    {
      //he pulsado 3
      ProcesaNumeros (3);
    }else if ((p.x>5) && (p.x<82) && (p.y>300) && (p.y<370))
    {
      //he pulsado 4
      ProcesaNumeros (4);
    }else if ((p.x>82) && (p.x<158) && (p.y>300) && (p.y<370))
    {
      //he pulsado 5
      ProcesaNumeros (5);
    } else if ((p.x>158) && (p.x<233) && (p.y>300) && (p.y<370))
    {
      //he pulsado 6
      ProcesaNumeros (6);
    } else if ((p.x>233) && (p.x<315) && (p.y>300) && (p.y<370))
    {
      //he pulsado 7
      ProcesaNumeros (7);
    }else if ((p.x>5) && (p.x<82) && (p.y>370) && (p.y<440))
    {
      //he pulsado 8
      ProcesaNumeros (8);
    }else if ((p.x>82) && (p.x<158) && (p.y>370) && (p.y<440))
    {
      //he pulsado 9
      ProcesaNumeros (9);
    } else if ((p.x>158) && (p.x<233) && (p.y>370) && (p.y<440))
    {
      //he pulsado E
      delay(DELAY_PRESS);
      if (iResUser == iResReal)
      {
        my_lcd.Set_Draw_color(BLUE);// Aquí se muestra los sumandos!!!
        my_lcd.Fill_Round_Rectangle(10, 220, 330, 165, 5);  
        my_lcd.Set_Draw_color(GREEN);// Aquí se muestra los sumandos!!!
        my_lcd.Fill_Round_Rectangle(110, 220, 215, 165, 5);  
        my_lcd.Set_Draw_color(WHITE);// Aquí se muestra los sumandos!!!
        my_lcd.Print_String("OK!",130,175);
        delay(1250);
        my_lcd.Set_Draw_color(BLACK);// Aquí se muestra los sumandos!!!
        my_lcd.Fill_Round_Rectangle(110, 110, 215, 160, 5);
        my_lcd.Set_Draw_color(BLUE);// Aquí se muestra los sumandos!!!
        my_lcd.Fill_Round_Rectangle(10, 220, 330, 165, 5);
        pulseWhite(5);
        if (bSumaNivel)
        {
          ShowSumatill_100();
        } else if (bRestas)
        {
          ShowRestasEasy_10();
        } else if (bSuma) 
        {
          ShowSumatill_10();  
        }
        iTotelResforLED++;
        if (iTotelResforLED == PREMIO)
        {
          // llama a funcion que enciende los LED´s. BONUS
          whiteOverRainbow(75, 5);
          pulseWhite(5);
          rainbowFade2White(3, 3, 1);
          iTotelResforLED=0;
        }
        ApagarLuces();
      } else 
      {
        Serial.print("iResUser: ");Serial.println(iResUser);
        Serial.print("iResReal: ");Serial.println(iResReal);
        my_lcd.Set_Draw_color(BLACK);// Aquí se muestra los sumandos!!!
        my_lcd.Fill_Round_Rectangle(110, 110, 215, 160, 5);  

        my_lcd.Set_Draw_color(RED);// Aquí se muestra los sumandos!!!
        my_lcd.Fill_Round_Rectangle(10, 220, 310, 165, 5);  
        my_lcd.Set_Draw_color(WHITE);// Aquí se muestra los sumandos!!!
        my_lcd.Set_Text_Size(3);
        my_lcd.Print_String("ERROR, OTRA VEZ.",20,180);
        my_lcd.Set_Text_Size(5);
      }
      bUnidad = false;
      bDecena = false;
      bCentena = false;
    } else if ((p.x>233) && (p.x<315) && (p.y>370) && (p.y<440))
    {
      //he pulsado C
      delay(DELAY_PRESS);    
      my_lcd.Set_Draw_color(BLACK);// Aquí se muestra los sumandos!!!
      my_lcd.Fill_Round_Rectangle(110, 110, 215, 160, 5);
      bUnidad = false;
      bDecena = false;
      bCentena = false;  
    }
  }
  p.x = 0;
  p.y = 0;
}

void ApagarLuces()
{
  
    for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
      strip.fill(strip.Color(0, 0, 0));
      strip.show();
      delay(10);
    }
}
// Fill strip pixels one after another with a color. Strip is NOT cleared
// first; anything there will be covered pixel by pixel. Pass in color
// (as a single 'packed' 32-bit value, which you can get by calling
// strip.Color(red, green, blue) as shown in the loop() function above),
// and a delay time (in milliseconds) between pixels.
void colorWipe(uint32_t color, int wait) {
  for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
    strip.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    strip.show();                          //  Update strip to match
    delay(wait);                           //  Pause for a moment
  }
}

void whiteOverRainbow(int whiteSpeed, int whiteLength) {

  if(whiteLength >= strip.numPixels()) whiteLength = strip.numPixels() - 1;

  int      head          = whiteLength - 1;
  int      tail          = 0;
  int      loops         = 3;
  int      loopNum       = 0;
  uint32_t lastTime      = millis();
  uint32_t firstPixelHue = 0;

  for(;;) { // Repeat forever (or until a 'break' or 'return')
    for(int i=0; i<strip.numPixels(); i++) {  // For each pixel in strip...
      if(((i >= tail) && (i <= head)) ||      //  If between head & tail...
         ((tail > head) && ((i >= tail) || (i <= head)))) {
        strip.setPixelColor(i, strip.Color(0, 0, 0, 255)); // Set white
      } else {                                             // else set rainbow
        int pixelHue = firstPixelHue + (i * 65536L / strip.numPixels());
        strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));
      }
    }

    strip.show(); // Update strip with new contents
    // There's no delay here, it just runs full-tilt until the timer and
    // counter combination below runs out.

    firstPixelHue += 40; // Advance just a little along the color wheel

    if((millis() - lastTime) > whiteSpeed) { // Time to update head/tail?
      if(++head >= strip.numPixels()) {      // Advance head, wrap around
        head = 0;
        if(++loopNum >= loops) return;
      }
      if(++tail >= strip.numPixels()) {      // Advance tail, wrap around
        tail = 0;
      }
      lastTime = millis();                   // Save time of last movement
    }
  }
}

void pulseWhite(uint8_t wait) {
  for(int j=0; j<256; j++) { // Ramp up from 0 to 255
    // Fill entire strip with white at gamma-corrected brightness level 'j':
    strip.fill(strip.Color(0, 150, 0));
    strip.show();
    delay(wait);
  }

  for(int j=0; j<256; j++) { // Ramp up from 0 to 255
    // Fill entire strip with white at gamma-corrected brightness level 'j':
    strip.fill(strip.Color(0, 0, 150));
    strip.show();
    delay(wait);
  }

    for(int j=0; j<256; j++) { // Ramp up from 0 to 255
    // Fill entire strip with white at gamma-corrected brightness level 'j':
    strip.fill(strip.Color(150, 0, 0));
    strip.show();
    delay(wait);
  }
}

void rainbowFade2White(int wait, int rainbowLoops, int whiteLoops) {
  int fadeVal=0, fadeMax=100;

  // Hue of first pixel runs 'rainbowLoops' complete loops through the color
  // wheel. Color wheel has a range of 65536 but it's OK if we roll over, so
  // just count from 0 to rainbowLoops*65536, using steps of 256 so we
  // advance around the wheel at a decent clip.
  for(uint32_t firstPixelHue = 0; firstPixelHue < rainbowLoops*65536;
    firstPixelHue += 256) {

    for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...

      // Offset pixel hue by an amount to make one full revolution of the
      // color wheel (range of 65536) along the length of the strip
      // (strip.numPixels() steps):
      uint32_t pixelHue = firstPixelHue + (i * 65536L / strip.numPixels());

      // strip.ColorHSV() can take 1 or 3 arguments: a hue (0 to 65535) or
      // optionally add saturation and value (brightness) (each 0 to 255).
      // Here we're using just the three-argument variant, though the
      // second value (saturation) is a constant 255.
      strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue, 255,
        255 * fadeVal / fadeMax)));
    }

    strip.show();
    delay(wait);

    if(firstPixelHue < 65536) {                              // First loop,
      if(fadeVal < fadeMax) fadeVal++;                       // fade in
    } else if(firstPixelHue >= ((rainbowLoops-1) * 65536)) { // Last loop,
      if(fadeVal > 0) fadeVal--;                             // fade out
    } else {
      fadeVal = fadeMax; // Interim loop, make sure fade is at max
    }
  }

  for(int k=0; k<whiteLoops; k++) {
    for(int j=0; j<256; j++) { // Ramp up 0 to 255
      // Fill entire strip with white at gamma-corrected brightness level 'j':
      strip.fill(strip.Color(strip.gamma8(j), strip.gamma8(j), strip.gamma8(j) ));
      strip.show();
    }
    delay(1000); // Pause 1 second
    for(int j=255; j>=0; j--) { // Ramp down 255 to 0
      strip.fill(strip.Color(strip.gamma8(j), strip.gamma8(j), strip.gamma8(j)));
      strip.show();
    }
  }

  delay(500); // Pause 1/2 second
}
