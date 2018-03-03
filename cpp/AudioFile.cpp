/*
 * Rates.cpp
 *
 *  Created on: 2 Dec 2017
 *      Author: julianporter
 */

#include "AudioFile.hpp"

void audiofile_Dealloc(AudioFileObject *self) {
	if(self->pcm != nullptr) {
		delete self->pcm;
	}
	Py_TYPE(self)->tp_free((PyObject *)self);
}

PyObject * audiofile_New(PyTypeObject *type, PyObject *args, PyObject *kwds) {
	AudioFileObject *self = (AudioFileObject *)type->tp_alloc(type,0);
	if(self!=NULL) { self->pcm=NULL; }
	return (PyObject *)self;
}

int audiofile_Init(AudioFileObject *self,PyObject *args, PyObject *kwds) {
	Py_buffer buffer;
	if(!PyArg_ParseTuple(args,"y*",&buffer)) {
		return -2;
	}
	try {
		char *orig=(char *)buffer.buf;
		pylame::data_t data(orig,orig+buffer.len);
		self->pcm=new pylame::pcm::PCMFile(data);
		if(self->pcm==nullptr) {
			return -2;
		}
	}
	catch(std::exception &e) {
		return -1;
	}
	return 0;

}

std::string kindOf(AudioFileObject *self) {
	auto kind=self->pcm->fileType();
	std::string out;
	switch(kind) {
	case pylame::FileType::WAV:
		out="WAV";
		break;
	case pylame::FileType::AIFF:
	case pylame::FileType::AIFC:
		out="AIFF";
		break;
	default:
		out="";
		break;
	}
	return out;
}

std::string formatOf(AudioFileObject *self) {
	auto format=self->pcm->sampleFormat();
	std::string out;
	switch(format) {
	case pylame::SampleFormat::Float32:
		out="Float32";
		break;
	case pylame::SampleFormat::Int32:
		out="Int32";
		break;
	case pylame::SampleFormat::Int16:
		out="Int16";
		break;
	default:
		out="";
		break;
	}
	return out;
}

PyObject * audiofile_Kind(AudioFileObject *self,void *closures) {
	auto out=kindOf(self);
	return PyUnicode_FromFormat("%s",out.c_str());
}

PyObject * audiofile_Format(AudioFileObject *self,void *closure) {
	auto out=formatOf(self);
	return PyUnicode_FromFormat("%s",out.c_str());
}

PyObject * audiofile_BytesPerSample(AudioFileObject *self,void *closure) {
	auto n=self->pcm->sampleSizeInBytes();
	return PyLong_FromUnsignedLong(n);
}

PyObject *audiofile_Repr(AudioFileObject *self) {
	std::stringstream s;
	s << kindOf(self);
	if(s.str().size()>0) s << " ";
	s << "(" << formatOf(self) << ")";
	return PyUnicode_FromFormat("%s",s.str().c_str());
}

