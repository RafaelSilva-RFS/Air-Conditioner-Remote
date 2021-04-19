#include "AirConditionerManager.h"

#include "AirConditionerState.h"
#include "AirConditionerRemote.h"

AirConditionerManager::AirConditionerManager() { 
    AirConditionerState EstadoAr = AirConditionerState();
    AirConditionerRemote RemoteAir;
    RemoteAir = AirConditionerRemote(EstadoAr); 
    Serial.begin(9600);
};

void AirConditionerManager::LigaDesliga() {
    RemoteAir.LigaDesliga();
    PrintEstadoAtual();
};

void AirConditionerManager::EnviaParametroIR(int endProtocolIndex) {
    RemoteAir.EnviaParametroIR();
    PrintEstadoAtual();
};

void AirConditionerManager::PrintEstadoAtual() {
    EstadoAr.PrintEstadoAtual();
};       

void AirConditionerManager::AlteraForcaVentilador() {
    EstadoAr.AlteraForcaVentilador();
    PrintEstadoAtual();
};

void AirConditionerManager::AlteraTemperaturaAumentar() {
    EstadoAr.AlteraTemperaturaAumentar();
    PrintEstadoAtual();
};

void AirConditionerManager::AlteraTemperaturaDiminuir() {
    EstadoAr.AlteraTemperaturaDiminuir();
    PrintEstadoAtual();
};

void AirConditionerManager::AlteraModoTrabalho() {
    EstadoAr.AlteraModoTrabalho();
    PrintEstadoAtual();
};

void AirConditionerManager::AlteraTempoTimerAumentar() {
    EstadoAr.AlteraTempoTimerAumentar();
    PrintEstadoAtual();
};

void AirConditionerManager::AlteraTempoTimerDiminuir() {
    EstadoAr.AlteraTempoTimerDiminuir();
    PrintEstadoAtual();
};  

void AirConditionerManager::AlteraTipoGraus() {
    EstadoAr.AlteraTipoGraus();
    PrintEstadoAtual();
};   