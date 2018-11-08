///// c++ Program for a simple Timing Analyzer
/// Author : Ali Abyaneh , Spring 2017
#include "Trip.h"
void Trip::print()
{
	cout << passenger_username << " ";
	for (int i = 0; i < address.size(); i++)
		cout << address[i] << " ";
	cout << cost << " " << distance << endl;
}
void Trip::accept(string _driver_username)
{
	driver_username = _driver_username;
	status = true;
}
void Trip::start(Time start)
{
	start_time = start;
	cout << "start!" << endl;
}
void Trip::end(Time end)
{
	cout << "start: " << start_time << endl;
	end_time = end;
	finished = true;
	cout << "end: " << end_time << endl;

}
Time Trip::time_interval(Trip t1)
{
	return end_time.interval(t1.start_time);
}
