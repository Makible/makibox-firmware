#ifndef PINS_H
#define PINS_H
#define ALARM_PIN          -1


/****************************************************************************************
* Printrboard Rev. B pin assingments (ATMEGA90USB1286)
* Requires the Teensyduino software with Teensy2.0++ selected in arduino IDE!
* See http://reprap.org/wiki/Printrboard for more info
****************************************************************************************/

#define X_STEP_PIN         28
#define X_DIR_PIN          29
#define X_ENABLE_PIN       19
#define X_MIN_PIN          47
#define X_MAX_PIN          -1

#define Y_STEP_PIN         30
#define Y_DIR_PIN          31
#define Y_ENABLE_PIN       18
#define Y_MIN_PIN           20
#define Y_MAX_PIN          -1

#define Z_STEP_PIN         32
#define Z_DIR_PIN          33
#define Z_ENABLE_PIN       17
#define Z_MIN_PIN          36
#define Z_MAX_PIN          -1

#define E_STEP_PIN         34
#define E_DIR_PIN          35
#define E_ENABLE_PIN       13

#define HEATER_0_PIN       15  // Extruder
#define HEATER_1_PIN       14  // Bed
#define FAN_PIN            16  // Fan

#define TEMP_0_PIN          1  // Extruder
#define TEMP_1_PIN          0  // Bed

#define SDPOWER            -1
#define SDSS                2
#define LED_PIN            -1
#define PS_ON_PIN          -1
#define KILL_PIN           -1

#define SCK_PIN          21
#define MISO_PIN         22
#define MOSI_PIN         23



//List of pins which to ignore when asked to change by gcode, 0 and 1 are RX
//and TX, do not mess with those!
const int sensitive_pins[] = {0, 1, X_STEP_PIN, X_DIR_PIN, X_ENABLE_PIN, X_MIN_PIN, X_MAX_PIN, Y_STEP_PIN, Y_DIR_PIN, Y_ENABLE_PIN, Y_MIN_PIN, Y_MAX_PIN, Z_STEP_PIN, Z_DIR_PIN, Z_ENABLE_PIN, Z_MIN_PIN, Z_MAX_PIN, E_STEP_PIN, E_DIR_PIN, E_ENABLE_PIN, LED_PIN, PS_ON_PIN, HEATER_0_PIN, HEATER_1_PIN, FAN_PIN, TEMP_0_PIN, TEMP_1_PIN};


#endif      // PINS_H
