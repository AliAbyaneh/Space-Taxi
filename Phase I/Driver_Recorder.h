#ifndef DRIVERRECORDER_HEADER
#define DRIVERRECORDER_HEADER

#include <iostream>
#include "Time.h"
#include <vector>
using namespace std;
class driver_record_data
{
public:
	int trips_count;
	int total_income;
	int total_rate;
	Time time;
	driver_record_data() : time("0-0:0:0"){}
	driver_record_data( int _trips_count,
						int _total_income,
						int _total_rate,
						Time _time
						) : trips_count(_trips_count),
						 total_income(_total_income),
						 total_rate(_total_rate),
						 time(_time){} 

};

class Driver_Recorder{
private:
	vector<driver_record_data> data;
public:
	Driver_Recorder() 
	{
		Time t("0-0:0:0");
		driver_record_data d(0,0,0,t);
		data.push_back(d);
	}
	void add_record(
		int trips_count,
		float total_income,
		int total_rate,
		Time time
		);
	void print(Time start, Time End);
};
#endif