/*
 * MP3Encoder.hpp
 *
 *  Created on: 30 Nov 2017
 *      Author: julianporter
 */

#ifndef MP3ENCODER_HPP_
#define MP3ENCODER_HPP_

#include <lame/lame.h>
#include <vector>
#include "PCMFile.hpp"
#include "enums.hpp"
#include <fstream>






class MP3Encoder {
friend std::ofstream & operator<<(std::ofstream &o,const MP3Encoder &e);
private:
	PCMFile *data;
	unsigned nSamples;
	unsigned mp3Size;
	unsigned char *mp3Out;
	lame_global_flags *gf; 
	
	static unsigned mp3SizeCalc(unsigned);

public:
	
	MP3Encoder(PCMFile *data_,const unsigned quality,const unsigned rate);
	virtual ~MP3Encoder();
	
	void transcode();
	unsigned size() { return mp3Size; };
};

std::ofstream & operator<<(std::ofstream &o,const MP3Encoder &e);



#endif /* MP3ENCODER_HPP_ */
