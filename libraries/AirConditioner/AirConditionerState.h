#ifndef AirConditionerState_h
#define AirConditionerState_h

#include "Arduino.h"

class AirConditionerState {
    public:
        AirConditionerState();
        AirConditionerState(int forcaVentilador, int temperatura, int modoTrabalho, int tempoTimer, int tipoGraus, int timer);   
        
        void PrintEstadoAtual();       
        uint32_t AlteraLigadoDesligado();
        void AlteraForcaVentilador();
        void AlteraTemperaturaAumentar();
        void AlteraTemperaturaDiminuir();
        void AlteraModoTrabalho();
        void AlteraTempoTimerAumentar();
        void AlteraTempoTimerDiminuir();         
        void AlteraTipoGraus();       
        void AlteraEndProtocol(int endProtocolIndex);

        uint32_t GetEstadoCode();

    private:        
        void AlteraTimer();
        uint32_t StringToInt32(String binary);

        // DECLARAÇÃO DE TODOS OS CÓDIGOS POSSÍVEIS ------------------------------
        String ForcaVentiladorCodes[4] = {"100", "101", "110", "111"}; // Forte = 100; Médio = 101; Fraco = 110; Inteligente = 111;
        String TemperaturaCodes[29] = {"000010", "000100", "000110", "001000", "001010", "001100", "001110", "010000", "010010", "010100", "010110", "011000", "011010", "011100", "011110", "100000", "100010", "100100", "100110", "101000", "101010", "101100", "101110", "110000", "110010", "110100", "110110", "111000", "111010"}; // 61ºF = 000010; 89ºF = 111010; => Atributo em Fº e pula de 2 em 2 (decimal)
        String ModoTrabalhoCodes[5] = {"inteligente", "00", "01", "10", "11"}; // Inteligente = Inteligente; Ar = 00; Desumidificar = 01; Ventilador = 10; Aquecedor = 11;
        String TempoTimerCodes[25] = {"00000", "00001", "00010", "00011", "00100", "00101", "00110", "00111", "01000", "01001", "01010", "01011", "01100", "01101", "01110", "01111", "10000", "10001", "10010", "10011", "10100", "10101", "10110", "10111", "11000"}; // 0h = 00000; 24h = 11000; => Esse atributo vai de 0 a 2
        String TipoGrausCodes[2] = {"000", "001"}; // Cº = 000 ; Fº = 001
        String TimerCodes[2] = {"0", "1"}; // Timer Desligado = 0; Timer Ligado = 1;
        String EndProtocolCodes[16] = {"0000", "0001", "0010", "0011", "0100", "0101", "0110", "0111", "1000", "1001", "1010", "1011", "1100", "1101", "1110", "1111"}; // NÃO SEI O QUE É ESSA PUTARIA, POSSÍVEL PADRÃO DO INFERNO QUE ALGUM ENGENHEIRO FILHO DA PUTA TIROU DO CU PRA ME FODER

        String offCode = "100000000000000101010100111"; //0x4000AA7 // WHYNTER  {800,750, 2950,2900, 800,750, 750,750, 800,700, 800,750, 750,750, 850,2200, 800,700, 800,700, 800,750, 800,700, 800,750, 750,750, 800,750, 750,750, 750,750, 800,750, 750,750, 800,750, 750,750, 750,750, 800,2200, 800,700, 800,2250, 800,700, 800,2200, 800,750, 800,2150, 850,700, 800,750, 750,2200, 800,2200, 800,2200, 800};
        String desumidificarCodeCelsius = "11101011001000000000101010100000"; //0xEB200AA0
        String desumidificarCodeFahrenheit = "11101011001000000010101010101110"; //0xEB202AAE
        String inteligenteCodeCelsius = "11101011100000000000101010101010"; //0xEB800AAA
        String inteligenteCodeFahrenheit = "11101011100000000010101010101000"; //0xEB800AAA

        bool Ligado = false;
        int ForcaVentilador;
        int Temperatura;
        int ModoTrabalho;
        int TempoTimer;
        int TipoGraus;
        int Timer;
        String Adress = "10101010"; // Padrão em todo código => Possível Adress
        int EndProtocol;
};

#endif