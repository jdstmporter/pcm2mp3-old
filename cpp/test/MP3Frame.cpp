/*
 * MP3Frame.cpp
 *
 *  Created on: 23 Apr 2018
 *      Author: julianporter
 */
#include "MP3Frame.hpp"

#include <iomanip>

namespace mp3 {

size_t frame_size_index[] = {24000, 72000, 72000};

union MP3HeaderConverter {
	uint32_t bytes;
	MP3Header header;
	uint8_t b[4];

	MP3HeaderConverter() : bytes(0) {};
	void push(const char c) {
		bytes=(bytes<<8)+(uint8_t)c;
	};
};





offset_t MP3Frame::match(const MP3ValidFrame &v)  {

	auto it=data.begin()+start;
	auto end=data.end();

	MP3HeaderConverter block;
	block.bytes=0;
	while ((it!=end) && !v(block.header) ) block.push(*(it++));

	if(it==end) throw std::runtime_error("No frame header found");
	offset=it-data.begin();
	header=block.header;


	mpeg = static_cast<MPEGVersion>(header.version);
	layer=static_cast<MPEGLayer>(header.layer);
	mode=static_cast<MPEGMode>(header.mode);
	mp3=MP3(mpeg,layer);


	bitRate=mp3.rate(block.header.rate);
	sampleRate=mp3.frequency(block.header.frequency);
	return offset-MP3::FrameHeaderSize+size();
}


size_t MP3Frame::size() const {
	return (mp3.frameSizeIndex()*bitRate/sampleRate)+header.pad;
}



}

std::ostream & operator<<(std::ostream &o,const mp3::MP3Frame &f) {
	o << std::oct << f.fileOffset() << std::dec << " ";
	o << f.Header() << " : " << std::setw(8) <<  " Samples " << f.SampleRate();
	o << " Bits " << f.BitRate() << " " << f.Version() << " " << f.Layer() << " " << f.Mode() << " Length " << f.size();
	return o;
}


