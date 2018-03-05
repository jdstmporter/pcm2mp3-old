/*
 * PException.hpp
 *
 *  Created on: 4 Mar 2018
 *      Author: julianporter
 */

#ifndef PEXCEPTION_HPP_
#define PEXCEPTION_HPP_

#include <exception>
#include <string>
#include <Python.h>

enum class PError {
	TypeError,
	MP3Error
};

class PException : public std::exception {
private:

	PError ex;
	std::string message;
public:
	static PyObject *mp3Error;

	static void init(PyObject *module,const std::string &name);
	PException(const PError type,const std::string &message_);
	virtual ~PException() = default;

	virtual const char *what() const noexcept { return message.c_str(); };
	PyObject *operator()();
};


#endif /* PEXCEPTION_HPP_ */
