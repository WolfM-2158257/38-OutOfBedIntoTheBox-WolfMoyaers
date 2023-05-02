#pragma once
#include "Arduino.h"
#include "HX711_ADC.h"

class Scale{
    private:
        // Define connections for HX711 Amplifier Module
        static const int HX711_sck = 25;  // mcu > HX711 sck pin
        static const int HX711_dout = 26; // mcu > HX711 dout pin

        HX711_ADC loadCell;

        const float calibrationValue = 22.95;
        unsigned long prevTime = 0;

        float weight = 0.0f;
        float treshhold = 500.0f;

    public:
        Scale();
        float measureWeight();
        bool containsItem();
        void tare();
        void setTreshhold(std::string treshhold);
        float getTreshhold();
};