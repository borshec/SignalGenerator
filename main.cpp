#include <Arduino.h>
#include <LiquidCrystal.h>
#include <EEPROM.h>
#include "WorkWithButtons.h"

const String states_list[] = {"[RAMP]", "[MEANDER]", "[SINE]"};
const int size_of_state_list = sizeof (states_list) / sizeof (states_list[0]);
int cur_wt = 0;
int cur_fq = 0;
int cur_fu = 0;

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

void setup() {
    lcd.begin(16, 2);
    lcd.clear();

    // Prints text to the LCD. lcd.print(data, BASE), where BASE (optional):
    // the base in which to print numbers: BIN for binary (base 2), DEC for
    // decimal (base 10), OCT for octal (base 8), HEX for hexadecimal (base 16). 
    // lcd.print(size_of_state_list);

 //   lcd.setCursor(0, 1);
 //   lcd.print(states_list[1]);
 //   EEPROM[0] = cur_wt;
 //   EEPROM[1] = cur_fq;
 //   EEPROM[2] = cur_fu;
}

void loop() {
    cur_wt = EEPROM[0];
    cur_fq = EEPROM[1];
    cur_fu = EEPROM[2];
    lcd.setCursor(0, 1);
    lcd.print(cur_wt);
    lcd.setCursor(3, 1);
    lcd.print(cur_fq);
    lcd.setCursor(6, 1);
    lcd.print(cur_fu);
    delay(1000);      
}