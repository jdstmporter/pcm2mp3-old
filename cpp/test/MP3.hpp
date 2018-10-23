/*
 * MP3.hpp
 *
 *  Created on: 23 Apr 2018
 *      Author: julianporter
 */

#ifndef LIB_MP3_HPP_
#define LIB_MP3_HPP_

#include <cstdint>
#include <string>
#include <exception>
#include <stdexcept>
#include <iostream>
#include <vector>
#include <map>
#include <memory>
#include <algorithm>
#include <sstream>
#include <array>

namespace mp3 {

enum class MPEGVersion : unsigned {
	MPEG2 = 0,
	MPEG1 = 1,
	Unknown = 255
};

enum class MPEGLayer : unsigned {
	Unknown=0,
	Layer3=1,
	Layer2=2,
	Layer1=3
};

enum class MPEGMode : unsigned {
	Stereo=0,
	JointStereo=1,
	DualChannel=2,
	Mono=3,
	Unknown=255
};

struct MP3Header {
	unsigned emphasis : 2;
	unsigned original : 1;
	unsigned copyright : 1;
	unsigned modeExtension : 2;
	unsigned mode : 2;
	unsigned extension : 1;
	unsigned pad : 1;
	unsigned frequency : 2;
	unsigned rate : 4;
	unsigned crc : 1;
	unsigned layer : 2;
	unsigned version : 1;
	unsigned id : 12;
};

using mdata_t = std::vector<char>;
using size_t = unsigned long;
using offset_t = unsigned long;

struct MPEGSpecification {
	MPEGVersion version;
	MPEGLayer layer;
	MPEGMode mode;

	MPEGSpecification() : version(MPEGVersion::Unknown), layer(MPEGLayer::Unknown), mode(MPEGMode::Unknown) {};
	MPEGSpecification(const MPEGVersion &v,const MPEGLayer &l,const MPEGMode &m) : version(v), layer(l), mode(m) {};
	MPEGSpecification(const MPEGSpecification &) = default;
	MPEGSpecification &operator=(const MPEGSpecification &) = default;
	virtual ~MPEGSpecification() = default;

	bool isGood() const;
	std::string str(const unsigned) const;
};




class MP3 {
	static std::map<MPEGLayer,unsigned> sizeIndex;
	static std::map<MPEGVersion,std::vector<unsigned>> freqs;
	static std::map<std::pair<MPEGVersion,MPEGLayer>,std::vector<unsigned>> rates;
	static void init();
private:
	MPEGVersion version;
	MPEGLayer layer;
public:
	static const offset_t FrameHeaderSize;
	static const size_t MinimumFrameSize;
	static bool verbose;

	MP3(const MPEGVersion &v = MPEGVersion::Unknown,const MPEGLayer &l = MPEGLayer::Unknown);
	virtual ~MP3() = default;

	unsigned frequency(const unsigned f) const;
	unsigned rate(const unsigned f) const;
	size_t frameSizeIndex() const;
};


}


std::ostream & operator<<(std::ostream &o,const mp3::MPEGVersion header);
std::ostream & operator<<(std::ostream &o,const mp3::MPEGLayer header);
std::ostream & operator<<(std::ostream &o,const mp3::MPEGMode mode);
std::ostream & operator<<(std::ostream &o,const mp3::MP3Header &header);
std::ostream & operator<<(std::ostream &o,const mp3::MPEGSpecification &spec);

#endif /* LIB_MP3_HPP_ */
