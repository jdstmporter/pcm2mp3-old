/*
 * Lame.c
 *
 *  Created on: 1 Dec 2017
 *      Author: julianporter
 */

#include "PyHeader.hpp"
#include "lib/WAVFile.hpp"
#include "lib/MP3Encoder.hpp"
#include "lib/Iterator32.hpp"
#include "lib/transcoder.hpp"
#include <sstream>
#include <regex>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include "lib/lamer.hpp"
#include "PCMObject.hpp"
#include "MP3Object.hpp"


static PyObject *mp3Error;

const char* ModuleName="pcm2mp3";
const char* ErrorName="MP3Error";

static const char *keywords[] = {"","bitrate","quality",NULL};

static PyObject * mp3stream(PyObject *self, PyObject *args, PyObject *keywds) {
	unsigned bitRate = 64;
	unsigned quality = 5;
	Py_buffer buffer;

	try {
		if(!PyArg_ParseTupleAndKeywords(args,keywds,"y*|$II",(char **)keywords,&buffer,&bitRate,&quality)) {
			throw PException(PyExc_TypeError,"API is transcode(stream,bitrate=64,quality=5)");
		}
		try {
			char *orig=(char *)buffer.buf;
			pylame::data_t data(orig,orig+buffer.len);
			pylame::Transcode transcode(data,quality,bitRate);
			std::cerr << "Transcode call completed: making output";
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
		{"transcode",(PyCFunction) mp3stream, METH_VARARGS | METH_KEYWORDS, "Transcode bytes"},
		{NULL, NULL, 0, NULL}
};

static struct PyModuleDef module = {
		PyModuleDef_HEAD_INIT,
		ModuleName,
		"",			/// Documentation string
		-1,			/// Size of state (-1 if in globals)
		methods,
		NULL,		/// Slots
		NULL,		/// traverse
		NULL,		/// clear
		NULL		/// free
};






PyMODINIT_FUNC PyInit_pcm2mp3(void) {
	PyObject *m = PyModule_Create(&module);
	if(m==NULL) return NULL;
	try {
		std::stringstream s;
		s << ModuleName << "." << ErrorName;
		mp3Error=PyErr_NewException(s.str().c_str(),NULL,NULL);
		if(mp3Error==NULL) throw std::runtime_error("Cannot allocate MP3Error");
		Py_INCREF(mp3Error);
		auto result=PyModule_AddObject(m,ErrorName,mp3Error);
		if(result<0) throw std::runtime_error("Cannot attach MP3Error to module");


	    PCMManager mgr;
	    //inotify_WatcherType.tp_new = PyType_GenericNew;
	    if (!mgr.isReady()) throw std::runtime_error("Cannot attach PCM to module");
	    mgr.inc();
	    mgr.add(m,"PCM");

	    MP3Manager mp3;
	    	    //inotify_WatcherType.tp_new = PyType_GenericNew;
	    	    if (!mp3.isReady()) throw std::runtime_error("Cannot attach MP3 to module");
	    	    mp3.inc();
	    	    mp3.add(m,"MP3");

	    for(auto it=MP3Manager::ID3Modes.begin();it!=MP3Manager::ID3Modes.end();it++) {
	    	PyModule_AddIntConstant(m,it->first.c_str(),static_cast<unsigned>(it->second));
	    }

		return m;
	}
	catch(std::exception &e) {
		PyErr_SetString(PyExc_ImportError,e.what());
		return NULL;
	}
}



