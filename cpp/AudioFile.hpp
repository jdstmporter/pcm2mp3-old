/*
 * AudioFile.hpp
 *
 *  Created on: 3 Mar 2018
 *      Author: julianporter
 */

#ifndef AUDIOFILE_HPP_
#define AUDIOFILE_HPP_

#include "PyHeader.hpp"
#include "lib/PCMFile.hpp"

typedef struct {
	PyObject_HEAD
	pylame::pcm::PCMFile *pcm;
} AudioFileObject;


 void audiofile_Dealloc(AudioFileObject *self);
 PyObject * audiofile_New(PyTypeObject *type, PyObject *args, PyObject *kwds);
 int audiofile_Init(AudioFileObject *self,PyObject *args, PyObject *kwds);
 PyObject * audiofile_Kind(AudioFileObject *self,void *closures);
 PyObject * audiofile_Format(AudioFileObject *self,void *closure);
 PyObject * audiofile_BytesPerSample(AudioFileObject *self,void *closure);
 PyObject *audiofile_Repr(AudioFileObject *self);


#endif /* AUDIOFILE_HPP_ */
