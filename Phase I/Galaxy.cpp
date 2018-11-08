///// c++ Program for a simple Timing Analyzer
/// Author : Ali Abyaneh , Spring 2017
#include "Galaxy.h"
void split_string_by_comma(string s, string &s1, string &s2)
{
	int index = s.find(",",0);
	for(int i = 0; i < index; i++)
		s1.push_back(s[i]);
	for (int i = index + 1; i < s.size(); i++)
		s2.push_back(s[i]);
}
bool Planet::calculate_distance(Planet* destination, Planet* last_checked, int &distance)
{
	if(this == destination)
		return true;
	for (int i = 0; i < connected_plants.size(); ++i)
	{
		int new_distance = distance + 1;
		if(connected_plants[i] != last_checked)
			if (connected_plants[i]->calculate_distance(destination, this,new_distance))
			{
				distance = new_distance;
				//cout << "distanc :" << distance << endl;
				return true;
			}
	}
	return false;
}
int Galaxies::calculate_distance(string address1, string address2)
{
	string planet1, planet2, galaxy1, galaxy2;
	split_string_by_comma(address1, galaxy1, planet1);
	split_string_by_comma(address2, galaxy2, planet2);
	if(galaxy1 == galaxy2)
		for(int i = 0; i < galaxies.size(); i++)
			if(galaxies[i]->get_name() == galaxy1)
				return galaxies[i]-> calculate_distance(planet1, planet2);
	int distanc1 = 0 , distanc2 = 0;
	for(int i = 0; i < galaxies.size(); i++)
	{
		if(galaxies[i]->get_name() == galaxy1)
			distanc1 =  galaxies[i]-> calculate_distance(planet1, galaxies[i]->get_gate_name());
		if(galaxies[i]->get_name() == galaxy2)
			distanc2 =  galaxies[i]-> calculate_distance(galaxies[i]->get_gate_name(), planet2);
	}
	return distanc1 + distanc2;
}
void Galaxies::fill_class()
{
	ifstream galaxies_file;
	galaxies_file.open("topology.cfg");
	int num_of_galaxies;
	galaxies_file >> num_of_galaxies;
	//cout << num_of_galaxies << endl;
	for (int i = 0; i < num_of_galaxies; i++)
	{
		string galaxy_name;
		galaxies_file >> galaxy_name;
		Galaxy* galaxy = new Galaxy(galaxy_name);
		galaxies.push_back(galaxy);
		int num_of_plantes;
		galaxies_file >> num_of_plantes;
		//cout << num_of_plantes << endl;
		for (int j = 0; j < num_of_plantes; j++)
		{
			string planet_name;
			galaxies_file >> planet_name;
			Planet* planet;
			galaxies[i]->get_planet(planet_name, planet);
			if(planet == NULL)
			{
				planet = new Planet(planet_name);
				galaxies[i]->add_planet(planet_name, planet);
			}
			if(j == 0)
				galaxies[i]->set_gate(planet);
			int num_of_connected_plants;
			galaxies_file >> num_of_connected_plants;
			for (int k = 0; k < num_of_connected_plants; ++k)
			{
				string planet_name;
				galaxies_file >> planet_name;
				Planet* sub_planet;
				galaxy->get_planet(planet_name, sub_planet);
				if(sub_planet == NULL)
				{
					sub_planet = new Planet(planet_name);
					galaxies[i]->add_planet(planet_name, sub_planet);
				}
				planet->add_connection(sub_planet);
			}
		}
	//	galaxies[i]->print();
	}
	//cout << "fill compeleted!" << endl;
}
bool Galaxies::valid_address(string address)
{
	string galaxy_name, planet_name;
	split_string_by_comma(address, galaxy_name, planet_name);
	for (int i = 0; i < galaxies.size(); i++)
		if(galaxies[i]->get_name() == galaxy_name)
			if(galaxies[i]->have_this_planet(planet_name))
				return true;
	else
		return false;
}
string Galaxy::get_gate_name()
{
	return gate->get_name();
}
int Galaxy::calculate_distance(string start, string destinstion)
{
	int distance = 0;
	planets[start]->calculate_distance(planets[destinstion],planets[start], distance);
	return distance;
}
