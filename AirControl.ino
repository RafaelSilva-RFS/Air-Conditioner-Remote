// INCLUSÃO DE BIBLIOTECA ------------------------------
#include <IRremote.h>
#include <PushButton.h>


// DIRETIVAS DE COMPILAÇÃO ------------------------------
#define tempoTecla 350
#define frequencia 38 // kHz

// DEFINIÇÃO DOS PINOS ------------------------------
#define pinReceptor 11
#define pinBot1 8
#define pinLed 12

// INSTANCIANDO OBJETOS ------------------------------
IRrecv receptorIR(pinReceptor);
IRsend emissorIR;
PushButton botaoLeitura(pinBot1);

// DECLARAÇÃO DAS FUNÇÕES DE LEITURA ------------------------------
void ircode   (decode_results *results);
void encoding (decode_results *results);
void dumpInfo (decode_results *results);
void dumpRaw  (decode_results *results);
void dumpCode (decode_results *results);

//  DECLARAÇÃO DAS TECLAS CLONADAS ------------------------------
uint32_t offCode = 0x4000AA7; // WHYNTER  {800,750, 2950,2900, 800,750, 750,750, 800,700, 800,750, 750,750, 850,2200, 800,700, 800,700, 800,750, 800,700, 800,750, 750,750, 800,750, 750,750, 750,750, 800,750, 750,750, 800,750, 750,750, 750,750, 800,2200, 800,700, 800,2250, 800,700, 800,2200, 800,750, 800,2150, 850,700, 800,750, 750,2200, 800,2200, 800,2200, 800};
uint32_t desumidificarCodeCelsius = 0xEB200AA0;
uint32_t desumidificarCodeFahrenheit = 0xEB202AAE;
uint32_t autoCodeCelsius = 0xEB800AAA;
uint32_t autoCodeFahrenheit = 0xEB800AAA;

// DECLARAÇÃO DE TODOS OS CÓDIGOS POSSÍVEIS ------------------------------
String ForcaVentilador[4] = {"100", "101", "110", "111"}; // Forte = 100; Médio = 101; Fraco = 110; Inteligente = 111;
String Temperatura[29] = {"000010", "000100", "000110", "001000", "001010", "001100", "001110", "010000", "010010", "010100", "010110", "011000", "011010", "011100", "011110", "100000", "100010", "100100", "100110", "101000", "101010", "101100", "101110", "110000", "110010", "110100", "110110", "111000", "111010"}; // 61ºF = 000010; 89ºF = 111010; => Atributo em Fº e pula de 2 em 2 (decimal)
String ModoTrabalho[4] = {"00", "01", "10", "11"}; // Ar = 00; Desumidificar = 01; Ventilador = 10; Aquecedor = 11;
String TempoTimer[25] = {"00000", "00001", "00010", "00011", "00100", "00101", "00110", "00111", "01000", "01001", "01010", "01011", "01100", "01101", "01110", "01111", "10000", "10001", "10010", "10011", "10100", "10101", "10110", "10111", "11000"}; // 0h = 00000; 24h = 11000; => Esse atributo vai de 0 a 2
String TipoGraus[2] = {"000", "001"}; // Cº = 000 ; Fº = 001
String Timer[2] = {"0", "1"}; // Desligado = 0; Ligado = 1;
String EndProtocol[16] = {"0000", "0001", "0010", "0011", "0100", "0101", "0110", "0111", "1000", "1001", "1010", "1011", "1100", "1101", "1110", "1111"}; //NÃO SEI O QUE É ESSA PUTARIA, POSSÍVEL PADRÃO DO INFERNO QUE ALGUM ENGENHEIRO TIROU DO CU PRA ME FODER]

// CLASSE DE ESTADO ------------------------------
class State{
  public:  
    State();
    State(int forcaVentilador, int temperatura, int modoTrabalho, int tempoTimer, int tipoGraus, int timer);   

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

State::State(): ForcaVentilador(3), Temperatura(14), ModoTrabalho(0), TempoTimer(0), TipoGraus(0), Timer(0), EndProtocol(12) { }

State::State(int forcaVentilador, int temperatura, int modoTrabalho, int tempoTimer, int tipoGraus, int timer) 
  : ForcaVentilador(forcaVentilador), Temperatura(temperatura), ModoTrabalho(modoTrabalho), TempoTimer(tempoTimer), TipoGraus(tipoGraus), Timer(timer) { }

// DECLARAÇÃO VARIÁVEIS GLOBAIS ------------------------------
bool lerComando = false;
State EstadoAtual;

// DECLARAÇÃO DE FUNÇÕES GLOBAIS ------------------------------
void EnviaParametroIR(State estado, int endProtocolIndex);
void LigaDesliga();
void ModoDesumidificar();
void ModoInteligente();
uint32_t MontaParametroIR(State estado, int endProtocolIndex);
uint32_t StringToInt32(String binary);
void PrintEstadoAtual();
 
void setup() {
  Serial.begin(9600);
  pinMode(pinLed, OUTPUT);

  // INICIANDO RECEPTOR IR
  receptorIR.enableIRIn();

  // Parametros de Estado Default  
  Serial.println("Setup Concluído"); 
  //Serial.println(MontaParametroIR(EstadoAtual, EstadoAtual.EndProtocol));
  Serial.println(" ------ ");
  PrintEstadoAtual();
}

void loop() {
  // MÉTODO PARA LEITURA E ATUALIZAÇÃO DAS PROPRIEDADES DO BOTÃO ------------------------------
  botaoLeitura.button_loop();

  // BLOCO CONDIÇÕES PARA INICIAR LEITURA ------------------------------
  if (botaoLeitura.pressed() && !lerComando) {
    lerComando = true;
    digitalWrite(pinLed, HIGH);
  } else if (botaoLeitura.pressed() && lerComando) {
    lerComando = false;
    digitalWrite(pinLed, LOW);
  }

  // LAÇO PARA LEITURA DO RECEPTOR IR QUANDO FOR PRESSIONADO O BOTÃO ------------------------------
  while (lerComando) {
    decode_results  results;
    if (receptorIR.decode(&results)) {
      ircode   (&results);
      Serial.println("");
      encoding (&results);
      dumpInfo (&results);
      dumpRaw  (&results);
      dumpCode (&results);
      receptorIR.resume();
      lerComando = false;
      digitalWrite(pinLed, LOW);
    }
  }

  // BLOCO PARA RECEBER DADOS DA SERIAL E INICIAR EMISSOR IR ------------------------------
  if (Serial.available()) {
    char tecla = Serial.read();
    switch (tecla) {
      case 'a':
      {
        uint32_t funcaoAr = StringToInt32("11001011010000000000101010100000");      
        emissorIR.sendWhynter(funcaoAr, 32);
        Serial.println("Enviando Tecla A clonada");
        delay(tempoTecla);
        break;
      }   
      case 'b':
      {
        // Liga / Desliga o ar     
        LigaDesliga();
        break;
      }  
    }
  }
}

void EnviaParametroIR(State estado, int endProtocolIndex = -1) {

  if(endProtocolIndex == -1) { //Passa por todos os parametros possíveis do EndProtocolo
    for(int i = 0; i < 16; i++) { 
      uint32_t parametroIR = MontaParametroIR(estado, i);  
      emissorIR.sendWhynter(parametroIR, 32);
      //delay(tempoTecla);
    }
  } else { //Sabe exatamente qual parametro passar para o EndProtocolo
    uint32_t parametroIR = MontaParametroIR(estado, endProtocolIndex);
    emissorIR.sendWhynter(parametroIR, 32);
    //delay(tempoTecla);
  }
  
}

void LigaDesliga() {
  if(EstadoAtual.Ligado == true) {
    Serial.println("Desligando"); 
    EstadoAtual.Ligado = false;
    emissorIR.sendWhynter(offCode, 32);
    //delay(tempoTecla);   
  } else {    
    Serial.println("Ligando");    
    EstadoAtual.Ligado = true;
    EnviaParametroIR(EstadoAtual);  
  }  
}

void ModoDesumidificar() {
  if(EstadoAtual.TipoGraus == 0) {
    emissorIR.sendWhynter(desumidificarCodeCelsius, 32);
  } else {
    emissorIR.sendWhynter(desumidificarCodeFahrenheit, 32);
  }  
}

void ModoInteligente() {
  if(EstadoAtual.TipoGraus == 0) {
    emissorIR.sendWhynter(autoCodeCelsius, 32);
  } else {
    emissorIR.sendWhynter(autoCodeFahrenheit, 32);
  } 
}

uint32_t MontaParametroIR(State estado, int endProtocolIndex) {  
  String forcaVentilador = ForcaVentilador[estado.ForcaVentilador];
  String temperatura = Temperatura[estado.Temperatura];
  String modoTrabalho = ModoTrabalho[estado.ModoTrabalho];
  String tempoTimer = TempoTimer[estado.TempoTimer];
  String tipoGraus = TipoGraus[estado.TipoGraus];
  String timer = Timer[estado.Timer];
  String endProtocol = EndProtocol[endProtocolIndex];
  String parametroCompleto = forcaVentilador + temperatura + modoTrabalho + tempoTimer + tipoGraus + timer + estado.Adress + endProtocol;  
  return StringToInt32(parametroCompleto);
}

uint32_t StringToInt32(String binary) {
  //String sample_str = "11001011010000000000101010100000"; //0xCB400AA0
  uint32_t result = 0;
  for (unsigned int i = 0; i < binary.length(); ++i)
  {
    result = result << 1;
    result = result | (binary[i] & 1);
  }
  return result;
}

void PrintEstadoAtual() {
  Serial.println("Estado Atual");
  
  Serial.print("Ligado: ");
  Serial.println(EstadoAtual.Ligado);
  
  Serial.print("Força Ventilador: ");
  Serial.print(EstadoAtual.ForcaVentilador);
  Serial.print("    Bin: ");  
  Serial.println(ForcaVentilador[EstadoAtual.ForcaVentilador]);
  
  Serial.print("Temperatura: ");
  Serial.print(EstadoAtual.Temperatura);
  Serial.print("        Bin: ");  
  Serial.println(Temperatura[EstadoAtual.Temperatura]);
  
  Serial.print("Modo Trabalho: ");
  Serial.print(EstadoAtual.ModoTrabalho);
  Serial.print("       Bin: ");  
  Serial.println(ModoTrabalho[EstadoAtual.ModoTrabalho]);
  
  Serial.print("Tempo Timer: ");
  Serial.print(EstadoAtual.TempoTimer);
  Serial.print("         Bin: ");  
  Serial.println(TempoTimer[EstadoAtual.TempoTimer]);
  
  Serial.print("Tipo Graus: ");
  Serial.print(EstadoAtual.TipoGraus);
  Serial.print("          Bin: ");  
  Serial.println(TipoGraus[EstadoAtual.TipoGraus]);
  
  Serial.print("Timer: ");
  Serial.print(EstadoAtual.Timer); 
  Serial.print("               Bin: ");  
  Serial.println(Timer[EstadoAtual.Timer]);
  
  Serial.println(" ------ ");
}
