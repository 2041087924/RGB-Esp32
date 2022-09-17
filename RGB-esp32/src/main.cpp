#define BLINKER_WIFI
#include <Arduino.h>
#include <Blinker.h>
#include "FastLED.h"

#define NUM_LEDS 15 //定义 led 数量
#define DATA_PIN 5  //定义 开发板的IO 5 引脚为数据引脚

CRGB leds[NUM_LEDS]; //定义CRGB的led等数组

char auth[] = "d3cdbfb012cd";
// char ssid[] = "ESATA2_4";
// char pswd[] = "dianxieniubi";
// char ssid[] = "HUAWEI Mate X2";
// char pswd[] = "123654789";
char ssid[] = "Xiaomi_545A";
char pswd[] = "1236547890";

uint16_t R, G, B;
enum State
{
  NO,
  OFF,
  Mode_1
} RGB_state;

// 新建组件对象
BlinkerButton Button1("btn-abc");
BlinkerRGB RGB1("col-lme");
BlinkerSlider RGB_R("ran-hwc");
BlinkerSlider RGB_G("ran-oo0");
BlinkerSlider RGB_B("ran-50q");
BlinkerButton Button2("btn-k11");

// 按下按键即会执行该函数
void button1_callback(const String &state)
{
  BLINKER_LOG("get button state: ", state);
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  if (RGB_state == NO || RGB_state == Mode_1)
  {
    RGB_state = OFF;
  }
  else
  {
    RGB_state = NO;
  }
}

void button2_callback(const String &state)
{
  BLINKER_LOG("get button state: ", state);
  if (RGB_state == OFF || RGB_state == NO)
  {
    RGB_state = Mode_1;
  }
  else
  {
    RGB_state = NO;
  }
}

void rgb_callback(uint8_t r_value, uint8_t g_value,
                  uint8_t b_value, uint8_t bright_value)
{
  BLINKER_LOG("R value: ", r_value);
  BLINKER_LOG("G value: ", g_value);
  BLINKER_LOG("B value: ", b_value);
  BLINKER_LOG("Rrightness value: ", bright_value);
  R = r_value;
  G = g_value;
  B = b_value;
}

void Slider_R(int32_t value)
{
  R = value;
  RGB_R.print(value);
  BLINKER_LOG("get slider_r value: ", value);
}

void Slider_G(int32_t value)
{
  G = value;
  RGB_G.print(value);
  BLINKER_LOG("get slider_g value: ", value);
}

void Slider_B(int32_t value)
{
  B = value;
  RGB_B.print(value);
  BLINKER_LOG("get slider_b value: ", value);
}

void setup()
{
  // 初始化串口，并开启调试信息
  Serial.begin(115200);
  BLINKER_DEBUG.stream(Serial);
  // 初始化有LED的IO
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  // 初始化blinker
  Blinker.begin(auth, ssid, pswd);
  Button1.attach(button1_callback);
  Button2.attach(button2_callback);
  RGB_R.attach(Slider_R);
  RGB_G.attach(Slider_G);
  RGB_B.attach(Slider_B);
  RGB1.attach(rgb_callback);
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
}

void RGB_Blinker()
{
  // 第一个led灯闪烁白色
  leds[0] = CRGB::White; // 序号为0的led设置白色，点亮
  FastLED.show();
  delay(500);
  leds[0] = CRGB::Black; // 序号为0的led设置黑色，熄灭
  FastLED.show();
  delay(500);

  delay(500);

  // 每一个led逐个闪烁绿色
  for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = CRGB::Green;
    FastLED.show();
    delay(100);
    leds[i] = CRGB::Black;
    FastLED.show();
    delay(100);
  }

  delay(1000);

  // 整个灯条 彩虹色渐变
  int h = 0;
  for (int j = 0; j < 255; j++)
  {
    for (int i = 0; i < NUM_LEDS; i++)
    {
      leds[i] = CHSV((h + (255 / NUM_LEDS) * i), 255, 255); //用HSV色彩空间，不断改变H即可
      FastLED.show();
    }
    delay(1);
    h = (h + 2) % 255;
  }

  // 整个灯条熄灭
  for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = CRGB::Black;
  }
  FastLED.show();
  delay(1000);
}

void RGB_Blinker1()
{
  for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = CRGB(R, G, B);
  }
  FastLED.show();
}

void RGB_CHSV()
{
  // 整个灯条 彩虹色渐变
  int h = 0;
  for (int j = 0; j < 255; j++)
  {
    for (int i = 0; i < NUM_LEDS; i++)
    {
      leds[i] = CHSV((h + (255 / NUM_LEDS) * i), 255, 255); //用HSV色彩空间，不断改变H即可
      FastLED.show();
      if (RGB_state == NO || RGB_state == OFF)
      {
        break;
      }
    }
    if (RGB_state == NO || RGB_state == OFF)
      {
        break;
      }
    delay(1);
    h = (h + 2) % 255;
  }
}

void RGB_Running(){
  for(int i = 0; i < 255 ;i += 10){
    for(int j = 0; j < 255 ;j += 10){
      for(int k = 0; k < 255 ;k += 10){


    
      }  
   }
  }
}

void RGB_OFF()
{
  for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = CRGB::Black;
  }
  FastLED.show();
}

void xTaskOne(void *xTask1) //多任务一
{
  while (1)
  {
    if (RGB_state == NO)
    {
      RGB_Blinker1();
    }
    else if (RGB_state == Mode_1)
    {
      RGB_CHSV();
    }
    else
    {
      RGB_OFF();
    }
    delay(10);
  }
}

void xTaskTwo(void *xTask2)
{
  while (1)
  {
    delay(10);
  }
}

void loop()
{
  xTaskCreate(xTaskOne, "TaskOne", 5120, NULL, 1, NULL); //对任务一进行相关的设置
  xTaskCreate(xTaskTwo, "TaskTwo", 5120, NULL, 2, NULL); //对任务二进行相关的设置
  while (1)
  {
    Blinker.run();
    delay(10);
  }
}
