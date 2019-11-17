#include "Date.h"
#include <algorithm>

using std::string;
using std::find;

Date::Date(int year, int month, int day) {
	y = year;
	m = month;
	d = day;
}

Date::Date(const string& input, char delim) {
	size_t position = 0, position_end = 0;

	position_end = input.find_first_of(delim);
	string sday = input.substr(0, position_end - position);
	d = atoi(sday.c_str());

	position = position_end;
	position_end = input.find_first_of(delim, position + 1);
	string smonth = input.substr(position + 1, position_end - position - 1);
	m = atoi(smonth.c_str());

	position = position_end;
	string syear = input.substr(position + 1);
	y = atoi(syear.c_str());
}

bool Date::read_from_string(const string& s) {
	string sday = s.substr(0, 2);
	this->d = atoi(sday.c_str());
	string smonth = s.substr(3, 2);
	this->m = atoi(smonth.c_str());
	string syear = s.substr(6, 4);
	this->y = atoi(syear.c_str());

	return true;
}

Date& Date::operator=(const Date& rhs) {
	y = rhs.get_year();
	m = rhs.get_month();
	d = rhs.get_day();

	return *this;
}
 
bool Date::operator==(const Date& rhs) const {
	if ( (y == rhs.get_year()) && (m == rhs.get_month()) && (d == rhs.get_day()) )
		return true;
	else
		return false;
}

bool Date::operator<(const Date& rhs) const {
	if (y < rhs.get_year())
		return true;
	else if (y > rhs.get_year())
		return false;
	else {//years are equal in this case
		if (m < rhs.get_month())
			return true;
		else if (m > rhs.get_month())
			return false;
		else {//months are equal in this case
			if (d < rhs.get_day())
				return true;
			else if (d > rhs.get_day())
				return false;
			else
				return true;//return true if everything is equal
		}//else check days
	}//else check months
}

bool Date::operator>(const Date& rhs) const {
	return !(*this < rhs);
}
 
