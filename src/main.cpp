#include <Arduino.h>
#include <MotorControl.h>
#include <PHprobe.h>
#include <ofStats.h>
#include <pumpFunctions.h>

MotorControl pHGoUp(2, 3); //(D.pin1, D.pin2, specify 1 to change direction)
MotorControl pHGoDown(12, 13, 1);
PHprobe probe0(A0);

// ph Probe
MilliSeconds probeCheckInterval = 500;
PH lowPH = 4.00f;
RawVal rawLowPH = 626;
PH highPH = 9.18f;
RawVal rawHighPH = 433;

// Controller
PH setPoint = 7;
float Kp = 800;
float Ki = 0.0001f;

// Dose Pump
Seconds checkInterval_S = 12;

void setup() {
	Serial.begin(9600);
  pHGoUp.begin();
  pHGoDown.begin();
  probe0.begin();

  probe0.config(lowPH, rawLowPH, highPH, rawHighPH);
}

void loop() {
	MilliSeconds Output = pumpFunctions::PIcontroller(
      setPoint, Kp, Ki, probe0.results.Mean, probeCheckInterval);

  bool samplingIsDone = probe0.measure(probeCheckInterval);
  if (samplingIsDone) {
    if (Output < 0) {
      pumpFunctions::controlledDose(Output, pHGoDown, checkInterval_S);
    } else if (Output > 0) {
      pumpFunctions::controlledDose(Output, pHGoUp, checkInterval_S);
    }

    { // Bunch of print statements
      Serial.print("Mean pH: ");
      Serial.println(probe0.results.Mean);

      Serial.print("Controller Output: ");
      Serial.println(Output);
    }
  }
}