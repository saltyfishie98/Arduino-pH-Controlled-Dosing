#include <Arduino.h>
#include <MotorControl.h>
#include <PHprobe.h>
#include <ofStats.h>

typedef int RawVal;
typedef long MilliSeconds;
typedef float PH;
typedef unsigned int Seconds;

MotorControl pHGoUp(2, 3); //(D.pin1, D.pin2, specify 1 to change direction)
MotorControl pHGoDown(12, 13, 1);
PHprobe probe0(A0);

MilliSeconds checkInterval = 500;
PH lowPH = 4.00f;
RawVal rawLowPH = 626;
PH highPH = 9.18f;
RawVal rawHighPH = 433;

struct Inputs {
  PH setPoint = 0;
  MilliSeconds checkInterval = 0;
  MilliSeconds unitDoseLimits = 0; /* both max. and min.
                                      depending where it's input;
                                      min. when used in controlledDose();
                                            -> how long in millisecond
                                                the SHORTEST dose can last

                                      max. when used in controller();
                                            -> how long in milliseconds
                                                the LONGEST dose can last
                                    */
  Seconds checkInterval_S = 0;
  uint8_t mode = 0;
};
Inputs controllerInputs = {7, 500, 10000};
Inputs dosePumpInputs = {0, 0, 12, 10};

namespace pumpFunctions {

struct controlTimer {
  MilliSeconds lasttime;
  MilliSeconds now;
  controlTimer() { lasttime = millis(); }
  void resetLasttime() { lasttime = now; }
  void setNow() { now = millis(); }
};
controlTimer t_controlledDose;
controlTimer t_justDose;

// REQUIRE FOR CURRENT CONTROLLER---
float prevError = 0;
float integrator = 0;
const float Kp = 800;
const float Ki = 0.0001f;
long controllerOutput = 0;
// ---------------------------------
// Dependent on 'Inputs' struct
long controller(PH readFromProbe, Inputs &objName) {

  // find error
  float Err = objName.setPoint - readFromProbe;

  // proportional term
  float proportional = Err * Kp;

  // integral term
  integrator += 0.5f * Ki * ((float)objName.checkInterval / 1000) *
                (float)(Err + prevError);

  // controller output
  prevError = Err;
  long ans = proportional + integrator;

  // check is the output longer than
  // the unitDoseLimits specified
  if (ans < 0 - objName.unitDoseLimits)
    ans = -objName.unitDoseLimits;
  else if (ans > objName.unitDoseLimits)
    ans = objName.unitDoseLimits;
  return ans;
}

bool controlledDose(long controllerOutput, MotorControl &dosePumpObjName,
                    PH fromProbe, Inputs &objName) {
  /* Dose specified 'dosePumpObjName' pump after
  'objName.checkInterval_S' amount of second\s had passed*/
  t_controlledDose.setNow();
  if ((t_controlledDose.now - t_controlledDose.lasttime) >=
      (objName.checkInterval_S * 1000)) { // check have the time passed

    switch (objName.mode) {
    case 0:
      /*set dose for how long, using output from the controller*/
      dosePumpObjName.setUnitDose(abs(controllerOutput));

      /*to prevent very short doses; based on the specified unitDoseLimits.
        Smaller the limit the more accurate*/
      if (abs(controllerOutput) >= objName.unitDoseLimits) {
        { // Bunch of print statements
          Serial.print("Dose duration: ");
          Serial.print(abs(controllerOutput));
          Serial.println("ms");
          Serial.println();
        }
        dosePumpObjName.dose();
        t_controlledDose.resetLasttime();
        return true;
      }

    default:
      Serial.begin(9600);
      Serial.println("ERROR: controlledDose() settings error.");
      Serial.end();
      break;
    }
  }
  return false;
}

void justDose(MotorControl &dosePumpObjName, PH fromProbe, PH _setPoint,
              PH setPointMargin, int amount, Seconds tillCheckPH) {
  auto pHUpperBound = _setPoint + setPointMargin;
  auto pHLowerBound = _setPoint - setPointMargin;

  t_justDose.setNow();
  if ((t_justDose.now - t_justDose.lasttime) >= (tillCheckPH * 1000)
      // check for whether tillCheckPH amount of seconds have passed
      && (fromProbe <= pHLowerBound || fromProbe >= pHUpperBound))
  // check for whether pH fromProbe is not within bound
  {
    dosePumpObjName.dose(amount);
    t_justDose.resetLasttime();
  }
}
} // namespace pumpFunctions

void setup() {
  Serial.begin(9600);
  pHGoUp.begin();
  pHGoDown.begin();
  probe0.begin();

  // dependent on 'pHProbesInputs' stuct
  probe0.config(lowPH, rawLowPH, highPH, rawHighPH);
}

void loop() {
  long Output =
      pumpFunctions::controller(probe0.results.Mean, controllerInputs);

  bool samplingIsDone = probe0.measure(checkInterval);
  if (samplingIsDone) {
    { // Bunch of print statements
      Serial.print("Mean pH: ");
      Serial.println(probe0.results.Mean);

      Serial.print("Controller Output: ");
      Serial.println(Output);
    }

    if (Output < 0) {
      // controlledDose('using' controller, 'on this' pump, 'sampling from'
      // phProbe, inputs)
      pumpFunctions::controlledDose(Output, pHGoDown, probe0.results.Mean,
                                    dosePumpInputs);
    } else if (Output > 0) {
      pumpFunctions::controlledDose(Output, pHGoUp, probe0.results.Mean,
                                    dosePumpInputs);
    }
  }
}