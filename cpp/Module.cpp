/*
 * Module.cpp
 *
 *  Created on: 4 Mar 2018
 *      Author: julianporter
 */

#include "PyHeader.hpp"
#include "AudioFile.hpp"
#include "Lame.hpp"


const char* ModuleName="pcm2mp3";
const char* ErrorName="MP3Error";
const char* AudioFileName="PCMAudioFile";


static PyGetSetDef audiofile_Properties[] = {
		{"bytesPerSample",(getter)audiofile_BytesPerSample, NULL,"Bytes per sample",NULL},
		{"fileType",(getter)audiofile_Kind, NULL,"File type",NULL},
		{"sampleFormat",(getter)audiofile_Format, NULL,"File sample format",NULL},
		{NULL}
};

static PyMethodDef audiofile_Methods[] = {
		{NULL}
};

static PyTypeObject audiofile_AudioFileObject = {
		PyVarObject_HEAD_INIT(NULL,0)
		"AudioFile",
		sizeof(AudioFileObject),
		0,                         /* tp_itemsize */
		(destructor)audiofile_Dealloc,/* tp_dealloc */
		0,                         /* tp_print */
		0,                         /* tp_getattr */
		0,                         /* tp_setattr */
		0,                         /* tp_reserved */
		(reprfunc)audiofile_Repr,  /* tp_repr */
		0,                         /* tp_as_number */
		0,                         /* tp_as_sequence */
		0,                         /* tp_as_mapping */
		0,                         /* tp_hash  */
		0,                         /* tp_call */
		(reprfunc)audiofile_Repr,  /* tp_str */
		0,                         /* tp_getattro */
		0,                         /* tp_setattro */
		0,                         /* tp_as_buffer */
		Py_TPFLAGS_DEFAULT,        /* tp_flags */
		"Audio File object",           /* tp_doc */
		0,                         /* tp_traverse */
		0,                         /* tp_clear */
		0,                         /* tp_richcompare */
		0,                         /* tp_weaklistoffset */
		0,                         /* tp_iter */
		0,                         /* tp_iternext */
		audiofile_Methods,             /* tp_methods */
		0,             /* tp_members */
		audiofile_Properties,       /* tp_getset */
		0,                         /* tp_base */
		0,                         /* tp_dict */
		0,                         /* tp_descr_get */
		0,                         /* tp_descr_set */
		0,                         /* tp_dictoffset */
		(initproc)audiofile_Init,      /* tp_init */
		0,                         /* tp_alloc */
		audiofile_New,                 /* tp_new */
};






static struct PyMethodDef methods[] = {
		{"transcodeF",(PyCFunction) mp3file, METH_VARARGS | METH_KEYWORDS, "Transcode file"},
		{"transcodeS",(PyCFunction) mp3stream, METH_VARARGS | METH_KEYWORDS, "Transcode bytes"},
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




PyMODINIT_FUNC PyInit__pcm2mp3(void) {

	if(PyType_Ready(&audiofile_AudioFileObject)<0) return NULL;

	PyObject *m = PyModule_Create(&module);
	if(m==NULL) return NULL;
	try {
		std::stringstream s;
		s << ModuleName << "." << ErrorName;
		PException::init(m,s.str());


		Py_INCREF(&audiofile_AudioFileObject);
		auto result=PyModule_AddObject(m,AudioFileName,(PyObject *)&audiofile_AudioFileObject);
		if(result<0) throw std::runtime_error("Cannot attach PCMAudioFile object to module");

		return m;
	}
	catch(std::exception &e) {
		PyErr_SetString(PyExc_ImportError,e.what());
		return NULL;
	}
}







