/*
 * transcoder.cpp
 *
 *  Created on: 4 Dec 2017
 *      Author: julianporter
 */

#include "transcoder.hpp"

Transcode::Transcode(const WAVFile &w,const unsigned quality,const unsigned rate) : wav(w), mp3(&wav,quality,rate) {
	mp3.transcode();
}



std::ostream & operator<<(std::ostream &o,const Transcode &t) {
	o << t.mp3;
	return o;
}

std::istream & operator>>(std::istream &i,Transcode &t) {
	t=Transcode(i,5,48);
	return i;
}