#include "Driver_Recorder.h"
void Driver_Recorder::print(Time start, Time end)
{
	int start_index = 0;
	int end_index = 0;
	for(int i = 0; i < data.size(); i++)
	{
		if(data[i].time == start)
			start_index = i;
		if(data[i].time == end)
			end_index = i;
	}
	cout << data[end_index].trips_count - data[start_index].trips_count << " " << data[end_index].total_income - data[start_index].total_income << " " << data[end_index].total_rate << endl;
	
}
void Driver_Recorder::add_record(int trips_count,float total_income,int total_rate, Time time)
{
	trips_count += data[data.size() - 1].trips_count ;
	total_income += data[data.size() - 1].total_income ;
	total_rate += data[data.size() - 1].total_rate ;
	driver_record_data d(trips_count, total_income, total_rate, time);
	for (int i = 0; i < data.size(); i++)
		if(data[i].time == time)
		{
			data[i] = d;
			cout << "driver data" << " : ";
			print(Time("0-0:0:0"), time);
			return;
		}
	data.push_back(d);
	//cout << "driver data" << " : ";
	//print(Time("0-0:0:0"), time);
}

