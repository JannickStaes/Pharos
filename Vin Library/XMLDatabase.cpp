#include "XMLDatabase.h"

using std::string;
using std::wstring;
using std::to_string;
using std::vector;
using tinyxml2::XMLDocument;
using tinyxml2::XMLError;
using tinyxml2::XMLNode;
using tinyxml2::XMLElement;
using tinyxml2::XML_SUCCESS;
using std::unique_ptr;
using std::shared_ptr;

void XMLFile::init(string input_filename, XMLDocument& doc) {
	filename = input_filename;
	XMLError result = doc.LoadFile(filename.c_str());
}
XMLError XMLFile::save_file(XMLDocument& doc) {
	return doc.SaveFile(filename.c_str());
}
XMLFileMock::XMLFileMock(std::shared_ptr<XMLDocument>& doc_ptr) {
	mock_doc = doc_ptr;
}
void XMLFileMock::add_mock_item_with_value(XMLElement* parent, string name, string text) {
	XMLDocument* doc = parent->GetDocument();
	XMLElement* child = doc->NewElement(name.c_str());
	child->SetText(text.c_str());
	parent->InsertEndChild(child);
}
void XMLFileMock::init(string input_filename, XMLDocument& doc) {
	filename = input_filename;
	if (filename == "Single.xml") {
		//add a root with one element that has one amount
		XMLNode* root = doc.NewElement(root_const.c_str());
		doc.InsertFirstChild(root);
		XMLElement* item = doc.NewElement("Item");
		item->SetAttribute("ID", 1);
		root->InsertFirstChild(item);
		add_mock_item_with_value(item, "Amount", "40.9");
	}//filename Single.xml check
	else if (filename == "Empty.xml") {
		XMLNode* root = doc.NewElement(root_const.c_str());
		doc.InsertFirstChild(root);
	}
	else if (filename == "Table.xml") {
		//add a root and table
		XMLNode* root = doc.NewElement(root_const.c_str());
		doc.InsertFirstChild(root);
		XMLElement* table = doc.NewElement("LineItems");
		root->InsertEndChild(table);
		//add item1
		XMLElement* item1 = doc.NewElement("Item");
		table->InsertEndChild(item1);
		item1->SetAttribute("ID", 1);
		add_mock_item_with_value(item1, "Amount", "40.9");
		add_mock_item_with_value(item1, "Sign", "-");
		add_mock_item_with_value(item1, "Currency", "EUR");
		add_mock_item_with_value(item1, "Account", "Delhaize");
		add_mock_item_with_value(item1, "Comment", "Inkopen");
		add_mock_item_with_value(item1, "Date", "8.1.2018");
		//add item2
		XMLElement* item2 = doc.NewElement("Item");
		table->InsertEndChild(item2);
		item2->SetAttribute("ID", 2);
		add_mock_item_with_value(item2, "Amount", "450");
		add_mock_item_with_value(item2, "Sign", "+");
		add_mock_item_with_value(item2, "Currency", "EUR");
		add_mock_item_with_value(item2, "Account", "Lego");
		add_mock_item_with_value(item2, "Comment", "Hogwarts Castle");
		add_mock_item_with_value(item2, "Date", "20.11.2018");
	}
}
XMLError XMLFileMock::save_file(XMLDocument& doc) {
	doc.DeepCopy(mock_doc.get());
	return XML_SUCCESS;
}

XMLHandler::XMLHandler(string input_filename, unique_ptr<XMLFile> file_access) {
	file = move(file_access);
	file->init(input_filename, doc);
	open_root();
}
void XMLHandler::open_root() {
	root = doc.FirstChild();
}
void XMLHandler::clear_and_add_root(string root_name) {
	doc.Clear();
	root = doc.NewElement(root_name.c_str());
	doc.InsertFirstChild(root);
}
XMLElement* XMLHandler::add_item_to_root(string item_name) {
	XMLElement* item = doc.NewElement(item_name.c_str());
	root->InsertEndChild(item);
	return item;
}
XMLElement* XMLHandler::add_item(XMLElement* parent, string item_name) {
	XMLElement* item = doc.NewElement(item_name.c_str());
	parent->InsertEndChild(item);
	return item;
}
XMLElement* XMLHandler::add_item(XMLElement* parent, XMLElement* after_child, string item_name) {
	XMLElement* item = doc.NewElement(item_name.c_str());
	parent->InsertAfterChild(after_child, item);
	return item;
}
void XMLHandler::set_ID(XMLElement* item, int value) {
	item->SetAttribute("ID", value);
}
void XMLHandler::add_item_with_value(XMLElement* parent, string name, double value) {
	XMLElement* child = doc.NewElement(name.c_str());
	child->SetText(value);
	parent->InsertEndChild(child);
}
void XMLHandler::add_item_with_value(XMLElement* parent, string name, int value) {
	XMLElement* child = doc.NewElement(name.c_str());
	child->SetText(value);
	parent->InsertEndChild(child);
}
void XMLHandler::add_item_with_value(XMLElement* parent, string name, char character) {
	XMLElement* child = doc.NewElement(name.c_str());
	child->SetText(&character);//pass a reference to trigger the char* version of settext and not an implicit conversion to int
	parent->InsertEndChild(child);
}
void XMLHandler::add_item_with_value(XMLElement* parent, string name, string value) {
	XMLElement* child = doc.NewElement(name.c_str());
	child->SetText(value.c_str());
	parent->InsertEndChild(child);
}
XMLElement* XMLHandler::read_child(XMLNode* parent, string name) {
	if (name == "First")
		return parent->FirstChildElement();
	else
		return parent->FirstChildElement(name.c_str());
}
XMLElement* XMLHandler::read_last_child(XMLNode* parent) {
	return parent->LastChildElement();
}
XMLElement* XMLHandler::read_next_item(XMLElement* sibling) {
	return sibling->NextSiblingElement();
}
int XMLHandler::get_ID(XMLElement* element) {
	return element->IntAttribute("ID");
}

XMLDatabase::XMLDatabase(unique_ptr<XMLHandler> handler_input) {
	handler = move(handler_input);
}
XMLDatabase::XMLDatabase(string filename) {
	handler = unique_ptr<XMLHandler>(new XMLHandler(filename));
}
int XMLDatabase::get_current_index(XMLElement* table) {
	XMLElement* last_item = 0;
	if (table)
		last_item = handler->read_last_child(table);
	if (last_item)
		return handler->get_ID(last_item);
	else
		return 0;
}
bool XMLDatabase::not_duplicate(LineItem new_item) {
	bool duplicate_found = false;
	XMLElement* lineitems_table = handler->read_child(handler->get_root(), "LineItems");
	XMLElement* current_element = handler->read_child(lineitems_table);

	while (!duplicate_found && current_element) {
		double amount = read_amount(current_element);
		char sign = read_sign(current_element);
		string currency = read_currency(current_element);
		Date date = read_date(current_element);
		string account = read_account(current_element);
		string comment = read_comment(current_element);

		LineItem current_item(amount, sign, currency, date, account, comment);

		if (current_item == new_item)
			duplicate_found = true;
		else
			current_element = current_element->NextSiblingElement();
	}

	return !duplicate_found;
}
void XMLDatabase::add_item_with_values(XMLElement* lineitems_table, LineItem& lineitem_to_add, int id) {
	Date date_iterator;
	XMLElement* element_to_insert_after = 0;
	//check if start table and element exists
	if (lineitems_table) {
		XMLElement* last_element = lineitems_table->LastChildElement();

		if (last_element) {
			element_to_insert_after = last_element;
			string sdate = last_element->FirstChildElement("Date")->GetText(); //normally, we would need to check if the Date XMLElement actually exists. 
																			   //No need to do that there since we know that the database will contain correct entries (else, it's corrupted). 
			date_iterator = Date(sdate, '.');
		}																		
	}//end lineitems_table existence check
	
	//find first existing element with data no earlier than the new one
	while (date_iterator > lineitem_to_add.get_date() ) { //if no elements exist in the table, then date_iterator will be initial and the while will break immediately
		if (element_to_insert_after)
			element_to_insert_after = element_to_insert_after->PreviousSiblingElement();

		if (element_to_insert_after) {
			string sdate = element_to_insert_after->FirstChildElement("Date")->GetText();
			date_iterator = Date(sdate, '.');
		}
	}

	XMLElement* new_element;
	if (element_to_insert_after) //element found -> insert new element after it
		new_element = handler->add_item(lineitems_table, element_to_insert_after, "Item");
	else //table is empty -> insert the first element
		new_element = handler->add_item(lineitems_table, "Item");

	//TO DO: fix IDs of new elements and all elements after it
	
	handler->set_ID(new_element, id); 
	handler->add_item_with_value(new_element, "Amount", lineitem_to_add.get_amount());
	handler->add_item_with_value(new_element, "Sign", lineitem_to_add.get_sign());
	handler->add_item_with_value(new_element, "Currency", lineitem_to_add.get_currency());
	handler->add_item_with_value(new_element, "Account", lineitem_to_add.get_account());
	handler->add_item_with_value(new_element, "Comment", lineitem_to_add.get_comment());
	string date = to_string(lineitem_to_add.get_date().get_day()) + '.' +
		to_string(lineitem_to_add.get_date().get_month()) + '.' +
		to_string(lineitem_to_add.get_date().get_year());
	handler->add_item_with_value(new_element, "Date", date);
}

double XMLDatabase::read_amount(XMLElement* element) {
	XMLElement * amount_element = element->FirstChildElement("Amount");
	double amount;
	amount_element->QueryDoubleText(&amount);
	return amount;
}
char XMLDatabase::read_sign(XMLElement* element) {
	XMLElement * sign_element = element->FirstChildElement("Sign");
	char sign = sign_element->GetText()[0];
	return sign;
}
string XMLDatabase::read_currency(XMLElement* element) {
	XMLElement * currency_element = element->FirstChildElement("Currency");
	string currency = currency_element->GetText();
	return currency;
}
Date XMLDatabase::read_date(XMLElement* element) {
	XMLElement* date_element = element->FirstChildElement("Date");
	string date_string = date_element->GetText();
	Date date(date_string, '.');
	return date;
}
string XMLDatabase::read_account(XMLElement* element) {
	XMLElement* account_element = element->FirstChildElement("Account");
	XMLNode* account_text = account_element->FirstChild();
	string account;
	if (account_text)
		account = account_text->Value();
	return account;
}
string XMLDatabase::read_comment(XMLElement* element) {
	XMLElement * comment_element = element->FirstChildElement("Comment");
	string comment = comment_element->GetText();
	return comment;
}
void XMLDatabase::add_all_lineitems(vector<LineItem>& lineitems_to_add) {
	//fixed elements: check if the DB already contains a LineItems table. If not, create it.
	XMLElement* lineitems_table = handler->read_child(handler->get_root(), "LineItems");
	if (lineitems_table == 0)
		lineitems_table = handler->add_item_to_root("LineItems");

	//add all items
	int index = get_current_index(lineitems_table);
	for (vector<LineItem>::size_type i = 0; i != lineitems_to_add.size(); i++) {
		if (not_duplicate(lineitems_to_add[i]) ) {
			add_item_with_values(lineitems_table, lineitems_to_add[i], index + i + 1); //ID's start at 1, not 0
		}
	}

	handler->save_file();
}
void XMLDatabase::read_lineitems(vector<LineItem>& lineitems) {
	XMLElement* lineitems_table = handler->read_child(handler->get_root(), "LineItems");
	XMLElement* current_element = handler->read_child(lineitems_table);

	while (current_element) {
		double amount = read_amount(current_element);
		char sign = read_sign(current_element);
		string currency = read_currency(current_element);
		Date date = read_date(current_element);
		string account = read_account(current_element);
		string comment = read_comment(current_element);

		LineItem new_item(amount, sign, currency, date, account, comment);
		lineitems.push_back(new_item);

		current_element = current_element->NextSiblingElement();
	}//end while(current_element)
}