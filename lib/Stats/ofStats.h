#ifndef ofStats_h
#define ofStats_h

#define pin A0

//#define debug
//#define avrstub

#include <Arduino.h>

class ofStats{
	protected: 
		struct data{
			unsigned int size;
			float average, var, stdD;
		};

	private: 
		data _results;
		unsigned int n = 0;
		unsigned long sum = 0;
		unsigned long sum2diff = 0;
		unsigned long now = 0;
		unsigned long lasttime = 0;

	public:
		ofStats(){lasttime = millis();}
		bool dataInput(int sampleData, unsigned long sampleInterval);
		data results;
};

#endif