#ifndef GALAXY_HEADER
#define GALAXY_HEADER

#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
using namespace std;
class Planet {
private:
	string name;
	vector<Planet*> connected_plants;
public:
	Planet(string _name) : name(_name){}
	void add_connection(Planet* planet) { connected_plants.push_back(planet);}
	string get_name() { return name;}
	void print()
	{
		cout << name << " : " << endl;
		for (int i = 0; i < connected_plants.size(); i++)
		{
			cout << connected_plants[i]->name << "  " ;
		}
		cout << endl << connected_plants.size() << endl;
	}
	bool calculate_distance(Planet* destination, Planet* last_checked, int &distance);
};
class Galaxy{
private:
	Planet* gate;
	map<string,Planet*> planets;
	string name;
public:
	Galaxy(string _name) : name(_name){}
	void get_planet(string planet_name, Planet* &planet)
	{
		planet = planets[planet_name];
	}
	void add_planet(string planet_name, Planet* &planet)
	{
		planets[planet_name] = planet;
	}
	void set_gate(Planet* &planet){ gate = planet;}
	void print()
	{
		cout << name << endl;
		for (map<string,Planet*>::iterator it=planets.begin(); it!=planets.end(); ++it)
			if(it->second != NULL)
				it->second->print();

	}
	bool have_this_planet(string planet_name)
	{
		if(planets[planet_name] == NULL)
			return false;
		else
			return true;
	}
	string get_name() { return name;}
	string get_gate_name();
	int calculate_distance(string start, string destinstion);
};
class Galaxies{
	vector<Galaxy*> galaxies;
public:
	void fill_class();
	bool valid_address(string address);
	int calculate_distance(string address1, string address2);
};

#endif