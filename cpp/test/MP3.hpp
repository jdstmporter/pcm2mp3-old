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

class MP3CheckError : public std::exception {
private:
	std::string message;
public:
	MP3CheckError() noexcept = default;
	MP3CheckError(const std::string &m) noexcept : std::exception(), message(m) {};

	const char * what() const noexcept { return message.c_str(); };
};

enum class MPEGVersion : unsigned {
	MPEG25  = 0, // 0xb00,
	MPEG2   = 2, // 0xb01,
	MPEG1   = 3, // 0xb11,
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
	JointStereo=2,
	DualChannel=1,
	Mono=3,
	Unknown=255
};

enum class MPEGEmphasis : unsigned {
	None=0,
	MS50_15=2,
	CCITJ17=3
};

struct MP3Header {
	/*
	 * AAAAAAAA AAABBCCD EEEEFFGH IIJJKLMM
	 */

	unsigned emphasis : 2;			// M
	unsigned original : 1;			// L
	unsigned copyright : 1;			// K
	unsigned modeExtension : 2;		// J
	unsigned mode : 2;				// I
	unsigned extension : 1;			// H
	unsigned pad : 1;				// G
	unsigned frequency : 2;			// F
	unsigned rate : 4;				// E
	unsigned crc : 1;				// D
	unsigned layer : 2;				// C
	unsigned version : 2;			// B
	unsigned id : 11;				// A
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


union MP3HeaderConverter {
	uint32_t bytes;
	MP3Header header;
	uint8_t b[4];

	MP3HeaderConverter() : bytes(0) {};
	void push(const char c) {
		bytes=(bytes<<8)+(uint8_t)c;
	};
	MP3HeaderConverter(const MP3Header h) : header(h) {};
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
