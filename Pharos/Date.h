#pragma once
#include <string>

class Date {
	int y;
	int m;
	int d;

	//read from string with format dd.mm.yyyy
	bool read_from_string(const std::string&);
public:
	//constructors
	Date(): y(0),m(0),d(0) { }
	Date(int, int, int);
	Date(const std::string& is) { read_from_string(is); }
	Date(const std::string& is, char d);
	
	Date& operator=(const Date&);
	bool operator==(const Date&) const;
	bool operator<(const Date&) const;
	bool operator>(const Date&) const;
	int get_year() const { return y; }
	int get_month() const { return m; }
	int get_day() const { return d; }
};
