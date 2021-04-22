#ifndef MotorControl_h
#define MotorControl_h

#include <Arduino.h>
// #include "Timer.h"

typedef unsigned long Time;
typedef bool Use;

enum speed{
	constant = -1
};

//#define debug

struct buildMotor{
	uint8_t In1 = 0;	// input pin 1
	uint8_t In2 = 0;	// input pin 2
	int8_t SpeedPin = -1;	// speed ctrl pin
	bool Direction = 0;	// for software direction switching
	int Speed = constant; // speed constant
	int ID = 0;
	Time doseTime = 100;
	// Use Timestamp = false;
	Use Verbose = false;
};

class MotorControl{
	public:
		MotorControl(uint8_t in1, uint8_t in2, bool direction = 0);
		void begin();
		void setSpeed(uint8_t speedVal = 255, int8_t enA = -1);
		void on();
		void off();
		void setUnitDose(Time m_seconds); // set how long a unit dose last; in milliseconds
		void dose(int quantity = 1);
		// void showOnOffTimestamp();
			
	private:
		uint8_t i = 0;
		bool ranOnce = false;
		buildMotor motor;
		struct buildMotor* motorptr = &motor;
		static uint8_t id;
		// Timer uptime;
};

#endif

 