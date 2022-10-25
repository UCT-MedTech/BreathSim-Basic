//==========Calibration varaibles
float dpfsFlowOffset = 0.0;
float dpfsPressureOffset = 0.0;
float lungPressureOffset = 0.0;
float fanPressureOffset = 0;

//==========raw pressure values
float lungPressure = 0; //pressure value
float dpfsPressure = 0;
float fanPressure = 0;

//==========calculated globale variables
int delayTime;
float inspTime = ((60/bpm)/(1+ie_ratio))*ie_ratio;
float expTime = (60/bpm)/(1+ie_ratio);
