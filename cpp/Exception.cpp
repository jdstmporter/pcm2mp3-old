/*
 * Exception.cpp
 *
 *  Created on: 4 Mar 2018
 *      Author: julianporter
 */

#include "PException.hpp"
#include <stdexcept>

void PException::init(PyObject *module,const std::string &name) {
	mp3Error=PyErr_NewException(name.c_str(),NULL,NULL);
	if(mp3Error==NULL) throw std::runtime_error("Cannot allocate MP3Error");
	Py_INCREF(mp3Error);

	auto result=PyModule_AddObject(module,name.c_str(),mp3Error);
	if(result<0) throw std::runtime_error("Cannot attach MP3Error to module");

}

PException::PException(const PError type,const std::string &message_) :
	std::exception(), ex(type), message(message_) {};

PyObject *PException::operator()() {
	PyObject *obj=(ex==PError::MP3Error) ? PException::mp3Error : PyExc_TypeError;
	PyErr_SetString(obj,message.c_str());
	return nullptr;
}
