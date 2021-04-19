#ifndef AirConditionerManager_h
#define AirConditionerManager_h

#include "Arduino.h"
#include "AirConditionerState.h"
#include "AirConditionerRemote.h"

class AirConditionerManager {
    public:
        AirConditionerManager();
        AirConditionerState EstadoAr;
        AirConditionerRemote RemoteAir;

        void LigaDesliga();
        void EnviaParametroIR(int endProtocolIndex = -1);
        void PrintEstadoAtual();       
        void AlteraForcaVentilador();
        void AlteraTemperaturaAumentar();
        void AlteraTemperaturaDiminuir();
        void AlteraModoTrabalho();
        void AlteraTempoTimerAumentar();
        void AlteraTempoTimerDiminuir();
        void AlteraTipoGraus();
};

#endif