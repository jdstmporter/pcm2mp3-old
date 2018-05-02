/*
 * MP3File.hpp
 *
 *  Created on: 2 May 2018
 *      Author: julianporter
 */

#ifndef MP3FILE_HPP_
#define MP3FILE_HPP_

#include "base.hpp"
#include "MP3Data.hpp"
#include "PCMFile.hpp"

namespace pylame {

class MP3File {
private:
	mp3::MP3Parameters parameters;
	cdata_t out;
	double duration;

public:
	MP3File(const unsigned quality,const unsigned rate) : parameters(quality,rate), out(), duration(0.0) {};
	MP3File(const mp3::MP3Parameters &p) : parameters(p), out(), duration(0.0) {};
	virtual ~MP3File() = default;

	void transcode(pcm::PCMFile *pcm);
	cdata_t::const_iterator cbegin() const { return out.cbegin(); };
	cdata_t::const_iterator cend() const { return out.cend(); };
	double getDuration() const { return duration; };
	unsigned getRate() const { return parameters.Rate(); };
	unsigned size() const { return (unsigned)out.size(); };
	const unsigned char * bytes() const { return out.data(); };
};

} /* namespace pylam */



#endif /* MP3FILE_HPP_ */