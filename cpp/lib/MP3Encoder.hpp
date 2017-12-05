/*
 * MP3Encoder.hpp
 *
 *  Created on: 30 Nov 2017
 *      Author: julianporter
 */

#ifndef MP3ENCODER_HPP_
#define MP3ENCODER_HPP_

#include "enums.hpp"
#include "PCMFile.hpp"
#include <lame/lame.h>








class MP3Encoder {
friend std::ostream & operator<<(std::ostream &o,const MP3Encoder &e);
private:
	PCMFile * data;
	unsigned nSamples;
	unsigned mp3Size;
	unsigned char *mp3Out;
	lame_global_flags *gf; 
	cdata_t output;
	
	static unsigned mp3SizeCalc(unsigned);

public:
	MP3Encoder() : data(nullptr), nSamples(0), mp3Size(0), mp3Out(nullptr), gf(nullptr), output() {};
	MP3Encoder(PCMFile *data_,const unsigned quality,const unsigned rate);
	virtual ~MP3Encoder();
	
	void transcode();
	unsigned size() { return mp3Size; };

	cdata_t::const_iterator cbegin() const { return output.cbegin(); };
	cdata_t::const_iterator cend() const { return output.cend(); };
	unsigned size() const { return mp3Size; };
	char *ptr() { return (char *)output.data(); };
	cdata_t & operator()() { return output; };

};

std::ostream & operator<<(std::ostream &o,const MP3Encoder &e);



#endif /* MP3ENCODER_HPP_ */
