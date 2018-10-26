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

	header=block.header;


	mpeg = static_cast<MPEGVersion>(header.version);
	layer=static_cast<MPEGLayer>(header.layer);
	mode=static_cast<MPEGMode>(header.mode);
	mp3=MP3(mpeg,layer);


	bitRate=mp3.rate(block.header.rate);
	sampleRate=mp3.frequency(block.header.frequency);
	crc=block.header.crc!=0;

#ifdef COMPUTE_CRC
	bool
	if(hasCRC) {
		CRCConverter c;
		if(it+2>end) std::runtime_error("Block claims to have CRC, but does not contain one");
		c.push(*(it++));
		c.push(*(it++));
		crc=CRC16(c.crc,it+size());
	}
	else {
		crc=CRC16();
	}
#endif
	offset=it-data.begin();
	return offset-MP3::FrameHeaderSize+size();
}


size_t MP3Frame::size() const {
	return (mp3.frameSizeIndex()*bitRate/sampleRate)+header.pad;
}



}



std::ostream & operator<<(std::ostream &o,const mp3::MP3Frame &f) {
	auto c=(f.hasCRC()) ? "Yes" : "No";
	o << std::oct << f.fileOffset() << std::dec << " "
			<< f.Header() << " : " << std::setw(8) <<  " Samples " << f.SampleRate()
			<< " Bits " << f.BitRate() << " " << "Has CRC: " << c << " "
			<< f.Version() << " " << f.Layer() << " " << f.Mode()
			<< " Length " << f.size();
	return o;
}


