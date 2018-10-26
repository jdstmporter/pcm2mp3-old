/*
 * MP3Frame.hpp
 *
 *  Created on: 23 Apr 2018
 *      Author: julianporter
 */

#ifndef LIB_MP3FRAME_HPP_
#define LIB_MP3FRAME_HPP_


#include "MP3.hpp"
#ifdef COMPUTE_CRC
#include "CRC16.hpp"
#endif


namespace mp3 {

class MP3ValidFrame {
private:
	bool initial;
	MP3Header header;

public:
	MP3ValidFrame() : initial(true), header() {};
	MP3ValidFrame(const MP3Header &h) : initial(false), header(h) {};
	virtual ~MP3ValidFrame() = default;

	bool operator()(const MP3Header &h) const {
		bool validID = h.id==0x7ff;
		bool validHeader = (h.version!=2) && (h.layer!=0);
		bool validRate = (h.rate!=0xf) && (h.frequency!=3);
		bool valid1=validID && validHeader && validRate;
		//bool valid1=((h.id&0xffe)==0xffe) && (h.layer!=0) && (h.rate!=0xf)
		//		&& (h.frequency!=3);
		bool valid2=(initial) ? true : (header.mode==h.mode) &&
				(header.modeExtension==h.modeExtension) &&
				(header.emphasis==h.emphasis) && (header.layer==h.layer);
		return valid1 && valid2;
	};
};


class MP3Frame {


private:

	MPEGVersion mpeg;
	MPEGLayer layer;
	MPEGMode mode;
	MP3 mp3;
	offset_t offset;

	unsigned bitRate;
	unsigned sampleRate;
	bool crc;
	MP3Header header;

	mdata_t data;
	offset_t start;



public:
	MP3Frame(const mdata_t &data_=mdata_t(),const offset_t start_=0) :
		mpeg(MPEGVersion::Unknown), layer(MPEGLayer::Unknown), mode(MPEGMode::Unknown),
		mp3(), offset(0), bitRate(0), sampleRate(0), crc(false), data(data_), start(start_) {};
	MP3Frame(const MP3Frame &)=default;
	MP3Frame & operator=(const MP3Frame &)=default;
	virtual ~MP3Frame() = default;

	offset_t match(const MP3ValidFrame &v);
	offset_t match() { return match(MP3ValidFrame()); };


	unsigned SampleRate() const { return sampleRate; };
	unsigned BitRate() const { return bitRate; };
	MPEGMode Mode() const { return mode; };
	MPEGLayer Layer() const { return layer; };
	MPEGVersion Version() const { return mpeg; };

	size_t size() const;
	double duration() const;
	bool isMono() const { return mode==MPEGMode::Mono; };
	bool isStereo() const { return mode==MPEGMode::Stereo || mode==MPEGMode::JointStereo; };
	bool isCopyright() const { return header.copyright!=0; };
	bool isOriginal() const { return header.original!=0; };

	offset_t fileOffset() const { return offset; }
	MP3Header Header() const { return header; }
	bool hasCRC() const { return crc; }

};



}

std::ostream & operator<<(std::ostream &o,const mp3::MP3Frame &f);



#endif /* LIB_MP3FRAME_HPP_ */
