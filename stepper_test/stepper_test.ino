
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
next_limit(expTime);
next_limit(inspTime);
read_pressure();

}
//============================================================================================================
//              FUNCTIONS
//============================================================================================================
void home_piston(int speedRPM){
  float steps_r = steps_turn*gear_ratio;
  float microseconds_rotation = 60000000/speedRPM;
  delayTime = int(microseconds_rotation/2);
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

void next_limit(int duration){
  float steps_breath = (steps_turn*gear_ratio)/2;
  delayTime = int(((duration*1000000)/steps_breath)/2); // get the time per step (divide by two because this delayTime is used twice per step)
  //previousDelayTime = newDelayTime;
Serial.println("MOVE OUT OF LIMIT");
  //move out of the limit trigger area at the above calculated speed
  int stepsToDebounce = 0;
  while(stepsToDebounce<500){//digitalRead(limit_pin)==LOW){
    stepsToDebounce++;
    digitalWrite(pulse, HIGH);
    delayMicroseconds(delayTime);
    digitalWrite(pulse, LOW);
    delayMicroseconds(delayTime);
  }
Serial.println("BREATH");
  //move at the above calculated speed until the half breath is complete
  while(digitalRead(limit_pin)==HIGH){
    digitalWrite(pulse, HIGH);
    delayMicroseconds(delayTime);
    digitalWrite(pulse, LOW);
    delayMicroseconds(delayTime);
  }
  Serial.println("DOIEN WOTH BREATH");
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
