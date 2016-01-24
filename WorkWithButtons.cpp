
#include "WorkWithButtons.h"
#include <Arduino.h>

// Function read and return one pressed button on Arduino LCD keypad Shield

int read_LCD_button(void) {
#define default_delay 20
#define skip_number 15
#define key_log_array_size skip_number+1
#define repeat_time default_delay*skip_number
#define max_repeat_time_share_in_decreaser 9/10
#define repeat_time_decrease_step 20

    static int key_log_array[key_log_array_size];
    static unsigned long prev_key_measuring_time = 0, prev_key_return_time = 0, repeat_time_decreaser = 0;
    int analog_key_value, cur_key_pressed, time_delta;
    bool long_press = true;

    time_delta = millis() - prev_key_measuring_time;

    if (time_delta < default_delay) {
        return btnNONE;
    }

    prev_key_measuring_time = millis();

    analog_key_value = analogRead(0); // read the value from the sensor
    if (analog_key_value > 1000) {
        cur_key_pressed = btnNONE;
    } else if (analog_key_value < 50) {
        cur_key_pressed = btnRIGHT;
    } else if (analog_key_value < 250) {
        cur_key_pressed = btnUP;
    } else if (analog_key_value < 450) {
        cur_key_pressed = btnDOWN;
    } else if (analog_key_value < 650) {
        cur_key_pressed = btnLEFT;
    } else { // analog_key_value < 850
        cur_key_pressed = btnSELECT;
    }

    // shift array to left

    for (int i = 1; i < key_log_array_size; i++) {
        key_log_array[i - 1] = key_log_array[i];
    }

    key_log_array[key_log_array_size - 1] = cur_key_pressed;

    for (int i = 1; i < key_log_array_size; i++) {
        if (key_log_array[0] != key_log_array[i]) {
            long_press = false;
            break;
        }
    }

    if ((long_press == true) && (millis() - prev_key_return_time > repeat_time - repeat_time_decreaser)) {
        prev_key_return_time = millis();
        repeat_time_decreaser = repeat_time_decreaser + repeat_time_decrease_step;
        if (repeat_time_decreaser > repeat_time * max_repeat_time_share_in_decreaser) {
            repeat_time_decreaser = repeat_time * max_repeat_time_share_in_decreaser;
        }
        return cur_key_pressed;
    } else if ((cur_key_pressed == key_log_array[key_log_array_size - 2]) &&
            (key_log_array[key_log_array_size - 2] != key_log_array[key_log_array_size - 3])) {
        prev_key_return_time = millis();
        repeat_time_decreaser = 0;
        return cur_key_pressed;
    } else {
        return btnNONE;
    }

}