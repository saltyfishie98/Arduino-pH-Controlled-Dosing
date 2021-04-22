#include "PHprobe.h"
#include <Arduino.h>


// #define _M_ -0.0268
// #define _C_ 20.705
#define _M_ ((_ph1 - _ph2) / (_volt1 - _volt2))
#define _C_ _ph1 - _volt1 *(_M_)

/*!
 * @brief A class to instantiate object that use the PH-4502C module to read pH
 * @param analogPin Analog pin number
 */
PHprobe::PHprobe(uint8_t analogPin) { _pin = pin; }

void PHprobe::begin() { pinMode(_pin, INPUT); }

/*!
* @brief Configure the rawVal-to-pH straight line equation
* @param ph1 Low pH
* @param volt1 rawVal that correspond to ph1
* @param ph2 High pH
* @param volt2 rawVal that correspond to ph2
*/
void PHprobe::config(float ph1, double volt1, float ph2, double volt2) {
  _ph1 = ph1;
  _volt1 = volt1;
  _ph2 = ph2;
  _volt2 = volt2;
}

/*!
* @brief Perform a pH measurement for the specified length
*         of millisecond/s when called
* @param sampleInterval In millisecond, how long a measurement last
* @return "true" when measurment has completed
*/
bool PHprobe::measure(long sampleInterval) {

  int val = analogRead(_pin);
  bool statAns = phstats.dataInput(val, sampleInterval);
  if (statAns == true) {
    _results.sampleSize = phstats.results.size;
    _results.rMean = phstats.results.average;
    _results.rVariance = phstats.results.var;
    _results.rStddev = phstats.results.stdD;

    _results.Mean = (float)_M_ * _results.rMean + _C_;
    _results.Min = (float)_M_ * (_results.rMean + _results.rStddev) + _C_;
    _results.Stddev = _results.Mean - _results.Min;

    results = _results;

    return true;
  } else {
    return false;
  }
}

/*!
* @brief Read the rawVal (raw analog value) from the PH-4502C module
* @return rawVal
*/
int PHprobe::readRawVal() { return analogRead(_pin); }
//-----------------------------------------
// To make CSV file in Putty
//-----------------------------------------

// void PHprobe::makeCSV_Title(){
// 	Serial.print("Mean PH, ");
// 	Serial.print("Min, ");
// 	Serial.print("Max, ");
// 	Serial.print("Stddev, ");
// 	Serial.print("Temp. aval mean, ");
// 	Serial.print("temp. astddev, ");
// 	Serial.println("Time, ");
// }

// void PHprobe::makeCSV(){
// 	Serial.print(results.Mean);
// 	Serial.print(",");
// 	Serial.print(results.Min);
// 	Serial.print(",");
// 	Serial.print(results.Mean + results.Stddev);
// 	Serial.print(",");
// 	Serial.print(results.Stddev);
// }

// void PHprobe::printAveRaw(){
// 	Serial.print("rMean:");
// 	Serial.print(_results.rMean);
// 	Serial.print(", rVariance:");
// 	Serial.print(_results.rVariance);
// 	Serial.print(", rStddev:");
// 	Serial.println(_results.rStddev);
// }

// void PHprobe::printPH(){
// 	Serial.print("Mean pH:");
// 	Serial.print(results.Mean);
// 	Serial.print(", rStddev:");
// 	Serial.print(results.Stddev);
// 	Serial.print(", Min pH:");
// 	Serial.print(results.Min);
// 	Serial.print(", Max pH:");
// 	Serial.println(results.Mean+results.Stddev);
// }

// void PHprobe::linearRegCali(){

// }