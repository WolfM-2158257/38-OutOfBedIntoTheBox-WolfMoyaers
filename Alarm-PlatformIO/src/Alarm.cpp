#include "Alarm.h"


Alarm::Alarm()
{
    alarmClock.setWakeupTime("08:00:00");
	tft.init();
	tft.setRotation(2);   //setRotation: 2: Screen upside down in landscape
}

void Alarm::update()
{
	tft.fillScreen(TFT_BLACK);   //Fill screen with random colour
	tft.setCursor(0, 0, 2);   //(cursor at 0,0; font 2, println autosets the cursor on the next line)

	tft.setTextColor(TFT_WHITE, TFT_BLACK); // Textcolor, BackgroundColor; independent of the fillscreen
	tft.setTextSize(3);     //Set text size multiplier to 1

	tft.println(alarmClock.getWakeupTimeStr());    //Print on cursorpos 0,0

    delay(2000);
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