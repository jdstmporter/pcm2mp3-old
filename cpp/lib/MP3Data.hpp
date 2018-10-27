/*
 * MP3Data.hpp
 *
 *  Created on: 2 May 2018
 *      Author: julianporter
 */

#ifndef LIB_MP3DATA_HPP_
#define LIB_MP3DATA_HPP_

#include "base.hpp"
#include <lame/lame.h>

namespace pylame { namespace mp3 {

enum class ID3Tag {
	Title, Artist, Album, Year, Comment, Track, Genre
};
enum class ID3Versions : unsigned {
	OneOnly, TwoOnly, OneAndTwo
};

class MP3Parameters {
private:
	std::map<ID3Tag,std::string> id3;
	ID3Versions versions;
	int copyright;
	int original;

	unsigned quality;
	unsigned rate;

	bool has(const ID3Tag tag) const;

public:
	static const std::map<std::string,ID3Tag> id3names;

	MP3Parameters(const unsigned q=5,const unsigned r=64);
	MP3Parameters(const MP3Parameters &) = default;
	MP3Parameters & operator=(const MP3Parameters &) = default;
	virtual ~MP3Parameters() = default;

	std::string &operator[](const ID3Tag &tag) { return id3[tag]; };
	void isCopyright(const bool c)  { copyright=(c) ? 1 : 0; };
	void isOriginal(const bool c)  { original=(c) ? 1 : 0; };
	void useID3Version(const ID3Versions &id) { versions=id; };

	unsigned Quality() const { return quality; }
	unsigned Rate() const { return rate; }
	void setQuality(const unsigned q)  { quality=q; }
	void setRate(unsigned r)  { rate=r; }



	void write(lame_global_flags *gf) const;
};



}}




#endif /* LIB_MP3DATA_HPP_ */
