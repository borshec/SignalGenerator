#include <Arduino.h>
#include <LiquidCrystal.h>
#include <EEPROM.h>
#include "WorkWithButtons.h"

extern HardwareSerial Serial;

#define buttons_read_delay 250

// Function declaration
void UpdateLCD(void);
void WaveTypeMenuState(void);
void FreqValueMenuState(void);
void FreqUnitMenuState(void);

const String wave_types_list[] = {"RAMP", "MEANDER", "SINE"}; // array of possible wave types 
const int size_of_wave_types_list = sizeof (wave_types_list) / sizeof (wave_types_list[0]); // amount of possible wave types
const String freq_units_list[] = {"Hz", "KHz"}; // array of possible frequency units 
const int size_of_freq_units_list = sizeof (freq_units_list) / sizeof (freq_units_list[0]); // amount of possible frequency units
const int freq_limits[][2] = {
    {1, 999},
    {1, 500}
};

int cur_st = 0; // set "current state" to default value 
int cur_wt = 0; // set "current wave type" to default value 
int cur_fq = freq_limits[0][0]; // set "current frequency" to default value 
int cur_fu = 0; // set "frequency unit" to default value

LiquidCrystal lcd(8, 9, 4, 5, 6, 7); // create global seen object for LCD display

// Available pins on Arduino UNO: 
//  - digital: 0, 1, 2 ,3 ,4 ,5, 6, 7, 8, 9, 10, 11, 12 ,13
//  - analog: A0, A1, A2, A3, A4, A5
// busy pins:
//  - digital: 4, 5, 6, 7, 8, 9, 10 (backlight on LCD board)
//  - analog: A0
// free pins: 
//  - digital: 0, 1, 2, 3, 11, 12, 13
//  - analog: A1, A2, A3, A4, A5
// Using pins 0, 1, 2, 3, A5, 11, 12, 13 for DAC
const int da_pins[] = {0, 1, 2, 3, A5, 11, 12, 13}; // Pins used for digital to analog conversion
const int da_pins_amount = sizeof (da_pins) / sizeof (da_pins[0]); // number of pins used

void setup() {
    Serial.begin(9600);
    for (int i = 0; i < da_pins_amount; i++) {
        pinMode(da_pins[i], OUTPUT);
    }
    lcd.begin(16, 2); // LCD keypad shield has display with 2 rows and 16 columns in every row
    lcd.clear();
    if (EEPROM[0] == 0) { // "program executed first time" (0 - no, other - yes) 
        cur_wt = EEPROM[1];
        cur_fq = EEPROM[2];
        cur_fu = EEPROM[3];
    }
}

void loop() {
    UpdateLCD();
    switch (cur_st) {
        case 0:
            WaveTypeMenuState();
            break;
        case 1:
            FreqValueMenuState();
            break;
        case 2:
            FreqUnitMenuState();
            break;
    }
}

void WaveTypeMenuState(void) {
    while (true) {
        switch (read_LCD_button()) {
            case btnNONE:
                continue;
            case btnDOWN:
                cur_st = 1;
                return;
            case btnUP:
                cur_st = 2;
                return;
            case btnLEFT:
                cur_wt--;
                if (cur_wt < 0) {
                    cur_wt = size_of_wave_types_list - 1;
                }
                UpdateLCD();
                break;
            case btnRIGHT:
                cur_wt++;
                if (cur_wt > size_of_wave_types_list - 1) {
                    cur_wt = 0;
                }
                UpdateLCD();
                break;
        }
    }
    return;
}

void FreqValueMenuState(void) {
    while (true) {
        switch (read_LCD_button()) {
            default:
                continue;
            case btnUP:
                cur_st = 0;
                return;
            case btnDOWN:
                cur_st = 2;
                return;
            case btnRIGHT:
                cur_fq++;
                if (cur_fq > freq_limits[cur_fu][1]) {
                    cur_fq = freq_limits[cur_fu][0];
                }
                UpdateLCD();
                break;
            case btnLEFT:
                cur_fq--;
                if (cur_fq < freq_limits[cur_fu][0]) {
                    cur_fq = freq_limits[cur_fu][1];
                }
                UpdateLCD();
                break;
        }
    }
    return;
}

void FreqUnitMenuState(void) {
    while (true) {
        switch (read_LCD_button()) {
            default:
                continue;
            case btnUP:
                cur_st = 1;
                return;
            case btnDOWN:
                cur_st = 0;
                return;
            case btnLEFT:
                cur_fu--;
                if (cur_fu < 0) {
                    cur_fu = size_of_freq_units_list - 1;
                }
                if (cur_fq > freq_limits[cur_fu][1]) {
                    cur_fq = freq_limits[cur_fu][1];
                }
                if (cur_fq < freq_limits[cur_fu][0]) {
                    cur_fq = freq_limits[cur_fu][0];
                }
                UpdateLCD();
                break;
            case btnRIGHT:
                cur_fu++;
                if (cur_fu > size_of_freq_units_list - 1) {
                    cur_fu = 0;
                }
                if (cur_fq > freq_limits[cur_fu][1]) {
                    cur_fq = freq_limits[cur_fu][1];
                }
                if (cur_fq < freq_limits[cur_fu][0]) {
                    cur_fq = freq_limits[cur_fu][0];
                }
                UpdateLCD();
                break;
        }
    }
    return;
}

void UpdateLCD(void) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Wave:");
    if (cur_st == 0) {
        lcd.print("[");
        lcd.print(wave_types_list[cur_wt]);
        lcd.print("]");
    } else {
        lcd.print(" ");
        lcd.print(wave_types_list[cur_wt]);
    }
    lcd.setCursor(0, 1);
    lcd.print("Freq:");
    if (cur_st == 1) {
        lcd.print("[");
        lcd.print(cur_fq);
        lcd.print("]");
    } else {
        lcd.print(" ");
        lcd.print(cur_fq);
        lcd.print(" ");
    }
    if (cur_st == 2) {
        lcd.print("[");
        lcd.print(freq_units_list[cur_fu]);
        lcd.print("]");
    } else {
        lcd.print(" ");
        lcd.print(freq_units_list[cur_fu]);
    }
    return;
}

void GenerationState() {
    EEPROM[0] = 0;
    EEPROM[1] = cur_wt;
    EEPROM[2] = cur_fq;
    EEPROM[3] = cur_fu;
    return;
}