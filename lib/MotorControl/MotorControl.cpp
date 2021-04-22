#include <Arduino.h>
#include <MotorControl.h>

/*!
 * @brief Instantiate a object to control one inputs of the L298N module
 * @param in1 Specify digital pin number used for pin 1
 * @param in2 Specify digital pin number used for pin 2
 * @param direction Specify "1" to change the direction of rotation
 */
MotorControl::MotorControl(uint8_t in1, uint8_t in2, bool direction) {
  motor.In1 = in1;
  motor.In2 = in2;
  motor.Direction = direction;
}

uint8_t MotorControl::id = 0;

void MotorControl::begin() {
  pinMode(motorptr->In1, OUTPUT);
  pinMode(motorptr->In2, OUTPUT);
  if (motorptr->SpeedPin != -1) {
    pinMode(motorptr->SpeedPin, OUTPUT);
  }
  motorptr->ID = id;
  id++;
}

void MotorControl::setSpeed(uint8_t speedVal, int8_t enA) {
  motor.Speed = speedVal;
  motor.SpeedPin = enA;
}

/*!
 * @brief Turn the motor controlled by this object ON
 */
void MotorControl::on() {
  uint8_t a;
  uint8_t b;
  switch (motorptr->Direction) {
  case 0:
    a = motorptr->In1;
    b = motorptr->In2;
    break;

  case 1:
    a = motorptr->In2;
    b = motorptr->In1;
    break;
  }

  switch (motorptr->Speed) {
  case constant:
    digitalWrite(a, HIGH);
    digitalWrite(b, LOW);
    break;

  default:
    digitalWrite(a, HIGH);
    digitalWrite(b, LOW);
    if (ranOnce == false) {
      analogWrite(motorptr->SpeedPin, 255);
      delay(100);
      for (i = 255; i >= (motorptr->Speed); i--) {
        analogWrite(motorptr->SpeedPin, i);
        delay(10);
      }
      ranOnce = true;
    }
    analogWrite(motorptr->SpeedPin, i);
    break;
  }

#ifndef debug
  Serial.print("Motor #");
  Serial.print(motor.ID);
  Serial.println(": ON");

// if(motor.Timestamp == 0){
// 	Serial.println(": ON");
// }else{
// 	Serial.print(": ON    Uptime: ");
// 	uptime.timestamp();
// }
#endif
}

/*!
 * @brief Turn the motor controlled by this object OFF
 */
void MotorControl::off() {
  digitalWrite(motorptr->In1, LOW);
  digitalWrite(motorptr->In2, LOW);

#ifndef debug
  Serial.print("Motor #");
  Serial.print(motor.ID);
  Serial.println(": OFF");

// if(motor.Timestamp == 0){
// 	Serial.println(": OFF");
// }else{
// 	Serial.print(": OFF   Uptime: ");
// 	uptime.timestamp();
// }
#endif
}

/*!
 * @brief Set the "Period" in millisecond\s of a dose, i.e. how long a single
 * dose last
 * @param m_seconds "Period" or dose length in millisecond\s
 */
void MotorControl::setUnitDose(Time m_seconds) {
  motorptr->doseTime = m_seconds;
}

void MotorControl::dose(int quantity) {
  for (int i = 0; i < quantity; i++) {
    on();
    delay(motor.doseTime);
    off();
    if (i != quantity)
      delay(50);
  }
}

// void MotorControl::showOnOffTimestamp(){
// 	uptime.run();
// 	motor.Timestamp = true;
// }