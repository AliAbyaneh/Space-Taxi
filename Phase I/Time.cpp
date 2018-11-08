///// c++ Program for a simple Timing Analyzer
/// Author : Ali Abyaneh , Spring 2017

#include "Time.h"
void exclusive_split_string(string line, vector <string> &result)
{
	string temp;
	result.push_back(string());
	int index = 0;
	for (int i = 0; i < line.size(); i++)
	{
		if(line[i] == '-' || line[i] ==':')
		{
			index++;
			i++;
			result.push_back(string());
		}
		result[index].push_back(line[i]);
	}
}


int exclusive_string_to_int (string s)
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
void Time::set_time(string _time)
{
	vector<string> splited_time;
	exclusive_split_string(_time, splited_time);
	day = exclusive_string_to_int(splited_time[0]);
	hour = exclusive_string_to_int(splited_time[1]);
	minute = exclusive_string_to_int(splited_time[2]);
	second = exclusive_string_to_int(splited_time[3]);
	if(hour > 23 || hour < 0)
		hour = 0;
	if(day > 31 || day < 1)
		day = 1;
	if(minute > 59 || minute < 0)
		minute = 0;
	if(second > 59 || second < 0)
		second = 0;
	//cout << day << "-" << hour << ":" << minute << ":"<< second << endl;
}

Time::Time(string _time)
{
	this->set_time(_time);
}
int Time::operator-(const Time& t) const
{
	return day - t.day;
}
Time Time::operator+(const Time& t) const
{
	int new_day = day + t.day;
	int new_h = hour + t.hour;
	int new_min = minute + t.minute;
	int new_sec = second + t.second;
	if(new_sec >= 60)
	{
		new_sec -= 60;
		new_min ++;
	}
	if(new_min >= 60)
	{
		new_min -= 60;
		new_h ++;
	}
	if(new_h >= 24)
	{
		new_h -= 24;
		new_day ++;
	}
	return Time(new_day, new_h, new_min, new_sec);
}

Time Time::interval(const Time& t) const
{
	int new_day = day - t.day;
	int new_h = hour - t.hour;
	int new_min = minute - t.minute;
	int new_sec = second - t.second;
	if(new_sec < 0)
	{
		new_sec += 60;
		new_min --;
	}
	if(new_min < 0)
	{
		new_min += 60;
		new_h --;
	}
	if(new_h < 0)
	{
		new_h += 24;
		new_day --;
	}
	return Time(new_day, new_h, new_min, new_sec);
}
bool Time::operator< (const Time& t) const
{
	if(day <= t.day)
		return true;
	if(hour <= t.hour)
		return true;
	if(minute <= t.minute)
		return true;
	if(second <= t.second)
		return true;
	return false;
}
bool Time::operator== (const Time& t) const
{
	if(day == t.day && hour == t.hour && minute == t.minute && second == t.second)
		return true;
	return false;
}
ostream& operator<<(ostream& out, const Time& t)
{
	out << t.day << "-" << t.hour << ":" << t.minute << ":"<< t.second << endl;
	return out;

}
