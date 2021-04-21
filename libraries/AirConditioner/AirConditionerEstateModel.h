#ifndef AirConditionerEstateModel_h
#define AirConditionerEstateModel_h

#include "Arduino.h"

class AirConditionerEstateModel {
    public:
        AirConditionerEstateModel();
        int OnOff;
        int FanMode;
        int Degrees;
        int CoolMode;
        int SleepClock;
        int DegreesMode;
        int SleepOnOff;
};

#endif