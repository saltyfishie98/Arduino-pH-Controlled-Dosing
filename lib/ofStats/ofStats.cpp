#include <Arduino.h>
#include <ofStats.h>

#ifdef debug //control at ofStats.h
int aq = 0;
int debugq_millis(){
  return aq;
}
#endif 

/*!
* @brief method to calculate average, variance, and standard deviation
* @param sampleData which data to sample
* @param sampleInterval how long to sample for calculation
* @return true when done; results in "results" object
*/
bool ofStats::dataInput(int sampleData, unsigned long sampleInterval){
	n++;
	sum += sampleData;
	sum2diff += ((sum/n)-sampleData)*((sum/n)-sampleData);
	
	#ifdef debug
	now = debugq_millis();
	aq+=250;
	#else 
	now = millis();
	#endif

	if((now-lasttime)>=sampleInterval){
		lasttime = now;
		
		//==================
		// Average cal.
		//==================
		_results.size = n;
		_results.average = (float) sum/_results.size;
		
		//==================
		// Variance cal.
		//==================
		_results.var = (float) sum2diff/_results.size;
		
		//========================
		// Standard deviation cal.
		//========================
		_results.stdD = (float) sqrt(_results.var);
		
		//=======
		// Reset
		//=======
		results = _results;
		sum = sum2diff = n = 0;
		
		return true;
		
	}else{
		return false;
	}
}