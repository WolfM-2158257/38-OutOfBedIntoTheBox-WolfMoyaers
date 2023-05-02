/*
  Uses MAX98357 I2S Amplifier Module
  Uses ESP32-audioI2S Library - https://github.com/schreibfaul1/ESP32-audioI2S
  Uses HX711_ADC library - https://github.com/olkal/HX711_ADC

  Source for setting up MAX98357: https://dronebotworkshop.com/esp32-i2s/
  source for setting up HX711: testing and calibration examples
*/

#include <SPIFFS.h>
#include "Alarm.h"
#include "esp_wifi.h"

// include credentials for network defined in `credentials.h`
// String ssid =       "NETWORK";
// String password =   "PASSWORD";
#include "credentials.h"

#define BTN_PIN 32

Alarm* weightedAlarm;
bool stateButton = false;
bool doSnooze = false;

int lastSwitchTime = 0;

// pre-define functions
void setupWifi();
void btnHandler();

void setup()
{
	// Start Serial Monitor
	Serial.begin(115200); delay(10);

	pinMode(BTN_PIN, INPUT_PULLUP);

	weightedAlarm = new Alarm{};
}

void loop()
{
	weightedAlarm->update();
	if (doSnooze){
		doSnooze = false;
		Serial.print("INTERRUPTION! ");
		weightedAlarm->snooze();
	}
	stateButton = !digitalRead(BTN_PIN);
	if (stateButton) {
		btnHandler();
	}

	// if (!weightedAlarm->shouldSound()){
	// 	delay(5000);
	// }
}

void btnHandler() {
  unsigned long switchTime = millis();
  if (switchTime - lastSwitchTime > 200)
  {	
	doSnooze = true;
    lastSwitchTime = switchTime;
  }
}