#ifndef pumpfunction_h
#define pumpfunction_h

#include <Arduino.h>
#include <MotorControl.h>

typedef int RawVal;
typedef long MilliSeconds;
typedef float PH;
typedef unsigned int Seconds;

namespace pumpFunctions {
struct controlTimer {
  MilliSeconds lasttime;
  MilliSeconds now;
  controlTimer() { lasttime = millis(); }
  void resetLasttime() { lasttime = now; }
  void setNow() { now = millis(); }
};

MilliSeconds controller(PH &setPoint, PH &readFromProbe,
                        MilliSeconds &checkInterval);

void controlledDose(MilliSeconds &controllerOutput,
                    MotorControl &dosePumpObjName, Seconds &checkInterval_S,
                    MilliSeconds minDoseInterval = 100);

} // namespace pumpFunctions
#endif