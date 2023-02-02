/*!
 * @file DFRobot_PH_Test.h
 * @brief This is the sample code for Gravity: Analog pH Sensor / Meter Kit V2, SKU:SEN0161-V2.
 * @n In order to guarantee precision, a temperature sensor such as DS18B20 is needed, to execute automatic temperature compensation.
 * @n You can send commands in the serial monitor to execute the calibration.
 * @n Serial Commands:
 * @n    enterph -> enter the calibration mode
 * @n    calph   -> calibrate with the standard buffer solution, two buffer solutions(4.0 and 7.0) will be automaticlly recognized
 * @n    exitph  -> save the calibrated parameters and exit from calibration mode
 *
 * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license     The MIT License (MIT)
 * @author [Jiawei Zhang](jiawei.zhang@dfrobot.com)
 * @version  V1.0
 * @date  2018-11-06
 * @url https://github.com/DFRobot/DFRobot_PH
 */

#include "DFRobot_PH.h"
#include <EEPROM.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 4 //sensor suhu di pin D4
#define PH_PIN A6 //sensor ph di pin A1
float voltage,phValue,temperature;

LiquidCrystal_I2C lcd(0x27,16,2);
DFRobot_PH ph;

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensorSuhu(&oneWire);


void setup()
{
    Serial.begin(115200);  
    lcd.begin();
    lcd.backlight();
    ph.begin();
    sensorSuhu.begin();
    
}

void loop()
{
    static unsigned long timepoint = millis();
    if(millis()-timepoint>1000U){                  //time interval: 1s
        timepoint = millis();
        temperature = readTemperature();         // read your temperature sensor to execute temperature compensation
        voltage = analogRead(PH_PIN)/1024.0*5000;  // read the voltage
        phValue = ph.readPH(voltage,temperature);  // convert voltage to pH with temperature compensation
        Serial.print("temperature:");
        Serial.print(temperature,1);
        Serial.print("^C  pH:");
        Serial.println(phValue,2);

        lcd.setCursor(0,0);
        lcd.print("suhu");
        lcd.print(" ");
        lcd.print(temperature);
        lcd.print(" ");
        lcd.write(0xDF);
        lcd.print("C");

        lcd.setCursor(0,1);
        lcd.print("PH");
        lcd.print(" ");
        lcd.print(phValue);

        delay(180000);
    }
    ph.calibration(voltage,temperature);           // calibration process by Serail CMD
}

float readTemperature()
{
  //add your code here to get the temperature from your temperature sensor
sensorSuhu.requestTemperatures();
float suhu = sensorSuhu.getTempCByIndex(0);
return suhu;
}
