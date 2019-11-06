#include "LineItem.h"
#include "TextImport.h"
#include "XMLDatabase.h"
#include <iostream>
#include <algorithm>

using std::string;
using std::vector;
using std::cout;
using std::sort;
using tinyxml2::XMLDocument;
using tinyxml2::XMLNode;
using tinyxml2::XMLElement;
using tinyxml2::XMLError;

int main()
{
	//load existing XML Database
	string xmldb = "C:\\Users\\staes\\Desktop\\Pharos\\TestDB.xml";
	XMLDatabase db(xmldb.c_str()); 
	vector<LineItem> existing_items;
	db.read_lineitems(existing_items);

	//load new items from CSV
	string csv = "C:\\Users\\staes\\Desktop\\Pharos\\New Keytrade Download.csv";
	TextImport import(csv.c_str());
	vector<LineItem> new_items = import.get_imported_items();
	sort(new_items.begin(), new_items.end(), compare_lineitems_by_date);

	//write to DB again and save
	db.add_all_lineitems(new_items);

	return 1;
}

/*Current status: 
fix IDs of new elements and all elements after it in "add_item_with_values" method
New class to define account and comment contain rules. 
*/