#pragma once
#include <TFT_eSPI.h> // Hardware-specific library
#include "Radio.h"
#include "Scale.h"
#include "AlarmClock.h"

class Alarm{
    private:
        Radio radio{};
        Scale scale{};
        AlarmClock alarmClock{};

        TFT_eSPI tft = TFT_eSPI();
    public:
        Alarm();
        void update();
        bool shouldSound();
};