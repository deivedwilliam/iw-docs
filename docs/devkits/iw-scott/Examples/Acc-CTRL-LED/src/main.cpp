#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <HardwareSerial.h>
#include <driver/gpio.h>
#include <Adafruit_LIS3DH.h>
#include <Adafruit_Sensor.h>


#define I2C_SDA_PIN     GPIO_NUM_7
#define I2C_SCL_PIN     GPIO_NUM_6

#define PWR_HOLD        GPIO_NUM_20
#define PLAY_ON         10

#define RMT_OUTPUT      21


#define CLICKTHRESHHOLD 80

const uint16_t PIXEL_COUNT = 1;
Adafruit_NeoPixel NeoPixel(1, RMT_OUTPUT, NEO_GRB + NEO_KHZ800);
Adafruit_LIS3DH lis = Adafruit_LIS3DH();
TaskHandle_t blinkTask = NULL;

uint32_t  updateNeoPixelByAccel(float x, float y, float z);

#define FILTER_ALPHA 0.1f   // menor = mais suave (0.05 bem suave)
#define PI 3.14159265f

void Blink_Task(void *pvParameters)
{
    float filteredX = 0;
    float filteredY = 0;
    float filteredZ = 0;

    sensors_event_t event;

    for (;;)
    {
        lis.read();
        lis.getEvent(&event);

        // ===== FILTRO PASSA-BAIXA =====
        filteredX = filteredX + FILTER_ALPHA * (event.acceleration.x - filteredX);
        filteredY = filteredY + FILTER_ALPHA * (event.acceleration.y - filteredY);
        filteredZ = filteredZ + FILTER_ALPHA * (event.acceleration.z - filteredZ);

        // ===== CALCULA ÂNGULO NO PLANO X-Y =====
        float angle = atan2(filteredY, filteredX);  // -PI a PI

        // Normaliza para 0 a 1
        float normalized = (angle + PI) / (2 * PI);

        // Converte para Hue 0–65535 (NeoPixel HSV usa 16 bits)
        uint16_t hue = (uint16_t)(normalized * 65535.0f);

        float magnitude = sqrt(filteredX * filteredX + filteredY * filteredY + filteredZ * filteredZ);
       
        uint8_t brightness = constrain(map(magnitude * 100, 800, 1200, 50, 255), 50, 255);

        NeoPixel.setPixelColor(0, NeoPixel.ColorHSV(hue, 255, brightness));
        NeoPixel.show();

        vTaskDelay(pdMS_TO_TICKS(20));
    }
}

void setup()
{
    gpio_config_t gpioConfig;

    gpioConfig.intr_type = GPIO_INTR_DISABLE;
    gpioConfig.mode = GPIO_MODE_OUTPUT;
    gpioConfig.pin_bit_mask = ((uint64_t)1 << PWR_HOLD);
    gpioConfig.pull_down_en = GPIO_PULLDOWN_DISABLE;
    gpioConfig.pull_up_en = GPIO_PULLUP_DISABLE;
    gpio_config(&gpioConfig);

    gpio_set_level(PWR_HOLD, 1);
    pinMode(PLAY_ON, INPUT);

    Serial.begin(115200);

    pinMode(I2C_SDA_PIN, INPUT_PULLUP);
	pinMode(I2C_SCL_PIN, INPUT_PULLUP);

    Wire.setPins(I2C_SDA_PIN, I2C_SCL_PIN);
    

    if (! lis.begin(0x19)) {   
        while(1){
              Serial.println("Couldnt start");
              delay(1000);
        }

    }

     lis.setRange(LIS3DH_RANGE_2_G);   // 2, 4, 8 or 16 G!
  
  Serial.print("Range = "); Serial.print(2 << lis.getRange());  
  Serial.println("G");

    // 0 = turn off click detection & interrupt
    // 1 = single click only interrupt output
    // 2 = double click only interrupt output, detect single click
    // Adjust threshhold, higher numbers are less sensitive
    lis.setClick(2, CLICKTHRESHHOLD);

    NeoPixel.begin();
    NeoPixel.setPixelColor(0, 0);
    NeoPixel.show();

    xTaskCreate(Blink_Task, "blink_task", 1024 * 4, NULL, 5, &blinkTask);
}

void loop()
{
    delay(100);
    if (digitalRead(PLAY_ON) == true)
    {
        int8_t timeToOff = 10;

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
   
}
