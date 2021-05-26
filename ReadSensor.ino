#include "SparkFun_AS7265X.h"
AS7265X sensor;
#include <Wire.h>
#include <numeric>


// Settings
const boolean normalize = false;



const int LEDPin = 12;
const int timezero = millis();

unsigned long timeStamp;
float sensorData[18];
float sum = 0;


void setup() {
  Serial.begin(115200);

  pinMode(LEDPin, OUTPUT);

  sensor.begin();
  sensor.disableIndicator(); //Turn off the blue status LED

  //Drive current can be set for each LED
  //4 levels: 12.5, 25, 50, and 100mA
  //-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
  //White LED has max forward current of 120mA
  sensor.setBulbCurrent(AS7265X_LED_CURRENT_LIMIT_12_5MA, AS7265x_LED_WHITE); //Default
  //sensor.setBulbCurrent(AS7265X_LED_CURRENT_LIMIT_25MA, AS7265x_LED_WHITE); //Allowed
  //sensor.setBulbCurrent(AS7265X_LED_CURRENT_LIMIT_50MA, AS7265x_LED_WHITE); //Allowed
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
}


void loop() {
  sensor.takeMeasurementsWithBulb(); //This is a hard wait while all 18 channels are measured

  sensorData[0] = sensor.getCalibratedA();
  sensorData[1] = sensor.getCalibratedB();
  sensorData[2] = sensor.getCalibratedC();
  sensorData[3] = sensor.getCalibratedD();
  sensorData[4] = sensor.getCalibratedE();
  sensorData[5] = sensor.getCalibratedF();
  sensorData[6] = sensor.getCalibratedG();
  sensorData[7] = sensor.getCalibratedH();
  sensorData[8] = sensor.getCalibratedI();
  sensorData[9] = sensor.getCalibratedJ();
  sensorData[10] = sensor.getCalibratedK();
  sensorData[11] = sensor.getCalibratedL();
  sensorData[12] = sensor.getCalibratedR();
  sensorData[13] = sensor.getCalibratedS();
  sensorData[14] = sensor.getCalibratedT();
  sensorData[15] = sensor.getCalibratedU();
  sensorData[16] = sensor.getCalibratedV();
  sensorData[17] = sensor.getCalibratedW();


  Serial.print(sensor.getTemperature());
  Serial.print(",");


  Serial.print(sensor.getTemperatureAverage());
  Serial.print(",");


  timeStamp = millis() - timezero;
  Serial.print(timeStamp);
  Serial.print(",");


  if (normalize) {
    sum = 0;
    sum = std::accumulate(sensorData, sensorData+18, sum);
  }

  for(int i = 0; i < 18; i++) {
    if (normalize) {
      sensorData[i] = sensorData[i] / sum;
    }

    Serial.print(sensorData[i], 5);

    if (i < 17) {
      Serial.print(",");
    }
  }

  Serial.println();
}
