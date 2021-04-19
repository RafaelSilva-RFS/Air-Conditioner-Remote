#ifndef AirConditionerDisplay_h
#define AirConditionerDisplay_h

#include "Arduino.h"

#include "SPI.h"
#include "Wire.h"
#include "Adafruit_GFX.h"
#include "Adafruit_SSD1306.h"

// Global icon variables
static const unsigned char PROGMEM autoIcon[] = {0x00,0x00,0x3c,0x3c,0x42,0x42,0x41,0x82,0x42,0x42,0x44,0x22,0x28,0x14,0x11,0x88,0x11,0x88,0x28,0x14,0x44,0x22,0x42,0x42,0x41,0x82,0x42,0x42,0x3c,0x3c,0x00,0x00};
static const unsigned char PROGMEM fan1Icon[] = {0x00,0x00,0x00,0x38,0x00,0x7c,0x00,0x7c,0x00,0x7e,0x00,0xfe,0x00,0xfe,0x01,0xc0,0x03,0x80,0x7f,0x00,0x7f,0x00,0x7e,0x00,0x3e,0x00,0x3e,0x00,0x1c,0x00,0x00,0x00};
static const unsigned char PROGMEM fan2Icon[] = {0x00,0x00,0x0e,0x00,0x3e,0x00,0x7e,0x1c,0x7c,0x3e,0x3e,0x7e,0x1f,0xfe,0x03,0xfe,0x01,0x8c,0x03,0x00,0x0f,0x80,0x1f,0x80,0x1f,0x80,0x0f,0x80,0x07,0x00,0x00,0x00};
static const unsigned char PROGMEM fan3Icon[] = {0x00,0x00,0x0e,0x38,0x3e,0x7c,0x7e,0x7c,0x7e,0x7e,0x7e,0xfe,0x3f,0xfe,0x07,0xc0,0x03,0xe0,0x7f,0xfc,0x7f,0x7e,0x7e,0x7e,0x3e,0x7e,0x3e,0x7c,0x1c,0x70,0x00,0x00};

static const unsigned char PROGMEM airConditionerIcon []={0x00,0x00,0x01,0x80,0x05,0xa0,0x03,0xc0,0x29,0x94,0x19,0x98,0x39,0x9c,0x06,0x60,0x06,0x60,0x39,0x9c,0x19,0x98,0x29,0x94,0x03,0xc0,0x05,0xa0,0x01,0x80,0x00,0x00};
static const unsigned char PROGMEM heaterIcon []={0x00,0x00,0x01,0x00,0x01,0x80,0x18,0x18,0x13,0xc8,0x07,0xe0,0x0f,0xf0,0x2f,0xf6,0x6f,0xf4,0x0f,0xf0,0x07,0xe0,0x13,0xc8,0x18,0x18,0x01,0x80,0x00,0x80,0x00,0x00};
static const unsigned char PROGMEM umidityIcon []={0x00,0x00,0x00,0x80,0x01,0xc0,0x03,0xe0,0x07,0xf0,0x0f,0xf0,0x0f,0xf8,0x1f,0xf8,0x17,0xf8,0x17,0xf8,0x17,0xf8,0x17,0xf8,0x0b,0xf0,0x05,0xe0,0x03,0xc0,0x00,0x00};
static const unsigned char PROGMEM clockTimeIcon []={0x00,0x00,0x03,0xc0,0x0f,0xf0,0x1f,0xf8,0x3e,0xfc,0x3e,0xfc,0x7e,0xfe,0x7e,0xfe,0x7e,0x7e,0x7f,0x3e,0x3f,0x9c,0x3f,0xfc,0x1f,0xf8,0x0f,0xf0,0x07,0xe0,0x00,0x00};
static const unsigned char PROGMEM sleepOnIcon []={0x00,0x00,0x03,0xc0,0x0f,0xf0,0x1f,0xf8,0x3e,0xfc,0x7e,0xde,0x7e,0x9e,0x7e,0x3e,0x7e,0x7e,0x7c,0x00,0x79,0xce,0x3a,0x29,0x1a,0x29,0x0a,0x29,0x01,0xc9,0x00,0x00};


class AirConditionerDisplay {
    public:       
        AirConditionerDisplay();
        void Init(Adafruit_SSD1306 _display, int ParametrosDto[7]); // Ligado, ForcaVentilador, Temperatura, ModoTrabalho, TempoTimer, TipoGraus, Timer
        void DisplayUpdate(int ParametrosDto[7]); // Ligado, ForcaVentilador, Temperatura, ModoTrabalho, TempoTimer, TipoGraus, Timer
        void DrawClock(int hour, int min);

        Adafruit_SSD1306 display; 

    private:        
        void DrawCentreString(const String &buf, int x, int y);
        void DrawDegrees(int degreeIndex, int temperatureIndex);
        void CleanDegrees();
        void DrawFanMode(int fanModeIndex);
        void CleanFanMode();
        void DrawCoolMode(int coolModeIndex);
        void CleanCoolMode();
        void DrawTemperatureMode(int temperatureIndex);
        void CleanTemperatureMode();
        void DrawSleepMode(int sleepModeIndex);
        void CleanSleepMode();        
        void CleanDrawClock();

        String TemperatureFahrenheit[29] = {"61", "62", "63", "64", "65", "66", "67", "68", "69", "70", "71", "72", "73", "74", "75", "76", "77", "78", "79", "80", "81", "82", "83", "84", "85", "86", "87", "88", "89"};
        String TemperatureCelsius[29] = {"16.1", "16.7", "17.2", "17.8", "18.3", "18.9", "19.4", "20.0", "20.6", "21.1", "21.7", "22.2", "22.8", "23.3", "23.9", "24.4", "25.0", "25.6", "26.1", "26.7", "27.2", "27.8", "28.3", "28.9", "29.4", "30", "30.6", "31.1", "31.7"};
        int ParametrosAnterioresDto[7]; // Ligado, ForcaVentilador, Temperatura, ModoTrabalho, TempoTimer, TipoGraus, Timer
        String sleepHours;
        String clockHours;
};

#endif