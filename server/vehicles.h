#ifndef VHEADER
#define VHEADER
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;
class Vehicles{
private:
	vector <string> name;
public:
	Vehicles() {
		{
		vector <string> vehicles_name;
		ifstream file;
		file.open("vehicles.cfg");
		while(!file.eof())
		{
			string s;
			file >> s;
			vehicles_name.push_back(s);
		}
	//	cout << vehicles_name.size() << endl;
		file.close();
		name = vehicles_name;
	}

	};
	vector <string> fill()
	{
		vector <string> vehicles_name;
		ifstream file;
		file.open("vehicles.cfg");
		while(!file.eof())
		{
			string s;
			file >> s;
			vehicles_name.push_back(s);
		}
	//	cout << vehicles_name.size() << endl;
		file.close();
		return vehicles_name;
	}
	bool valid_vehicle(string nname)
	{
		cout << name.size();
		for (int i = 0; i < name.size(); i++)
			if(name[i] == nname)
				return true;
		return false;
	}
};
#endif