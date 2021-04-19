#include "Arduino.h"
#include "AirConditionerDisplay.h"

AirConditionerDisplay::AirConditionerDisplay(){ };

void AirConditionerDisplay::Init(Adafruit_SSD1306 _display, int ParametrosDto[7]){ 

    display = _display;    

    // Show initial display buffer contents on the screen --  the library initializes this with an Adafruit splash screen.
    display.display();
    delay(2000); // Pause for 2 seconds

    // Clear the buffer
    display.clearDisplay();    

    DisplayUpdate(ParametrosDto);
    // DrawClock(10, 33);

    display.display();
};

void AirConditionerDisplay::DisplayUpdate(int ParametrosDto[7]){    

    //On/Off
    if(ParametrosDto[0] != ParametrosAnterioresDto[0] && ParametrosDto[0] == 0) {
        display.clearDisplay();
        display.display();
        ParametrosAnterioresDto[0] = ParametrosDto[0];
        return;
    } else if(ParametrosDto[0] != ParametrosAnterioresDto[0] && ParametrosDto[0] == 1) {
        display.clearDisplay();
        DrawDegrees(ParametrosDto[2], ParametrosDto[5]);
        DrawTemperatureMode(ParametrosDto[5]);
        DrawCoolMode(ParametrosDto[3]);
        DrawFanMode(ParametrosDto[1]);
        DrawSleepMode(ParametrosDto[4]);
        ParametrosAnterioresDto[0] = ParametrosDto[0];
        ParametrosAnterioresDto[1] = ParametrosDto[1];
        ParametrosAnterioresDto[2] = ParametrosDto[2];
        ParametrosAnterioresDto[3] = ParametrosDto[3];
        ParametrosAnterioresDto[4] = ParametrosDto[4];
        ParametrosAnterioresDto[5] = ParametrosDto[5];
        return;
    };

    // DrawDegrees
    if(ParametrosDto[2] != ParametrosAnterioresDto[2]) {
        DrawDegrees(ParametrosDto[2], ParametrosDto[5]);
        ParametrosAnterioresDto[2] = ParametrosDto[2];
    };

    // DrawTemperatureMode
    if(ParametrosDto[5] != ParametrosAnterioresDto[5]) {
        DrawTemperatureMode(ParametrosDto[5]);
        DrawDegrees(ParametrosDto[2], ParametrosDto[5]);
        ParametrosAnterioresDto[5] = ParametrosDto[5];
    };

    // DrawCoolMode
    if(ParametrosDto[3] != ParametrosAnterioresDto[3]) {
        DrawCoolMode(ParametrosDto[3]);
        ParametrosAnterioresDto[3] = ParametrosDto[3];
    };

    // DrawFanMode
    if(ParametrosDto[1] != ParametrosAnterioresDto[1]) {
        DrawFanMode(ParametrosDto[1]);
        ParametrosAnterioresDto[1] = ParametrosDto[1];
    };

    // DrawSleepMode
    if(ParametrosDto[4] != ParametrosAnterioresDto[4]) {
        DrawSleepMode(ParametrosDto[4]);
        ParametrosAnterioresDto[4] = ParametrosDto[4];
    };    
    
};

void AirConditionerDisplay::DrawCentreString(const String &buf, int x, int y)
{
    display.setTextSize(3);
    display.setTextColor(SSD1306_WHITE);

    int16_t x1, y1;
    uint16_t w, h;
    display.getTextBounds(buf, x, y, &x1, &y1, &w, &h); //calc width of new string

    int16_t cursorX = (display.width() - w) / 2;
    int16_t cursorY = (display.height() - h) / 2;

    display.setCursor(cursorX, cursorY);
    
    display.print(buf);
    display.display();
};

void AirConditionerDisplay::DrawDegrees(int degreeIndex, int temperatureIndex){
    CleanDegrees();

    if(temperatureIndex == 0) {
        DrawCentreString(TemperatureCelsius[degreeIndex], 0, -38);  
    };

    if(temperatureIndex == 1) {
        DrawCentreString(TemperatureFahrenheit[degreeIndex], 0, -38);  
    };
      
    display.display();
};

void AirConditionerDisplay::CleanDegrees(){
    display.fillRect (0, 13, 128, 21, SSD1306_BLACK);
};

void AirConditionerDisplay::DrawFanMode(int fanModeIndex){
    CleanFanMode();

    switch(fanModeIndex) {
        case 0: 
            display.drawBitmap(20, 47, fan3Icon, 16, 16, 1);
            break;
        case 1: 
            display.drawBitmap(20, 47, fan2Icon, 16, 16, 1);
            break;
        case 2: 
            display.drawBitmap(20, 47, fan1Icon, 16, 16, 1);
            break;
        case 3: 
            display.drawBitmap(20, 47, autoIcon, 16, 16, 1);
            break;   
    };

    display.display();
};

void AirConditionerDisplay::CleanFanMode(){
    display.fillRect (19, 48, 16, 16, SSD1306_BLACK);
};

void AirConditionerDisplay::DrawCoolMode(int coolModeIndex){

    CleanCoolMode();

    switch(coolModeIndex) {
        case 0: 
            display.drawBitmap(0, 48, autoIcon, 16, 16, 1);
            break;
        case 1: 
            display.drawBitmap(0, 48, airConditionerIcon, 16, 16, 1);
            break;
        case 2: 
            display.drawBitmap(0, 48, umidityIcon, 16, 16, 1);
            break;
        case 3: 
            display.drawBitmap(0, 48, fan3Icon, 16, 16, 1);
            break;
        case 4: 
            display.drawBitmap(0, 48, heaterIcon, 16, 16, 1);
            break;      
    };
    
    display.display();
};

void AirConditionerDisplay::CleanCoolMode(){
    display.fillRect (0, 48, 16, 16, SSD1306_BLACK);
};

void AirConditionerDisplay::DrawTemperatureMode(int temperatureIndex){
    CleanTemperatureMode();

    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(113, -2); 
    display.write(9);
    display.setCursor(119, 0); 

    if(temperatureIndex == 0) {
        display.print(F("C"));
    };

    if(temperatureIndex == 1) {
        display.print(F("F"));
    };

    display.display();
};

void AirConditionerDisplay::CleanTemperatureMode(){
    display.fillRect (114, 0, 14, 7, SSD1306_BLACK);
};

void AirConditionerDisplay::DrawSleepMode(int sleepModeIndex){
    sleepHours = "";
    sleepHours.concat(sleepModeIndex);
    sleepHours.concat("h");

    CleanSleepMode(); 

    if(sleepModeIndex > 0) {
        display.drawBitmap(40, 48, sleepOnIcon, 16, 16, 1);
        display.setCursor(60, 53);  
        display.setTextSize(1);
        display.setTextColor(SSD1306_WHITE);
        display.print(sleepHours);
    }    
    
    display.display();
};

void AirConditionerDisplay::CleanSleepMode(){
    display.fillRect (38, 48, 40, 16, SSD1306_BLACK);
};

void AirConditionerDisplay::DrawClock(int hour, int min){    
    CleanDrawClock();

    clockHours = "";
    clockHours.concat(hour);
    clockHours.concat(":");
    if(min < 10 ) { clockHours.concat("0"); };
    clockHours.concat(min);       

    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(95, 53);  
    display.print(clockHours);    

    // Corrige falha na tela
    display.fillRect (0, 0, 10, 10, SSD1306_BLACK);  

    display.display();
};

void AirConditionerDisplay::CleanDrawClock(){    
    display.fillRect (78, 48, 50, 16, SSD1306_BLACK); 
};

