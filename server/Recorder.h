#ifndef RECORDER_HEADER
#define RECORDER_HEADER

#include <iostream>
#include "Time.h"
#include <vector>
using namespace std;
class System_record_data
{
public:
	int trips_count;
	float system_credit;
	int drivers_count;
	int passengers_count;
	Time time;
	System_record_data() : time("0-0:0:0"){}
	System_record_data( int _trips_count,
						float _system_credit,
						int _drivers_count,
						int _passengers_count,
						Time _time
						) : trips_count(_trips_count),
						 system_credit(_system_credit),
						 drivers_count(_drivers_count),
						 passengers_count(_passengers_count),
						 time(_time){} 

};

class System_Recorder{
private:
	vector<System_record_data> data;
public:
	void start() { 	Time t("1-0:0:0");
		System_record_data d(0,0.0,0,0,t);
		data.push_back(d);
	}
	void erase()
	{
		data.erase(data.begin(), data.end());
	}
	void add_record(
		int trips_count,
		float system_credit,
		int drivers_cout,
		int passengers_count,
		Time time
		);
	void print(Time start, Time End);
};
#endif