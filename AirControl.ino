// INCLUSÃO DE BIBLIOTECA ------------------------------
#include <IRremote.h>
#include <PushButton.h>

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <AirConditioner.h>
#include <AirConditionerDisplay.h>

#include <TimeLib.h>
unsigned long minuteControl = millis();
unsigned int hourClock;
unsigned int minuteClock;

#define SCREEN_WIDTH 128    // OLED display width, in pixels
#define SCREEN_HEIGHT 64    // OLED display height, in pixels
#define OLED_RESET -1       // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// DIRETIVAS DE COMPILAÇÃO ------------------------------
#define tempoTecla 350
#define frequencia 38 // kHz

// DEFINIÇÃO DOS PINOS ------------------------------
#define pinReceptor 11
#define pinBot1 8
#define pinLed 12

#define pinLigaDesliga 36
#define pinClock 38
#define pinSleep 40
#define pinModoTrabalho 42
#define pinVentilador 44
#define pinModoTemperatura 46
#define pinMais  48
#define pinMenos 50

// INSTANCIANDO OBJETOS ------------------------------
IRrecv receptorIR(pinReceptor);
IRsend emissorIR;
PushButton botaoLeitura(pinBot1);

PushButton botaoLigaDesliga(pinLigaDesliga);
PushButton botaoClock(pinClock);
PushButton botaoSleep(pinSleep);
PushButton botaoModoTrabalho(pinModoTrabalho);
PushButton botaoVentilador(pinVentilador);
PushButton botaoModoTemperatura(pinModoTemperatura);
PushButton botaoMais(pinMais);
PushButton botaoMenos(pinMenos);

AirConditioner arCondicionado;
AirConditionerDisplay arCondicionadoDisplay;

// DECLARAÇÃO DAS FUNÇÕES DE LEITURA ------------------------------
void ircode   (decode_results *results);
void encoding (decode_results *results);
void dumpInfo (decode_results *results);
void dumpRaw  (decode_results *results);
void dumpCode (decode_results *results);

// DECLARAÇÃO VARIÁVEIS GLOBAIS ------------------------------
bool lerComando = false;
bool mudarClockAtivo = false;
bool mudarSleepAtivo = false;
 
void setup() {
  Serial.begin(9600);
  pinMode(pinLed, OUTPUT);

  // INICIANDO RECEPTOR IR
  receptorIR.enableIRIn();  

   // Inicia Funções Ar Condicionado - Parametros de Estado Default
  arCondicionado.Init(3, 14, 1, 0, 1, 0);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
      Serial.println(F("SSD1306 allocation failed"));
      for(;;); // Don't proceed, loop forever
  }   

  // Inicia Display Ar Condicionado - Parametros de Estado Default
  arCondicionadoDisplay.Init(display, arCondicionado.ParametrosDto);  

  // Inicia relógio zerado e passa para o display
  setTime(00,00,00,6,12,1990);  
  arCondicionadoDisplay.DrawClock(hour(), minute());

 
  Serial.println("Setup Concluído"); 
  Serial.println(" ------ ");  
}

void loop() {
  // MÉTODO PARA LEITURA E ATUALIZAÇÃO DAS PROPRIEDADES DO BOTÃO ------------------------------
  botaoLeitura.button_loop();

  botaoLigaDesliga.button_loop();
  botaoClock.button_loop();
  botaoSleep.button_loop();
  botaoModoTrabalho.button_loop();
  botaoVentilador.button_loop();
  botaoModoTemperatura.button_loop();
  botaoMais.button_loop();
  botaoMenos.button_loop();

  // BLOCO CONDIÇÕES PARA INICIAR LEITURA ------------------------------
  if (botaoLeitura.pressed() && !lerComando) {
    lerComando = true;
    digitalWrite(pinLed, HIGH);
  } else if (botaoLeitura.pressed() && lerComando) {
    lerComando = false;
    digitalWrite(pinLed, LOW);
  }

  if (botaoLigaDesliga.pressed()) {
    arCondicionado.LigaDesliga();
    arCondicionadoDisplay.DisplayUpdate(arCondicionado.ParametrosDto);  

    if(arCondicionado.ParametrosDto[0] == 1) {
      arCondicionadoDisplay.DrawClock(hour(), minute());
    }
  }
  if (botaoClock.pressed() && !mudarSleepAtivo) { Serial.print("botaoClock"); }
  
  if (botaoSleep.pressed() && !mudarClockAtivo) {        
    mudarSleepAtivo = !mudarSleepAtivo;
  }

  if(mudarSleepAtivo){
    if (botaoMais.pressed()){
      arCondicionado.AlteraTempoTimerAumentar();  
      arCondicionadoDisplay.DisplayUpdate(arCondicionado.ParametrosDto);  
    }    
    if (botaoMenos.pressed()){
      arCondicionado.AlteraTempoTimerDiminuir();  
      arCondicionadoDisplay.DisplayUpdate(arCondicionado.ParametrosDto);  
    }
  }
  
  if (botaoModoTrabalho.pressed() && !mudarClockAtivo && !mudarSleepAtivo) {
    arCondicionado.AlteraModoTrabalho();  
    arCondicionadoDisplay.DisplayUpdate(arCondicionado.ParametrosDto); 
  }
  if (botaoVentilador.pressed() && !mudarClockAtivo && !mudarSleepAtivo) {
    arCondicionado.AlteraForcaVentilador();  
    arCondicionadoDisplay.DisplayUpdate(arCondicionado.ParametrosDto);
  }
  if (botaoModoTemperatura.pressed() && !mudarClockAtivo && !mudarSleepAtivo) {
    arCondicionado.AlteraTipoGraus();
    arCondicionadoDisplay.DisplayUpdate(arCondicionado.ParametrosDto);
  }
  if (botaoMais.pressed() && !mudarClockAtivo && !mudarSleepAtivo) {
    arCondicionado.AlteraTemperaturaAumentar();   
    arCondicionadoDisplay.DisplayUpdate(arCondicionado.ParametrosDto);
  }
  if (botaoMenos.pressed() && !mudarClockAtivo && !mudarSleepAtivo) {
    arCondicionado.AlteraTemperaturaDiminuir();   
    arCondicionadoDisplay.DisplayUpdate(arCondicionado.ParametrosDto);
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

  // ATUALIZA RELÓGIO ------------------------------
  if((millis() - minuteControl) > 59999){
    arCondicionadoDisplay.DrawClock(hour(), minute());
    minuteControl = millis();
  } 

  // BLOCO PARA RECEBER DADOS DA SERIAL E INICIAR EMISSOR IR ------------------------------
  if (Serial.available()) {
    char tecla = Serial.read();
    switch (tecla) {
      case 'z':
      { 
        arCondicionado.AlteraTemperaturaAumentar();   
        arCondicionadoDisplay.DisplayUpdate(arCondicionado.ParametrosDto);
        break;
      } 
      case 'x':
      {
        arCondicionado.AlteraTemperaturaDiminuir();   
        arCondicionadoDisplay.DisplayUpdate(arCondicionado.ParametrosDto);
        //arCondicionado.PrintEstadoAtual();   
        break;
      } 
      case 'c':
      {
        arCondicionado.AlteraTipoGraus();
        arCondicionadoDisplay.DisplayUpdate(arCondicionado.ParametrosDto);
        break;
      }   
      case 'v':
      {
        arCondicionado.AlteraModoTrabalho();  
        arCondicionadoDisplay.DisplayUpdate(arCondicionado.ParametrosDto);  
        break;
      } 
      case 'b':
      {
        arCondicionado.AlteraForcaVentilador();  
        arCondicionadoDisplay.DisplayUpdate(arCondicionado.ParametrosDto);  
        break;
      } 
      case 'n':
      {
        arCondicionado.AlteraTempoTimerAumentar();  
        arCondicionadoDisplay.DisplayUpdate(arCondicionado.ParametrosDto);  
        //arCondicionado.PrintEstadoAtual();
        break;
      }       
      case 'm':
      {
        arCondicionado.AlteraTempoTimerDiminuir();  
        arCondicionadoDisplay.DisplayUpdate(arCondicionado.ParametrosDto);  
        //arCondicionado.PrintEstadoAtual();
        break;
      } 
      case 'q':
      {
        arCondicionado.LigaDesliga();
        arCondicionadoDisplay.DisplayUpdate(arCondicionado.ParametrosDto);  
        break;
      } 
      
      case 'a':
      {
        //Aumenta hora
        hourClock =  hour() + 1;
        setTime(hourClock,minute(),00,6,12,1990); 
        arCondicionadoDisplay.DrawClock(hourClock, minuteClock);        
        break;
      } 
      case 's':
      {
        //Aumenta minuto
        minuteClock = minute() + 1;
        setTime(hour(),minuteClock,00,6,12,1990); 
        arCondicionadoDisplay.DrawClock(hourClock, minuteClock);    
        break;
      }        
    }
  }
}
