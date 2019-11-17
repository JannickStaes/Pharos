#include "LineItem.h"

using std::string;

LineItem::LineItem(double input_amount, char input_sign, string input_currency, 
	Date input_date, string input_account, string input_comment) {
	amount = input_amount;
	sign = input_sign;
	currency = input_currency;
	date = input_date;
	account = input_account;
	comment = input_comment;
}

bool LineItem::operator==(const LineItem& rhs) const {
	if ( (amount == rhs.get_amount()) &&
		(sign == rhs.get_sign()) &&
		(currency == rhs.get_currency()) &&
		(date == rhs.get_date()) &&
		(account == rhs.get_account()) &&
		(comment == rhs.get_comment()) )
		return true;
	else
		return false;
}

bool compare_lineitems_by_date(LineItem item_one, LineItem item_two) {
	return item_one.get_date() < item_two.get_date();
}