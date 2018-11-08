#include "Recorder.h"

void System_Recorder::print(Time start, Time end)
{
	if(data.size() == 0)
	{
		cout << "!" << endl;
		return;
	}
	int start_index = 0;
	int end_index = 0;
	for(int i = 0; i < data.size(); i++)
	{
		if(data[i].time == start)
			start_index = i;
		if(data[i].time == end)
			end_index = i;
	}
	cout << data[end_index].system_credit  << " " << data[end_index].trips_count - data[start_index].trips_count 
		 << " " << data[end_index].drivers_count << " " << data[end_index].passengers_count; 
}
void System_Recorder::add_record(int trips_count,float system_credit,int drivers_count,int passengers_count,Time time)
{
	trips_count += data[data.size() - 1].trips_count ;
	system_credit += data[data.size() - 1].system_credit ;
	drivers_count += data[data.size() - 1].drivers_count ;
	passengers_count += data[data.size() - 1].passengers_count ;
	System_record_data d(trips_count, system_credit, drivers_count, passengers_count, time);
	for (int i = 0; i < data.size(); i++)
		if(data[i].time == time)
		{
			data[i] = d;
			//cout << "sysyem data : " << endl;
			//print(Time("0-0:0:0"), time);
			return;
		}
	data.push_back(d);
	//cout << "sysyem data : " << endl;
	//print(Time("0-0:0:0"), time);
}