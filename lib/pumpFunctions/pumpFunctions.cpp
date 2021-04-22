#include <pumpFunctions.h>

pumpFunctions::controlTimer t_controlledDose;
pumpFunctions::controlTimer t_justDose;

float prevError = 0;
float integrator = 0;
const float Kp = 800;
const float Ki = 0.0001f;

/*!
 * @brief A PD controller
 * @param setPoint Specify the setpoint (goal) for the controller
 * @param readFromProbe Specify which PHprobe object to read from
 * @param checkInterval Specify the PHprobe object checkInterval
 * @return MilliSeconds
 */
MilliSeconds pumpFunctions::controller(PH &readFromProbe, PH &setPoint,
                                       MilliSeconds &checkInterval) {
  // Error, Proportional, Integral
  float Err = setPoint - readFromProbe;
  float proportional = Err * Kp;
  integrator +=
      0.5f * Ki * ((float)checkInterval / 1000) * (float)(Err + prevError);

  // controller output
  prevError = Err;
  long ans = proportional + integrator;

  return ans;
}

/*!
 * @brief Use a controller to control the unit dose of the specify MotorControl object
 * @param controllerOutput Input a controller output
 * @param dosePumpObjName Specify which MotorControl object the controller control
 * @param checkInterval_S In seconds, specify how long should the time between dose be 
 *                          i.e. the output of the controller is actually input into the dose controls 
 * @param minDoseInterval If controller output time goes below this value it won't dose
 */
void pumpFunctions::controlledDose(MilliSeconds &controllerOutput,
                                   MotorControl &dosePumpObjName,
                                   Seconds &checkInterval_S,
                                   MilliSeconds minDoseInterval) {

  t_controlledDose.setNow();
  if ((t_controlledDose.now - t_controlledDose.lasttime) >=
      (checkInterval_S * 1000)) { // check have the time passed

    dosePumpObjName.setUnitDose(abs(controllerOutput));
    if (abs(controllerOutput) >= minDoseInterval) {
      { // Bunch of print statements
        Serial.print("Dose duration: ");
        Serial.print(abs(controllerOutput));
        Serial.println("ms");
        Serial.println();
      }
      dosePumpObjName.dose();
      t_controlledDose.resetLasttime();
    }
  }
}