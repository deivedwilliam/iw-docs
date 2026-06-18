#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <HardwareSerial.h>
#include <driver/gpio.h>
#include <TFT_eSPI.h> 
#include <WiFi.h>
#include <WiFiClient.h>
/* #include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h> */
#include "gameover.h" 
#include "noInternet.h"  
#include "sprites.h"
#include "joystick_ble.h"

#define PWR_HOLD GPIO_NUM_20
#define PLAY_ON 10
#define RMT_OUTPUT 21
#define LCD_BACKLIGHT GPIO_NUM_1
#define TFT_BL 0
#define LCD_CS 7
#define LCD_DC 2
#define LCD_RST 1
#define LCD_SDI 6
#define LCD_SCK 4

const uint16_t PIXEL_COUNT = 1;
const char WIFI_SSID [] = "WIFI-2C9E_2.4G";
const char WIFI_PASSWD [] = "30268504f";


Adafruit_NeoPixel NeoPixel(PIXEL_COUNT, RMT_OUTPUT, NEO_GRB + NEO_KHZ800);

TFT_eSPI    tft = TFT_eSPI(128, 160);        
TFT_eSprite img = TFT_eSprite(&tft);
TFT_eSprite img2 = TFT_eSprite(&tft);
TFT_eSprite e = TFT_eSprite(&tft);
TFT_eSprite e2 = TFT_eSprite(&tft);



const int pwmFreq = 5000;
const int pwmResolution = 8;
const int pwmLedChannelTFT = 0;  
const int dinoW = 33;
const int dinoH = 35;
float linesX[6];
int linesW[6];
float linesX2[6];
int linesW2[6];
float clouds[2] = {(float)random(0,80), (float) random(100,180)};
float bumps[2];
int bumpsF[2];
int eW=18;
int eH=38;

float eX[2]={(float)random(240,310), (float)random(380,460)};
int ef[2]={0,1};

float sped = 1;
int x = 30;
int y = 58;
float dir=-1.4;
bool pressed = 0;
int frames = 0;
int f = 0;
float cloudSpeed = 0.4;
bool gameRun = 1;
int score = 0;
int t = 0;
int press2 = 0;
int brightnes[6] = {70,100,130,160,200,220};
byte b = 1;

BLE_Client_Joystick BLE_Joystick;


void movement(int x, int y) {
  //Serial.printf("move(%d, %d)\r\n", x, y);
}

void button_A(bool state) {
  Serial.print("Button A ");
  if (state) {
    if(pressed == 0){
       pressed = true;

    f = 0;
    }
   
    Serial.println("pressed");
  } else {
 
    Serial.println("released");
  }
}

void button_B(bool pressed) {
  Serial.print("Button B ");
  if (pressed) {
    Serial.println("pressed");
  } else {
    Serial.println("released");
  }
}

void button_X(bool pressed) {
  Serial.print("Button X ");
  if (pressed) {
    Serial.println("pressed");
  } else {
    Serial.println("released");
  }
}

void button_Y(bool pressed) {
  Serial.print("Button Y ");
  if (pressed) {
    Serial.println("pressed");
  } else {
    Serial.println("released");
  }
}

void button_start(bool state) {
  Serial.print("Button start ");
  if (state) {
    if(gameRun == 0){
      sped = 1;
      x = 30;
      y = 58;
      dir = -1.4;
      pressed = 0;
      frames = 0;
      f = 0;
      cloudSpeed = 0.4;
      gameRun = 1;
      score = 0;
      t = 0;
      score = 0;
      for(int i=0;i<6;i++){
        linesX[i]=random(i*40,(i+1)*40);
        linesW[i]=random(1,14);
        linesX2[i]=random(i*40,(i+1)*40);
        linesW2[i]=random(1,14);
      }
      for(int n=0;n<2;n++){
        bumps[n]=random(n*90,(n+1)*120);
        bumpsF[n]=random(0,2);
      }
      eW=18;
      eH=38;
      eX[0] = (float)random(240,310);
      eX[1] = (float)random(380,460);
      ef[0] = 0;
      ef[1] = 1;
      clouds[0] = (float)random(0,80);
      clouds[1] = (float) random(100,180);

      tft.fillScreen(TFT_WHITE);  
    }
  } else {
    Serial.println("released");
  }
}

void button_select(bool pressed) {
  Serial.print("Button select ");
  if (pressed) {
    Serial.println("pressed");
  } else {
    Serial.println("released");
  }
}

void connection(bool connected) {
  Serial.print("Joystick is ");
  if (!connected) {
    Serial.println("not ");
  }
  Serial.println("connected");
}

void Blink_Task(void *pvParameters)
{
  float fade;
  float brightness;
  uint32_t color;
  int fadeVal = 0, fadeMax = 100;
  for (;;)
  {
    brightness = cos(fade + PI);
    brightness = (brightness + 1) / 2;
    brightness *= 255;

    if (fade >= TWO_PI)
    {
      fade = 0;
      color = random(65535);
    }
    fade += 0.08f;
    NeoPixel.setPixelColor(0, NeoPixel.ColorHSV(color, 255, (uint8_t)brightness));
    NeoPixel.show();

    vTaskDelay(50);
  }
}



void setup()
{
  gpio_config_t gpioConfig;

  Serial.begin(115200);

  gpioConfig.intr_type = GPIO_INTR_DISABLE;
  gpioConfig.mode = GPIO_MODE_OUTPUT;
  gpioConfig.pin_bit_mask = ((uint64_t)1 << PWR_HOLD) | ((uint64_t)1 << LCD_BACKLIGHT);
  gpioConfig.pull_down_en = GPIO_PULLDOWN_DISABLE;
  gpioConfig.pull_up_en = GPIO_PULLUP_DISABLE;
  gpio_config(&gpioConfig);

  gpio_set_level(PWR_HOLD, 1);
  gpio_set_level(LCD_BACKLIGHT, 1);
  pinMode(PLAY_ON, INPUT);

  BLE_Joystick.set_connect_callback(connection);
  BLE_Joystick.set_movement_callback(movement);
  BLE_Joystick.set_button_A_callback(button_A);
  BLE_Joystick.set_button_B_callback(button_B);
  BLE_Joystick.set_button_X_callback(button_X);
  BLE_Joystick.set_button_Y_callback(button_Y);
  BLE_Joystick.set_button_select_callback(button_select);
  BLE_Joystick.set_button_start_callback(button_start);

  BLE_Joystick.begin();

  tft.init();
  tft.fillScreen(TFT_WHITE);
  tft.setRotation(1);
  img.setTextColor(TFT_BLACK,TFT_WHITE);
  img.setColorDepth(1);
  img2.setColorDepth(1);
  e.setColorDepth(1);
  e2.setColorDepth(1);


  
  img.createSprite(160, 100);
  img2.createSprite(33, 35);
  e.createSprite(eW, eH);
  e2.createSprite(eW, eH);
  tft.fillScreen(TFT_WHITE);

  ledcSetup(pwmLedChannelTFT, pwmFreq, pwmResolution);
  ledcAttachPin(TFT_BL, pwmLedChannelTFT);
  ledcWrite(pwmLedChannelTFT, brightnes[b]);

  for(int i=0;i<6;i++){
    linesX[i]=random(i*40,(i+1)*40);
    linesW[i]=random(1,14);
    linesX2[i]=random(i*40,(i+1)*40);
    linesW2[i]=random(1,14);
  }
  for(int n=0;n<2;n++){
    bumps[n]=random(n*90,(n+1)*120);
    bumpsF[n]=random(0,2);
  }
  tft.pushImage(0,0,217,135,noInternet);
 /*  while(digitalRead(0)!=0)
  {
    if(digitalRead(35) == 0){
      if(press2==0)
      {
        press2 = 1;
        b++;
        if(b >= 6){
          b = 0;
        }
    ledcWrite(pwmLedChannelTFT, brightnes[b]);}
    delay(200);
    } 
    else {
      press2 = 0;
    }
  } */
  tft.fillScreen(TFT_WHITE);   
  NeoPixel.begin();
  NeoPixel.setPixelColor(0, 0);
  NeoPixel.show();
  xTaskCreate(Blink_Task, "blink_task", 1024 * 4, NULL, 5, NULL);
}

void drawS(int x, int y, int frame)
{
 
  img.fillSprite(TFT_WHITE);
  img.drawLine(0,84,240,84,TFT_BLACK);

  for(int i=0;i<6;i++){
    img.drawLine(linesX[i],87 ,linesX[i]+linesW[i],87,TFT_BLACK);
    linesX[i]=linesX[i]-sped;
    if(linesX[i]<-14){
      linesX[i]=random(245,280);
      linesW[i]=random(1,14);
    }
    img.drawLine(linesX2[i],98 ,linesX2[i]+linesW2[i],98,TFT_BLACK);
    linesX2[i]=linesX2[i]-sped;
    if(linesX2[i]<-14){
      linesX2[i]=random(245,280);
      linesW2[i]=random(1,14);
    }
  }
  for(int j=0;j<2;j++)
  {
    img.drawXBitmap(clouds[j], 20, cloud, 38, 11, TFT_BLACK,TFT_WHITE);
    clouds[j]=clouds[j]-cloudSpeed;
    if(clouds[j]<-40)
      clouds[j]=random(244,364);
  }

  for(int n=0;n<2;n++)
  {
    img.drawXBitmap(bumps[n], 80, bump[bumpsF[n]], 34, 5, TFT_BLACK,TFT_WHITE);
    bumps[n]=bumps[n]-sped;
    if(bumps[n]<-40){
      bumps[n] = random(244,364)*(n+1);
      bumpsF[n]=random(0,2);
    }
  }

  for(int m = 0;m < 2; m++)
  {
    eX[m] = eX[m]-sped;
    if(eX[m] < -20)
      eX[m] = random(240,300)*(m+1);
    ef[m] = random(0,2);
  }
  
  e.drawXBitmap(0, 0, enemy[0], eW, eH, TFT_BLACK,TFT_WHITE);
  e2.drawXBitmap(0, 0, enemy[1], eW, eH, TFT_BLACK,TFT_WHITE);  
  img2.drawXBitmap(0, 0, dino[frame], 33, 35, TFT_BLACK,TFT_WHITE);

  e.pushToSprite(&img,eX[0],56,TFT_WHITE);
  e2.pushToSprite(&img,eX[1],56,TFT_WHITE);
  img2.pushToSprite(&img,x,y,TFT_WHITE);
  
  score++;
  img.drawString(String(score),130,0,2);
  //img.drawString(String(sped),160,0,2);
  img.pushSprite(0, 17);

  if(score > t + 100)
  {
    t = score;
    sped = sped +0.1;
  }
}

  void checkColision()
  {
    for(int i = 0; i < 2; i++){
      if(eX[i] < x + dinoW/2 && eX[i] > x && y > 25)
      {
        gameRun = 0;
        //  tft.fillRect(0,30,160,110,TFT_BLUE);
        
        tft.setTextSize(2);
        tft.drawString("PERDEU FRACO!", 5, 64);
        //tft.drawXBitmap(-20,30,gameover,160,100,TFT_BLUE,TFT_WHITE);

        delay(500);
      }
    }
  }


void loop()
{
  BLE_Joystick.loop();
  if (digitalRead(PLAY_ON) == true)
  {
    int8_t timeToOff = 5;

    do
    {
      timeToOff--;
      vTaskDelay(500);
    } while (digitalRead(PLAY_ON) == true && timeToOff > 0);
    if (timeToOff <= 0)
    {
      NeoPixel.setPixelColor(0, 0);
      NeoPixel.show();
      gpio_set_level(PWR_HOLD, 0);
      Serial.println("Poweroff");
    }
  }
  if(gameRun==1){
  /*   if(digitalRead(0)==0 && pressed==0){
      pressed=1;
      f=0;
    }  */



  if(pressed==1)
  {
    y = y + dir;
    if(y == 2){
      dir = dir * -1.00;
    }
    if(y == 58){
      pressed = 0; 
      dir = dir *-1.00;
    }
  }
    
  if(frames<8 && pressed==0){
    f = 1;
  }
  if(frames>8 && pressed==0){
    f=2;
  }

  drawS(x,y,f);
  frames++;
  if(frames==16)
    frames=0;

  checkColision();
  }


 /*  if(digitalRead(35)==0){
    if(press2==0)
    {
      press2=1;
      b++;
      if(b >= 6){
        b=0;
      }
      ledcWrite(pwmLedChannelTFT, brightnes[b]);
    }
  }else{
    press2 = 0;
  }  */
}
