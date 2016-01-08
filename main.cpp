#include <Arduino.h>
#include <LiquidCrystal.h>

const String states_list[] = {"[ RAMP WAVE ]", "[ MEANDER WAVE ] ", "[ SINE WAVE ]" };
const int size_of_state_list = sizeof(states_list)/sizeof(states_list[0]);

// declaraing and defining lcd object
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

void setup() {
    
    // Initializes the interface to the LCD screen, and specifies the 
    // dimensions (width and height) of the display. begin() needs to be 
    // called before any other LCD library commands. 
    lcd.begin(16, 2);
    
    // Position the LCD cursor; Syntax: lcd.setCursor(col, row), where 
    // lcd - a variable of type LiquidCrystal; 
    // col - the column at which to position the cursor (with 0 being the first column); 
    // row - the row at which to position the cursor (with 0 being the first row)
    lcd.setCursor(0,0);
    
    // Prints text to the LCD. lcd.print(data, BASE), where BASE (optional):
    // the base in which to print numbers: BIN for binary (base 2), DEC for
    // decimal (base 10), OCT for octal (base 8), HEX for hexadecimal (base 16). 
    lcd.print(size_of_state_list);
    
    lcd.setCursor(0,1);
    lcd.print(states_list[0]);
}

void loop() {

}