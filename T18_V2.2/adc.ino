#include <Arduino.h>
#include "esp32_adc.h"
#include "esp_adc_cal.h"
#include <WiFi.h>
#include <Button2.h>

#define ENABLE_SSD1306
#define ENABLE_BUTTON

#ifdef ENABLE_BUTTON
Button2 *pBtn = nullptr;
#endif

#define ADC_PINS 35

int loop_mode = 0;

enum {
    WIFI_SCAN = 32,
    VOLTAG_DETECT = 33,
    DEEPSLEEP = 34,
    KEY1 = 36,
    KEY2 = 39,
};


struct {
    uint8_t pin;
    const char *name;
} btns[] = {
    {32, "up"},
    {33, "down"},
    {34, "center"},
    {36, "left"},
    {39, "right"},
};

#define BTNSIZES    sizeof(btns)/sizeof(btns[0])

#ifdef ENABLE_SSD1306
#include "SSD1306.h"
#define SSD1306_ADDRESS 0x3c
#define I2C_SDA 21
#define I2C_SCL 22
SSD1306 oled(SSD1306_ADDRESS, I2C_SDA, I2C_SCL);
#endif

int vref = 1100;

void battery_test()
{
    oled.clear();
    oled.drawString(oled.getWidth() / 2, oled.getHeight() / 2, "battery test");
    oled.display();
    delay(1000);
}

void wifi_test()
{
    oled.clear();
    oled.drawString(oled.getWidth() / 2, oled.getHeight() / 2, "wifi scan");
    oled.display();

    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    int n = WiFi.scanNetworks();
    oled.clear();
    if (n == 0) {
        oled.drawString(oled.getWidth() / 2, oled.getHeight() / 2, "no networks found");
        oled.display();
        return;
    }
    oled.drawString(oled.getWidth() / 2, oled.getHeight() / 2, "Found" + String(n) + "net");
    oled.display();
}

void enter_sleep()
{
    oled.clear();
    oled.drawString(oled.getWidth() / 2, oled.getHeight() / 2, "enter sleep 10 sec");
    oled.display();
    delay(1000);
    oled.clear();
    oled.display();
    system_deep_sleep(10 * 1000 * 1000);
}


void showVoltage()
{
    static uint64_t timeStamp = 0;
    if (millis() - timeStamp > 1000) {
        timeStamp = millis();
        uint16_t v = analogRead(ADC_PINS);
        float battery_voltage = ((float)v / 4095.0) * 2.0 * 3.3 * (vref / 1000.0);
#ifdef ENABLE_SSD1306
        String voltage = "Voltage :" + String(battery_voltage) + "V";
        oled.clear();
        oled.drawString(oled.getWidth() / 2, oled.getHeight() / 2, voltage);
        oled.display();
#else
        Serial.println("DEBUG: Value: " + String(v) + " ,Battery Voltage is" + String(battery_voltage) + "V");
#endif
    }
}

#ifdef ENABLE_BUTTON
void btn_loop()
{
    for (int i = 0; i < BTNSIZES; i++) {
        pBtn[i].loop();
    }
}

void btn_callback(Button2 &b)
{
    for (int i = 0; i < BTNSIZES; i++) {
        if (pBtn[i] == b) {
            loop_mode = pBtn[i].getAttachPin();
            Serial.println(pBtn[i].getAttachPin());
            return;
        }
    }
}
#endif

void setup()
{
    Serial.begin(115200);

#ifdef ENABLE_BUTTON
    pBtn = new Button2 [BTNSIZES]();
    if (pBtn != nullptr) {
        for (int i = 0; i < BTNSIZES; i++) {
            pBtn[i] = Button2(btns[i].pin);
            pBtn[i].setPressedHandler(btn_callback);
        }
    }
#endif


#ifdef ENABLE_SSD1306
    oled.init();
    oled.flipScreenVertically();
    oled.setFont(ArialMT_Plain_16);
    oled.setTextAlignment(TEXT_ALIGN_CENTER);
    delay(50);
    oled.drawString(oled.getWidth() / 2, oled.getHeight() / 2, "TTGO");
    oled.display();
#endif
    esp_adc_cal_characteristics_t adc_chars;
    esp_adc_cal_value_t val_type = esp_adc_cal_characterize((adc_unit_t)ADC_UNIT_1, (adc_atten_t)ADC1_CHANNEL_7, (adc_bits_width_t)ADC_WIDTH_BIT_12, 1100, &adc_chars);
    //Check type of calibration value used to characterize ADC
    if (val_type == ESP_ADC_CAL_VAL_EFUSE_VREF) {
        Serial.printf("eFuse Vref:%u mV", adc_chars.vref);
        vref = adc_chars.vref;
    } else if (val_type == ESP_ADC_CAL_VAL_EFUSE_TP) {
        Serial.printf("Two Point --> coeff_a:%umV coeff_b:%umV\n", adc_chars.coeff_a, adc_chars.coeff_b);
    } else {
        Serial.println("Default Vref: 1100mV");
    }
}

void loop()
{
    btn_loop();
    switch (loop_mode) {
    case WIFI_SCAN:
        wifi_test();
        loop_mode = -1;
        break;
    case VOLTAG_DETECT:
        showVoltage();
        break;
    case DEEPSLEEP:
        enter_sleep();
        break;
    case KEY1:
        oled.clear();
        oled.drawString(oled.getWidth() / 2, oled.getHeight() / 2, "DOWN");
        oled.display();
        loop_mode = -1;
        break;
    case KEY2:
        oled.clear();
        oled.drawString(oled.getWidth() / 2, oled.getHeight() / 2, "OK");
        oled.display();
        loop_mode = -1;
        break;
    default:
        break;
    }
}