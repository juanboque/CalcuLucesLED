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

#include <TouchScreen.h> //touch library
#include <LCDWIKI_GUI.h> //Core graphics library
#include <LCDWIKI_KBV.h> //Hardware-specific library

//if the IC model is known or the modules is unreadable,you can use this constructed function
LCDWIKI_KBV my_lcd(ILI9486,A3,A2,A1,A0,A4); //model,cs,cd,wr,rd,reset
//if the IC model is not known and the modules is readable,you can use this constructed function
//LCDWIKI_KBV my_lcd(320,480,A3,A2,A1,A0,A4);//width,height,cs,cd,wr,rd,reset

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

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

bool bUnidad = true;

//display string
void show_string(uint8_t *str,int16_t x,int16_t y,uint8_t csize,uint16_t fc, uint16_t bc,boolean mode)
{
    my_lcd.Set_Text_Mode(mode);
    my_lcd.Set_Text_Size(csize);
    my_lcd.Set_Text_colour(fc);
    my_lcd.Set_Text_Back_colour(bc);
    my_lcd.Print_String(str,x,y);
}

//Check whether to press or not
boolean is_pressed(int16_t x1,int16_t y1,int16_t x2,int16_t y2,int16_t px,int16_t py)
{
    if((px > x1 && px < x2) && (py > y1 && py < y2))
    {
        return true;  
    } 
    else
    {
        return false;  
    }
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

void ShowSumatill_10(void)
{// como la suma sumará como mucho 10 los números a sumar tienen que cumplir este requisito
  int iRand1, iRand2;
  String str1, str2;
 
  iRand1 = random(0,10);
  iRand2 = random(0,(9-iRand1));

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
  my_lcd.Print_String("+",115,42); 
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
   ShowSumatill_10();
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

//  my_lcd.Set_Draw_color(RED);
//  my_lcd.Draw_Line(5, 230, 315, 230);//primera linea horizontal que es común a todas las areas
//  my_lcd.Draw_Line(5, 300, 315, 300);//segunda linea horizontal que es común a todas las areas
//  my_lcd.Draw_Line(5, 370, 315, 370);//tercera linea horizontal que es común a todas las areas
//  my_lcd.Draw_Line(5, 440, 315, 440);//cuarta linea horizontal que es común a todas las areas
//  my_lcd.Draw_Line(5, 230, 5, 440); //primera linea vertical que es común a todas las areas
//  my_lcd.Draw_Line(82, 230, 82, 440); //segunda linea vertical que es común a todas las areas
//  my_lcd.Draw_Line(158, 230, 158, 440); //tercera linea vertical que es común a todas las areas
//  my_lcd.Draw_Line(233, 230, 233, 440); //cuarta linea vertical que es común a todas las areas
  
  if (p.z > MINPRESSURE && p.z < MAXPRESSURE)
  {
    //p.x = my_lcd.Get_Display_Width()-map(p.x, TS_MINX, TS_MAXX, my_lcd.Get_Display_Width(), 0);
    //p.y = my_lcd.Get_Display_Height()-map(p.y, TS_MINY, TS_MAXY, my_lcd.Get_Display_Height(), 0);
    p.x = map(p.x, TS_MINX, TS_MAXX, my_lcd.Get_Display_Width(),0);
    p.y = map(p.y, TS_MINY, TS_MAXY, my_lcd.Get_Display_Height(),0);

    if ((p.x>5) && (p.x<82) && (p.y>230) && (p.y<300))
    {
      //he pulsado 0
      delay(250);
      if (bUnidad)
      {
        my_lcd.Print_String("0",180,115);
        bUnidad = false; 
      } else
      {
        my_lcd.Print_String("0",150,115);
      }
      p.x = 0;
      p.y = 0;
    } else if ((p.x>82) && (p.x<158) && (p.y>230) && (p.y<300))
    {
      //he pulsado 1
      delay(250);
      if (bUnidad)
      {
        my_lcd.Print_String("1",180,115);
        bUnidad = false; 
      } else
      {
        my_lcd.Print_String("1",150,115);
      }
      p.x = 0;
      p.y = 0;
    }else if ((p.x>158) && (p.x<233) && (p.y>230) && (p.y<300))
    {
      //he pulsado 2
      delay(250);
      if (bUnidad)
      {
        my_lcd.Print_String("2",180,115);
        bUnidad = false; 
      } else
      {
        my_lcd.Print_String("2",150,115);
      }
      p.x = 0;
      p.y = 0;
    }else if ((p.x>233) && (p.x<315) && (p.y>230) && (p.y<300))
    {
      //he pulsado 3
      delay(250);
      if (bUnidad)
      {
        my_lcd.Print_String("3",180,115);
        bUnidad = false; 
      } else
      {
        my_lcd.Print_String("3",150,115);
      }
      p.x = 0;
      p.y = 0;
    }else if ((p.x>5) && (p.x<82) && (p.y>300) && (p.y<370))
    {
      //he pulsado 4
      delay(250);
      if (bUnidad)
      {
        my_lcd.Print_String("4",180,115);
        bUnidad = false; 
      } else
      {
        my_lcd.Print_String("4",150,115);
      }
      p.x = 0;
      p.y = 0;
    }else if ((p.x>82) && (p.x<158) && (p.y>300) && (p.y<370))
    {
      //he pulsado 5
      delay(250);
      if (bUnidad)
      {
        my_lcd.Print_String("5",180,115);
        bUnidad = false; 
      } else
      {
        my_lcd.Print_String("5",150,115);
      }
      p.x = 0;
      p.y = 0;
    } else if ((p.x>158) && (p.x<233) && (p.y>300) && (p.y<370))
    {
      //he pulsado 6
      delay(250);
      if (bUnidad)
      {
        my_lcd.Print_String("6",180,115);
        bUnidad = false; 
      } else
      {
        my_lcd.Print_String("6",150,115);
      }
      p.x = 0;
      p.y = 0;
    } else if ((p.x>233) && (p.x<315) && (p.y>300) && (p.y<370))
    {
      //he pulsado 7
      delay(250);
      if (bUnidad)
      {
        my_lcd.Print_String("7",180,115);
        bUnidad = false; 
      } else
      {
        my_lcd.Print_String("7",150,115);
      }
      p.x = 0;
      p.y = 0;
    }else if ((p.x>5) && (p.x<82) && (p.y>370) && (p.y<440))
    {
      //he pulsado 8
      delay(250);
      if (bUnidad)
      {
        my_lcd.Print_String("8",180,115);
        bUnidad = false; 
      } else
      {
        my_lcd.Print_String("8",150,115);
      }
      p.x = 0;
      p.y = 0;
    }else if ((p.x>82) && (p.x<158) && (p.y>370) && (p.y<440))
    {
      //he pulsado 9
      delay(250);
      if (bUnidad)
      {
        my_lcd.Print_String("9",180,115);
        bUnidad = false; 
      } else
      {
        my_lcd.Print_String("9",150,115);
      }
      p.x = 0;
      p.y = 0;
    } else if ((p.x>158) && (p.x<233) && (p.y>300) && (p.y<370))
    {
      //he pulsado E
      
      p.x = 0;
      p.y = 0;
    } else if ((p.x>233) && (p.x<315) && (p.y>300) && (p.y<370))
    {
      //he pulsado C
      delay(250);
      
      p.x = 0;
      p.y = 0;
    }
    
  }
}
