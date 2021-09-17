#include "SparkFun_AS7265X.h"
AS7265X sensor;
#include <Wire.h>
#include <numeric>

// Settings
const boolean normalize = false;

const int LEDPin = 12;
const int timezero = millis();

unsigned long timeStamp;
float sensorData[2];
float sum = 0;


void setup() {
  Serial.begin(115200);

  pinMode(LEDPin, OUTPUT);

  sensor.begin();
  sensor.disableIndicator(); //Turn off the blue status LED


  //Once the sensor is started we can increase the I2C speed
  Wire.setClock(400000);

  //Integration cycles is from 0 (2.78ms) to 255 (711ms)
  //sensor.setIntegrationCycles(49); //Default 50*2.8ms = 140ms per reading
  //sensor.setIntegrationCycles(1); //2*2.8ms = 5.6ms per reading

  //Drive current can be set for each LED
  //4 levels: 12.5, 25, 50, and 100mA
  //-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
  //White LED has max forward current of 120mA
  //sensor.setBulbCurrent(AS7265X_LED_CURRENT_LIMIT_12_5MA, AS7265x_LED_WHITE); //Default
  //sensor.setBulbCurrent(AS7265X_LED_CURRENT_LIMIT_25MA, AS7265x_LED_WHITE); //Allowed
  sensor.setBulbCurrent(AS7265X_LED_CURRENT_LIMIT_50MA, AS7265x_LED_WHITE); //Allowed
  //sensor.setBulbCurrent(AS7265X_LED_CURRENT_LIMIT_100MA, AS7265x_LED_WHITE); //Allowed

  //UV LED has max forward current of 30mA so do not set the drive current higher
  sensor.setBulbCurrent(AS7265X_LED_CURRENT_LIMIT_12_5MA, AS7265x_LED_UV); //Default
  //sensor.setBulbCurrent(AS7265X_LED_CURRENT_LIMIT_25MA, AS7265x_LED_UV-bad); //Not allowed
  //sensor.setBulbCurrent(AS7265X_LED_CURRENT_LIMIT_50MA, AS7265x_LED_UV-bad); //Not allowed
  //sensor.setBulbCurrent(AS7265X_LED_CURRENT_LIMIT_100MA, AS7265x_LED_UV-bad); //Not allowed

  //IR LED has max forward current of 65mA
  sensor.setBulbCurrent(AS7265X_LED_CURRENT_LIMIT_12_5MA, AS7265x_LED_IR); //Default
  //sensor.setBulbCurrent(AS7265X_LED_CURRENT_LIMIT_25MA, AS7265x_LED_IR); //Allowed
  //sensor.setBulbCurrent(AS7265X_LED_CURRENT_LIMIT_50MA, AS7265x_LED_IR); //Allowed
  //sensor.setBulbCurrent(AS7265X_LED_CURRENT_LIMIT_100MA, AS7265x_LED_IR-bad); //Not allowed
  //-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

  sensor.disableIndicator();
  sensor.setMeasurementMode(AS7265X_MEASUREMENT_MODE_6CHAN_CONTINUOUS); //All 6 channels on all devices
  sensor.enableBulb(AS7265x_LED_WHITE);
  sensor.enableBulb(AS7265x_LED_IR);
  sensor.enableBulb(AS7265x_LED_UV);
}


void loop() {

  while(sensor.dataAvailable() == false) {} //Do nothing

  sensorData[0] = sensor.getCalibratedI();
  sensorData[1] = sensor.getCalibratedR();
  sensorData[2] = sensor.getCalibratedS();

  Serial.print(sensor.getTemperatureAverage());
  Serial.print(",");


  if (normalize) {
    sum = 0;
    sum = std::accumulate(sensorData, sensorData+18, sum);
  }

  for(int i = 0; i < 3; i++) {
    if (normalize) {
      sensorData[i] = sensorData[i] / sum;
    }

    Serial.print(sensorData[i], 5);

    if (i < 2) {
      Serial.print(",");
    }
  }


  Serial.println();
}
