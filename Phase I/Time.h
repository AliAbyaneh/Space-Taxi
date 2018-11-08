#ifndef TIME_HEADER 
#define TIME_HEADER

#include <iostream>
#include <string>
#include <vector>
using namespace std;

class Time{
private:
	int day;
	int hour;
	int minute;
	int second;
public:
	Time() {}
	Time(string _time);
	Time(int d, int h, int m, int s): day(d), hour(h), minute(m), second(s) {}
	void set_time(string _time);
	//string get_time() {return time;}
	bool operator< (const Time& t) const;
	bool operator== (const Time& t) const;
	int operator-(const Time& t) const;
	Time operator+(const Time& t) const;
	Time interval(const Time& t) const;
	friend ostream& operator<<(ostream& out, const Time& t);
};

#endif