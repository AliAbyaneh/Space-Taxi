#include "Trip.h"
string int_to_string ( int Number )
{
 std::ostringstream ss;
 ss << Number;
 return ss.str();
}
string Trip::print()
{
	string result;
	result = passenger_username + " ";
	for (int i = 0; i < address.size(); i++)
		result += address[i] + " ";
	result += int_to_string(cost) + " " + int_to_string(distance) + "\n";
	return result;
}
void Trip::accept(string _driver_username)
{
	driver_username = _driver_username;
	status = true;
}
void Trip::start(Time start)
{
	start_time = start;
}
void Trip::end(Time end)
{
	//cout << "start: " << start_time << endl;
	end_time = end;
	finished = true;
	//cout << "end: " << end_time << endl;
	
}
Time Trip::time_interval(Trip t1)
{
	return end_time.interval(t1.start_time);
}