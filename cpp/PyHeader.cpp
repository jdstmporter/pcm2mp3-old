/*
 * PyHeader.cpp
 *
 *  Created on: 2 May 2018
 *      Author: julianporter
 */

#import "PyHeader.hpp"


long toLong(PyObject *obj) {
	if(!PyLong_Check(obj)) throw std::runtime_error("Object is not a long");
	return PyLong_AsLong(obj);
}

bool toBool(PyObject *obj) {
	return PyObject_IsTrue(obj);
}
double toDouble(PyObject *obj)  {
	if(!PyFloat_Check(obj)) throw std::runtime_error("Object is not a float");
	return PyFloat_AsDouble(obj) != 0;
}

std::string toString(PyObject *obj) {
	if(!PyUnicode_Check(obj)) throw std::runtime_error("Object is not a string");
	Py_ssize_t size;
	auto c=PyUnicode_AsUTF8AndSize(obj,&size);
	if(c==NULL) throw std::runtime_error("Could not convert as String");
	return std::string(c,size);
}
