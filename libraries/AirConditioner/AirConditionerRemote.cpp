#include "Arduino.h"
#include "IRremote.h"

#include "AirConditionerRemote.h"

AirConditionerRemote::AirConditionerRemote(){};

AirConditionerRemote::AirConditionerRemote(AirConditionerState _estadoAtual)
{
    EstadoAtual = _estadoAtual;
    IRsend emissorIR;
};

void AirConditionerRemote::LigaDesliga()
{
    uint32_t LigaDesligaCode = EstadoAtual.AlteraLigadoDesligado();
};

void AirConditionerRemote::EnviaParametroIR(int endProtocolIndex) {
    if(endProtocolIndex == -1) { //Passa por todos os parametros possíveis do EndProtocolo
        for(int i = 0; i < 16; i++) { 
        EstadoAtual.AlteraEndProtocol(i); 
        emissorIR.sendWhynter(EstadoAtual.GetEstadoCode(), 32);
        //delay(tempoTecla);
        }
    } else { //Sabe exatamente qual parametro passar para o EndProtocolo        
        EstadoAtual.AlteraEndProtocol(endProtocolIndex); 
        emissorIR.sendWhynter(EstadoAtual.GetEstadoCode(), 32);
        //delay(tempoTecla);
    }
};