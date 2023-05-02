#pragma once

#include "time.h"
#include <ctime>
#include <sstream>
#include <string>
#include "Arduino.h"

class AlarmClock{
    private:
        std::tm wakeupTime = {};
        int alarmSpanSeconds = 3600;

        std::tm getCurrentTime();
    public:
        AlarmClock();
        void setWakeupTime(std::tm newTime);
        void setWakeupTime(std::string newTime);
        std::tm getWakeupTime();
        std::string getWakeupTimeStr();

        bool isAlarming();

};

std::string toReadableTime(std::tm time);
std::tm toTmTime(std::string timeStr);
int toSeconds(std::tm time);