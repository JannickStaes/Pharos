#pragma once
#include "LineItem.h"
#include "tinyxml2.h"
#include <memory>

/*This class handles file opening and saving. It is owner of the filepath. It accepts an XMLdocument managed by the
DatabaseHandler class as reference. It is an architectural boundary of the program, so a mocking test double is 
created to allow for unit testing.
*/
class XMLFile {
protected:
	std::string filename; 
public:
	XMLFile() { }
	virtual void init(std::string, tinyxml2::XMLDocument&);
	virtual tinyxml2::XMLError save_file(tinyxml2::XMLDocument&);
};
class XMLFileMock : public XMLFile {
	std::shared_ptr<tinyxml2::XMLDocument> mock_doc;
	const std::string root_const = "Pharos";

	void add_mock_item_with_value(tinyxml2::XMLElement*, std::string, std::string);
public:
	XMLFileMock() {}
	XMLFileMock(std::shared_ptr<tinyxml2::XMLDocument>&);
	void init(std::string, tinyxml2::XMLDocument&);
	tinyxml2::XMLError save_file(tinyxml2::XMLDocument&);
};

/*This class contains all the logic for reading from and writing to an XMLDocument.
The goal of the XML document is to save it as a file, so it can function as a rudimentary database.
*/
class XMLHandler {
	tinyxml2::XMLDocument doc;
	tinyxml2::XMLNode* root;
	/*We use unique pointer because the Handler class takes ownership of the XMLFile object. If the Handler destructs,
	the XMLFile object will too. */
	std::unique_ptr<XMLFile> file; 

	void open_root();
public:
	XMLHandler() { }
	XMLHandler(std::string, std::unique_ptr<XMLFile> file_access = std::unique_ptr<XMLFile>(new XMLFile()) );
	//Taking an unique pointer by reference guarantees that the caller will pass ownership of the XMLDatabase object to this class.
	//The optional parameter declaration is a bit wonky. It constructs a new unique pointer of the base XMLDatabase class.
	tinyxml2::XMLDocument& get_XMLDocument() { return doc; }
	tinyxml2::XMLNode* get_root() { return root; }
	void clear_and_add_root(std::string);
	tinyxml2::XMLElement* add_item_to_root(std::string);
	tinyxml2::XMLElement* add_item(tinyxml2::XMLElement*, std::string);
	tinyxml2::XMLElement* add_item(tinyxml2::XMLElement*, tinyxml2::XMLElement*, std::string);
	void set_ID(tinyxml2::XMLElement*, int);
	void add_item_with_value(tinyxml2::XMLElement*, std::string, double);
	void add_item_with_value(tinyxml2::XMLElement*, std::string, int);
	void add_item_with_value(tinyxml2::XMLElement*, std::string, char);
	void add_item_with_value(tinyxml2::XMLElement*, std::string, std::string);
	void save_file() { file->save_file(doc); }

	tinyxml2::XMLElement* read_child(tinyxml2::XMLNode*, std::string name = "First");
	tinyxml2::XMLElement* read_last_child(tinyxml2::XMLNode*);
	tinyxml2::XMLElement* read_next_item(tinyxml2::XMLElement*);
	int get_ID(tinyxml2::XMLElement*);
};

/*This class is responsible for preparing and filling the data to be saved in the XML File.
It primarily uses the methods of the XML handler class to do this.
*/
class XMLDatabase {
	/*We use unique pointer because this Database class takes ownership of the XMLHandler object. 
	If the Database destructs, the Handler will too. */
	std::unique_ptr<XMLHandler> handler;

	int get_current_index(tinyxml2::XMLElement* table);
	bool not_duplicate(LineItem new_item);
	void add_item_with_values(tinyxml2::XMLElement*, LineItem&, int);

	double read_amount(tinyxml2::XMLElement*);
	char read_sign(tinyxml2::XMLElement*);
	std::string read_currency(tinyxml2::XMLElement*);
	Date read_date(tinyxml2::XMLElement*);
	std::string read_account(tinyxml2::XMLElement*);
	std::string read_comment(tinyxml2::XMLElement*);
public:
	XMLDatabase() { }
	XMLDatabase(std::unique_ptr<XMLHandler>);
	XMLDatabase(std::string);

	void add_all_lineitems(std::vector<LineItem>&);
	void read_lineitems(std::vector<LineItem>&);
};