/*
 * transcoder.hpp
 *
 *  Created on: 4 Dec 2017
 *      Author: julianporter
 */

#ifndef TRANSCODER_HPP_
#define TRANSCODER_HPP_

#include <iostream>

#include "base.hpp"
#include "MP3Encoder.hpp"
#include "WAVFile.hpp"

namespace pylame {

class Transcode {
private:
	pcm::WAVFile wav;
	mp3::MP3Encoder mp3;
	
public:
	Transcode(const pcm::WAVFile &w,const unsigned quality,const unsigned rate);
	Transcode(std::istream &in,const unsigned quality,const unsigned rate) : Transcode(pcm::WAVFile(in),quality,rate) {};
	Transcode(const data_t &in,const unsigned quality,const unsigned rate) : Transcode(pcm::WAVFile(in),quality,rate) {};
	
	cdata_t::const_iterator cbegin() const { return mp3.cbegin(); };
	cdata_t::const_iterator cend() const { return mp3.cend(); };  
	const unsigned char *ptr() { return mp3.ptr(); };
	unsigned size() const { return mp3.size(); };

	std::ostream & out(std::ostream &o) const;
};

}

std::ostream & operator<<(std::ostream &o,const pylame::Transcode &t);
std::istream & operator>>(std::istream &i,pylame::Transcode &t);

#endif /* TRANSCODER_HPP_ */
