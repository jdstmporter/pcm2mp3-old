/*
 * Lame.c
 *
 *  Created on: 1 Dec 2017
 *      Author: julianporter
 */

#include "PyHeader.hpp"
#include "lib/enums.hpp"
#include "lib/WAVFile.hpp"
#include "lib/MP3Encoder.hpp"
#include "lib/Iterator32.hpp"
#include "lib/transcoder.hpp"
#include <sstream>
#include <regex>
#include <iostream>
#include <fstream>
#include <stdexcept>

static PyObject *mp3Error;

const char* PackageName="wav2mp3";
const char* ModuleName="wav2mp3._wav2mp3";
const char* ErrorName="MP3Error";

const char * fullModuleName() {
	std::stringstream s;
	s << PackageName << "." << ModuleName;
	return s.str().c_str();
}

std::vector<std::string> kw {"","","bitrate","quality"};
std::vector<char *> keywords(kw.size()+1,nullptr);



bool checkName(const char *name,const char *suffix) {
	std::stringstream s;
	s << ".*\\." << suffix << "$";
	return std::regex_match(name,std::regex(s.str()));
}

static PyObject * mp3(PyObject *self, PyObject *args, PyObject *keywds) {
	const char *inFile;
	const char *outFile;
	unsigned bitRate = 64;
	unsigned quality = 5;

	try {

		if(!PyArg_ParseTupleAndKeywords(args,keywds,"ss|$II",keywords.data(),&inFile,&outFile,&bitRate,&quality)) {
			throw PException(PyExc_TypeError,"API is transcode(infile,outfile,bitrate=64,quality=5)");
		}
		try {
			if(!checkName(inFile,"wav")) throw MP3Error("Source file does not have .wav suffix");
			if(!checkName(outFile,"mp3")) throw MP3Error("Destination file does not have .mp3 suffix");

			std::ifstream wavFile(inFile,std::ifstream::binary);
			Transcode transcode(wavFile,quality,bitRate);
			std::ofstream out(outFile,std::ofstream::binary);
			out << transcode;
			out.close();
			return PyLong_FromUnsignedLong((unsigned long)transcode.size());
		}
		catch(std::exception &e) {
			throw PException(mp3Error,e.what());
		}
	}
	catch(PException & p) {
		return p();
	}
}

static PyObject * mp3stream(PyObject *self, PyObject *args, PyObject *keywds) {
	unsigned bitRate = 64;
	unsigned quality = 5;
	Py_buffer buffer;

	try {
		if(!PyArg_ParseTupleAndKeywords(args,keywds,"y*|$II",keywords.data()+1,&buffer,&bitRate,&quality)) {
			throw PException(PyExc_TypeError,"API is transcode(stream,bitrate=64,quality=5)");
		}
		try {
			char *orig=(char *)buffer.buf;
			data_t data(orig,orig+buffer.len);
			Transcode transcode(data,quality,bitRate);
			auto out = Py_BuildValue("y#",transcode.ptr(),transcode.size());
			PyBuffer_Release(&buffer);
			return out;
		}
		catch(std::exception &e) {
			throw PException(mp3Error,e.what());
		}
	}
	catch(PException &p) {
		return p();
	}
}





static struct PyMethodDef methods[] = {
		{"transcodeF",(PyCFunction) mp3, METH_VARARGS | METH_KEYWORDS, "Transcode file"},
		{"transcodeS",(PyCFunction) mp3stream, METH_VARARGS | METH_KEYWORDS, "Transcode bytes"},
		{NULL, NULL, 0, NULL}
};

static struct PyModuleDef module = {
		PyModuleDef_HEAD_INIT,
		fullModuleName(),
		"",			/// Documentation string
		-1,			/// Size of state (-1 if in globals)
		methods,
		NULL,		/// Slots
		NULL,		/// traverse
		NULL,		/// clear
		NULL		/// free
};






PyMODINIT_FUNC PyInit__wav2mp3(void) {
	PyObject *m = PyModule_Create(&module);
	if(m==NULL) return NULL;
	try {
		std::stringstream s;
		s << fullModuleName() << "." << ErrorName;
		mp3Error=PyErr_NewException(s.str().c_str(),NULL,NULL);
		if(mp3Error==NULL) throw std::runtime_error("Cannot allocate MP3Error");
		Py_INCREF(mp3Error);
		auto result=PyModule_AddObject(m,ErrorName,mp3Error);
		if(result<0) throw std::runtime_error("Cannot attach MP3Error to module");

		// a bit of housekeeping
		std::transform(kw.begin(),kw.end(),keywords.begin(),[](const std::string &s) {
			return const_cast<char *>(s.c_str()); });
		return m;
	}
	catch(std::exception &e) {
		PyErr_SetString(PyExc_ImportError,e.what());
		return NULL;
	}
}



