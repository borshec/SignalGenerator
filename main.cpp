#include <Arduino.h>
#include <LiquidCrystal.h>
#include <EEPROM.h>
#include "WorkWithButtons.h"

const String states_list[] = {"[RAMP]", "[MEANDER]", "[SINE]"}; // array of possible wave types 
const int size_of_state_list = sizeof (states_list) / sizeof (states_list[0]); // amount of possible wave types

int exec_first_time; // "program executed first time" (0 - no, other - yes) 
int cur_wt = 0; // set "current wave type" to default value 
int cur_fq = 0; // set "current frequency" to default value 
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
  for(int i=0; i<da_pins_amount; i++) {
    pinMode(da_pins[i], OUTPUT); 
  }
    lcd.begin(16, 2); // LCD keypad shield has display with 2 rows and 16 columns in every row
    lcd.clear();
    exec_first_time = EEPROM[0];
    if (exec_first_time == 0) {
        cur_wt = EEPROM[1];
        cur_fq = EEPROM[2];
        cur_fu = EEPROM[3];
    }
}

void loop() {
    lcd.setCursor(0, 1);
    lcd.print(cur_wt);
    lcd.setCursor(3, 1);
    lcd.print(cur_fq);
    lcd.setCursor(6, 1);
    lcd.print(cur_fu);
    lcd.setCursor(12, 1);
    lcd.print(EEPROM[6]);
    lcd.setCursor(12, 0);
    lcd.print(da_pins_amount);
    delay(1000);
}

void GenerationState() {
    EEPROM[0] = 0;
    EEPROM[1] = cur_wt;
    EEPROM[2] = cur_fq;
    EEPROM[3] = cur_fu;
    return;
}