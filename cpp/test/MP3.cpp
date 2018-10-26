/*
 * MP3.cpp
 *
 *  Created on: 23 Apr 2018
 *      Author: julianporter
 */

#include "MP3.hpp"


namespace mp3 {

bool MPEGSpecification::isGood() const {
	return (version!=MPEGVersion::Unknown) &&
			(layer!=MPEGLayer::Unknown) &&
			(mode!=MPEGMode::Unknown);
}

std::string MPEGSpecification::str(const unsigned n) const {
	std::stringstream s;
	switch(n) {
	case 0:
		s << version;
		break;
	case 1:
		s << layer;
		break;
	case 2:
		s << mode;
		break;
	default:
		break;
	}
	return s.str();
}

std::map<MPEGLayer,unsigned> MP3::sizeIndex;
std::map<MPEGVersion,std::vector<unsigned>> MP3::freqs;
std::map<std::pair<MPEGVersion,MPEGLayer>,std::vector<unsigned>> MP3::rates;

const offset_t MP3::FrameHeaderSize=4;
const size_t MP3::MinimumFrameSize=21;
bool MP3::verbose=false;

void MP3::init() {
	if(rates.empty()) {
		rates[std::make_pair(MPEGVersion::MPEG1 ,MPEGLayer::Layer1)]={0, 32, 64, 96,128,160,192,224,256,288,320,352,384,416,448,0};
		rates[std::make_pair(MPEGVersion::MPEG1 ,MPEGLayer::Layer2)]={0, 32, 48, 56, 64, 80, 96,112,128,160,192,224,256,320,384,0};
		rates[std::make_pair(MPEGVersion::MPEG1 ,MPEGLayer::Layer3)]={0, 32, 40, 48, 56, 64, 80, 96,112,128,160,192,224,256,320,0};
		rates[std::make_pair(MPEGVersion::MPEG2 ,MPEGLayer::Layer1)]={0, 32, 48, 56, 64, 80, 96,112,128,144,160,176,192,224,256,0};
		rates[std::make_pair(MPEGVersion::MPEG2 ,MPEGLayer::Layer2)]={0,  8, 16, 24, 32, 40, 48, 56, 64, 80, 96,112,128,144,160,0};
		rates[std::make_pair(MPEGVersion::MPEG2 ,MPEGLayer::Layer3)]={0,  8, 16, 24, 32, 40, 48, 56, 64, 80, 96,112,128,144,160,0};
		rates[std::make_pair(MPEGVersion::MPEG25,MPEGLayer::Layer1)]={0, 32, 48, 56, 64, 80, 96,112,128,144,160,176,192,224,256,0};
		rates[std::make_pair(MPEGVersion::MPEG25,MPEGLayer::Layer2)]={0,  8, 16, 24, 32, 40, 48, 56, 64, 80, 96,112,128,144,160,0};
		rates[std::make_pair(MPEGVersion::MPEG25,MPEGLayer::Layer3)]={0,  8, 16, 24, 32, 40, 48, 56, 64, 80, 96,112,128,144,160,0};

	}
	if(freqs.empty()) {
		freqs[MPEGVersion::MPEG1]  = { 44100, 48000, 32000, 50000 };
		freqs[MPEGVersion::MPEG2]  = { 22050, 24000, 16000, 50000 };
		freqs[MPEGVersion::MPEG25] = { 11025, 12000,  8000, 50000 };
	}
	if(sizeIndex.empty()) {
		sizeIndex[MPEGLayer::Layer1]=24000;
		sizeIndex[MPEGLayer::Layer2]=72000;
		sizeIndex[MPEGLayer::Layer3]=72000;
	}
}

MP3::MP3(const MPEGVersion &v,const MPEGLayer &l) : version(v), layer(l) {
	init();
}
unsigned MP3::rate(const unsigned f) const {
	try {
		return rates.at(std::make_pair(version,layer))[f];
	}
	catch(...) {
		return 0;
	}
}
unsigned MP3::frequency(const unsigned f) const {
	try {
		return freqs.at(version)[f];
	}
	catch(...) { return 0; }
}
size_t MP3::frameSizeIndex() const {
	try {
		auto i = sizeIndex.at(layer);
		return (version==MPEGVersion::MPEG1) ? 2*i : i;
	}
	catch(...) { return 0; }
}



}


std::ostream & operator<<(std::ostream &o,const mp3::MP3Header &header) {
	o << "HDR[" << std::hex << header.id << "] VERSION[" << std::dec << header.version << "] LAYER [" << header.layer;
	o << "] CRC[" << header.crc << "] RATE[" << header.rate << "] FREQ [" << header.frequency << "] PAD[" << header.pad;
	o << "] EXT[" << header.extension << "] MODE[" << header.mode << "] MODE_EXT[" << header.modeExtension;
	o << "] COPYRIGHT [" << header.copyright << "] ORIGINAL[" << header.original << "] EMPHASIS[" << header.emphasis << "]";
	return o;
}



std::ostream & operator<<(std::ostream &o,const mp3::MPEGLayer header) {
	int v=4-static_cast<unsigned>(header);
	if(v<0 || v>3) o << "Unknown";
	else o << "Layer" << v;
	return o;
}
static std::vector<std::string> modeNames={"stereo","dual channel","joint stereo","mono"};
std::ostream & operator<<(std::ostream &o,const mp3::MPEGMode mode) {
	try {
		o<<modeNames.at(static_cast<unsigned>(mode));
	}
	catch(...) {
		o<<"unknown";
	}
	return o;
}
std::ostream & operator<<(std::ostream &o,const mp3::MPEGVersion header) {
	switch(header) {
	case mp3::MPEGVersion::MPEG1:
		o << "MPEG1";
		break;
	case mp3::MPEGVersion::MPEG2:
		o << "MPEG2";
		break;
	case mp3::MPEGVersion::MPEG25:
		o << "MPEG2.5";
		break;
	default:
		o << "unknown";
		break;
	}
	return o;
}

std::ostream & operator<<(std::ostream &o,const mp3::MPEGSpecification &spec) {
	o << "(" << spec.version << ", " << spec.layer << ", " << spec.mode << ")";
	return o;
}


