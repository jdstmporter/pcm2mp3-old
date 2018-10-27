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

struct MP3FrameFailureMode {
public:
	bool id;
	bool version;
	bool layer;
	bool rate;
	bool freq;

	bool modeExt;
	bool emphasis;

	bool sequence;

	MP3FrameFailureMode() : id(false), version(false), layer(false), rate(false), freq(false),
			modeExt(false), emphasis(false), sequence(false) {};
	MP3FrameFailureMode(const MP3Header &);
	virtual ~MP3FrameFailureMode() = default;

	bool Fails() const;
	void setSequence() { sequence=true; }
	operator bool() const { return Fails(); }
};

class MP3ValidFrame {
public:
	bool initial;
	MP3Header header;

public:
	MP3ValidFrame() : initial(true), header() {};
	MP3ValidFrame(const MP3Header &h) : initial(false), header(h) {};
	virtual ~MP3ValidFrame() = default;

	MP3FrameFailureMode operator()(const MP3Header &h) const {
		MP3FrameFailureMode mode(h);
		if(!initial) {
			bool valid2=(header.mode==h.mode) &&
				(header.modeExtension==h.modeExtension) &&
				(header.emphasis==h.emphasis) && (header.layer==h.layer);
			if(!valid2) mode.setSequence();
		}
		return mode;
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
	bool zero;
	MP3Header header;
	MPEGSpecification spec;

	mdata_t data;
	offset_t start;



public:
	MP3Frame(const mdata_t &data_=mdata_t(),const offset_t start_=0) :
		mpeg(MPEGVersion::Unknown), layer(MPEGLayer::Unknown), mode(MPEGMode::Unknown),
		mp3(), offset(0), bitRate(0), sampleRate(0), crc(false), zero(false), data(data_), start(start_) {};
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
	bool hasModeExtension() const { return header.modeExtension; };
	bool isMono() const { return mode==MPEGMode::Mono; };
	bool isStereo() const { return mode==MPEGMode::Stereo || mode==MPEGMode::JointStereo; };
	bool isCopyright() const { return header.copyright!=0; };
	bool isOriginal() const { return header.original!=0; };

	offset_t address() const { return offset-4; }
	offset_t fileOffset() const { return offset; }
	MP3Header Header() const { return header; }
	MPEGSpecification Specification() const { return spec; }
	bool hasCRC() const { return crc; }
	bool allZero() const { return zero; }

};



}

std::ostream & operator<<(std::ostream &o,const mp3::MP3Frame &f);
std::ostream & operator<<(std::ostream &o,const mp3::MP3FrameFailureMode &m);


#endif /* LIB_MP3FRAME_HPP_ */
