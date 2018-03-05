/*
 * Lame.c
 *
 *  Created on: 1 Dec 2017
 *      Author: julianporter
 */

#include "Lame.hpp"
#include "lib/WAVFile.hpp"
#include "lib/MP3Encoder.hpp"
#include "lib/Iterator32.hpp"
#include "lib/transcoder.hpp"
#include <sstream>
#include <regex>
#include <iostream>
#include <fstream>







//std::vector<std::string> kw {"","","bitrate","quality"};
//

std::vector<char *> keywordsFile{"","","bitrate","quality",nullptr};
std::vector<char *> keywordsStream{"","bitrate","quality",nullptr};


bool checkName(const char *name,const char *suffix) {
	std::stringstream s;
	s << ".*\\." << suffix << "$";
	return std::regex_match(name,std::regex(s.str()));
}

PyObject * mp3file(PyObject *self, PyObject *args, PyObject *keywds) {
	const char *inFile;
	const char *outFile;
	unsigned bitRate = 64;
	unsigned quality = 5;

	try {

		if(!PyArg_ParseTupleAndKeywords(args,keywds,"ss|$II",keywordsFile.data(),&inFile,&outFile,&bitRate,&quality)) {
			throw PException(PError::TypeError,"API is transcode(infile,outfile,bitrate=64,quality=5)");
		}
		try {
			std::ifstream wavFile(inFile,std::ifstream::binary);
			pylame::Transcode transcode(wavFile,quality,bitRate);
			std::ofstream out(outFile,std::ofstream::binary);
			out << transcode;
			out.close();
			return PyLong_FromUnsignedLong((unsigned long)transcode.size());
		}
		catch(std::exception &e) {
			throw PException(PError::MP3Error,e.what());
		}
	}
	catch(PException & p) {
		return p();
	}
}

PyObject * mp3stream(PyObject *self, PyObject *args, PyObject *keywds) {
	unsigned bitRate = 64;
	unsigned quality = 5;
	Py_buffer buffer;

	try {
		if(!PyArg_ParseTupleAndKeywords(args,keywds,"y*|$II",keywordsStream.data(),&buffer,&bitRate,&quality)) {
			throw PException(PError::TypeError,"API is transcode(stream,bitrate=64,quality=5)");
		}
		try {
			char *orig=(char *)buffer.buf;
			pylame::data_t data(orig,orig+buffer.len);
			pylame::Transcode transcode(data,quality,bitRate);
			auto out = Py_BuildValue("y#",transcode.ptr(),transcode.size());
			PyBuffer_Release(&buffer);
			return out;
		}
		catch(std::exception &e) {
			throw PException(PError::MP3Error,e.what());
		}
	}
	catch(PException &p) {
		return p();
	}
}

/**
	Now for the audio object
 */




