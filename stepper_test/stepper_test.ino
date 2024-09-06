
#include "Filters.h"
#include "Configuration.h"
#include "Global.h"
#include <MCP3202.h>
#include <SPI.h>
#include <ContinuousStepper.h>


MCP3202 hiResAdc1 = MCP3202(adcCS1); 
MCP3202 hiResAdc2 = MCP3202(adcCS2); 

ContinuousStepper<StepperDriver> stepper;



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
  pinMode(breath_pin, OUTPUT);
  pinMode(en, OUTPUT);
  pinMode(on_off_pin, INPUT_PULLUP);

  //disable the stepper motor
  digitalWrite(en, HIGH);

  stepper.begin(/*step=*/pulse, /*dir=*/dir);

  //initialisation functions:
  hiResAdc1.begin();
  hiResAdc2.begin();
  get_pressure_offset();
  delay(2000);
  
}
//============================================================================================================
//              MAIN LOOP
//============================================================================================================
void loop() {
  if(digitalRead(on_off_pin)){
    if(startFlag){
      steps_per_sec = int((float(bpm)/60.0)*steps_turn*gear_ratio);
      digitalWrite(en, LOW);
      stepper.spin(steps_per_sec);
      //enable the stepper motor
      startFlag=false;
    }
    stepper.loop(); 
  }else{
    startFlag=true;
    stepper.spin(0);
    stepper.loop(); 
    //disable the stepper motor
    digitalWrite(en, HIGH);
  }

}
//============================================================================================================
//              FUNCTIONS
//============================================================================================================

//============================================================================================================
//move at a fixed speed up to the next trigger of the limit switch that measures cylinder endpoints



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

void move_cont(float duration){
  unsigned int steps_per_rotation = steps_turn*gear_ratio;
  unsigned int steps_completed =0;
//weirdly fragmetned calculation
  float steps_breath = (float(steps_turn)*float(gear_ratio))/2.0;
  previousDelayTime = delayTime;
  float temp = (float(duration)*1000000.0)/steps_breath;
  delayTime = int(temp/2.0); // get the time per step (divide by two because this delayTime is used twice per step)
 
  int stepsToDebounce = 0;
  int accel_step_change = int((float(delayTime)-float(previousDelayTime))/float(debounceSteps));
  
  while(stepsToDebounce<debounceSteps){
    
    stepsToDebounce++;
    //if target speed not yet reached adjust the delay between steps to accelerate
    if(accel_step_change<0 && previousDelayTime>= delayTime){
    previousDelayTime+=accel_step_change;}
    else if(accel_step_change>0 && previousDelayTime<= delayTime){
    previousDelayTime+=accel_step_change;}      

    //move one step
    digitalWrite(pulse, HIGH);
    delayMicroseconds(previousDelayTime);
    digitalWrite(pulse, LOW);
    delayMicroseconds(previousDelayTime);
  }
  
  Serial.println(previousDelayTime);
  Serial.println(delayTime);
  //move at the above calculated speed until the half breath is complete
  while(steps_completed < steps_per_rotation){
    digitalWrite(pulse, HIGH);
    delayMicroseconds(delayTime);
    digitalWrite(pulse, LOW);
    delayMicroseconds(delayTime);
    steps_completed++;
  }
  Serial.println("DONE WITH BREATH");
  return;
  return;
}
