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

} /* namespace pylam */
