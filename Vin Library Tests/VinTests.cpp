#include "CppUnitTest.h"
#include "../Vin Library/LineItem.h"
#include "../Vin Library/TextImport.h"
#include "../Vin Library/XMLDatabase.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using std::string;
using std::wstring;
using std::vector;
using std::unique_ptr;
using std::shared_ptr;
using std::make_shared;
using tinyxml2::XMLDocument;
using tinyxml2::XMLElement;
using tinyxml2::XMLNode;
using tinyxml2::XMLAttribute;
using tinyxml2::XMLError;
	
TEST_CLASS(LineItemTest)
{
public:
	TEST_METHOD(EmptyConstructor) {
		LineItem LineItemTest;
		string empty = "";
		Assert::AreEqual(0.0, LineItemTest.get_amount());
		Assert::AreEqual(empty, LineItemTest.get_account());
		Assert::AreEqual(empty, LineItemTest.get_currency());
		Assert::AreEqual(empty, LineItemTest.get_comment());
	}
	TEST_METHOD(FullConstructor) {
		//given
		string currency = "EUR";
		string account = "Colruyt";
		string comment = "aankopen 21/05";
		Date date(2017, 05, 21);
		char sign = '-';
		//When
		LineItem LineItemTest(0.0, sign, currency, date, account, comment);
		//Then
		Assert::AreEqual(0.0, LineItemTest.get_amount());
		Assert::AreEqual(currency, LineItemTest.get_currency());
		Assert::AreEqual(2017, LineItemTest.get_date().get_year());
		Assert::AreEqual(05, LineItemTest.get_date().get_month());
		Assert::AreEqual(21, LineItemTest.get_date().get_day());
		Assert::AreEqual(account, LineItemTest.get_account());
		Assert::AreEqual(comment, LineItemTest.get_comment());
	}
	TEST_METHOD(CompareLineItems) {
		//given
		Date date(2017, 05, 21);
		LineItem item_one(20.0, '-', "EUR", date, "Colruyt", "aankopen 21/05");
		Date date2(2017, 05, 30);
		LineItem item_two(30.0, '-', "EUR", date2, "Delhaize", "aankopen 30/05");
		Date date3(2017, 04, 20);
		LineItem item_three(40.0, '-', "EUR", date3, "Lidl", "aankopen whatevs");
		//when
		//then
		Assert::IsTrue(compare_lineitems_by_date(item_one, item_two), 
			L"Compare not correct.");
		Assert::IsFalse(compare_lineitems_by_date(item_one, item_three),
			L"Compare not correct.");
	}
};

TEST_CLASS(TextImportTest) {
public:
	TEST_METHOD(read_from_csv_keytrade_standard) {
		//given
		TextImportFileMock mock_file;
		//when
		TextImport import("Dummy Path", mock_file);
		vector<LineItem> items = import.get_imported_items();
		//then
		Assert::AreEqual(2018, items[0].get_date().get_year(), 
						 L"Date LineItem1 not correct");
		Assert::AreEqual(5, items[0].get_date().get_month(),
						 L"Date LineItem1 not correct");
		Assert::AreEqual(16, items[0].get_date().get_day(),
						 L"Date LineItem1 not correct");
		Assert::AreEqual(string(), items[0].get_account(),
						 L"Account LineItem1 not empty");
		Assert::AreEqual(string("Betaling Trans : 16/05/2018 18:07 - E5-MODE W01 SINT 9051 SINT-DENIJ"), 
						 items[0].get_comment(),
						 L"Comment LineItem1 not correct");
		Assert::AreEqual(60.00, items[0].get_amount(),
						 L"Amount LineItem1 not correct");
		Assert::AreEqual('-', items[0].get_sign(),
						 L"Sign LineItem1 not correct");
		Assert::AreEqual(string("EUR"), items[0].get_currency(),
						 L"Currency LineItem1 not correct");

		Assert::AreEqual(2018, items[1].get_date().get_year(),
						 L"Date LineItem2 not correct");
		Assert::AreEqual(5, items[1].get_date().get_month(),
						 L"Date LineItem2 not correct");
		Assert::AreEqual(14, items[1].get_date().get_day(),
						 L"Date LineItem2 not correct");
		Assert::AreEqual(string("BE19651155918412"), items[1].get_account(),
						 L"Account LineItem2 not correct");
		Assert::AreEqual(string("Overschrijving naar BE19651155918412 Gemeenschappelijk   Maandelijkse storting"), 
						 items[1].get_comment(),
						 L"Comment LineItem2 not correct");
		Assert::AreEqual(850.00, items[1].get_amount(),
						 L"Amount LineItem2 not correct");
		Assert::AreEqual('-', items[1].get_sign(),
						L"Sign LineItem2 not correct");
		Assert::AreEqual(string("EUR"), items[1].get_currency(),
						 L"Currency LineItem2 not correct");
	}
	TEST_METHOD(read_from_csv_keytrade_positive) {
		//given
		TextImportFileMock mock_file;
		//when
		TextImport import("Dummy Path Positive", mock_file);
		vector<LineItem> items = import.get_imported_items();
		//then
		Assert::AreEqual(2018, items[0].get_date().get_year(),
			L"Date not correct");
		Assert::AreEqual(5, items[0].get_date().get_month(),
			L"Date not correct");
		Assert::AreEqual(2, items[0].get_date().get_day(),
			L"Date not correct");
		Assert::AreEqual(string(""), items[0].get_account(),
			L"Account not correct");
		Assert::AreEqual(string("5 eurocent/verrichting - van 01/04/2018 tot 30/04/2018"),
			items[0].get_comment(),
			L"Comment not correct");
		Assert::AreEqual(0.40, items[0].get_amount(),
			L"Amount not correct");
		Assert::AreEqual('+', items[0].get_sign(),
			L"Sign not correct");
		Assert::AreEqual(string("EUR"), items[0].get_currency(),
			L"Currency not correct");
	}
	TEST_METHOD(empty_line) {
		//given
		TextImportFileMock mock_file;
		//when
		TextImport import("Dummy Path Empty", mock_file);
		vector<LineItem> items = import.get_imported_items();
		//then
		Assert::IsTrue(items.empty(), L"Vector should be empty");
	}
};

TEST_CLASS(DateTest) {
public:
	TEST_METHOD(EmptyConstructor) {
		//given
		//when
		Date date;
		//then
		Assert::AreEqual(0, date.get_year(), L"Year is not initial");
		Assert::AreEqual(0, date.get_month(), L"Month is not initial");
		Assert::AreEqual(0, date.get_day(), L"Day is not initial");
	}
	TEST_METHOD(FullConstructor) {
		//given
		//when
		Date date(2018, 05, 27);
		//then
		Assert::AreEqual(2018, date.get_year(), L"Year not constructed correctly");
		Assert::AreEqual(05, date.get_month(), L"Month not constructed correctly");
		Assert::AreEqual(27, date.get_day(), L"Day not constructed correctly");
	}
	TEST_METHOD(StringConstructor) {
		//given
		string input("27.05.2018");
		//when
		Date date(input);
		//then
		Assert::AreEqual(2018, date.get_year(), L"Year not constructed correctly");
		Assert::AreEqual(05, date.get_month(), L"Month not constructed correctly");
		Assert::AreEqual(27, date.get_day(), L"Day not constructed correctly");
	}
	TEST_METHOD(AssignOperator) {
		//given
		Date date1(2018, 05, 27);
		//when
		Date date2;
		date2 = date1;
		//then
		Assert::AreEqual(2018, date2.get_year(), L"Year not constructed correctly");
		Assert::AreEqual(05, date2.get_month(), L"Month not constructed correctly");
		Assert::AreEqual(27, date2.get_day(), L"Day not constructed correctly");
	}
	TEST_METHOD(OperatorEqualTrue) {
		//given
		Date date1(2018, 05, 27);
		Date date2(2018, 05, 27);
		//when
		//then
		Assert::IsTrue(date1 == date2, L"Dates should be equal");
	}
	TEST_METHOD(OperatorEqualFalse) {
		//given
		Date date1(2019, 05, 27);
		Date date2(2018, 05, 27);
		//when
		//then
		Assert::IsFalse(date1 == date2, L"Dates should not be equal");
	}
	TEST_METHOD(OperatorLower) {
		//given
		Date date1(2019, 05, 27);
		Date date2(2018, 05, 27);
		Date date3(2019, 06, 30);
		Date date4(2019, 05, 30);
		Date date5(2019, 05, 27);
		//when
		//then
		Assert::IsTrue(date2 < date1, L"Incorrect year comparison");
		Assert::IsFalse(date1 < date2, L"Incorrect year comparison");
		Assert::IsTrue(date1 < date3, L"Incorrect month comparison");
		Assert::IsFalse(date3 < date1, L"Incorrect month comparison");
		Assert::IsTrue(date1 < date4, L"Incorrect day comparison");
		Assert::IsFalse(date4 < date1, L"Incorrect day comparison");
		Assert::IsTrue(date1 < date5, L"Incorrect same day comparison");
		Assert::IsTrue(date5 < date1, L"Incorrect same day comparison");
	}
	TEST_METHOD(OperatorHigher) {
		//given
		Date date1(2019, 05, 27);
		Date date2(2018, 05, 27);
		Date date3(2019, 06, 30);
		Date date4(2019, 05, 30);
		Date date5(2019, 05, 27);
		//when
		//then
		Assert::IsFalse(date2 > date1, L"Incorrect year comparison");
		Assert::IsTrue(date1 > date2, L"Incorrect year comparison");
		Assert::IsFalse(date1 > date3, L"Incorrect month comparison");
		Assert::IsTrue(date3 > date1, L"Incorrect month comparison");
		Assert::IsFalse(date1 > date4, L"Incorrect day comparison");
		Assert::IsTrue(date4 > date1, L"Incorrect day comparison");
		Assert::IsFalse(date1 > date5, L"Incorrect same day comparison");
		Assert::IsFalse(date5 > date1, L"Incorrect same day comparison");
	}
};

TEST_CLASS(XMLHandlerTest) {
public:
	TEST_METHOD(LoadAndReadFile) {
		//given
		string filename("Single.xml");
		unique_ptr<XMLFile> mock_file(new XMLFileMock);
		//when
		XMLHandler handler(filename, move(mock_file));
		XMLElement* root = handler.get_XMLDocument().FirstChildElement();
		XMLElement* first_item = root->FirstChildElement();
		XMLElement* first_item_amount = first_item->FirstChildElement();
		//then
		Assert::AreEqual("Pharos", root->Name());
		Assert::AreEqual("Item", first_item->Name());
		Assert::AreEqual("ID", first_item->FirstAttribute()->Name());
		Assert::AreEqual(1, first_item->FirstAttribute()->IntValue());
		Assert::AreEqual("Amount", first_item_amount->Name());
		double amount;
		first_item_amount->QueryDoubleText(&amount);
		Assert::AreEqual(40.9, amount);
	}//LoadAndReadFile test method
	TEST_METHOD(AddLineItemsElement) {
		//given
		string filename("Empty.xml");
		unique_ptr<XMLFile> mock_file(new XMLFileMock);
		//when
		XMLHandler handler(filename, move(mock_file));
		XMLElement* root = handler.get_XMLDocument().FirstChildElement();
		XMLElement* item_added = handler.add_item(root, "LineItems");
		XMLElement* first_item = root->FirstChildElement();
		//then
		Assert::AreEqual("LineItems", item_added->Name(), 
			L"ItemAdded does not have correct name");
		Assert::IsNotNull(first_item, L"Item not added to XML correctly");
		if (first_item) {
			Assert::AreEqual(item_added->Name(), first_item->Name(),
				L"Item not added to XML correctly");
		}//nullptr check			
	}//AddLineItemsElement test method
	TEST_METHOD(AddLineItemsWithID) {
		//given
		string filename("Empty.xml");
		unique_ptr<XMLFile> mock_file(new XMLFileMock);
		//when
		XMLHandler handler(filename, move(mock_file));
		XMLElement* root = handler.get_XMLDocument().FirstChildElement();
		XMLElement* lineitems_element = handler.add_item(root, "LineItems");
		XMLElement* lineitem1 = handler.add_item(lineitems_element, "Item");
		handler.set_ID(lineitem1, 1);
		XMLElement* lineitem2 = handler.add_item(lineitems_element, "Item");
		handler.set_ID(lineitem2, 2);
		//then
		Assert::IsNotNull(lineitems_element->FirstChildElement(),
			L"LineItem1 not added correctly");
		Assert::IsNotNull(lineitems_element->LastChildElement(),
			L"LineItem2 not added correctly");
		if (lineitem1) {
			Assert::AreEqual(1, lineitem1->IntAttribute("ID"),
				L"ID of item 1 not correct");
		}//check nullptr lineitem1
		if (lineitem2) {
			Assert::AreEqual(2, lineitem2->IntAttribute("ID"),
				L"ID item 2 not correct");
		}//check nullptr lineitem2
	}//AddLineItems test method
	TEST_METHOD(AddItemWithValues) {
		//given
		string filename("Empty.xml");
		unique_ptr<XMLFile> mock_file(new XMLFileMock);
		//when
		XMLHandler handler(filename, move(mock_file));
		XMLElement* root = handler.get_XMLDocument().FirstChildElement();
		XMLElement* item = handler.add_item(root, "Item");
		handler.set_ID(item, 1);
		handler.add_item_with_value(item, "Value", 20.5);
		handler.add_item_with_value(item, "Comment", "Dit is een comment.");
		//then
		double value;
		XMLElement* value_element = item->FirstChildElement("Value");
		if (value_element)
			XMLError error = value_element->QueryDoubleText(&value);
		Assert::AreEqual(20.5, value, L"Double value of item not correct.");

		string comment;
		XMLElement* comment_element = item->FirstChildElement("Comment");
		if (comment_element)
			comment = comment_element->GetText();
		Assert::AreEqual("Dit is een comment.", comment.c_str(), L"Comment not correct.");
	}
};

TEST_CLASS(XMLDatabaseTest) {
public:
	TEST_METHOD(ReadLineItems) {
		//given
		string filename("Table.xml");
		unique_ptr<XMLFile> mock_file(new XMLFileMock);
		unique_ptr<XMLHandler> mock_handler(new XMLHandler(filename, move(mock_file)));
		XMLDatabase db(move(mock_handler));
		//when
		vector<LineItem> test_lineitems;
		db.read_lineitems(test_lineitems);
		//then
		vector<LineItem>::size_type expected_vec_size = 2;
		Assert::AreEqual(expected_vec_size, test_lineitems.size(),
			L"Vector does not contain all items.");
		/*by asserting size first, we are sure that the following statements
		will not dump on the [] statement as the assert above will exit
		this test when there are no 2 items. */
		//item 1
		Assert::AreEqual(40.9, test_lineitems[0].get_amount(),
			L"Amount Item 1 not correct.");
		Assert::AreEqual('-', test_lineitems[0].get_sign(),
			L"Sign Item 1 not correct.");
		Assert::AreEqual("EUR", test_lineitems[0].get_currency().c_str(),
			L"Currency Item 1 not correct.");
		Assert::AreEqual("Delhaize", test_lineitems[0].get_account().c_str(),
			L"Account Item 1 not correct.");
		Assert::AreEqual("Inkopen", test_lineitems[0].get_comment().c_str(),
			L"Comment Item 1 not correct.");
		Assert::AreEqual(8, test_lineitems[0].get_date().get_day(),
			L"Date Item 1 not correct.");
		Assert::AreEqual(1, test_lineitems[0].get_date().get_month(),
			L"Date Item 1 not correct.");
		Assert::AreEqual(2018, test_lineitems[0].get_date().get_year(),
			L"Date Item 1 not correct.");
		//item 2
		Assert::AreEqual(450.0, test_lineitems[1].get_amount(),
			L"Amount Item 2 not correct.");
		Assert::AreEqual('+', test_lineitems[1].get_sign(),
			L"Sign Item 2 not correct.");
		Assert::AreEqual("EUR", test_lineitems[1].get_currency().c_str(),
			L"Currency Item 2 not correct.");
		Assert::AreEqual("Lego", test_lineitems[1].get_account().c_str(),
			L"Account Item 2 not correct.");
		Assert::AreEqual("Hogwarts Castle", test_lineitems[1].get_comment().c_str(),
			L"Comment Item 2 not correct.");
		Assert::AreEqual(20, test_lineitems[1].get_date().get_day(),
			L"Date Item 2 not correct.");
		Assert::AreEqual(11, test_lineitems[1].get_date().get_month(),
			L"Date Item 2 not correct.");
		Assert::AreEqual(2018, test_lineitems[1].get_date().get_year(),
			L"Date Item 2 not correct.");
	}
	TEST_METHOD(AddLineItemsToEmptyDB) {
/*Defining tests for this class was quite difficult. This class basically writes to a
XML file and reads from it. To unit test, we need to rely on mocking.
I wanted to test the write function separately from the read function. This test for the
write function writes to a mock file but how can we check the content of this mock file
without using the read functions?
To do this, I opted to define an separate constructor for the mock XML file class only
that accepts a shared pointer of an empty XML document. The real XML file class will save
to an actual XML file but the mock file will save it to the empty XML document to which
the shared pointer points. We can not access the content of the XMLFileMock directly but
we can via the shared pointer which we declared first before creating the XMLFileMock object.*/
		//given
		string filename("Empty.xml");
		shared_ptr<XMLDocument> test_doc_ptr = make_shared<XMLDocument> ();
		unique_ptr<XMLFile> mock_file(new XMLFileMock(test_doc_ptr));
		unique_ptr<XMLHandler> mock_handler(new XMLHandler(filename, move(mock_file)));
		XMLDatabase db(move(mock_handler));

		Date test_date(2018, 12, 22);
		LineItem test_item(20.5, '+', "EUR", test_date, "Delhaize", "Inkopen");
		vector<LineItem> test_lineitems;
		test_lineitems.push_back(test_item);
		//when
		db.add_all_lineitems(test_lineitems);
		//then
		XMLElement* root = test_doc_ptr->FirstChildElement();
		Assert::IsNotNull(root, L"Root not created correctly.");

		if (root) {
			XMLElement* table = root->FirstChildElement();
			Assert::IsNotNull(table, L"Lineitems table not created correctly.");
		
			if (table) {
				XMLElement* item = table->FirstChildElement();
				Assert::IsNotNull(item, L"Item not created correctly.");

				if (item) {
					int ID = item->IntAttribute("ID");
					Assert::AreEqual(1, ID, L"ID not written correctly.");

					double amount;
					XMLElement* amount_element = item->FirstChildElement("Amount");
					if (amount_element)
						amount_element->QueryDoubleText(&amount);
					Assert::AreEqual(20.5, amount, L"Amount not written correctly.");

					char sign;
					XMLElement* sign_element = item->FirstChildElement("Sign");
					if (sign_element)
						sign = sign_element->GetText()[0];
					Assert::AreEqual('+', sign, L"Sign not written correctly.");

					string currency;
					XMLElement* currency_element = item->FirstChildElement("Currency");
					if (currency_element)
						currency = currency_element->GetText();
					Assert::AreEqual("EUR", currency.c_str(), L"Currency not written correctly.");

					string account;
					XMLElement* account_element = item->FirstChildElement("Account");
					if (account_element)
						account = account_element->GetText();
					Assert::AreEqual("Delhaize", account.c_str(), L"Account not written correctly.");

					string comment;
					XMLElement* comment_element = item->FirstChildElement("Comment");
					if (comment_element)
						comment = comment_element->GetText();
					Assert::AreEqual("Inkopen", comment.c_str(), L"Comment not written correctly.");

					string date;
					XMLElement* date_element = item->FirstChildElement("Date");
					if (date_element)
						date = date_element->GetText();
					Assert::AreEqual("22.12.2018", date.c_str(), L"Date not written correctly.");
				}//if item exists
			}//if table exists
		}//if root exists
	}
	TEST_METHOD(AddLineItemsToExistingDB) {
		//given
		string filename("Table.xml");
		shared_ptr<XMLDocument> test_doc_ptr = make_shared<XMLDocument>();
		unique_ptr<XMLFile> mock_file(new XMLFileMock(test_doc_ptr));
		unique_ptr<XMLHandler> mock_handler(new XMLHandler(filename, move(mock_file)));
		XMLDatabase db(move(mock_handler));

		Date test_date(2018, 12, 22);
		LineItem test_item(20.5, '+', "EUR", test_date, "Delhaize", "Inkopen");
		vector<LineItem> test_lineitems;
		test_lineitems.push_back(test_item);
		//when
		db.add_all_lineitems(test_lineitems);
		//then
		XMLElement* root = test_doc_ptr->FirstChildElement();
		if (root) {
			XMLElement* table = root->FirstChildElement();
			if (table) {
				XMLElement* item = table->FirstChildElement();
				int count = 1;
				while (item->NextSiblingElement()) {//while will be true as long as a next sibling exists. If not, pointer will be 0(null) and break the loop.
					item = item->NextSiblingElement();
					count++;
				}
				Assert::AreEqual(3, count, L"Not correct number of items (3).");
				Assert::AreEqual(3, table->LastChildElement()->IntAttribute("ID"),
								L"ID not correct.");
				//all other tests to check the correctness of the added item are checked in the Empty DB test method
			}//if table exists
		}//if root exists
	}
	TEST_METHOD(CheckDuplicate) {
		//given
		string filename("Table.xml");
		shared_ptr<XMLDocument> test_doc_ptr = make_shared<XMLDocument>();
		unique_ptr<XMLFile> mock_file(new XMLFileMock(test_doc_ptr));
		unique_ptr<XMLHandler> mock_handler(new XMLHandler(filename, move(mock_file)));
		XMLDatabase db(move(mock_handler));

		Date test_date(2018, 11, 20);
		LineItem test_item(450, '+', "EUR", test_date, "Lego", "Hogwarts Castle");
		vector<LineItem> test_lineitems;
		test_lineitems.push_back(test_item);
		//when
		db.add_all_lineitems(test_lineitems);
		//then
		XMLElement* root = test_doc_ptr->FirstChildElement();
		if (root) {
			XMLElement* table = root->FirstChildElement();
			if (table) {
				XMLElement* item = table->FirstChildElement();
				int count = 1;
				while (item->NextSiblingElement()) {//while will be true as long as a next sibling exists. If not, pointer will be 0(null) and break the loop.
					item = item->NextSiblingElement();
					count++;
				}
				Assert::AreEqual(2, count, L"Not correct number of items (2).");
				Assert::AreEqual(2, table->LastChildElement()->IntAttribute("ID"),
					L"ID not correct.");
				//all other tests to check the correctness of the added item are checked in the Empty DB test method
			}//if table exists
		}//if root exists
	}
	TEST_METHOD(AddItemWithEarlierDate) {
		//given
		string filename("Table.xml");
		shared_ptr<XMLDocument> test_doc_ptr = make_shared<XMLDocument>();
		unique_ptr<XMLFile> mock_file(new XMLFileMock(test_doc_ptr));
		unique_ptr<XMLHandler> mock_handler(new XMLHandler(filename, move(mock_file)));
		XMLDatabase db(move(mock_handler));

		Date test_date(2018, 7, 18);
		LineItem test_item(20.5, '-', "EUR", test_date, "Amazon", "HP Puzzel");
		vector<LineItem> test_lineitems;
		test_lineitems.push_back(test_item);
		//when
		db.add_all_lineitems(test_lineitems);
		//then
		XMLElement* root = test_doc_ptr->FirstChildElement();
		if (root) {
			XMLElement* table = root->FirstChildElement();
			if (table) {
				XMLElement* item = table->FirstChildElement();
				int count = 1;
				while (item->NextSiblingElement()) {//while will be true as long as a next sibling exists. If not, pointer will be 0(null) and break the loop.
					item = item->NextSiblingElement();
					count++;
				}
				Assert::AreEqual(3, count, L"Not correct number of items (3).");
				Assert::AreEqual(3, table->LastChildElement()->IntAttribute("ID"),
					L"ID item 3 not correct.");
				Assert::AreEqual(2, table->LastChildElement()->PreviousSiblingElement()
					->IntAttribute("ID"), L"ID item 2 is not correct.");

				string comment_new;
				XMLElement* comment_element_new = table->LastChildElement()->PreviousSiblingElement()
					->FirstChildElement("Comment");
				if (comment_element_new)
					comment_new = comment_element_new->GetText();
				Assert::AreEqual("HP Puzzel", comment_new.c_str(), L"Item not added at right place");

				string comment_old;
				XMLElement* comment_element_old = table->LastChildElement()->FirstChildElement("Comment");
				if (comment_element_old)
					comment_old = comment_element_old->GetText();
				Assert::AreEqual("Hogwarts Castle", comment_old.c_str(), L"Item not added at right place");
			}//if table exists
		}//if root exists
	}
};