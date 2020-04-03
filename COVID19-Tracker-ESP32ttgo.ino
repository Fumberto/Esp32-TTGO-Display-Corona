
#include <TFT_eSPI.h> // Graphics and font library for ILI9341 driver chip
#include <SPI.h>
#include <stdlib.h>


#include <WiFi.h>
#include <HTTPClient.h>
 
const char* ssid = "votre SSID";
const char* password = "votre Password";
int nbcar=0;
int nblig=0;
int impr=1;
int jo=1;
int okc=1;
int idep=0;
int nbpays=5;

#define BUTTON_1        35
#define BUTTON_2        0

TFT_eSPI tft = TFT_eSPI();  // Invoke library

int flagX = 25;
int flagY = 100;
int zflagX = 0;
int zflagY = 180;
uint16_t  cellColor1 ;
String stringOne, stringTwo;
const int flagImgW = 60;
const int flagImgH = 69;
String lechar;
String country;


void setup() {
 
  Serial.begin(115200);
  delay(4000);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    //Serial.println("Connecting to WiFi..");
  }
  tft.init();
  tft.setRotation(0);

  stringTwo="0123456789ABCDEF";

  //Serial.println("Connected to the WiFi network");
  pinMode(BUTTON_1, INPUT_PULLUP);
  pinMode(BUTTON_2, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BUTTON_1),right, FALLING);
  attachInterrupt(digitalPinToInterrupt(BUTTON_2),left, FALLING);
 
}

int  donnev()
{
cellColor1 = stringTwo.indexOf(lechar);
  // Serial.println (cellColor1);
  return cellColor1;
}
 
void loop() {
 if (okc>0)
 {
  if ((WiFi.status() == WL_CONNECTED)) { //Check the current connection status
 
    HTTPClient http;
   switch (jo) {
      case 1:
      country="belgium";
      break;
      case 2:
      country="france";
      break;
      case 3:
      country="germany";
      break;
      case 4:
      country="netherlands";
      break;
      case 5:
      country="spain";
      break;
      case 6:
      country="us";
      break;
   }      
 
String lameteo="http://www.frescina.be/corona/liscorona2.php?l="+country;
 http.begin(lameteo); //Meteo Liege
    int httpCode = http.GET();                                        //Make the request
 
    if (httpCode > 0) { //Check for the returning code
        String payload ="";
        payload = http.getString();
        nbcar=payload.length();
        if (payload.substring((nbcar-2), nbcar)=="$$")
        {
          tft.fillScreen(TFT_BLACK);
          tft.setCursor(0, 0, 2);
          tft.setTextFont(2);
          nblig=0;
          nbcar=nbcar-2;
          idep=(60*69*4);
     drawBitmap1(payload, flagImgW, flagImgH, flagX, flagY, 2);
         
          
          for (int i =idep;i<nbcar;i++)
          {
            if (payload.substring(i, i+1)=="$")
            {
              impr=0;
              tft.setTextColor(TFT_RED);
            }
            if (payload.substring(i, i+1)=="#")
            {
              impr=0;
              tft.setTextColor(TFT_WHITE);
            }
            if (payload.substring(i, i+1)=="*")
            {
              impr=0;
             // Serial.println("");
              tft.println("");
              nblig++;
          }
          if (impr>0)
          {
            tft.print(payload.substring(i, i+1));
          }
          else
          {
            impr=1;
          }
        }
        }    
      }
 
    else {
      Serial.println("Error on HTTP request");
    }
 
    http.end(); //Free the resources
    okc=0;
  }
 }
  delay(1000);
 }
 void right() 
 {
    if (okc<1)
    {
      okc=1;
       if (jo<nbpays)
      {
        jo=jo+1;
      }
      else
      {
        jo=1;
      }
    }
 }
  
 void left() 
 {
  if (okc<1)
  {
    okc=1;
    if (jo>0)
    {
      jo=jo-1;
    }
    else
    {
      jo=nbpays;
    }
  }
 }

 void drawBitmap1(const String img, int imgW, int imgH, int x, int y, int scale) 
{
  uint16_t cellColor;
  String curPix;
  char lecar;
  zflagX=0;
  zflagY=180;
  for (int i = 0; i < imgW*imgH*4; i++) {
     stringOne = img.substring(i, i+6);
   i=i+3 ; 

  lechar=stringOne.substring(1,0);
  cellColor = (donnev()*4096);
  lechar=stringOne.substring(2,1);
  cellColor = cellColor+(donnev()*256);
  lechar=stringOne.substring(3,2);
  cellColor=cellColor+(donnev()*16);
  lechar=stringOne.substring(4,3);
  cellColor=cellColor+donnev();

  zflagX++;
  if (zflagX>68)
  {
    zflagX=0;
    zflagY=zflagY+1;
  }
    tft.drawPixel(zflagX+32, zflagY, cellColor);
    
  }
}

 
