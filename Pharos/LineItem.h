#pragma once
#include "Date.h"
#include <vector>

class LineItem {
	double amount;
	char sign;
	std::string currency;
	Date date;
	std::string account;
	std::string comment;
public:
	//constructors
	LineItem(): amount(0.0) {}
	LineItem(double, char, std::string, Date, std::string, std::string);
	bool operator==(const LineItem&) const;

	//Get functions
	double get_amount() const { return amount; }
	char get_sign() const { return sign;  }
	std::string get_currency() const { return currency; }
	Date get_date() const { return date; }
	std::string get_account() const { return account; }
	std::string get_comment() const { return comment; }
};

bool compare_lineitems_by_date(LineItem, LineItem);