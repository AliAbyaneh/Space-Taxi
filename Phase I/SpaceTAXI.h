#ifndef SPACETAXI_HEADER
#define SPACETAXI_HEADER

#include "User.h"
#include "Galaxy.h"
#include "Recorder.h"
#include "vehicles.h"
#include <map>
#define PENDING_STATUS false

using namespace std;

class SpaceTAXI{
private:
	map<string, Passenger*> passengers;
	map<string ,Driver*> drivers;
	map <string, bool> logged_on;
	Galaxies galaxies;
	Time current_time;
	vector<Trip*> trips;
	System_Recorder records;
	Vehicles* vehicles;
	float credit;
	bool valid_username(string user_name);
	bool valid_phonenumber(string);
	bool valid_spaceship_number(string);
	void register_passenger_command(vector <string> &commands);
	void register_driver_command(vector <string> &commands);
	bool available_discount(string discount_code);
	void get_discount_code(string username);
	void accept_registration(string username);
	void reject_registration(string username);
	void show_registration_request();
	bool check_logged_on(string username);
	void login(string username, string password);
	void logout(string username);
	void set_status(vector <string> &commands);
	int estimate_trip(vector <string> &commands);
	void request_trip1(vector <string> &commands);
	int driver_distance(string passenger_adr,string driver_username);
	vector<string> find_best_drivers(string passenger_adr, bool VIP);
	void show_trip_requests(string driver_username);
	void cancel_trip_request(string passenger_username);
	void cancel_trip_request_for_drivers(string passenger_username, string except_this_driver);
	void accept_trip_request(string driver_username, string passenger_username);
	void trip_status(string passenger_username);
	void get_credit(string passenger_username);
	void charge_account(string passenger_username, string credit_amount);
	void set_time(string time);
	void driver_arrived(string driver_username);
	void end_trip(string driver_username);
	void pay_to_drivers();
	void rate_driver(string passenger_username, int rate);
	void system_report(vector<string> &commands);
	void driver_report(vector<string> &commands);
	void passenger_report(string passenger_username);
	bool check_access(string);
	void print();
public:
	SpaceTAXI() : credit(0), current_time("1-0:0:0") {}
	void start();
};

#endif