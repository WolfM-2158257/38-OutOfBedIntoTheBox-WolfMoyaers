#pragma once

#include <ctime>
#include "time.h"
#include <sstream>
#include <iomanip>
#include "Arduino.h"

class AlarmClock{
    private:
        std::tm wakeupTime = {};
        int alarmSpanSeconds = 3600;

        std::tm getCurrentTime();
    public:
        AlarmClock();
        void setWakeupTime(std::tm newTime);
        void setWakeupTime(String newTime);
        std::tm getWakeupTime();
        String getWakeupTimeStr();

        bool isAlarming();

};

String toReadableTime(std::tm time);
std::tm toTmTime(String timeStr);
int toSeconds(std::tm time);