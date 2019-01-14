# TTGO-T18V2.2

![image](https://github.com/LilyGO/TTGO-T18V2.2/blob/master/image/image0.jpg)

## Lib install:
### lib1:https://github.com/lewisxhe/Button2
### lib2:https://github.com/ThingPulse/esp8266-oled-ssd1306

## Module 1 Button Left -- Battery voltage detection

```c
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
```        
![image](https://github.com/LilyGO/TTGO-T18V2.2/blob/master/image/image6.jpg)

## Module 2 Button Right -- Wifi scan

```c
switch (loop_mode) {
    case WIFI_SCAN:
        wifi_test();
        loop_mode = -1;
        break;
         }
}
```      

![image](https://github.com/LilyGO/TTGO-T18V2.2/blob/master/image/image7.jpg)

## Module 3 Button Up -- Deep sleep 10s

```c
void loop()
{
    btn_loop();
    switch (loop_mode) {
    
    case DEEPSLEEP:
        enter_sleep();
        break;
         }
}
```      
![image](https://github.com/LilyGO/TTGO-T18V2.2/blob/master/image/image8.jpg)
## Module 4 Button Down -- Down

```c
void loop()
{
    btn_loop();
    switch (loop_mode) {
    case KEY1:
        oled.clear();
        oled.drawString(oled.getWidth() / 2, oled.getHeight() / 2, "DOWN");
        oled.display();
        loop_mode = -1;
        break;
         }
}
```      
![image](https://github.com/LilyGO/TTGO-T18V2.2/blob/master/image/image9.jpg)
## Module 5 Button Center -- OK

```c
void loop()
{
    btn_loop();
    switch (loop_mode) {
    case KEY2:
        oled.clear();
        oled.drawString(oled.getWidth() / 2, oled.getHeight() / 2, "OK");
        oled.display();
        loop_mode = -1;
        break;
         }
}

```      
![image](https://github.com/LilyGO/TTGO-T18V2.2/blob/master/image/image10.jpg)
