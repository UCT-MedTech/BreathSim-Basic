//============================================================================================================
#define DWindowSize 25
#define SWindowSize 10
#define longSWindowSize 100
#define PWindowSize 5

float flowArray [SWindowSize];
float longFlowArray[longSWindowSize];
float flowDerivative[longSWindowSize];
long flowTimeStamps[longSWindowSize];
int FIndex = 0;
int longFIndex = 0;
float dFlowArray [DWindowSize];
int dFIndex = 0;
float presFiltArray[PWindowSize];
uint8_t filtIndex = 0;



float pressure_filter(float sensed_val){ //the filter for the patient pressure measured on the patient side of the differential pressure flow sensor
  presFiltArray[PWindowSize] = sensed_val;
  filtIndex++;
  if(filtIndex>=PWindowSize){filtIndex=0;}
  float array_sum = 0;
  for (int i = 0; i < PWindowSize; ++i){
        array_sum += presFiltArray[i];}
  return array_sum/PWindowSize;
}

float dpfs_flow_filter(float sensed_val){
  dFlowArray[dFIndex] = sensed_val;
  dFIndex++;
  if(dFIndex>=DWindowSize){dFIndex=0;}
  float array_sum = 0;
  for (int i = 0; i < DWindowSize; ++i){
        array_sum += dFlowArray[i];}
  return array_sum/DWindowSize;
}

float flow_filter(float sensed_val, long timeStamp){ //this filter is for the SFM3000 sensor it can be deleted in the final build along with the associated varibles (at the top of this file)
  flowArray[FIndex] = sensed_val;
  FIndex++;
  longFIndex++;
  if(FIndex>=SWindowSize){FIndex=0;}
  if(longFIndex>=longSWindowSize){longFIndex=0;}
  
  float array_sum = 0;
  for (int i = 0; i < SWindowSize; ++i){  
  flowDerivative[longFIndex] = (longFlowArray[longFIndex]-longFlowArray[longFIndex-1])/(flowTimeStamps[longFIndex]- flowTimeStamps[longFIndex-1]);
  return longFlowArray[longFIndex];
}
}
