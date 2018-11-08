#ifndef SPACETAXI_HEADER
#define SPACETAXI_HEADER

#include "server.h"
#include "User.h"
#include "Galaxy.h"
#include "Recorder.h"
#include "vehicles.h"
#include <map>
#include "server_socket.h"
#define PENDING_STATUS false

using namespace std;
class SpaceTAXIServer : public Server{
private:
	map<string, Passenger*> passengers;
	map<string ,Driver*> drivers;
	map <string, bool> logged_on;
	Galaxies galaxies;
	Time current_time;
	vector<Trip*> trips;
	Vehicles* vehicles;
	System_Recorder records;
	float credit;
	bool valid_username(string user_name);
	bool valid_phonenumber(string);
	bool valid_spaceship_number(string);
	string register_passenger_command(vector <string> &commands); //changed
	string register_driver_command(vector <string> &commands); // changed
	bool available_discount(string discount_code);
	string get_discount_code(string username);
	void accept_registration(string username);
	void reject_registration(string username);
	void show_registration_request();
	bool check_logged_on(string username);
	string login(string username, string password, int id); //changed
	string logout(string username, int id); //changed
	string set_status(vector <string> &commands);
	string estimate_trip(vector <string> &commands, int& distance);
	string request_trip1(vector <string> &commands);
	int driver_distance(string passenger_adr,string driver_username);
	vector<string> find_best_drivers(string passenger_adr, bool VIP);
	string show_trip_requests(string driver_username);
	string cancel_trip_request(string passenger_username);
	void cancel_trip_request_for_drivers(string passenger_username, string except_this_driver);
	string accept_trip_request(string driver_username, string passenger_username);
	string trip_status(string passenger_username);
	string get_credit(string passenger_username);
	string charge_account(string passenger_username, string credit_amount);
	void set_time(string time);
	string driver_arrived(string driver_username);
	string end_trip(string driver_username);
	void pay_to_drivers();
	string rate_driver(string passenger_username, int rate);
	void system_report(vector<string> &commands);
	void driver_report(vector<string> &commands);
	string passenger_client_command(vector<string> &commands, int id);
	string driver_client_command(vector<string> &commands, int id);	
	void passenger_report(string passenger_username);
	bool check_access(string);
	void admin_command(string);
	string users_command(string s, int id);
	void print();
	void initilize();
	map <int, string> client_id;
public:
	SpaceTAXIServer(int port) : Server(port), credit(0), current_time("1-0:0:0"){}
	void run();
	void on_standard_input(std::string);
	void on_new_connection(int identifier);
	void on_terminated_connection(int identifier);
	void on_new_message(int identifier, std::string message); 

};

#endif