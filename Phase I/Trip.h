#ifndef TRIP_HEADER
#define TRIP_HEADER

#include <iostream>
#include <string>
#include <vector>
#include "Time.h"
using namespace std;

class Trip{
private:
	string passenger_username;
	string driver_username;
	vector<string> address;
	float cost;
	int distance;
	bool status;
	bool finished;
	bool paid_to_driver;
	Time start_time;
	Time end_time;
public:
	Trip(string passenger, vector<string> _address, float _cost, int _distance,bool _status) :
			passenger_username(passenger), address(_address),
			cost(_cost), distance(_distance), status(_status), finished(false), paid_to_driver(false)
			{}
	void print();
	void set_start_time(Time time) { start_time = time; }
	void set_end_time(Time time) { end_time = time; }
	string get_passenger_username(){return passenger_username;}
	void accept(string _driver_username);
	bool get_status() {return status;}
	string get_driver_username() {return driver_username;}
	void start(Time t);
	bool is_finished() {return finished;}
	void end(Time t);
	bool paid_to_driver_status(){return paid_to_driver;}
	void pay_to_driver(){paid_to_driver = true;}
	string get_destination() { return address[address.size() - 1];}
	float get_cost() { return cost;}
	bool befor_this_time(Time t) { return (end_time < t || end_time == t);}
	Time time_interval(Trip t1);
	Time get_end_time() {return end_time;}
};

#endif