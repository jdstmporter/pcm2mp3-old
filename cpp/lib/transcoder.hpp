/*
 * transcoder.hpp
 *
 *  Created on: 4 Dec 2017
 *      Author: julianporter
 */

#ifndef TRANSCODER_HPP_
#define TRANSCODER_HPP_

#include "enums.hpp"
#include <iostream>
#include "MP3Encoder.hpp"
#include "WAVFile.hpp"

class Transcode {
friend std::ostream & operator<<(std::ostream &o,const Transcode &t);
friend std::istream & operator>>(std::istream &i,const Transcode &t);
private:
	WAVFile wav;
	MP3Encoder mp3;
	
public:
	Transcode(const WAVFile &w,const unsigned quality,const unsigned rate);
	Transcode(std::istream &in,const unsigned quality,const unsigned rate) : Transcode(WAVFile(in),quality,rate) {};
	Transcode(const data_t &in,const unsigned quality,const unsigned rate) : Transcode(WAVFile(in),quality,rate) {};
	
	cdata_t::const_iterator cbegin() const { return mp3.cbegin(); };
	cdata_t::const_iterator cend() const { return mp3.cend(); };  
	char *ptr() { return mp3.ptr(); };
	unsigned size() const { return mp3.size(); };
};

std::ostream & operator<<(std::ostream &o,const Transcode &t);
std::istream & operator>>(std::istream &i,Transcode &t);

#endif /* TRANSCODER_HPP_ */
