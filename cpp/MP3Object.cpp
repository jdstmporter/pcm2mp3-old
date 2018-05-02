/*
 * MP3Object.cpp
 *
 *  Created on: 1 May 2018
 *      Author: julianporter
 */

#include "MP3Object.hpp"
#include "PCMObject.hpp"


using namespace pylame::mp3;



const std::map<std::string,ID3Versions> MP3Manager::ID3Modes = {
		{"One", ID3Versions::OneOnly },
		{"Two", ID3Versions::TwoOnly },
		{"OneAndTwo", ID3Versions::OneAndTwo }
};

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


static char *keywordList[] =
{
		"title", "artist", "album", "year", "comment", "track", "genre",
		"copyright", "original", "id3", "quality", "rate"
};

int MP3_init(PyMP3 *self,PyObject *args,PyObject *keywords) {
	Py_buffer buffer;

	PyObject *pcmobject=nullptr;
	const char *title=nullptr;
	const char *artist=nullptr;
	const char *album=nullptr;
	const char *year=nullptr;
	const char *comment=nullptr;
	const char *track=nullptr;
	const char *genre=nullptr;
	PyObject *copyright=nullptr;
	PyObject *original=nullptr;
	unsigned id3mode = static_cast<unsigned>(ID3Versions::OneAndTwo);
	unsigned bitRate = 64;
	unsigned quality = 5;

	if(!PyArg_ParseTupleAndKeywords(args, keywords,"o|sssssssiiiiii",keywordList,
			&pcmobject,&title,&artist,&album,&year,&comment,&track,&genre,
			&copyright,&original,&id3mode,&quality,&bitRate)) {
		PyErr_SetString(PyExc_OSError,"Bad arguments to constructor");
		return -1;
	}

	MP3Parameters parameters(quality,bitRate);
	if(title!=nullptr) parameters[ID3Tag::Title]=title;
	if(artist!=nullptr) parameters[ID3Tag::Artist]=artist;
	if(album!=nullptr) parameters[ID3Tag::Album]=album;
	if(year!=nullptr) parameters[ID3Tag::Year]=year;
	if(comment!=nullptr) parameters[ID3Tag::Comment]=comment;
	if(track!=nullptr) parameters[ID3Tag::Track]=track;
	if(genre!=nullptr) parameters[ID3Tag::Genre]=genre;

	parameters.isCopyright(PyObject_IsTrue(copyright)!=0);
	parameters.isOriginal(PyObject_IsTrue(original)!=0);
	parameters.useID3Version(static_cast<ID3Versions>(id3mode));


	try {
		auto pcm=(PyPCM *)pcmobject;
		if (pcm->pcm == NULL) {
			PyErr_SetString(PyExc_OSError, "No data in PCM object");
			return nullptr;
		}
		auto mp3=new pylame::MP3File(parameters);
		mp3->transcode(pcm->pcm);
		self->mp3=mp3;
		return 0;
	}
	catch(std::exception &e) {
		PyErr_SetString(PyExc_OSError,e.what());
		return -1;
	}
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

PyObject *MP3_data(PyMP3 *self,PyObject *args,PyObject *keywords) {
	if(self->mp3==NULL) {
			PyErr_SetString(PyExc_OSError,"No data in MP3 object");
			return -1;
	}
	return PyByteArray_FromStringAndSize(self->mp3->bytes(),self->mp3->size());
}


PyMethodDef PyMP3_methods[] = {
		{"sampleRate",(PyCFunction)MP3_sampleRate,METH_NOARGS,"Get the number of samples per second"},
		{"duration",(PyCFunction)MP3_duration,METH_NOARGS,"Get the approximate duration of the data in seconds"},
		{"data",(PyCFunction)MP3_data,METH_NOARGS,"Get the MP3 file data"},
		{NULL}
};

PyMemberDef PyMP3_members[] = {{
				(char *)"_mp3",
				T_OBJECT_EX,
				offsetof(PyMP3,mp3),
				READONLY,
				(char *)"opaque data object"},
				{NULL}  /* Sentinel */
};

PySequenceMethods PyMP3_sequence = {
		(lenfunc)MP3_len,			/* sq_length */
		0,								/* sq_concat */
		0,								/* sq_repeat */
		0,								/* sq_item */
		0,								/* sq_slice */
		0,								/* sq_ass_item */
		0,								/* sq_ass_slice */
		0,								/* sq_contains */
		0,								/* sq_inplace_concat */
		0,								/* sq_inplace_repeat */
};

const char *PyMP3_Name="pcm2mp3.MP3";
const char *PyMP3_Docstr="Container / transcoder for MP3 data";


static PyTypeObject PyMP3_Type = {
		PyVarObject_HEAD_INIT(NULL,0)
		(char *)PyMP3_Name,             		/*tp_name*/
		sizeof(PyMP3), 			/*tp_basicsize*/
		0,                         	/*tp_itemsize*/
		(destructor)MP3_dealloc,		/*tp_dealloc*/
		0,                         			/*tp_print*/
		0,                         			/*tp_getattr*/
		0,                         			/*tp_setattr*/
		0,                         			/*tp_compare*/
		0,                         			/*tp_repr*/
		0,                         			/*tp_as_number*/
		0,                  /*tp_as_sequence*/
		0,                         			/*tp_as_mapping*/
		0,                         			/*tp_hash */
		0,                         			/*tp_call*/
		0,                         			/*tp_str*/
		0,                         			/*tp_getattro*/
		0,                         			/*tp_setattro*/
		0,                         			/*tp_as_buffer*/
		Py_TPFLAGS_DEFAULT, 				/*tp_flags*/
		PyMP3_Docstr,								/* tp_doc */
		0,   								/* tp_traverse */
		0,           						/* tp_clear */
		0,		               				/* tp_richcompare */
		0,		               				/* tp_weaklistoffset */
		0,		               				/* tp_iter */
		0,		               				/* tp_iternext */

		PyMP3_methods,             		/* tp_methods */
		PyMP3_members,             		/* tp_members */
		0,                         			/* tp_getset */
		0,                         			/* tp_base */
		0,                         			/* tp_dict */
		0,                         			/* tp_descr_get */
		0,                         			/* tp_descr_set */
		0,                         			/* tp_dictoffset */
		(initproc)MP3_init,      		/* tp_init */
		0,                         			/* tp_alloc */
		(newfunc)MP3_new,                 		/* tp_new */
};

bool PCMManager::isReady() { return PyType_Ready(&PyMP3_Type)>=0; }
void PCMManager::inc() { Py_INCREF(&PyMP3_Type); }
void PCMManager::add(PyObject *m,const char *name) {
	PyModule_AddObject(m,name,(PyObject *)&PyMP3_Type);
}
PyPCM * PCMManager::make() {
	return (PyPCM *)_PyObject_New(&PyMP3_Type);
}







