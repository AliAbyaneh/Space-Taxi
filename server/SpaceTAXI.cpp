#include "SpaceTAXI.h"
#include <sstream>

class bad_register_passenger_request{};
class bad_register_driver_request{};
class bad_driver_username{};
class bad_estimate_request{};
class bad_trip_request{};
class bad_report_request{};
class bad_command{};

#define DONE "done\n"
#define FAIL "fail\n"
#define DRIVER_KEY "$$$###"
#define PASSENGER_KEY "###&&&$$$"

string to_string ( int Number )
{
 std::ostringstream ss;
 ss << Number;
 return ss.str();
}
void split_string(string line, vector <string> &result)
{
	int index = 0;
	string temp;
	result.push_back(string());
	for (int i = 0; i < line.size(); i++)
	{
		if(line[i] == ' ')
		{
			index++;
			i++;
			result.push_back(string());
		}
		result[index].push_back(line[i]);
	}
}
int string_to_int (string s)
{
	int power = 1;
	int result = 0;
	for (int i = s.size() - 1; i >= 0; i--)
	{
		result += ((int) s[i] - 48) * power;
		power *= 10;
	}
	return result;
}
bool SpaceTAXIServer::check_access(string username)
{
	if(!logged_on[username])
	{
		cout << "first you must login" << endl;
		return false;
	}
	return true;
}
void SpaceTAXIServer::passenger_report(string passenger_username)
{
	if(!check_access("admin"))
		return;
	if(passengers[passenger_username] != NULL)
		cout << passengers[passenger_username]->get_credit() << endl;
}
void SpaceTAXIServer::driver_report(vector<string> &commands)
{
	if(!check_access("admin"))
		return;
	if(commands.size() < 4)
		return;
	else if(commands.size() == 4 && drivers[commands[1]] != NULL)
		drivers[commands[1]]->print_records(Time(commands[2]), Time(commands[3]));
	else
		throw bad_report_request();
}
void SpaceTAXIServer::system_report(vector<string> &commands)
{
	if(!check_access("admin"))
		return;
	if(commands.size() < 3)
	{
		cout << endl;
		records.print(Time("1-0:0:0"), current_time);
	}
	else if(commands.size() == 3)
		records.print(Time(commands[1]), Time(commands[2]));
	else
		throw bad_report_request();
}
string SpaceTAXIServer::rate_driver(string passenger_username, int rate)
{
	if(rate > 10 || rate < 1)
		return "invalid rate\n";
	if(!logged_on[passenger_username] || passengers[passenger_username] == NULL)
		return "first you must login\n";
	for (int i = trips.size() - 1; i >= 0 ; i--)
		if(trips[i]->get_passenger_username() == passenger_username && trips[i]->is_finished())
		{
			string driver_username = trips[i]->get_driver_username();
			drivers[driver_username]->rate(rate, current_time);
			passengers[passenger_username]->trip_is_completed();
			return DONE;
		}

}
void SpaceTAXIServer::pay_to_drivers()
{
	for (int i = trips.size() - 1; i >= 0; i--)
	{
		if(trips[i]->befor_this_time(current_time) && !trips[i]->paid_to_driver_status())
		{
			float cost = trips[i]->get_cost() * 0.8;
			credit -= cost;
			drivers[trips[i]->get_driver_username()]->add_credit(cost, current_time);
			records.add_record(CONSTANT, DECREAMENT * cost, CONSTANT, CONSTANT, current_time);
		}
	}
}
string SpaceTAXIServer::end_trip(string driver_username)
{
	if(drivers[driver_username] == NULL)
		return "invalid username \n";
	if(!check_access(driver_username))
		return "first you must login\n";
	string passenger_username;
	for (int i = trips.size() - 1; i >= 0 ; i--)
		if(trips[i]->get_driver_username() == driver_username && trips[i]->get_status() && !trips[i]->is_finished())
		{
			trips[i]->end(current_time);
			passenger_username = trips[i]->get_passenger_username();
			drivers[driver_username]->set_address(trips[i]->get_destination());
			drivers[driver_username]->set_status(true);
			drivers[driver_username]->end_trip(current_time);
			passengers[passenger_username]->arrived();
			float cost = trips[i]->get_cost();
			passengers[passenger_username]->pay(cost, current_time); // this cost can be changed !
			credit += cost;
			records.add_record(INCREAMENT, INCREAMENT * cost, CONSTANT, CONSTANT, current_time);
			cout << "system credit : " << credit << endl;
			return DONE;
		}
	return FAIL;
}
void SpaceTAXIServer::set_time(string time)
{
	if(check_access("admin"))
	{
		Time t = current_time;
		current_time.set_time(time);
		if((current_time - t) > 0 )
		{
			pay_to_drivers();
		}
		records.add_record(CONSTANT, CONSTANT, CONSTANT, CONSTANT, current_time);
	}
}
string SpaceTAXIServer::driver_arrived(string driver_username)
{
	if(drivers[driver_username] == NULL)
		return "invalid username\n";
	if(!check_access(driver_username))
		return "first you must login! \n ";
	string passenger_username;
	for (int i = trips.size() - 1; i >= 0 ; i--)
		if(trips[i]->get_driver_username() == driver_username && trips[i]->get_status() && !trips[i]->is_finished())
		{
			trips[i]->start(current_time);
			passenger_username = trips[i]->get_passenger_username();
			passengers[passenger_username]->arrived();
			return DONE;
		}
	return FAIL;
}
string SpaceTAXIServer::charge_account(string passenger_username, string credit_amount)
{
	if(check_access(passenger_username) && passengers[passenger_username] != NULL)
	{
		int amount_int = string_to_int(credit_amount);
		// << amount_int << endl;
		passengers[passenger_username]->add_credit(amount_int);
		//cout << amount_int << endl;
		return DONE;
	}
	return "you are not logged_on\n";
}
string SpaceTAXIServer::get_credit(string passenger_username)
{
	if(logged_on[passenger_username] && passengers[passenger_username] != NULL)
		return  to_string(passengers[passenger_username]->get_credit()) + "\n";
	else 
		return "first you must login!";
}
string SpaceTAXIServer::trip_status(string passenger_username)
{
	if(check_access(passenger_username) && passengers[passenger_username] != NULL)
	{
		string driver_username;
		if(passengers[passenger_username]->trip_status(driver_username))
			return "accepted " + drivers[driver_username]->print();
		else
			return "waiting";
	}
}
void SpaceTAXIServer::cancel_trip_request_for_drivers(string passenger_username, string except_this_driver )
{
	for (map<string,Driver*>::iterator it=drivers.begin(); it!=drivers.end(); ++it)
		if(it->second != NULL && it->first != except_this_driver)
			it->second->cancel_trip_request(passenger_username);
}	
string SpaceTAXIServer::accept_trip_request(string driver_username, string passenger_username)
{
	if(drivers[driver_username] == NULL || passengers[passenger_username] == NULL)
		return"invalid username\n";
	if(!check_access(driver_username))
		return "first you should login !\n";
	cancel_trip_request_for_drivers(passenger_username, driver_username);
	drivers[driver_username]->accept_trip_request();
	for (int i = trips.size() - 1; i >= 0; i--)
		if(trips[i]->get_passenger_username() == passenger_username && !trips[i]->get_status() && !trips[i]->is_finished())
		{
			trips[i]->accept(driver_username);
			return DONE;
		}
	return FAIL;
}
string SpaceTAXIServer::cancel_trip_request(string passenger_username)
{
	if(!check_access(passenger_username))
		return "You don't have access!\n";
	string temp;
	if(!passengers[passenger_username]->trip_status(temp))
	{
		cancel_trip_request_for_drivers(passenger_username, " ");
		passengers[passenger_username]->cancel_trip_request();
		for (int i = trips.size() - 1; i >= 0; i--)
		if(trips[i]->get_passenger_username() == passenger_username && !trips[i]->get_status() && !trips[i]->is_finished())
		{
			trips.erase(trips.begin() + i);
			return DONE;
		}
	}
	else
		return "you can not cancel your trip!\n";

}
string SpaceTAXIServer::show_trip_requests(string driver_username)
{
	string result;
	if(drivers[driver_username] != NULL && check_access(driver_username))
		result += drivers[driver_username]->show_trip_requests();
	else
		result = "first you should login!\n";
	return result;

}
vector<string> SpaceTAXIServer::find_best_drivers(string passenger_adr, bool VIP)
{
	vector<string> drivers_uname;
	int index = 0;
	for (map<string,Driver*>::iterator it=drivers.begin(); it!=drivers.end(); ++it)
	{
   		if(it->second != NULL)	
   			if(it->second->is_VIP() == VIP && it->second->available())
   				{
   					if(index < 4 && it->second->is_VIP() == VIP && it->second->available())
   						drivers_uname.push_back(it->first);
   					else
   					{
   						for (int i = 0; i < drivers_uname.size(); i++)
   							if(driver_distance(passenger_adr, drivers_uname[i]) > driver_distance(passenger_adr, it->first))
   								if(it->second->is_VIP() == VIP)
	   								drivers_uname[i] = it->first;
   					}
   					index ++;
   				}
	}
	return drivers_uname;

}
int SpaceTAXIServer::driver_distance(string passenger_adr,string driver_username)
{
	int distance = galaxies.calculate_distance(drivers[driver_username]->get_address(), passenger_adr);
	int score = drivers[driver_username]->get_score();
	return 5 * distance - score;
}

string SpaceTAXIServer::estimate_trip(vector <string> &commands, int &distance)
{
	if(commands.size() < 4)
		throw bad_estimate_request();
	distance = 0;
	int start = 2;
	if(commands[2] == "VIP")
		start = 3;
	if(passengers[commands[0]] != NULL && check_access(commands[0]))
	{
		for (int i = start; i < commands.size() - 1; i++)
			if(galaxies.valid_address(commands[i]) && galaxies.valid_address(commands[i + 1]))
				distance += galaxies.calculate_distance(commands[i], commands[i + 1]);
			else
				return FAIL;
		float cost = distance;
		if(commands[2] == "VIP")
			cost *= 2;
		if(passengers[commands[0]]->is_special())
			cost *= 0.5;
		return to_string(cost)  +  " " +  to_string(distance) +"\n";
	}
	return DONE;
}
string SpaceTAXIServer::request_trip1(vector <string> &commands)
{	
	if(commands.size() < 4)
		throw bad_trip_request();
	if(!logged_on[commands[0]] || passengers[commands[0]] == NULL)
		return commands[0] + ": first you must login\n";
	if(!passengers[commands[0]]->is_rated())
		return "you can not request for a travel until you rate your last trip!\n";
	if(passengers[commands[0]]->has_unfinished_trip())
		return "At this moment you are on a trip!\n";
	int distance;
	string result = estimate_trip(commands, distance);
	if (result == FAIL)
		return "invalid address\n";
	float cost = distance;
	bool VIP = false;
	int first_address_index = 2;
	if(commands[2] == "VIP")
	{
		VIP = true;
		first_address_index = 3;
		cost *= 2;
	}
	if(cost > passengers[commands[0]]->get_credit() + 10)
		return " Please charge your account, you don't have enough credit!\n";
	string passenger_adr = commands[first_address_index];
	vector <string> best_drivers = find_best_drivers(passenger_adr, VIP);
	if(best_drivers.size() == 0)
		return "Sorry, at this time we could not find any drivers for your address\n";
	vector <string> addresses;
	for (int i = first_address_index; i < commands.size(); i++)
		addresses.push_back(commands[i]);
	Trip* trip = new Trip(commands[0], addresses, cost, distance, PENDING_STATUS);
	for (int i = 0; i < best_drivers.size(); i++)
		drivers[best_drivers[i]]->add_trip_request(trip);
	passengers[commands[0]]->add_trip_request(trip);
	trips.push_back(trip);
	return result;
}
string SpaceTAXIServer::set_status(vector <string> &commands) // handle address
{													  // hnadle not registered drivers
	if(drivers[commands[0]] == NULL)
		return "you are not registered yet!\n";
	if(logged_on[commands[0]])
	{
		if(commands[2] == "available")
		{
			if (galaxies.valid_address(commands[3]))
			{
				drivers[commands[0]]->set_address(commands[3]);
				drivers[commands[0]]->set_status(true);
			}
			else return "Enter a valid address!\n";
		}
		else if(commands[2] == "unavailable")
			drivers[commands[0]]->set_status(false);
		else
			return "undefined status!\n";
	}
	else
		return "first you must login\n";
	return DONE;

}
string SpaceTAXIServer::login(string username, string password, int id)
{
	if(logged_on[username])
		return  "You've already logged in\n";
	if(username == "admin")
	{
		if(password == "s3cret")
		{
			logged_on["admin"] = true;
		}
	}
	else if(passengers[username] != NULL)
	{
		logged_on[username] = passengers[username]->check_password(password);
		if(logged_on[username])
			client_id[id] = username;
		else
			return "incorrcet password!\n";
	}
	else if (drivers[username] != NULL )
	{
		if(drivers[username]->registeration_result())
		{
			logged_on[username] = drivers[username]->check_password(password);
			if(logged_on[username])
				client_id[id] = username;
			else
				return "incorrcet password!\n";
		}
		else
			return  "You are not registered yet\n"; // segmantation fuuuuuuuuuuck
	}
	else
		return FAIL;
	return DONE;
}
string SpaceTAXIServer::logout(string username, int id)
{
	if(!logged_on[username])
		return "if you want to log out, first you must login!\n";
	if(passengers[username] != NULL || drivers[username] != NULL || username == "admin")
	{
		logged_on[username] = false;
		client_id[id] = "";
	}
	return DONE;
}

bool SpaceTAXIServer::check_logged_on(string username)
{
	if(passengers[username] != NULL || drivers[username] != NULL || username == "admin")
		if(logged_on[username])
			return true;
	return false;
}
void SpaceTAXIServer::show_registration_request()
{
	if(check_access("admin"))
	{
	 	for (map<string,Driver*>::iterator it=drivers.begin(); it!=drivers.end(); ++it)
			if(it->second != NULL)
				if(!it->second->registeration_result())
					it->second->print_in_detail();
	}
}
void SpaceTAXIServer::accept_registration(string username)
{
	if(drivers[username] == NULL)
		throw bad_driver_username();
	if(check_access("admin"))
	{
		drivers[username]->register_driver();
	}
}
void SpaceTAXIServer::reject_registration(string username)
{
	if(drivers[username] == NULL)
		throw bad_driver_username();
	if(check_access("admin"))
	{
		drivers.erase(username);
		records.add_record(CONSTANT,CONSTANT,DECREAMENT,CONSTANT,current_time);			
	}
}
string SpaceTAXIServer::get_discount_code(string username)
{
	if(logged_on[username] && passengers[username] != NULL)
		return passengers[username]->get_discount_code() + " " + to_string(passengers[username]->get_available_count()) + "\n";
	else
		return username  + ": first you must login\n";
}
void SpaceTAXIServer::print()
{
	for (map<string,Passenger*>::iterator it=passengers.begin(); it!=passengers.end(); ++it)
		if(it->second != NULL)
   			it->second->print();
   	for (map<string,Driver*>::iterator it=drivers.begin(); it!=drivers.end(); ++it)
   		if(it->second != NULL)	
   			it->second->print();
}
bool SpaceTAXIServer::available_discount(string discount_code)
{
	string user_name;
	for (map<string,Passenger*>::iterator it = passengers.begin(); it!=passengers.end(); ++it)
			if(it->second != NULL)
 		   		if (it->second->get_discount_code() == discount_code)
 		   			user_name = it->first;
 	if(passengers[user_name] != NULL && logged_on[user_name])
 	{
 		passengers[user_name]->use_discount_code();
 		if(passengers[user_name]->get_available_count() != 0)
 			return true;
 	}
 	return false; 	
}

bool SpaceTAXIServer::valid_phonenumber(string phone_number)
{
	for (map<string,Passenger*>::iterator it = passengers.begin(); it!=passengers.end(); ++it)
			if(it->second != NULL)
 		   		if (it->second->get_number() == phone_number)
 		   			return false;
 	return true;
}
bool SpaceTAXIServer::valid_spaceship_number(string spaceship_number)
{
	for (map<string,Driver*>::iterator it = drivers.begin(); it != drivers.end(); ++it)
		if(it->second != NULL)
 		 	if (it->second->get_number() == spaceship_number)
 		   		return false;
 	return true;
}

bool SpaceTAXIServer::valid_username(string username)
{	if(passengers[username] == NULL && drivers[username] == NULL && username != "admin")
		return true;
	return false;
}

string SpaceTAXIServer::register_passenger_command(vector <string> &commands)
{
	if(commands.size() < 4)
		throw bad_register_passenger_request();
	bool _valid_username = valid_username(commands[1]);
	if(!_valid_username)
		return "That username is taken. Try another.";
	bool _valid_number = valid_phonenumber(commands[3]);
	if(!_valid_number)
		return "That phone number is taken. Try another.";
	Passenger* passenger = new Passenger(commands[1], commands[2],commands[3]);
	passengers[commands[1]] = passenger;
	if (commands.size() == 5)
	{
		if(available_discount(commands[4]))
			passengers[commands[1]]->add_credit(10);
		//cout << passengers[commands[0]]->get_credit() << endl;
	}
	logged_on[commands[1]] = false;
	records.add_record(CONSTANT,CONSTANT,CONSTANT,INCREAMENT,current_time);
	return DONE;
}
string SpaceTAXIServer::register_driver_command(vector <string> &commands)
{
	if(commands.size() < 7)
		throw bad_register_driver_request();
	bool _valid_username = valid_username(commands[0]);
	if(!_valid_username)
		return	"That username is taken. Try another.";
	bool _valid_number = valid_spaceship_number(commands[3]);
	if(!_valid_number)
		return "That spaceship number is taken. Try another.";
	/*if(!vehicles->valid_vehicle(commands[4]))
		return " invalid spaceship model!\n";*/
	Driver* driver;
	int production_year = string_to_int(commands[5]);
	if(commands.size() < 8)
	{
		driver  = new Driver(commands[1], commands[2], commands[3], commands[4], production_year, commands[6]);
	}

	else
	{
		bool VIP = true;
		if(commands[7] != "VIP")
			VIP = false;
		driver  = new Driver(commands[1], commands[2], commands[3], commands[4], production_year, commands[6], VIP);
	}
	drivers[commands[1]] = driver;
	logged_on[commands[0]] = false;
	records.add_record(CONSTANT,CONSTANT,INCREAMENT,CONSTANT,current_time);
	return DONE;	
}

void SpaceTAXIServer::admin_command(string s)
{
	//records.add_record(CONSTANT,CONSTANT,CONSTANT,INCREAMENT,current_time);
	try
	{
		vector <string> commands;
		split_string(s, commands);
		string command;
		command = commands[0];
		if(command == "print")
			print();
 		else if(command == "accept_registration")
 			accept_registration(commands[1]);
 		else if(command == "reject_registration")
 			reject_registration(commands[1]);
 		else if (command == "show_registration_request")
 			show_registration_request();
 		else if(command == "login")
 			login("admin", commands[1], 0);
 		else if(command == "logout")
 			logout("admin", 0);
 		else if(command == "set_time")
 			set_time(commands[1]);
 		else if(command == "system_report")
 			system_report(commands);
 		else if(command == "driver_report")
 			driver_report(commands);
 		else if(command == "passenger_report")
 			passenger_report(commands[1]);
 		else 
 			throw bad_command();
 	}
 	catch(bad_register_passenger_request)
 	{
 		cout << "***ERROR : register passenger format is <passenger_username> register_passenger <password> <phone_number>" << endl;
 	}
 	catch(bad_register_driver_request)
 	{
 		cout << "***ERROR : register driver format is <driver_username> register_driver <password> <spaceship_number> <spaceship_model> <production_year> <spaceship_color>  [VIP]" << endl;
 	}
 	catch(bad_driver_username)
 	{
 		cout << "***ERROR : This username dosen't exist!" << endl;
 	}
 	catch(bad_report_request)
 	{
 		cout <<"***ERROR :  system_report format is  admin system_report OR admin system_report <from_date_time> <to_date_time> \n"; 
 	}
 	catch(bad_command)
 	{
 		cout << "***ERROR : invalid command \n";
 	}

}
string SpaceTAXIServer::driver_client_command(vector<string> &commands, int id)
{
	//cout << commands[0] << endl;
	bool new_id = false;
	if(client_id[id] == "")
		new_id = true;
	if(commands[0] == "register")
		if(new_id)
			return register_driver_command(commands);
		else
			return "You are logged on and can not register twice!\n";
	else if(commands[0] == "login")
		if(new_id)
			return login(commands[1], commands[2], id);
		else
			return "You are logged on and can not login again! first you must logging out.\n";
	commands.insert(commands.begin(),client_id[id]);
	if(commands[1] == "logout")
		return logout(commands[0], id);	
	if(commands[1] == "set_status")
		return set_status(commands);
	else if(commands[1] == "show_trip_requests")
		return show_trip_requests(commands[0]);
	else if(commands[1] == "accept_trip_request")
		return accept_trip_request(commands[0], commands[2]);
	else if(commands[1] == "arrived")
		return driver_arrived(commands[0]);
	else if(commands[1] == "end_trip")
		return end_trip(commands[0]);
	else 
		throw bad_command();
	return DONE;
}
string SpaceTAXIServer::passenger_client_command(vector<string> &commands, int id)
{
	int distance;
	bool new_id = false;
	if(client_id[id] == "")
		new_id = true;
	if(commands[0] == "register")
		if(new_id)
			return register_passenger_command(commands);
		else
			return "You are logged on and can not register twice!\n";
	else if(commands[0] == "login")
		if(new_id)
				return login(commands[1], commands[2], id);
			else
				return "You are logged on and can not login again! first you must logging out.\n";
	commands.insert(commands.begin(),client_id[id]);
	if(commands[1] == "logout")
		return logout(commands[0], id);
	string command = commands[1];
	if(command == "get_discount_code") //ok
		return get_discount_code(commands[0]);
	else if(command == "estimate_trip") //ok
		return estimate_trip(commands,distance);
	else if(commands[1] == "request_trip")
		return request_trip1(commands);
	else if(command == "cancel_trip_request")
		return cancel_trip_request(commands[0]);
	else if(command == "trip_status")
		return trip_status(commands[0]);
	else if(command == "get_credit") // ok
		return get_credit(commands[0]);
	else if(command == "charge_account") //ok
		return charge_account(commands[0],commands[2]);
	else if(command == "rate_driver")
		return rate_driver(commands[0], string_to_int(commands[2]));
	else 
		throw bad_command(); 	
}
string SpaceTAXIServer::users_command(string s, int id)
{
	try
	{	
		vector <string> commands;
		split_string(s, commands);
		string key = commands[0];
		commands.erase(commands.begin());
		if(key == PASSENGER_KEY)
 			return passenger_client_command(commands, id);
 		else if(key == DRIVER_KEY)
 			return driver_client_command(commands, id);
 		else 
 			throw bad_command();	
 		
 	}
 	catch(bad_register_passenger_request)
 	{
 		return "***ERROR : register passenger format is <passenger_username> register_passenger <password> <phone_number>";
 	}
 	catch(bad_register_driver_request)
 	{
 		return "***ERROR : register driver format is <driver_username> register_driver <password> <spaceship_number> <spaceship_model> <production_year> <spaceship_color>  [VIP]\n";
 	}
 	catch(bad_driver_username)
 	{
 		return "***ERROR : This username dosen't exist!\n";
 	}
 	catch(bad_estimate_request)
 	{
 		return "***ERROR : estimate format is <passenger_username> estimate_trip [VIP] <source_address> <destination0_address> <destination2_address> … <final_destination_address> \n";
 	}
 	catch(bad_trip_request)
 	{
 		return "***ERROR : request trip format is <passenger_username> request_trip [VIP] <source_address> <destination0_address> <destination2_address> … <final_destination_address>\n";

 	}
 	catch(bad_report_request)
 	{
 		return "***ERROR :  system_report format is  admin system_report OR admin system_report <from_date_time> <to_date_time> \n"; 
 	}
 	catch(bad_command)
 	{
 		return "***ERROR : invalid command \n";
 	}

}	

void SpaceTAXIServer::on_standard_input(string line)
{
  cout << "STDIN: " << line << endl;
  //admin_command(line);
  if (line == ":q")
    stop();
}

void SpaceTAXIServer::on_new_connection(int identifier) {
 // cout << "NEW CONNECTION: " << identifier << endl;
}

void SpaceTAXIServer::on_new_message(int identifier, string message) {
  cout << "NEW MESSAGE FROM " << identifier <<": " << message << endl;
  send(identifier, message);
}

void SpaceTAXIServer::on_terminated_connection(int identifier) {
	logout(client_id[identifier], identifier);
}
void SpaceTAXIServer::initilize()
{
	Vehicles* vehicles = new Vehicles();
	galaxies.fill_class();
	records.erase();
	records.start();
	client_id.erase(client_id.begin(), client_id.end());
	logged_on["admin"] = false;

}
void SpaceTAXIServer::run()
{
	initilize();	
	while(active)
  	{
	    FD_ZERO(&fds);
	    FD_SET(STDIN, &fds);
	    int maxfd = STDIN;
	    vector<int> sfds = socket->file_descriptors();
	    for (int i = 0 ; i < sfds.size(); i++) {
	      FD_SET(sfds[i], &fds);
	      maxfd = max(maxfd, sfds[i]);
	    }

	    // monitor multiple file descriptors and block until one or more of the fds become ready for some class of I/O operation
	    int activity = select(maxfd + 1, &fds, NULL, NULL, NULL);
	    if (activity < 0 && errno != EINTR)
	      throw InternalServerError("'select' procedure crashed unexpectedly.");
	    
	    if (FD_ISSET(STDIN, &fds))
	    {
	      string line;
	      getline(cin, line);
	      admin_command(line);
	    }
	    else
	    {
	      for (int i = 0 ; i < sfds.size(); i++) {
	        if (FD_ISSET(sfds[i], &fds)) {
	          NetworkEvent event = socket->interrupt(sfds[i]);
	          if(event.type == NetworkEvent::NEWCONNECTION) {
	            on_new_connection(event.identifier);
	          }
	          else if (event.type == NetworkEvent::NEWMESSAGE) {
	            try {
	              string message = receive(event.identifier);
	             // socket->send(event.identifier, "ki");
	             // cout << message << endl;
	              string result = users_command(message, event.identifier);
	              socket->send(event.identifier,result);
	            }
	            catch(ConnectionTerminationException ex) {
	              on_terminated_connection(event.identifier);
	            }
	          }
	        }
	      }
	    }    
	  }
}


