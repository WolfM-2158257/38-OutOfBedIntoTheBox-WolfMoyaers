#pragma once

#include "time.h"
#include <ctime>
#include <sstream>
#include <string>
#include "Arduino.h"

class AlarmClock{
    private:
        std::tm wakeupTime = {};
        std::tm snoozeTime = {};
        int alarmSpanSeconds = 3600; // 1 hour
        int snoozeDuration = 5; // 5 minutes

        std::tm getCurrentTime();

        bool isSnoozeUninitialized();
    public:
        AlarmClock();
        void setWakeupTime(std::tm newTime);
        void setWakeupTime(std::string newTime);
        std::tm getWakeupTime();
        std::string getWakeupTimeStr();
        std::string getTimeTillWakeupStr();
        void setCurrentTime(std::string time);
        void setCurrentTime(int timeUnix);
        void setSnoozeDuration(std::string snoozeDuration);
        int getSnoozeDuration();

        bool isAlarming();

        void snooze();
        void resetSnooze();
};

std::string toReadableTime(std::tm time);
std::tm toTmTime(std::string timeStr);
std::tm toTmTime(int time);
int toSeconds(std::tm time);