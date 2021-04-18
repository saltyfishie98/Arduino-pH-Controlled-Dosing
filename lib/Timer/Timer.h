#ifndef Timer_h
#define Timer_h

#include  <Arduino.h>

struct timeData{
	uint8_t h;
    uint8_t m;
    uint8_t s;
	uint8_t d;
};

class Timer{
  public:
    Timer();
    void timestamp();
	void init();
	void run();
	bool regularAlarm(uint8_t mode, uint8_t interval);
	timeData Time;
    
  private:
	timeData _Time;
	uint8_t regularAlarmSwitch;
	uint8_t n = 0;
    unsigned long _startTime;
    unsigned long _lastTime;
    unsigned long _now;
	bool toPrint = false;
	uint8_t _mode = 0;
	//char output[2];
	//void parseTime(int input);
};

#endif




  
