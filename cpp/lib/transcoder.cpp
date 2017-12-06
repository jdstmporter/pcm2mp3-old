/*
 * transcoder.cpp
 *
 *  Created on: 4 Dec 2017
 *      Author: julianporter
 */

#include "transcoder.hpp"

namespace pylame {

Transcode::Transcode(const pcm::WAVFile &w,const unsigned quality,const unsigned rate) : wav(w), mp3(&wav,quality,rate) {
	mp3.transcode();
}

std::ostream & Transcode::out(std::ostream &o) const {
		o << mp3;
		return o;
	}

}

std::ostream & operator<<(std::ostream &o,const pylame::Transcode &t) {
	return t.out(o);
}

std::istream & operator>>(std::istream &i,pylame::Transcode &t) {
	t=pylame::Transcode(i,5,48);
	return i;
}
