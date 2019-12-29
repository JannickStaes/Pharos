#include "TextImport.h"
#include <fstream>
#include <limits>
#include <ctype.h>
#include <codecvt>

using std::string;
using std::wstring;
using std::wstring_convert;
using std::codecvt_utf8;
using std::vector;
using std::wifstream;
using std::getline;

void TextImportFile::open_file(string filepath, vector<wstring>& input_vector) {
	wifstream input_file(filepath.c_str());
	wstring s;

	//ignore first header line
	input_file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	//read all other lines into vector
	while (getline(input_file, s))
		input_vector.push_back(s);
}
void TextImportFileMock::open_file(string filepath, vector<string>& input_vector) {
	if (filepath == "Dummy Path") {
		input_vector.push_back(" 2018/0005/0005 ;17.05.2018;16.05.2018;-; Betaling Trans : 16/05/2018 18:07 - E5-MODE W01 SINT 9051 SINT-DENIJ ;-60,00;EUR");
		input_vector.push_back(" 2018/0005/0004 ;14.05.2018;14.05.2018;BE19651155918412; Overschrijving naar BE19651155918412 Gemeenschappelijk   Maandelijkse storting ;-850,00;EUR");
	}
	else if (filepath == "Dummy Path Positive") {
		input_vector.push_back(" 2018/0005/0003 ;07.05.2018;02.05.2018;-; 5 eurocent/verrichting - van 01/04/2018 tot 30/04/2018 ;+0,40;EUR");
	}
	else if (filepath == "Dummy Path Empty") {
		input_vector.push_back("");
	}
}

TextImport::TextImport(string filepath, TextImportFile& file_access) {
	vector<wstring> input_vector;

	file_access.open_file(filepath, input_vector);
	read_from_csv_keytrade(input_vector);
}

bool TextImport::read_from_csv_keytrade(vector<wstring>& inputfile)
{
	for (vector<string>::size_type i = 0; i < inputfile.size(); i++) {
		LineItem input_line = read_line_from_csv_keytrade(inputfile[i]);
		if (input_line.valid()) //only add non zero LineItems
			this->ImportedItems.push_back(input_line);
	}

	if (ImportedItems.empty())
		return false;
	else
		return true;
}

LineItem TextImport::read_line_from_csv_keytrade(wstring& s)
{
	Date input_date;
	double input_amount;
	char input_sign;
	string input_account, input_currency, input_comment;

	size_t position, position_end = 0;
	position = s.find_first_of(L";");//ignore first value (uittreksel)
	if (position == string::npos) { //no ; found, abort any further processing
		LineItem empty_line;
		return empty_line;
	}
	position = s.find_first_of(L";", position + 1);//ignore second value (datum)
												  
	//date
	wstring date_wstring = s.substr(position + 1, 10);
	string date_string = wstring_to_utf8(date_wstring);
	Date d(date_string); //fixed length of 10 chars for the date
	input_date = d;
	position = s.find_first_of(L";", position + 1);

	//account
	position_end = s.find_first_of(L";", position + 1);
	wstring account_wstring = s.substr(position + 1, position_end - position - 1);//subtract 1 since the last character is the ';' itself
	if (account_wstring != L"-")
		input_account = wstring_to_utf8(account_wstring);
	position = position_end;

	//comment
	position_end = s.find_first_of(L";", position + 1);
	wstring comment_wstring = s.substr(position + 1, position_end - position - 1);//subtract 1 since the last character is the ';' itself
	//remove leading spaces
	while (comment_wstring.front() < 0 || !isalnum(comment_wstring.front()) )
		comment_wstring.erase(comment_wstring.begin());
	//remove trailing spaces
	while (comment_wstring.back() < 0 || !isalnum(comment_wstring.back()))
		comment_wstring.pop_back();
	input_comment = wstring_to_utf8(comment_wstring);
	position = position_end;
	
	//sign and amount
	position_end = s.find_first_of(L";", position + 1);
	input_sign = s.substr(position + 1, 1)[0];
	if (input_sign != '-') {
		input_sign = '+'; //if it's not an expense, it must be a receipt. 
	}
	else {
		position++; //the amount contains a '-' in front, the actual amount starts one position further
	}
	wstring substr = s.substr(position + 1, position_end - position - 1);//subtract 1 since the last character is the ';' itself
	//switch , and . since keytrade format is in European notation and C++ atof works with US notation
	wstring wstr_amount = substr;
	size_t pos_dec = substr.find_first_of(L",");
	if (pos_dec != string::npos)//check if character is actually found before replacing it
		wstr_amount.replace(pos_dec, 1, L".");
	size_t pos_sep = substr.find_first_of(L".");
	if (pos_sep != string::npos)//check if character is actually found before replacing it
		wstr_amount.erase(pos_sep, 1);

	string str_amount = wstring_to_utf8(wstr_amount);
	input_amount = atof(str_amount.c_str());//get substr and convert to double
	position = position_end;

	//currency
	wstring input_currency_wstring = s.substr(position + 1, 3); //always 3 chars
	input_currency = wstring_to_utf8(input_currency_wstring);

	LineItem input_line(input_amount, input_sign, input_currency, input_date, input_account, input_comment);

	return input_line;
}

// convert wstring to UTF-8 string
string wstring_to_utf8(const wstring& str)
{
	wstring_convert<codecvt_utf8<wchar_t>> myconv;
	return myconv.to_bytes(str);
}
