#ifndef AirConditionerRemote_h
#define AirConditionerRemote_h

#include "Arduino.h"
#include "AirConditionerState.h"
#include "IRremote.h"

class AirConditionerRemote {
    public:
        AirConditionerRemote();
        AirConditionerRemote(AirConditionerState _estadoAtual);

        void LigaDesliga();
        void EnviaParametroIR(int endProtocolIndex = -1);

        AirConditionerState EstadoAtual;
    private:
        uint32_t LigaDesligaCode;
        IRsend emissorIR;
};

#endif