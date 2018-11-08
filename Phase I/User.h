#ifndef USER_HEADER
#define USER_HEADER

#include <iostream>
#include <string>
#include <vector>
#include "Trip.h"
#include "Driver_Recorder.h"
#define CONSTANT 0
#define INCREAMENT 1
#define DECREAMENT -1

using namespace std;

class User{
protected:
	string username;
	string password;
	string address;
	float credit;
	bool on_trip;
	vector <Trip*> trips;
	public:
	User(string _username, string _password) :
		username(_username) , password(_password),on_trip(false) {}
	string get_username() {return username;}
	virtual string get_number() = 0;
	virtual void print() {cout << username << " , ";}
	bool check_password(string _pass) 
	{
		if(password == _pass)
			return true;
		return false;
	}
	void add_trip_request(Trip* trip) { trips.push_back(trip);}
		
};
class Discount{
public:
	string code;
	int availabe_count;
};
class Passenger : public User{
private:
	string phone_number;
	Discount discount;
	bool rated;
	bool special;
public:
	Passenger(string _username, string _password, string _phonenumber):
				User(_username, _password), phone_number(_phonenumber), rated(true), special(false)
			{
				discount.code = username + "_" +phone_number;
				discount.availabe_count = 5;
				credit = 0;
			}
	void cancel_trip_request();
	bool special_passenger();
	virtual string get_number() { return phone_number;}
	void use_discount_code();
	float get_credit() {return credit;}
	string get_discount_code() { return discount.code; }
	int get_available_count() { return discount.availabe_count; }
	void trip_accepted(string driver_username);
	bool trip_status(string& driver_username); //trips[trips.size() - 1]->
	void arrived();
	void pay(float &cost, Time current_time) ;
	void add_credit(int _credit);
	bool is_rated() { return rated;}
	bool is_special() {return special;}
	void trip_is_completed() { rated = true;}
	bool has_unfinished_trip() 
	{
		if(trips.size() == 0)
			return false; 
		return !trips[trips.size() - 1]->is_finished();
	}

};
class Driver : public User{ // trip end for driver
private:
	string spaceship_number;
	string spaceship_model;
	int prodution_year;
	string spaceship_color;
	string address;
	bool VIP;
	bool registered;
	bool status_available;
	int total_rate;
	Driver_Recorder records;
public:
	Driver(string _username,
		   string _password,
		   string _spaceship_number,
		   string _spaceship_model,
		   int _prodution_year,
		   string _spaceship_color,
		   bool _VIP = false) :
		   User(_username, _password),
		   spaceship_number(_spaceship_number),
		   spaceship_model(_spaceship_model),
		   prodution_year(_prodution_year),
		   spaceship_color(_spaceship_color),
		   VIP(_VIP),
		   registered(false),
		   status_available(false),
		   total_rate(0)
		   {}
	void cancel_trip_request(string passenger_username);
	void register_driver() { registered = true;}
	virtual string get_number() { return spaceship_number;}
	void print_in_detail();
	void set_address(string _address);
	bool registeration_result() {return registered;}
	void set_status(bool _availabe);
	bool available() { return (status_available && !on_trip);}
	bool is_VIP() {return VIP;}
	void go_to_trip() {on_trip = true;}
	string get_address() {return address;}
	int get_score() { return total_rate;}
	void show_trip_requests();
	void accept_trip_request();
	void end_trip(Time t);
	void rate(int rate, Time t);
	virtual void print();
	void add_credit(int _credit, Time t);
	void print_records(Time start, Time end)
	{
		records.print(start,end);	
	}
};

#endif