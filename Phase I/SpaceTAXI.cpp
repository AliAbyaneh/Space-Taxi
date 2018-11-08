///// c++ Program for a simple Timing Analyzer
/// Author : Ali Abyaneh , Spring 2017
#include "SpaceTAXI.h"
class bad_register_passenger_request{};
class bad_register_driver_request{};
class bad_driver_username{};
class bad_estimate_request{};
class bad_trip_request{};
class bad_report_request{};
class bad_command{};
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
bool SpaceTAXI::check_access(string username)
{
	if(!logged_on[username])
	{
		cout << "first you must login" << endl;
		return false;
	}
	return true;
}
void SpaceTAXI::passenger_report(string passenger_username)
{
	if(!check_access("admin"))
		return;
	if(passengers[passenger_username] != NULL)
		cout << passengers[passenger_username]->get_credit() << endl;
}
void SpaceTAXI::driver_report(vector<string> &commands)
{
	if(!check_access("admin"))
		return;
	if(commands.size() < 5)
		return;
	else if(commands.size() == 5 && drivers[commands[2]] != NULL)
		drivers[commands[2]]->print_records(Time(commands[3]), Time(commands[4]));
	else
		throw bad_report_request();
}
void SpaceTAXI::system_report(vector<string> &commands)
{
	if(!check_access("admin"))
		return;
	if(commands.size() < 4)
		records.print(Time("1-0:0:0"), current_time);
	else if(commands.size() == 4)
		records.print(Time(commands[2]), Time(commands[3]));
	else
		throw bad_report_request();
}
void SpaceTAXI::rate_driver(string passenger_username, int rate)
{
	if(rate > 10 || rate < 1)
	{
		cout << "invalid rate" << endl;
		return;
	}
	if(!logged_on[passenger_username] || passengers[passenger_username] == NULL)
	{
		cout << "first you must login" << endl;
		return;
	}
	for (int i = trips.size() - 1; i >= 0 ; i--)
		if(trips[i]->get_passenger_username() == passenger_username && trips[i]->is_finished())
		{
			string driver_username = trips[i]->get_driver_username();
			drivers[driver_username]->rate(rate, current_time);
			passengers[passenger_username]->trip_is_completed();
			return;
		}
	cout << "" << endl;

}
void SpaceTAXI::pay_to_drivers()
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
void SpaceTAXI::end_trip(string driver_username)
{
	if(drivers[driver_username] == NULL)
	{
		cout << "invalid username" << endl;
		return;
	}
	if(!check_access(driver_username))
		return;
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
			return;
		}
}
void SpaceTAXI::set_time(string time)
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
void SpaceTAXI::driver_arrived(string driver_username)
{
	if(drivers[driver_username] == NULL)
	{
		cout << "invalid username" << endl;
		return;
	}
	if(!check_access(driver_username))
		return;
	string passenger_username;
	for (int i = trips.size() - 1; i >= 0 ; i--)
		if(trips[i]->get_driver_username() == driver_username && trips[i]->get_status() && !trips[i]->is_finished())
		{
			trips[i]->start(current_time);
			passenger_username = trips[i]->get_passenger_username();
			passengers[passenger_username]->arrived();
		}
}
void SpaceTAXI::charge_account(string passenger_username, string credit_amount)
{
	if(check_access(passenger_username) && passengers[passenger_username] != NULL)
	{
		int amount_int = string_to_int(credit_amount);
		cout << amount_int << endl;
		passengers[passenger_username]->add_credit(amount_int);
	}
}
void SpaceTAXI::get_credit(string passenger_username)
{
	if(logged_on[passenger_username] && passengers[passenger_username] != NULL)
		cout << passengers[passenger_username] -> get_credit() << endl;
}
void SpaceTAXI::trip_status(string passenger_username)
{
	if(check_access(passenger_username) && passengers[passenger_username] != NULL)
	{
		string driver_username;
		if(passengers[passenger_username]->trip_status(driver_username))
		{
			cout << "accepted ";
			drivers[driver_username]->print();
		}
		else
			cout << "waiting" << endl;
	}
}
void SpaceTAXI::cancel_trip_request_for_drivers(string passenger_username, string except_this_driver )
{
	for (map<string,Driver*>::iterator it=drivers.begin(); it!=drivers.end(); ++it)
		if(it->second != NULL && it->first != except_this_driver)
			it->second->cancel_trip_request(passenger_username);
}
void SpaceTAXI::accept_trip_request(string driver_username, string passenger_username)
{
	if(drivers[driver_username] == NULL || passengers[passenger_username] == NULL)
	{
		cout << "invalid username" << endl;
		return;
	}
	if(!check_access(driver_username))
		return;
	cancel_trip_request_for_drivers(passenger_username, driver_username);
	drivers[driver_username]->accept_trip_request();
	for (int i = trips.size() - 1; i >= 0; i--)
		if(trips[i]->get_passenger_username() == passenger_username && !trips[i]->get_status() && !trips[i]->is_finished())
		{
			trips[i]->accept(driver_username);
			return;
		}
}
void SpaceTAXI::cancel_trip_request(string passenger_username)
{
	cout << 3 << endl;
	if(!check_access(passenger_username))
		return;
	string temp;
	if(!passengers[passenger_username]->trip_status(temp))
	{
		cancel_trip_request_for_drivers(passenger_username, " ");
		passengers[passenger_username]->cancel_trip_request();
		for (int i = trips.size() - 1; i >= 0; i--)
		if(trips[i]->get_passenger_username() == passenger_username && !trips[i]->get_status() && !trips[i]->is_finished())
		{
			trips.erase(trips.begin() + i);
			return;
		}
	}
	else
		cout << "you can not cancel your trip!" << endl;

}
void SpaceTAXI::show_trip_requests(string driver_username)
{
	if(drivers[driver_username] != NULL && check_access(driver_username))
		drivers[driver_username]->show_trip_requests();

}
vector<string> SpaceTAXI::find_best_drivers(string passenger_adr, bool VIP)
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
int SpaceTAXI::driver_distance(string passenger_adr,string driver_username)
{
	int distance = galaxies.calculate_distance(drivers[driver_username]->get_address(), passenger_adr);
	int score = drivers[driver_username]->get_score();
	return 5 * distance - score;
}

int SpaceTAXI::estimate_trip(vector <string> &commands)
{
	if(commands.size() < 4)
		throw bad_estimate_request();
	int distance = 0;
	int start = 2;
	if(commands[2] == "VIP")
		start = 3;
	if(passengers[commands[0]] != NULL && check_access(commands[0]))
	{
		for (int i = start; i < commands.size() - 1; i++)
			if(galaxies.valid_address(commands[i]) && galaxies.valid_address(commands[i + 1]))
				distance += galaxies.calculate_distance(commands[i], commands[i + 1]);
		float cost = distance;
		if(commands[2] == "VIP")
			cost *= 2;
		if(passengers[commands[0]]->is_special())
			cost *= 0.5;
		cout << cost << " " << distance << endl;
	}
	return distance;
}
void SpaceTAXI::request_trip1(vector <string> &commands)
{
	if(commands.size() < 4)
		throw bad_trip_request();

	if(!logged_on[commands[0]] || passengers[commands[0]] == NULL)
	{
		cout << commands[0] <<": first you must login" <<endl;
		return;
	}
	if(!passengers[commands[0]]->is_rated())
	{
		cout << "you can not request for a travel until you rate your last trip!" << endl;
		return;
	}
	if(passengers[commands[0]]->has_unfinished_trip())
	{
		cout << "At this moment you are on a trip!" << endl;
		return;
	}

	int distance = estimate_trip(commands);
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
	{
		cout << " Please charge your account, you don't have enough credit!\n";
		return;
	}
	string passenger_adr = commands[first_address_index];
	vector <string> best_drivers = find_best_drivers(passenger_adr, VIP);
	if(best_drivers.size() == 0)
	{
		cout << "Sorry, at this time we could not find any drivers for your address" << endl;
		return;
	}
	vector <string> addresses;
	for (int i = first_address_index; i < commands.size(); i++)
		addresses.push_back(commands[i]);
	Trip* trip = new Trip(commands[0], addresses, cost, distance, PENDING_STATUS);
	for (int i = 0; i < best_drivers.size(); i++)
		drivers[best_drivers[i]]->add_trip_request(trip);
	passengers[commands[0]]->add_trip_request(trip);
	trips.push_back(trip);
}
void SpaceTAXI::set_status(vector <string> &commands) // handle address
{													  // hnadle not registered drivers
	if(drivers[commands[0]] == NULL)
		return;
	if(logged_on[commands[0]])
	{
		if(commands[2] == "available")
		{
			if (galaxies.valid_address(commands[3]))
			{
				drivers[commands[0]]->set_address(commands[3]);
				drivers[commands[0]]->set_status(true);
			}
			else cout << "Enter a valid address!\n";
		}
		else if(commands[2] == "unavailable")
			drivers[commands[0]]->set_status(false);
		else
			cout << "undefined status!" << endl;
	}
	else
		cout << commands[0] <<": first you must login" <<endl;

}
void SpaceTAXI::login(string username, string password)
{
	if(logged_on[username])
	{
		cout << "You've already logged in\n";
		return;
	}
	if(username == "admin")
	{
		if(password == "s3cret")
		{
			logged_on["admin"] = true;
		}
	}
	else if(passengers[username] != NULL)
		logged_on[username] = passengers[username]->check_password(password);
	else if (drivers[username] != NULL )
		if(drivers[username]->registeration_result())
			logged_on[username] = drivers[username]->check_password(password);
	else
		cout << "You are not registered yet" << endl;
}
void SpaceTAXI::logout(string username)
{
	if(!logged_on[username])
	{
		cout << "if you want to log out, first you must login!\n";
		return;
	}
	if(passengers[username] != NULL || drivers[username] != NULL || username == "admin")
		logged_on[username] = false;
}

bool SpaceTAXI::check_logged_on(string username)
{
	if(passengers[username] != NULL || drivers[username] != NULL || username == "admin")
		if(logged_on[username])
			return true;
	return false;
}
void SpaceTAXI::show_registration_request()
{
	if(check_access("admin"))
	{
	 	for (map<string,Driver*>::iterator it=drivers.begin(); it!=drivers.end(); ++it)
			if(it->second != NULL)
				if(!it->second->registeration_result())
					it->second->print_in_detail();
	}
}
void SpaceTAXI::accept_registration(string username)
{
	if(drivers[username] == NULL)
		throw bad_driver_username();
	if(check_access("admin"))
	{
		drivers[username]->register_driver();
	}
}
void SpaceTAXI::reject_registration(string username)
{
	if(drivers[username] == NULL)
		throw bad_driver_username();
	if(check_access("admin"))
	{
		drivers.erase(username);
		records.add_record(CONSTANT,CONSTANT,DECREAMENT,CONSTANT,current_time);
	}
}
void SpaceTAXI::get_discount_code(string username)
{
	if(logged_on[username] && passengers[username] != NULL)
		cout << passengers[username]->get_discount_code() << " " << passengers[username]->get_available_count() << endl;
	else
		cout << username <<": first you must login" <<endl;
}
void SpaceTAXI::print()
{
	for (map<string,Passenger*>::iterator it=passengers.begin(); it!=passengers.end(); ++it)
		if(it->second != NULL)
   			it->second->print();
   	for (map<string,Driver*>::iterator it=drivers.begin(); it!=drivers.end(); ++it)
   		if(it->second != NULL)
   			it->second->print();
}
bool SpaceTAXI::available_discount(string discount_code)
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

bool SpaceTAXI::valid_phonenumber(string phone_number)
{
	for (map<string,Passenger*>::iterator it = passengers.begin(); it!=passengers.end(); ++it)
			if(it->second != NULL)
 		   		if (it->second->get_number() == phone_number)
 		   			return false;
 	return true;
}
bool SpaceTAXI::valid_spaceship_number(string spaceship_number)
{
	for (map<string,Driver*>::iterator it = drivers.begin(); it != drivers.end(); ++it)
		if(it->second != NULL)
 		 	if (it->second->get_number() == spaceship_number)
 		   		return false;
 	return true;
}

bool SpaceTAXI::valid_username(string username)
{	if(passengers[username] == NULL && drivers[username] == NULL && username != "admin")
		return true;
	return false;
}

void SpaceTAXI::register_passenger_command(vector <string> &commands)
{
	if(commands.size() < 4)
		throw bad_register_passenger_request();
	bool _valid_username = valid_username(commands[0]);
	if(!_valid_username)
	{
		cout << "That username is taken. Try another." << endl;
		return;
	}
	bool _valid_number = valid_phonenumber(commands[3]);
	if(!_valid_number)
	{
		cout << "That phone number is taken. Try another." << endl;
		return;
	}
	Passenger* passenger = new Passenger(commands[0], commands[2],commands[3]);
	passengers[commands[0]] = passenger;
	if (commands.size() == 5)
	{
		if(available_discount(commands[4]))
			passengers[commands[0]]->add_credit(10);
		//cout << passengers[commands[0]]->get_credit() << endl;
	}
	logged_on[commands[0]] = false;
	records.add_record(CONSTANT,CONSTANT,CONSTANT,INCREAMENT,current_time);
}

void SpaceTAXI::register_driver_command(vector <string> &commands)
{
	if(commands.size() < 7)
		throw bad_register_driver_request();
	bool _valid_username = valid_username(commands[0]);
	if(!_valid_username)
	{
		cout << "That username is taken. Try another." << endl;
		return;
	}
	bool _valid_number = valid_spaceship_number(commands[3]);
	if(!_valid_number)
	{
		cout << "That spaceship number is taken. Try another." << endl;
		return;
	}
	if(!vehicles->valid_vehicle(commands[4]))
	{
		cout << " invalid spaceship model!\n";
		return;
	}
	Driver* driver;
	int production_year = string_to_int(commands[5]);
	if(commands.size() == 7)
		driver  = new Driver(commands[0], commands[2], commands[3], commands[4], production_year, commands[6]);
	else
	{
		bool VIP = true;
		if(commands[7] != "VIP")
			VIP = false;
		driver  = new Driver(commands[0], commands[2], commands[3], commands[4], production_year, commands[6], VIP);
	}
	drivers[commands[0]] = driver;
	logged_on[commands[0]] = false;
	records.add_record(CONSTANT,CONSTANT,INCREAMENT,CONSTANT,current_time);
}
void SpaceTAXI::start()
{
	logged_on["admin"] = false;
	galaxies.fill_class();
	string s;
	vehicles = new Vehicles();
	while(getline(cin,s))
	{
		try
		{
			vector <string> commands;
			split_string(s, commands);
			string command;
			if(commands.size() > 1)
				command = commands[1];

			if(command == "register_passenger")
				register_passenger_command(commands);

			else if(command == "register_driver")
				register_driver_command(commands);
			else if(command == "print")
				print();
	 		else if(command == "get_discount_code")
	 			get_discount_code(commands[0]);
	 		else if(commands[0] == "admin" && command == "accept_registration")
	 			accept_registration(commands[2]);
	 		else if(commands[0] == "admin" && command == "reject_registration")
	 			reject_registration(commands[2]);
	 		else if (commands[0] == "admin" && command == "show_registration_request")
	 			show_registration_request();
	 		else if(command == "login")
	 			login(commands[0], commands[2]);
	 		else if(command == "logout")
	 			logout(commands[0]);
	 		else if(command == "set_status")
	 			set_status(commands);
	 		else if(command == "estimate_trip")
	 			estimate_trip(commands);
	 		else if(commands[1] == "request_trip")
	 		{
	 			request_trip1(commands);
	 		}
	 		else if(command == "show_trip_requests")
	 			show_trip_requests(commands[0]);
	 		else if(command == "cancel_trip_request")
	 			cancel_trip_request(commands[0]);
	 		else if(command == "accept_trip_request")
	 			accept_trip_request(commands[0], commands[2]);
	 		else if(command == "trip_status")
				trip_status(commands[0]);
	 		else if(command == "get_credit")
	 			get_credit(commands[0]);
	 		else if(command == "charge_account")
	 			charge_account(commands[0],commands[2]);
	 		else if(commands[0] == "admin" && commands[1] == "set_time")
	 			set_time(commands[2]);
	 		else if(command == "arrived")
	 			driver_arrived(commands[0]);
	 		else if(command == "end_trip")
	 			end_trip(commands[0]);
	 		else if(command == "rate_driver")
	 			rate_driver(commands[0], string_to_int(commands[2]));
	 		else if(command == "system_report")
	 			system_report(commands);
	 		else if(command == "driver_report")
	 			driver_report(commands);
	 		else if(command == "passenger_report")
	 			passenger_report(commands[2]);
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
	 	catch(bad_estimate_request)
	 	{
	 		cout << "***ERROR : estimate format is <passenger_username> estimate_trip [VIP] <source_address> <destination0_address> <destination2_address> … <final_destination_address> " << endl;
	 	}
	 	catch(bad_trip_request)
	 	{
	 		cout << "***ERROR : request trip format is <passenger_username> request_trip [VIP] <source_address> <destination0_address> <destination2_address> … <final_destination_address> " << endl;

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

}
