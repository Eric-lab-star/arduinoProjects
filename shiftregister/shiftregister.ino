#include <AdafruitIO.h>
#include <AdafruitIO_Dashboard.h>
#include <AdafruitIO_Data.h>
#include <AdafruitIO_Definitions.h>


#include <AdafruitIO_Feed.h>
#include <AdafruitIO_Group.h>
#include <AdafruitIO_MQTT.h>
#include <AdafruitIO_Time.h>
#include <AdafruitIO_WiFi.h>

#include "config.h"

AdafruitIO_Feed *blueSlider = io.feed("BLUE");
AdafruitIO_Feed *greenSlider = io.feed("GREEN");
AdafruitIO_Feed *redSlider = io.feed("RED");

const uint8_t RED = 15;
const uint8_t GREEN = 2;
const uint8_t BLUE = 4;
const uint8_t TACT_SWITCH = 12;
const uint8_t BUZZER = 14;
int clickMode[4][3] = {
  { 255, 255, 255 },
  { 0, 255, 255 },
  { 255, 0, 255 },
  { 255, 255, 0},
};
const int tones[6] = {261, 277, 294, 311, 330, 349};
const uint32_t redPWM = 80;
const uint32_t bluePWM = 120;
const uint32_t greenPWM = 255;

int dimmingStep = 1;
int cmi = 0;

void off(){
  ledcWrite(RED, 255);
  ledcWrite(GREEN, 255);
  ledcWrite(BLUE, 255);
}

void RGBAttach() {
  ledcAttach(RED, 5000, 8);
  ledcAttach(BLUE, 5000, 8);
  ledcAttach(GREEN, 5000, 8);
}

void rndRGB() {
  ledcWrite(RED, random(0, 255));
  ledcWrite(BLUE, random(0, 255));
  ledcWrite(GREEN, random(0,255));  
  delay(200);
}

void dimmingRGB(int i) {
  ledcWrite(RED, redPWM + i);
  ledcWrite(BLUE, bluePWM - i);
  ledcWrite(GREEN, greenPWM - i);
  delay(100);
}

void setup() {
  Serial.begin(115200);
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
  RGBAttach();
  pinMode(TACT_SWITCH, INPUT_PULLUP);
  
  off();

  while(! Serial);
  Serial.print("Connecting to Adafruit IO");
  io.connect();

  blueSlider->onMessage(handleBlueSlider);
  greenSlider->onMessage(handleGreenSlider);
  redSlider->onMessage(handleRedSlider);

  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.println(io.statusText());
  blueSlider->get();
  greenSlider -> get();
  redSlider -> get();
}


void loop() {
  io.run();

  
  if (cmi < 4) {
    ledcWrite(RED, clickMode[cmi][0]);
    ledcWrite(GREEN, clickMode[cmi][1]);
    ledcWrite(BLUE, clickMode[cmi][2]);
  } else if ( cmi == 4 ) {
    rndRGB();
  } if (cmi == 5) {
    dimmingRGB(dimmingStep);
    dimmingStep++;
    if (dimmingStep > 120) {
      dimmingStep = 0;
    } 
  }
  
  if (digitalRead(TACT_SWITCH) == 0) {
    tone(BUZZER, tones[cmi]);
    cmi++;
    delay(200);
    noTone(BUZZER);
  }

  if (cmi >= 6) {
    cmi = 0;
  }
}

void handleBlueSlider(AdafruitIO_Data *data) {
  // convert the data to integer
  int sliderValue = data->toInt();

  Serial.print("Blue received <- ");
  Serial.println(sliderValue);

  // write the current 'sliderValue' to the led
  clickMode[cmi][2] = sliderValue;
}

void handleGreenSlider(AdafruitIO_Data *data) {
  // convert the data to integer
  int sliderValue = data->toInt();

  Serial.print("Green received <- ");
  Serial.println(sliderValue);

  // write the current 'sliderValue' to the led
  clickMode[cmi][1] = sliderValue;
}

void handleRedSlider(AdafruitIO_Data *data) {
  // convert the data to integer
  int sliderValue = data->toInt();

  Serial.print("Red received <- ");
  Serial.println(sliderValue);

  // write the current 'sliderValue' to the led
  clickMode[cmi][0] = sliderValue;
}



