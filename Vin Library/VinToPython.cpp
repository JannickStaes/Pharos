#include <Python.h>
#include <datetime.h>
#include "TextImport.h"
#include <iostream>

PyObject* read_from_csv_keytrade(PyObject *self, PyObject* inputfile_location) {
	PyDateTime_IMPORT;
	PyObject* str = PyUnicode_AsUTF8String(inputfile_location);
	Py_DECREF(str);
	const char* inputfile_c = PyBytes_AsString(str);
	TextImport import(inputfile_c);
	std::vector<LineItem> new_items = import.get_imported_items();
	
	PyObject* ret = PyList_New(0);

	for (std::vector<LineItem>::const_iterator iter = new_items.begin(); iter != new_items.end(); iter++) {
		PyObject* list_item = PyDict_New();
		PyDict_SetItem(list_item, PyBytes_FromString("Amount"), PyFloat_FromDouble(iter->get_amount()) );
		std::string sign_str(1,iter->get_sign() );
		PyDict_SetItem(list_item, PyBytes_FromString("Sign"), PyBytes_FromString(sign_str.c_str()) );
		PyDict_SetItem(list_item, PyBytes_FromString("Currency"), PyBytes_FromString(iter->get_currency().c_str()) );
		PyDict_SetItem(list_item, PyBytes_FromString("Account"), PyBytes_FromString(iter->get_account().c_str()) );
		PyDict_SetItem(list_item, PyBytes_FromString("Comment"), PyBytes_FromString(iter->get_comment().c_str()) );
		PyObject* date = PyDate_FromDate(iter->get_date().get_year(), iter->get_date().get_month(), iter->get_date().get_day());
		PyList_Append(ret, list_item);
	}
		
	return ret;
}

PyObject* return_int(PyObject *, PyObject* x) {
	long i = PyLong_AsLong(x);
	PyObject* ret = PyLong_FromLong(i);
	return ret;
}

static PyMethodDef VinToPython_methods[] = {
	{ "read_from_csv_keytrade", (PyCFunction)read_from_csv_keytrade, METH_O, nullptr },
	{ "return_int", (PyCFunction)return_int, METH_O, nullptr },

	// Terminate the array with an object containing nulls.
	{ NULL, NULL, 0, NULL }
};

static struct PyModuleDef VinToPython = {
	PyModuleDef_HEAD_INIT,
	"VinToPython",                        
	"Vin C++ Functions",  
	0,
	VinToPython_methods                   
};

PyMODINIT_FUNC PyInit_VinLibrary() {
	return PyModule_Create(&VinToPython);
}