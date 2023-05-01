#include "Alarm.h"


TFT_eSPI tft = TFT_eSPI();

Alarm::Alarm()
{
    alarmClock.setWakeupTime("08:00:00");
	tft.init();
	tft.setRotation(2);   //setRotation: 2: Screen upside down in landscape

    // draw time
	tft.fillScreen(TFT_BLACK);
	tft.setCursor(0, 0, 2);

	tft.setTextColor(TFT_WHITE, TFT_BLACK);
	tft.setTextSize(2);

	tft.println("Alarm at:");
	tft.println(alarmClock.getWakeupTimeStr().c_str());
}

void Alarm::update()
{
    // only checks weight when alarm is going off
	if (this->shouldSound() && !scale.containsItem()){
		radio.play();
        radio.loop();
	}
	else{
		radio.stop();
	}
}

bool Alarm::shouldSound()
{
    if (alarmClock.isAlarming()){
        return true;
    }
    return false;
}