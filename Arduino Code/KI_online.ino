
// Sensor
#include "SparkFun_AS7265X.h"
AS7265X sensor;
#include <Wire.h>

// KI
#include <TensorFlowLite.h>
#include <tensorflow/lite/micro/all_ops_resolver.h>
#include <tensorflow/lite/micro/micro_error_reporter.h>
#include <tensorflow/lite/micro/micro_interpreter.h>
#include <tensorflow/lite/schema/schema_generated.h>
#include <tensorflow/lite/version.h>
#include "model_hell.h"
#include "model_ita.h"
#include <Arduino.h>
#include <U8g2lib.h>
#include <math.h>

// Monitor
#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#endif
U8G2_SH1106_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, /* reset=*/U8X8_PIN_NONE); //

// global variables used for TensorFlow Lite (Micro)
tflite::MicroErrorReporter tflErrorReporter_hell;
tflite::AllOpsResolver tflOpsResolver_hell;

const tflite::Model *tflModel_hell = nullptr;
tflite::MicroInterpreter *tflInterpreter_hell = nullptr;
TfLiteTensor *tflInputTensor_hell = nullptr;
TfLiteTensor *tflOutputTensor_hell = nullptr;

tflite::MicroErrorReporter tflErrorReporter_ita;
tflite::AllOpsResolver tflOpsResolver_ita;

const tflite::Model *tflModel_ita = nullptr;
tflite::MicroInterpreter *tflInterpreter_ita = nullptr;
TfLiteTensor *tflInputTensor_ita = nullptr;
TfLiteTensor *tflOutputTensor_ita = nullptr;

// Create a static memory buffer for TFLM, the size may need to
// be adjusted based on the model you are using
constexpr int tensorArenaSize_hell = 16 * 1024;
byte tensorArena_hell[tensorArenaSize_hell];
constexpr int tensorArenaSize_ita = 16 * 1024;
byte tensorArena_ita[tensorArenaSize_ita];

// KI-Index Namen zuweisen
const char *CLASSES_hell[] = {
    "01",
    "02",
    "03",
    "04",
    "05",
    "06",
    "07",
    "08",
    "09",
    "10",
    "11",
    "12",
    "13",
    "14",
    "15",
};
// Anzahl Index (Farben)
const int NUM_CLASSES_hell = 15;

// KI-Index Namen zuweisen
const char *CLASSES_ita[] = {
    "01",
    "02",
    "03",
    "04",
    "05",
    "06",
    "07",
    "08",
    "09",
    "10",
    "11",
};
// Anzahl Index (Farben)
const int NUM_CLASSES_ita = 11;

// Pinnummer Knopf
//const int buttonPinin = 5;

// Variable um Knopf Statur zu lesen
//int buttonState = 0;

void setup()
{

    // Zeug für die KI ..

    // get the TFL representation of the model byte array
    tflModel_hell = tflite::GetModel(model_hell);
    tflModel_ita = tflite::GetModel(model_ita);

    /* if (tflModel->version() != TFLITE_SCHEMA_VERSION)
    {
        Serial.println("Model schema mismatch!");
        while (1)
            ;
    } */

    // Create an interpreter to run the model
    tflInterpreter_hell = new tflite::MicroInterpreter(tflModel_hell, tflOpsResolver_hell, tensorArena_hell, tensorArenaSize_hell, &tflErrorReporter_hell);
    tflInterpreter_ita = new tflite::MicroInterpreter(tflModel_ita, tflOpsResolver_ita, tensorArena_ita, tensorArenaSize_ita, &tflErrorReporter_ita);

    // Allocate memory for the model's input and output tensors
    tflInterpreter_hell->AllocateTensors();
    tflInterpreter_ita->AllocateTensors();

    // Get pointers for the model's input and output tensors
    tflInputTensor_hell = tflInterpreter_hell->input(0);
    tflOutputTensor_hell = tflInterpreter_hell->output(0);

    tflInputTensor_ita = tflInterpreter_ita->input(0);
    tflOutputTensor_ita = tflInterpreter_ita->output(0);

    // buttonPinin als Pull-up Resistor
    //pinMode(buttonPinin, INPUT_PULLUP);

    // Sensor initialisieren
    sensor.begin();
    // Display initialisieren
    u8g2.begin();
    sensor.setBulbCurrent(AS7265X_LED_CURRENT_LIMIT_50MA, AS7265x_LED_WHITE); //Allowed
    //Statur LED am Sensor Deaktivieren
    sensor.disableIndicator();
}

void loop()
{

    float sensorData[18];
    float sensorDataNorm[18];
    int max_wert;

    // Wenn Knopf wieder Funktioniert -> aktivieren | ermöglicht betrieb durch Knopfdruck
    /* if (digitalRead(buttonPinin) == LOW) { */

    //This is a hard wait while all 18 channels are measured
    sensor.takeMeasurementsWithBulb();
    // Messwerte als Float speichern
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

    max_wert = 10000;

    float temp = sensor.getTemperatureAverage();
    Serial.print(temp);
    Serial.println();
    temp = temp / 60;

    tflInputTensor_hell->data.f[0] = temp;
    tflInputTensor_ita->data.f[0] = temp;

    for (int i = 0; i < 18; i++)
    {
        sensorDataNorm[i] = roundf(sensorData[i] / max_wert * 100000) / 100000;
        tflInputTensor_hell->data.f[i + 1] = sensorDataNorm[i];
        tflInputTensor_ita->data.f[i + 1] = sensorDataNorm[i];
    }

    // run inferencing
    TfLiteStatus invokeStatus_hell = tflInterpreter_hell->Invoke();
    if (invokeStatus_hell != kTfLiteOk)
    {
        Serial.println("Invoke failed!");
        while (1)
            ;
        return;
    }

    // run inferencing
    TfLiteStatus invokeStatus_ita = tflInterpreter_ita->Invoke();
    if (invokeStatus_ita != kTfLiteOk)
    {
        Serial.println("Invoke failed!");
        while (1)
            ;
        return;
    }

    int probs_hell[NUM_CLASSES_hell];
    for (int i = 0; i < NUM_CLASSES_hell; i++)
    {
        probs_hell[i] = int(tflOutputTensor_hell->data.f[i] * 100);
        Serial.print(probs_hell[i]);
        Serial.print(", ");
    }
    Serial.println();

    float helligkeit = 0;
    for (int i = 0; i < NUM_CLASSES_hell; i++)
    {
        helligkeit += (i + 1) * probs_hell[i];
    }
    helligkeit = helligkeit / 100;

    int probs_ita[NUM_CLASSES_ita];
    for (int i = 0; i < NUM_CLASSES_ita; i++)
    {
        probs_ita[i] = int(tflOutputTensor_ita->data.f[i] * 100);
        Serial.print(probs_ita[i]);
        Serial.print(", ");
    }
    Serial.println();

    float ita = 0;
    for (int i = 0; i < NUM_CLASSES_ita; i++)
    {
        ita += (i + 1) * probs_ita[i];
    }
    ita = ita / 100;


    Serial.print(helligkeit / 100);
    Serial.println();
    Serial.print(ita / 100);
    Serial.println();
    Serial.println();

    // Ausgabe der höchsten 3 Wahrscheinlichkeiten
  u8g2.firstPage();
   do {
    u8g2.setFont(u8g2_font_5x7_tf);
        u8g2.setCursor(10, 1 * 12);
        u8g2.print("Helligkeit:");
        u8g2.setCursor(80, 1 * 12);
        u8g2.print(helligkeit);
        u8g2.setCursor(10, 2 * 12);
        u8g2.print("ITA-Stufe:");
        u8g2.setCursor(80, 2 * 12);
        u8g2.print(ita);
        //u8g2.setCursor(93, (i)*12+30);
        //u8g2.print("%");

  } while ( u8g2.nextPage() );
    /*} Wenn Knopf wieder Funktioniert */
}