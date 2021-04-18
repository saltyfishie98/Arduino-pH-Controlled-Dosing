#ifndef PHprobe_h
#define PHprobe_h

#include <Arduino.h>
#include <ofStats.h>

class PHprobe{
	protected:
		struct data{
		unsigned int sampleSize;
		unsigned int rMean, rVariance, rStddev;
		float Mean, Stddev, Min;
		};
	
		typedef long millisecond;

	public:
		PHprobe(uint8_t analogPin);
		void begin();

		int readRawVal(); // Display analog value in serial monitor

		void config(float ph1, double volt1, float ph2, double volt2);
		// configure the ph-volt relationship 


		bool measure(millisecond sampleInterval = 500); /*	Method to measure the current pH value, 
															sampleInterval: sample for how long; 
																			used in statistical 
																			calculation           
																
															return "true" when sampling is done  */
		data results; 
		
		// void makeCSV_Title();
		// void makeCSV();
		// void printAveRaw();
		// void printPH();
		
	private:
		ofStats phstats;
		uint8_t _pin;
		float _ph1, _ph2;
		float _volt1, _volt2;
		data _results;
		
};

#endif