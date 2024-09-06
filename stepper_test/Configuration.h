#define bpm 25
#define ie_ratio 0.3

#define steps_turn 400
#define gear_ratio 8 //(160/20)

#define limit_pin 7
#define pulse 8
#define dir 9
#define en 10
#define breath_pin 19
//check which pins are used in the Mega prototype
#define adcCS1 48
#define adcCS2 49

//ADC parameters
#define ADC_mV 4.8828125            // conversion multiplier for Atmega 2560 ADC in mV per unit
#define mcpADCmv 1.220703

#define L20Dsensitivity 45.21                         //(2.25/49.768)*1000           // sensitivity of the pressure sensors in mV/mbar (scale 0-20inH20 in V)/(20inH20 in mBar)*(mv conversion factor)
#define L05Dsensitivity 180.825                       //(2.25/12.443)*1000           // sensitivity of the pressure sensors in mV/mbar (scale 0-20inH20 in V)/(20inH20 in mBar)*(mv conversion factor)

/*LCD Configuration=================
* Model number: PC0802LRU-AWA-B-Q
LCD Pinout:
Vss(gnd)    1
Vdd         2
Vo          3
RS          4
R/W         5
E           6
DB0-DB7     7-14
*/
