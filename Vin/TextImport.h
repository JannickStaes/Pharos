#pragma once
#include "LineItem.h"

/*Idee van de class:
Aparte reader om van text files te gaan. 
Voorlopig enkel Keytrade formaat: in toekomst kan elk formaat een subklasse worden van de hoofdklasse.
Dit lijkt me ook een goede kandidaat voor een singleton, eens opzoeken hoe je dit doet. */
class TextImportFile {
public:
	TextImportFile() { }

	//open a CSV file and read the contents into a vector<string>
	virtual void open_file(std::string, std::vector<std::string>&);
};
class TextImportFileMock : public TextImportFile {
public:
	void open_file(std::string, std::vector<std::string>&);
};

class TextImport {
	std::vector<LineItem> ImportedItems;

	//read from a vector of strings to ImportedItems
	bool read_from_csv_keytrade(std::vector<std::string>&);
	//read one string line into a LineItem
	LineItem read_line_from_csv_keytrade(std::string&);
public:
	TextImport() { }
	TextImport(std::string, TextImportFile& file_access = TextImportFile() );
	
	//return the number of lines read into ImportedItems
	std::vector<LineItem>& get_imported_items() { return ImportedItems; }
};

