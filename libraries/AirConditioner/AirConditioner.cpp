#include "Arduino.h"
#include "AirConditioner.h"
#include "IRremote.h"

AirConditioner::AirConditioner(){ };

// void AirConditioner::Init(int forcaVentilador, int temperatura, int modoTrabalho, int tempoTimer, int tipoGraus, int timer)
void AirConditioner::Init()
{
    AirConditionerEstateViewModel.OnOff = 1;
    AirConditionerEstateViewModel.FanMode = 3; // ForcaVentilador
    AirConditionerEstateViewModel.Degrees = 14; // Temperatura
    AirConditionerEstateViewModel.CoolMode = 1; // ModoTrabalho
    AirConditionerEstateViewModel.SleepClock = 0; // TempoTimer
    AirConditionerEstateViewModel.DegreesMode = 1; // TipoGraus
    AirConditionerEstateViewModel.SleepOnOff = 0; // Timer

    SetParametrosDto();

    IRsend emissorIR;
};

void AirConditioner::PrintEstadoAtual()
{
    Serial.println("Estado Atual");

    Serial.print("Ligado: ");
    Serial.println(AirConditionerEstateViewModel.OnOff);

    Serial.print("Força Ventilador: ");
    Serial.print(AirConditionerEstateViewModel.FanMode);
    Serial.print(" .. Bin: ");
    Serial.println(ForcaVentiladorCodes[AirConditionerEstateViewModel.FanMode]);

    Serial.print("Temperatura: ");
    Serial.print(AirConditionerEstateViewModel.Degrees);
    Serial.print(" ...... Bin: ");
    Serial.println(TemperaturaCodes[AirConditionerEstateViewModel.Degrees]);

    Serial.print("Modo Trabalho: ");
    Serial.print(AirConditionerEstateViewModel.CoolMode);
    Serial.print(" ..... Bin: ");
    Serial.println(ModoTrabalhoCodes[AirConditionerEstateViewModel.CoolMode]);

    Serial.print("Tempo Timer: ");
    Serial.print(AirConditionerEstateViewModel.SleepClock);
    Serial.print(" ....... Bin: ");
    Serial.print(TempoTimerCodes[AirConditionerEstateViewModel.SleepClock]);
    Serial.println("");

    Serial.print("Tipo Graus: ");
    Serial.print(AirConditionerEstateViewModel.DegreesMode);
    Serial.print(" ........ Bin: ");
    Serial.print(TipoGrausCodes[AirConditionerEstateViewModel.DegreesMode]);
    Serial.println("");

    Serial.print("Timer: ");
    Serial.print(AirConditionerEstateViewModel.SleepOnOff);
    Serial.print(" ............. Bin: ");
    Serial.print(TimerCodes[AirConditionerEstateViewModel.SleepOnOff]);
    Serial.println("");

    Serial.print("EndProtocol: ");
    Serial.print(EndProtocol);
    Serial.print(" ...... Bin: ");
    Serial.print(EndProtocolCodes[EndProtocol]);
    Serial.println("");

    Serial.print(ForcaVentiladorCodes[AirConditionerEstateViewModel.FanMode]);
    Serial.print(" ");
    Serial.print(TemperaturaCodes[AirConditionerEstateViewModel.Degrees]);
    Serial.print(" ");
    Serial.print(ModoTrabalhoCodes[AirConditionerEstateViewModel.CoolMode]);
    Serial.print(" ");
    Serial.print(TempoTimerCodes[AirConditionerEstateViewModel.SleepClock]);
    Serial.print(" ");
    Serial.print(TipoGrausCodes[AirConditionerEstateViewModel.DegreesMode]);
    Serial.print(" ");
    Serial.print(TimerCodes[AirConditionerEstateViewModel.SleepOnOff]);
    Serial.print(" ");
    Serial.println(EndProtocolCodes[EndProtocol]);

    Serial.println(" ------ ");
};

void AirConditioner::SetParametrosDto(){

    AirConditionerEstateDto = AirConditionerEstateViewModel;

};

void AirConditioner::LigaDesliga()
{
    if (AirConditionerEstateViewModel.OnOff == 1)
    {
        AirConditionerEstateViewModel.OnOff = 0;
        emissorIR.sendWhynter(StringToInt32(offCode), 32);
    }
    else
    {
        AirConditionerEstateViewModel.OnOff = 1;
        emissorIR.sendWhynter(GetEstadoCode(), 32);
    };

    SetParametrosDto();
};

void AirConditioner::AlteraForcaVentilador()
{
    int arraySize = sizeof(ForcaVentiladorCodes) / sizeof(ForcaVentiladorCodes[0]);

    if (AirConditionerEstateViewModel.FanMode >= (arraySize - 1))
    {
        AirConditionerEstateViewModel.FanMode = 0;
    }
    else
    {
        AirConditionerEstateViewModel.FanMode += 1;
    };

    SetParametrosDto();
};

void AirConditioner::AlteraTemperaturaAumentar()
{
    //Se modo Ventilador temperatura "010110"
    if (AirConditionerEstateViewModel.CoolMode == 3)
    {
        SetParametrosDto();
        return;
    };

    //Se modo Aquecer faixa de temperatura reduzida
    int arraySize;
    if (AirConditionerEstateViewModel.CoolMode == 4)
    {
        arraySize = 17;
    }
    else
    {
        arraySize = sizeof(TemperaturaCodes) / sizeof(TemperaturaCodes[0]);
    };

    if (AirConditionerEstateViewModel.Degrees < (arraySize - 1))
    {
        AirConditionerEstateViewModel.Degrees += 1;
    };

    SetParametrosDto();
};

void AirConditioner::AlteraTemperaturaDiminuir()
{
    //Se Modo Ventilador temperatura "010110"
    if (AirConditionerEstateViewModel.CoolMode == 3)
    {
        SetParametrosDto();
        return;
    };

    //Se modo Aquecer faixa de temperatura reduzida
    int arraySize;
    if (AirConditionerEstateViewModel.CoolMode == 4)
    {
        arraySize = 17;
    }
    else
    {
        arraySize = sizeof(TemperaturaCodes) / sizeof(TemperaturaCodes[0]);
    };

    if (AirConditionerEstateViewModel.Degrees > 0)
    {
        AirConditionerEstateViewModel.Degrees -= 1;
    };

    SetParametrosDto();
};

void AirConditioner::AlteraModoTrabalho()
{
    //Se Modo Ventilador temperatura "010110"
    // if (ModoTrabalho == 3)
    // {
    //     Temperatura = 10;
    //     SetParametrosDto();
    //     return;
    // };

    int arraySize = sizeof(ModoTrabalhoCodes) / sizeof(ModoTrabalhoCodes[0]);

    if (AirConditionerEstateViewModel.CoolMode >= (arraySize - 1))
    {
        AirConditionerEstateViewModel.CoolMode = 0;
    }
    else
    {
        AirConditionerEstateViewModel.CoolMode += 1;
    };

    SetParametrosDto();
};

void AirConditioner::AlteraTimer()
{
    if (AirConditionerEstateViewModel.SleepClock > 0)
    {
        AirConditionerEstateViewModel.SleepOnOff = 1;
    }
    else
    {
        AirConditionerEstateViewModel.SleepOnOff = 0;
    }
};

void AirConditioner::AlteraTempoTimerAumentar()
{
    int arraySize = sizeof(TempoTimerCodes) / sizeof(TempoTimerCodes[0]);

    if (AirConditionerEstateViewModel.SleepClock < (arraySize - 1))
    {
        AirConditionerEstateViewModel.SleepClock += 1;
    };

    AlteraTimer();
    SetParametrosDto();
};

void AirConditioner::AlteraTempoTimerDiminuir()
{
    int arraySize = sizeof(TempoTimerCodes) / sizeof(TempoTimerCodes[0]);

    if (AirConditionerEstateViewModel.SleepClock > 0)
    {
        AirConditionerEstateViewModel.SleepClock -= 1;
    };

    AlteraTimer();
    SetParametrosDto();
};

void AirConditioner::AlteraTipoGraus()
{
    AirConditionerEstateViewModel.DegreesMode == 0 ? AirConditionerEstateViewModel.DegreesMode = 1 : AirConditionerEstateViewModel.DegreesMode = 0;
    SetParametrosDto();
};

void AirConditioner::AlteraEndProtocol(int endProtocolIndex)
{
    EndProtocol = endProtocolIndex;
};

uint32_t AirConditioner::GetEstadoCode()
{

    // Modo Inteligente
    if (AirConditionerEstateViewModel.CoolMode == 0 && AirConditionerEstateViewModel.DegreesMode == 0)
    {
        return StringToInt32(inteligenteCodeCelsius);
    }
    else if (AirConditionerEstateViewModel.CoolMode == 0 && AirConditionerEstateViewModel.DegreesMode == 1)
    {
        return StringToInt32(inteligenteCodeFahrenheit);
    };

    // Modo Desumidificar
    if (AirConditionerEstateViewModel.CoolMode == 2 && AirConditionerEstateViewModel.DegreesMode == 0)
    {
        return StringToInt32(desumidificarCodeCelsius);
    }
    else if (AirConditionerEstateViewModel.CoolMode == 2 && AirConditionerEstateViewModel.DegreesMode == 1)
    {
        return StringToInt32(desumidificarCodeFahrenheit);
    };

    String parametroCompleto = ForcaVentiladorCodes[AirConditionerEstateViewModel.FanMode] + TemperaturaCodes[AirConditionerEstateViewModel.Degrees] + ModoTrabalhoCodes[AirConditionerEstateViewModel.CoolMode] + TempoTimerCodes[AirConditionerEstateViewModel.SleepClock] + TipoGrausCodes[AirConditionerEstateViewModel.DegreesMode] + TimerCodes[AirConditionerEstateViewModel.SleepOnOff] + Adress + EndProtocolCodes[EndProtocol];
    return StringToInt32(parametroCompleto);
};

uint32_t AirConditioner::StringToInt32(String binary)
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

void AirConditioner::EnviaParametroIR(int endProtocolIndex)
{

    /*
        FAZER FOR NESSE MÉTODO, COMPARAR MODO PARA ENVIAR CÓDIGO FIXO
    */
    if (AirConditionerEstateViewModel.OnOff == 0)
    {
        LigaDesliga();
    }

    if (AirConditionerEstateViewModel.CoolMode == 0 || AirConditionerEstateViewModel.CoolMode == 2)
    {
        emissorIR.sendWhynter(GetEstadoCode(), 32);
        return;
    };

    if (endProtocolIndex == -1)
    { //Passa por todos os parametros possíveis do EndProtocolo
        for (int i = 0; i < 16; i++)
        {
            AlteraEndProtocol(i);
            emissorIR.sendWhynter(GetEstadoCode(), 32);
            //delay(tempoTecla);
        }
    }
    else
    { //Sabe exatamente qual parametro passar para o EndProtocolo
        AlteraEndProtocol(endProtocolIndex);
        emissorIR.sendWhynter(GetEstadoCode(), 32);
        //delay(tempoTecla);
    }
};