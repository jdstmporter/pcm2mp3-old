/*
 * MP3File.cpp
 *
 *  Created on: 2 May 2018
 *      Author: julianporter
 */

#include "MP3File.hpp"
#include "MP3Encoder.hpp"

namespace pylame {

void MP3File::transcode(pcm::PCMFile *pcm) {
	mp3::MP3Encoder trans(pcm,parameters);
	trans.transcode();
	out.assign(trans.cbegin(),trans.cend());
	duration=pcm->duration();
}

void MP3File::transcode(const pcm::file_t &pcm) {
	mp3::MP3Encoder trans(pcm,parameters);
	trans.transcode();
	out.assign(trans.cbegin(),trans.cend());
	duration=pcm->duration();
}





} /* namespace pylam */

std::ostream & operator<<(std::ostream &o,const pylame::MP3File &mp3) {
	return o.write(mp3.chars(),mp3.size());
}
