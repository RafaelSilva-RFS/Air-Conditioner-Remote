#include "Arduino.h"
#include "AirConditioner.h"
#include "IRremote.h"

AirConditioner::AirConditioner(){ };

void AirConditioner::Init(int forcaVentilador, int temperatura, int modoTrabalho, int tempoTimer, int tipoGraus, int timer)
{
    ForcaVentilador = forcaVentilador;
    Temperatura = temperatura;
    ModoTrabalho = modoTrabalho;
    TempoTimer = tempoTimer;
    TipoGraus = tipoGraus;
    Timer = timer;
    EndProtocol = 12;

    SetParametrosDto();

    IRsend emissorIR;
};

void AirConditioner::PrintEstadoAtual()
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

    Serial.print(ForcaVentiladorCodes[ForcaVentilador]);
    Serial.print(" ");
    Serial.print(TemperaturaCodes[Temperatura]);
    Serial.print(" ");
    Serial.print(ModoTrabalhoCodes[ModoTrabalho]);
    Serial.print(" ");
    Serial.print(TempoTimerCodes[TempoTimer]);
    Serial.print(" ");
    Serial.print(TipoGrausCodes[TipoGraus]);
    Serial.print(" ");
    Serial.print(TimerCodes[Timer]);
    Serial.print(" ");
    Serial.println(EndProtocolCodes[EndProtocol]);

    Serial.println(" ------ ");
};

void AirConditioner::SetParametrosDto(){

    if (Ligado == true)
    {
        ParametrosDto[0] = 1;
    }
    else
    {
        ParametrosDto[0] = 0;
    };

    ParametrosDto[1] = ForcaVentilador;
    ParametrosDto[2] = Temperatura;
    ParametrosDto[3] = ModoTrabalho;
    ParametrosDto[4] = TempoTimer;
    ParametrosDto[5] = TipoGraus;
    ParametrosDto[6] = Timer;    
};

void AirConditioner::LigaDesliga()
{
    if (Ligado == true)
    {
        Ligado = false;
        emissorIR.sendWhynter(StringToInt32(offCode), 32);
    }
    else
    {
        Ligado = true;
        emissorIR.sendWhynter(GetEstadoCode(), 32);
    };

    SetParametrosDto();
};

void AirConditioner::AlteraForcaVentilador()
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

    SetParametrosDto();
};

void AirConditioner::AlteraTemperaturaAumentar()
{
    //Se modo Ventilador temperatura "010110"
    if (ModoTrabalho == 3)
    {
        SetParametrosDto();
        return;
    };

    //Se modo Aquecer faixa de temperatura reduzida
    int arraySize;
    if (ModoTrabalho == 4)
    {
        arraySize = 17;
    }
    else
    {
        arraySize = sizeof(TemperaturaCodes) / sizeof(TemperaturaCodes[0]);
    };

    if (Temperatura < (arraySize - 1))
    {
        Temperatura += 1;
    };

    SetParametrosDto();
};

void AirConditioner::AlteraTemperaturaDiminuir()
{
    //Se Modo Ventilador temperatura "010110"
    if (ModoTrabalho == 3)
    {
        SetParametrosDto();
        return;
    };

    //Se modo Aquecer faixa de temperatura reduzida
    int arraySize;
    if (ModoTrabalho == 4)
    {
        arraySize = 17;
    }
    else
    {
        arraySize = sizeof(TemperaturaCodes) / sizeof(TemperaturaCodes[0]);
    };

    if (Temperatura > 0)
    {
        Temperatura -= 1;
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

    if (ModoTrabalho >= (arraySize - 1))
    {
        ModoTrabalho = 0;
    }
    else
    {
        ModoTrabalho += 1;
    };

    SetParametrosDto();
};

void AirConditioner::AlteraTimer()
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

void AirConditioner::AlteraTempoTimerAumentar()
{
    int arraySize = sizeof(TempoTimerCodes) / sizeof(TempoTimerCodes[0]);

    if (TempoTimer < (arraySize - 1))
    {
        TempoTimer += 1;
    };

    AlteraTimer();
    SetParametrosDto();
};

void AirConditioner::AlteraTempoTimerDiminuir()
{
    int arraySize = sizeof(TempoTimerCodes) / sizeof(TempoTimerCodes[0]);

    if (TempoTimer > 0)
    {
        TempoTimer -= 1;
    };

    AlteraTimer();
    SetParametrosDto();
};

void AirConditioner::AlteraTipoGraus()
{
    TipoGraus == 0 ? TipoGraus = 1 : TipoGraus = 0;
    // if (TipoGraus == 0)
    // {
    //     TipoGraus = 1;
    // }
    // else
    // {
    //     TipoGraus = 0;
    // }

    SetParametrosDto();
};

void AirConditioner::AlteraEndProtocol(int endProtocolIndex)
{
    EndProtocol = endProtocolIndex;
};

uint32_t AirConditioner::GetEstadoCode()
{

    // Modo Inteligente
    if (ModoTrabalho == 0 && TipoGraus == 0)
    {
        return StringToInt32(inteligenteCodeCelsius);
    }
    else if (ModoTrabalho == 0 && TipoGraus == 1)
    {
        return StringToInt32(inteligenteCodeFahrenheit);
    };

    // Modo Desumidificar
    if (ModoTrabalho == 2 && TipoGraus == 0)
    {
        return StringToInt32(desumidificarCodeCelsius);
    }
    else if (ModoTrabalho == 2 && TipoGraus == 1)
    {
        return StringToInt32(desumidificarCodeFahrenheit);
    };

    String parametroCompleto = ForcaVentiladorCodes[ForcaVentilador] + TemperaturaCodes[Temperatura] + ModoTrabalhoCodes[ModoTrabalho] + TempoTimerCodes[TempoTimer] + TipoGrausCodes[TipoGraus] + TimerCodes[Timer] + Adress + EndProtocolCodes[EndProtocol];
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
    if (!Ligado)
    {
        LigaDesliga();
    }

    if (ModoTrabalho == 0 || ModoTrabalho == 2)
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