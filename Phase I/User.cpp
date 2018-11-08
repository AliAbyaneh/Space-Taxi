///// c++ Program for a simple Timing Analyzer
/// Author : Ali Abyaneh , Spring 2017

#include "User.h"
void Passenger::arrived()
{
	on_trip = true;
	rated = false;
	cout << "arrived near " << username << endl;
}
void Passenger::add_credit(int _credit)
{
	credit += _credit;
}
bool Passenger::trip_status(string& driver_username)
{
	if(trips.size() == 0)
		return false;
	if(trips[trips.size() - 1]->get_status())
	{
		driver_username = trips[trips.size() - 1]->get_driver_username();
		return true;
	}
	return false;
}
void Passenger::use_discount_code()
	{
		if(discount.availabe_count != 0)
		{
			discount.availabe_count --;
			credit += 10;
		}
	}

void Passenger::cancel_trip_request()
{
	if(trips.size() != 0)
	trips.erase(trips.end());
}
bool Passenger::special_passenger()
{
	if(trips.size() < 3)
		return false;
	Time ref("1-:0:0:0");
	for(int i = 0; i < trips.size() - 3; i += 3)
	{

		Time t = trips[i]->time_interval(*trips[i + 1]) + trips[i + 1]->time_interval(*trips[i + 2]);
		if(t < ref)
			return true;
	}
	return false;
}
void Passenger::pay(float &cost, Time current_time)
{
	Time ref("2-:0:0:0");
	if(special == false)
		special = special_passenger();
	else if (ref < current_time.interval(trips[trips.size() - 1]->get_end_time()))
		special = false;
	if(special)
		cost *= 0.5;
	credit -= cost;
}
void Driver::add_credit(int _credit, Time t)
{
	credit += _credit;
	records.add_record(CONSTANT, INCREAMENT * _credit, CONSTANT, t);
}
void Driver::rate(int rate, Time t)
{
	total_rate += rate;
	records.add_record(CONSTANT, CONSTANT, INCREAMENT * rate, t);
}
void Driver::print_in_detail()
{
	cout << username << " " << spaceship_number<< " " << spaceship_model << " " << prodution_year << " " << spaceship_color;
	if(VIP)
		cout << " " << "VIP" << endl;
	else
		cout << endl;
}

void Driver::print()
{
	cout << username << " " << address << " " << spaceship_model << " " << spaceship_color << endl;
}
void Driver::accept_trip_request()
{
	on_trip = true;
}
void Driver::set_address(string _address)
{
	address = _address;
}
void Driver::set_status(bool _availabe)
{
	status_available = _availabe;
	on_trip = false;
}
void Driver::show_trip_requests()
{
	bool not_requested = true;
	for (int i = 0; i < trips.size(); i++)
		if(!trips[i]->get_status())
		{
			trips[i]->print();
			not_requested = false;
		}
	if(not_requested)
		cout << "Dear " << username << ": at this moment system can not find any trip request for you!\n";
}
void Driver::cancel_trip_request(string passenger_username)
{
	for (int i = 0; i < trips.size(); ++i)
		if(trips[i]->get_passenger_username() == passenger_username && !trips[i]->get_status())
			trips.erase(trips.begin() + i);
}
void Driver::end_trip(Time t)
{
	records.add_record(INCREAMENT, CONSTANT, CONSTANT, t);
}
