
#include "Filters.h"
#include "Configuration.h"
#include "Global.h"
#include <MCP3202.h>
#include <SPI.h>


MCP3202 hiResAdc1 = MCP3202(adcCS1); 
MCP3202 hiResAdc2 = MCP3202(adcCS2); 


//============================================================================================================
//              SETUP
//============================================================================================================
void setup() {
  
  //serial port
  Serial.begin(115200);
  Serial.println("bpm: "+String(bpm));
  Serial.println("expTime: "+String(expTime));
  Serial.println("inspTime: "+String(inspTime));

  //pin setup
  pinMode(limit_pin, INPUT_PULLUP);
  pinMode(pulse, OUTPUT);
  pinMode(en, OUTPUT);
  pinMode(dir, OUTPUT);
  pinMode(breath_pin, OUTPUT);
  digitalWrite(en, LOW);
  digitalWrite(dir, HIGH);
  digitalWrite(pulse, LOW);

  //initialisation functions:
  hiResAdc1.begin();
  hiResAdc2.begin();
  get_pressure_offset();
  home_piston(1);
  delay(2000);
}
//============================================================================================================
//              MAIN LOOP
//============================================================================================================
void loop() {
outputBreathPulse();
next_limit(expTime);

outputBreathPulse();
next_limit(inspTime);
//read_pressure();

}
//============================================================================================================
//              FUNCTIONS
//============================================================================================================
void home_piston(int speedRPM){
  float steps_r = steps_turn*gear_ratio;
  float microseconds_rotation = 60000000/speedRPM;
  delayTime = int(microseconds_rotation/4);
  if(digitalRead(limit_pin)){
    Serial.println("HIGH");
  }
  Serial.println("Homing");
  while(digitalRead(limit_pin)==HIGH){
    digitalWrite(pulse, HIGH);
    delayMicroseconds(delayTime);
    digitalWrite(pulse, LOW);
    delayMicroseconds(delayTime);
  }
  Serial.println("done Homing");
  return;
}
//============================================================================================================
//move at a fixed speed up to the next trigger of the limit switch that measures cylinder endpoints
void next_limit(float duration){

  //weirdly fragmetned calculation
  float steps_breath = (float(steps_turn)*float(gear_ratio))/2.0;
  previousDelayTime = delayTime;
  float temp = (float(duration)*1000000.0)/steps_breath;
  delayTime = int(temp/2.0); // get the time per step (divide by two because this delayTime is used twice per step)
 
  int stepsToDebounce = 0;
  int accel_step_change = int((float(delayTime)-float(previousDelayTime))/float(debounceSteps));
  while(stepsToDebounce<debounceSteps){
    
    stepsToDebounce++;
    if(accel_step_change<0 && previousDelayTime>= delayTime){
    previousDelayTime+=accel_step_change;}
    else if(accel_step_change>0 && previousDelayTime<= delayTime){
    previousDelayTime+=accel_step_change;}      

    
    digitalWrite(pulse, HIGH);
    delayMicroseconds(previousDelayTime);
    digitalWrite(pulse, LOW);
    delayMicroseconds(previousDelayTime);
  }
Serial.println(previousDelayTime);
Serial.println(delayTime);
  //move at the above calculated speed until the half breath is complete
  while(digitalRead(limit_pin)==HIGH){
    digitalWrite(pulse, HIGH);
    delayMicroseconds(delayTime);
    digitalWrite(pulse, LOW);
    delayMicroseconds(delayTime);
  }
  Serial.println("DONE WITH BREATH");
  return;
}


//============================================================================================================
//============================================================================================================//============================================================================================================

void read_pressure(){
  lungPressure = ((hiResAdc1.readChannel(1)*mcpADCmv)-lungPressureOffset)/L20Dsensitivity;
  dpfsPressure = ((hiResAdc1.readChannel(0)*mcpADCmv)-dpfsPressureOffset)/L20Dsensitivity;
  Serial.println(lungPressure);

return;
}
//==================================Pressure sensor offset calibration routine========================== 
//(note this is blocking code and should only run when the ventilator is idle and not connected to the patient breathing circuit)
void get_pressure_offset(){
  float dpfsSum=0, lpSum=0;
  
  for (int i = 0; i < 50; ++i){
        dpfsSum += hiResAdc1.readChannel(0);
        lpSum += hiResAdc1.readChannel(1);
        delay(5);
        }
        
  lungPressureOffset = (lpSum/50.0)*mcpADCmv;
  dpfsPressureOffset = (dpfsSum/50.0)*mcpADCmv;
  Serial.println(lungPressureOffset);
  Serial.println(dpfsPressureOffset);

return;
}

void outputBreathPulse(){

      //toggle the state of the breathpin:
  digitalWrite(breath_pin, !digitalRead(breath_pin));
    }
