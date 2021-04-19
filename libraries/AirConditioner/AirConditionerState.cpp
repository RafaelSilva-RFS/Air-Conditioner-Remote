#include "Arduino.h"
#include "AirConditionerState.h"

AirConditionerState::AirConditionerState()
{
    ForcaVentilador = 3;
    Temperatura = 14;
    ModoTrabalho = 1;
    TempoTimer = 0;
    TipoGraus = 0;
    Timer = 0;
    EndProtocol = 12;
};

AirConditionerState::AirConditionerState(int forcaVentilador, int temperatura, int modoTrabalho, int tempoTimer, int tipoGraus, int timer)
{
    ForcaVentilador = forcaVentilador;
    Temperatura = temperatura;
    ModoTrabalho = modoTrabalho;
    TempoTimer = tempoTimer;
    TipoGraus = tipoGraus;
    Timer = timer;    
    EndProtocol = 12;
};

void AirConditionerState::PrintEstadoAtual()
{
    Serial.println("Estado Atual");

    Serial.print("Ligado: ");
    Serial.println(Ligado);

    Serial.print("Força Ventilador: ");
    Serial.print(ForcaVentilador);
    Serial.print(" .. Bin: ");
    Serial.println(ForcaVentiladorCodes[ForcaVentilador]);

    Serial.print("Temperatura: ");
    Serial.print(Temperatura);
    Serial.print(" ...... Bin: ");
    Serial.println(TemperaturaCodes[Temperatura]);

    Serial.print("Modo Trabalho: ");
    Serial.print(ModoTrabalho);
    Serial.print(" ..... Bin: ");
    Serial.println(ModoTrabalhoCodes[ModoTrabalho]);

    Serial.print("Tempo Timer: ");
    Serial.print(TempoTimer);
    Serial.print(" ....... Bin: ");
    Serial.print(TempoTimerCodes[TempoTimer]);
    Serial.println("");

    Serial.print("Tipo Graus: ");
    Serial.print(TipoGraus);
    Serial.print(" ........ Bin: ");
    Serial.print(TipoGrausCodes[TipoGraus]);        
    Serial.println("");

    Serial.print("Timer: ");
    Serial.print(Timer);
    Serial.print(" ............. Bin: ");
    Serial.print(TimerCodes[Timer]);    
    Serial.println("");
    
    Serial.print("EndProtocol: ");
    Serial.print(EndProtocol);
    Serial.print(" ...... Bin: ");
    Serial.print(EndProtocolCodes[EndProtocol]);    
    Serial.println("");

    Serial.println(" ------ ");
}

uint32_t AirConditionerState::AlteraLigadoDesligado()
{
    if (Ligado == true)
    {
        Ligado = false;
        return StringToInt32(offCode);
    }
    else
    {
        return GetEstadoCode();
    };    
};

void AirConditionerState::AlteraForcaVentilador()
{
    int arraySize = sizeof(ForcaVentiladorCodes) / sizeof(ForcaVentiladorCodes[0]);

    if (ForcaVentilador >= (arraySize - 1))
    {
        ForcaVentilador = 0;
    }
    else
    {
        ForcaVentilador += 1;
    };
};

void AirConditionerState::AlteraTemperaturaAumentar()
{
    //Se modo Ventilador temperatura "010110"
    if (ModoTrabalho == 3) { return; };

    //Se modo Aquecer faixa de temperatura reduzida 
    int arraySize;
    if (ModoTrabalho == 4) { 
        arraySize = 17;
    } else {
        arraySize = sizeof(TemperaturaCodes) / sizeof(TemperaturaCodes[0]);
    };    

    if (Temperatura < (arraySize - 1))
    {
        Temperatura += 1;
    };
};

void AirConditionerState::AlteraTemperaturaDiminuir()
{
    //Se Modo Ventilador temperatura "010110"
    if (ModoTrabalho == 3) { return; };

    //Se modo Aquecer faixa de temperatura reduzida 
    int arraySize;
    if (ModoTrabalho == 4) { 
        arraySize = 17;
    } else {
        arraySize = sizeof(TemperaturaCodes) / sizeof(TemperaturaCodes[0]);
    }; 

    if (Temperatura > 0)
    {
        Temperatura -= 1;
    };
};

void AirConditionerState::AlteraModoTrabalho()
{
    //Se Modo Ventilador temperatura "010110"
    if (ModoTrabalho == 3)
    {
        Temperatura = 10;
        return;
    };

    int arraySize = sizeof(ModoTrabalhoCodes) / sizeof(ModoTrabalhoCodes[0]);

    if (ModoTrabalho >= (arraySize - 1))
    {
        ModoTrabalho = 0;
    }
    else
    {
        ModoTrabalho += 1;
    };

};

void AirConditionerState::AlteraTimer()
{
    if (TempoTimer > 0)
    {
        Timer = 1;
    }
    else
    {
        Timer = 0;
    }
};

void AirConditionerState::AlteraTempoTimerAumentar()
{
    int arraySize = sizeof(TempoTimerCodes) / sizeof(TempoTimerCodes[0]);

    if (TempoTimer < (arraySize - 1))
    {
        TempoTimer += 1;
    };

    AlteraTimer();
};

void AirConditionerState::AlteraTempoTimerDiminuir()
{
    int arraySize = sizeof(TempoTimerCodes) / sizeof(TempoTimerCodes[0]);

    if (TempoTimer > 0)
    {
        TempoTimer -= 1;
    };

    AlteraTimer();
};

void AirConditionerState::AlteraTipoGraus()
{
    if (TipoGraus = 0)
    {
        TipoGraus = 1;
    }
    else
    {
        TipoGraus = 0;
    }
};

void AirConditionerState::AlteraEndProtocol(int endProtocolIndex)
{
    EndProtocol = endProtocolIndex;
};

uint32_t AirConditionerState::GetEstadoCode()
{
    // Modo Inteligente	
    if(ModoTrabalho == 0 && TipoGraus == 0) {
        return StringToInt32(inteligenteCodeCelsius);
    } else if(ModoTrabalho == 0 && TipoGraus == 1) {
        return StringToInt32(inteligenteCodeFahrenheit);
    }; 

    // Modo Desumidificar
    if(ModoTrabalho == 2 && TipoGraus == 0) {
        return StringToInt32(desumidificarCodeCelsius);
    } else if(ModoTrabalho == 2 && TipoGraus == 1) {
        return StringToInt32(desumidificarCodeFahrenheit);
    }; 

    String parametroCompleto = ForcaVentiladorCodes[ForcaVentilador] + TemperaturaCodes[Temperatura] + ModoTrabalhoCodes[ModoTrabalho] + TempoTimerCodes[TempoTimer] + TipoGrausCodes[TipoGraus] + TimerCodes[Timer] + Adress + EndProtocolCodes[EndProtocol];
    return StringToInt32(parametroCompleto);      
};

uint32_t AirConditionerState::StringToInt32(String binary)
{
    //String sample_str = "11001011010000000000101010100000"; //0xCB400AA0
    uint32_t result = 0;
    for (unsigned int i = 0; i < binary.length(); ++i)
    {
        result = result << 1;
        result = result | (binary[i] & 1);
    }
    return result;
};