#include <Arduino.h>
#include "Timer.h"

Timer::Timer(){
	_lastTime = millis();
}

void Timer::init(){
	_lastTime = millis();
}

void Timer::run(){
	
	_now = millis();
	if((_now-_lastTime)>=1000){
		_lastTime = _now;
		_Time.s++;
		regularAlarmSwitch = 1;
		
		for(int i=0;i<3;i++){
				
			if(_Time.s>59){
				_Time.m++;
				_Time.s=0;
				
				if(_Time.m>59){
					_Time.h++;
					_Time.m=0;
					
					if(_Time.h>23){
						_Time.h=0;
						_Time.s=0;
						_Time.m=0;	
						_Time.d++;
					}	
				}
				if(_mode>1){
					switch(_mode){
						case 2:
							regularAlarmSwitch = 2;
							break;
						
						case 3:
							regularAlarmSwitch = 3;
							break;
						
						case 4:
							regularAlarmSwitch = 4;
							break;
					}
				}
			}
		}
		toPrint = true;
		Time = _Time;
		
		// Serial.print("n: ");
		// Serial.print(n);
		// Serial.print(", regularAlarmSwitch: ");
		// Serial.println(regularAlarmSwitch);
	}
}

bool Timer::regularAlarm(uint8_t mode, uint8_t interval){
	_mode = mode;
	if(regularAlarmSwitch == _mode){
		n++;
		if(n == interval){
			n = 0;
			regularAlarmSwitch = 0;
			return 1;
		}
		regularAlarmSwitch = 0;
	}else{
		regularAlarmSwitch = 0;
		return 0;
	}
	return 0;
}

// void Timer::parseTime(int input){
// 	if(input<10){
// 		sprintf(output,"0%d", input);
// 	}else{
// 		sprintf(output,"%d", input);
// 		}
// }

// void Timer::timestamp(){
// 	if(toPrint == true){	
// 		Serial.print("T+");
// 		Serial.print(Time.d);
// 		Serial.print(":");
// 		parseTime(Time.h);
// 		Serial.print(output);
// 		Serial.print(":");
// 		parseTime(Time.m);
// 		Serial.print(output);
// 		Serial.print(":");
// 		parseTime(Time.s);
// 		Serial.println(output);
// 	}
// }
