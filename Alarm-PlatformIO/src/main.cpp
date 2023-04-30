/*
  Uses MAX98357 I2S Amplifier Module
  Uses ESP32-audioI2S Library - https://github.com/schreibfaul1/ESP32-audioI2S
  Uses HX711_ADC library - https://github.com/olkal/HX711_ADC

  Source for setting up MAX98357: https://dronebotworkshop.com/esp32-i2s/
  source for setting up HX711: testing and calibration examples
*/

#include "WiFi.h"
#include "Alarm.h"

// include credentials for network defined in `credentials.h`
// String ssid =       "NETWORK";
// String password =   "PASSWORD";
#include "credentials.h"

Alarm* weightedAlarm;

// pre-define functions
void setupWifi();

void setup()
{
	// Start Serial Monitor
	Serial.begin(115200); delay(10);

	weightedAlarm = new Alarm{};

	setupWifi();
}

void setupWifi()
{
	// Setup WiFi in Station mode
	WiFi.disconnect();
	WiFi.mode(WIFI_STA);
	WiFi.begin(ssid.c_str(), password.c_str());

	while (WiFi.status() != WL_CONNECTED)
	{
		delay(500);
		Serial.print(".");
	}

	// WiFi Connected, print IP to serial monitor
	Serial.println("");
	Serial.println("WiFi connected");
	Serial.println("IP address: ");
	Serial.println(WiFi.localIP());
	Serial.println("");
	const char* ntpServer = "pool.ntp.org";
	const long gmtOffset_sec = 3600;
	const int daylightOffset_sec = 3600;
	configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
}

void loop()
{
	weightedAlarm->update();
	// if (!weightedAlarm->shouldSound()){
	// 	delay(5000);
	// }
}