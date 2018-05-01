/*
 * MP3Object.cpp
 *
 *  Created on: 1 May 2018
 *      Author: julianporter
 */

#include "MP3Object.hpp"
#include "lib/MP3Encoder.hpp"

using namespace pylame::mp3;

PyObject *MP3_new(PyTypeObject *type,PyObject *args,PyObject *keywords) {
	auto self = (PyMP3 *)type->tp_alloc(type, 0);
	if (self != NULL) {
		self->mp3 = NULL;
	}
	return (PyObject *)self;
}

// Release

void MP3_dealloc(PyMP3 *self) {
	if(self->mp3) {
		delete self->mp3;
	}
	auto p=(PyObject *)self;
	p->ob_type->tp_free(p);
}

// Initialisation



int MP3_init(PyMP3 *self,PyObject *args,PyObject *keywords) {
	Py_buffer buffer;

	if(!PyArg_ValidateKeywordArguments(keywords)) {
		PyErr_SetString(PyExc_OSError,"Bad arguments to constructor");
		return -1;
	}
	if(!PyArg_ParseTuple(args,"y*",&buffer)) {
		PyErr_SetString(PyExc_TypeError,"No data buffer provided in PCM constructor");
		return -1;
	}
	MP3Parameters parameters;
	try {
		PyObject *key, *value;
		Py_ssize_t pos=0;
		while(PyDict_Next(keywords,&pos,&key,&value)) {
			std::string k=toString(key);

			if(MP3Parameters::id3names.find(k)!=MP3Parameters::id3names.end()) {
				auto p=MP3Parameters::id3names.at(k);
				std::string v=toString(value);
				parameters[k]=v;
			}
			else if(k=="copyright") {
				parameters.isCopyright(toBool(value));
			}
			else if(k=="original") {
				parameters.isOriginal(toBool(value));
			}
			else if(k=="id3_version") {
				auto v=toLong(value);
				auto vers=(v==1) ? ID3Versions::OneOnly : (v==2) ? ID3Versions::TwoOnly : ID3Versions::OneAndTwo;
				parameters.useID3Version(vers);
			}
			else if(k=="quality") {
				parameters.setQuality(toLong(value));
			}
			else if(k=="rate") {
				parameters.setRate(toLong(value));
			}
		}
	}
	catch(std::exception &e) {
		PyErr_SetString(PyExc_OSError,e.what());
		return -1;
	}
	self->mp3=new pylame::MP3File(parameters);
	return 0;
}

PyObject *MP3_sampleRate(PyMP3 *self,PyObject *args,PyObject *keywords) {
	if(self->mp3==NULL) {
		PyErr_SetString(PyExc_OSError,"No data in MP3 object");
		return nullptr;
	}
	return PyLong_FromUnsignedLong((unsigned long)self->mp3->getRate());
}
PyObject *MP3_duration(PyMP3 *self,PyObject *args,PyObject *keywords) {
	if(self->mp3==NULL) {
		PyErr_SetString(PyExc_OSError,"No data in MP3 object");
		return nullptr;
	}
	return PyFloat_FromDouble(self->mp3->getDuration());
}

Py_ssize_t MP3_len(PyMP3 *self) {
	if(self->mp3==NULL) {
			PyErr_SetString(PyExc_OSError,"No data in MP3 object");
			return -1;
		}
	return self->mp3->size();
}
