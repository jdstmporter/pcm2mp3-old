/*
 * MP3Encoder.hpp
 *
 *  Created on: 30 Nov 2017
 *      Author: julianporter
 */

#ifndef MP3ENCODER_HPP_
#define MP3ENCODER_HPP_

#include "PCMFile.hpp"
#include <lame/lame.h>
#include "base.hpp"





namespace pylame { namespace mp3 {

enum class ID3Tag {
	Title, Artist, Album, Year, Comment, Track, Genre
};
enum class ID3Versions {
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

public:
	static const std::map<std::string,ID3Tag> id3names;

	MP3Parameters(const unsigned q=5,const unsigned r=64) : id3(), versions(ID3Versions::OneAndTwo), copyright(false), original(false), quality(q), rate(r) {};
	MP3Parameters(const MP3Parameters &) = default;
	MP3Parameters & operator=(const MP3Parameters &) = default;
	virtual ~MP3Parameters() = default;

	std::string &operator[](const ID3Tag &tag) { return id3[tag]; };
	void isCopyright(const bool c) const { copyright=c ? 1 : 0; };
	void isOriginal(const bool c) const { original=c ? 1 : 0; };
	void useID3Version(const ID3Versions &id) { versions=id; };

	unsigned Quality() const { return quality; }
	unsigned Rate() const { return rate; }
	void setQuality(const unsigned q) const { quality=q; }
	void setRate(unsigned r) const { rate=r; }

	void write(lame_global_flags *gf) const;
};


class MP3Encoder {
private:
	pcm::file_t data;
	unsigned nSamples;
	unsigned mp3Size;
	unsigned char *mp3Out;
	lame_global_flags *gf; 
	cdata_t output;
	
	static unsigned mp3SizeCalc(unsigned);

public:
	MP3Encoder() : data(nullptr), nSamples(0), mp3Size(0), mp3Out(nullptr), gf(nullptr), output() {};
	MP3Encoder(const pcm::file_t &data_,const unsigned quality,const unsigned rate);
	MP3Encoder(const pcm::file_t &data_,const MP3Parameters &parameters);
	virtual ~MP3Encoder();
	
	void transcode();
	unsigned size() { return mp3Size; };

	cdata_t::const_iterator cbegin() const { return output.cbegin(); };
	cdata_t::const_iterator cend() const { return output.cend(); };
	unsigned size() const { return mp3Size; };
	const unsigned char *ptr() const { return output.data(); };
	cdata_t & operator()() { return output; };

};

}}

std::ostream & operator<<(std::ostream &o,const pylame::mp3::MP3Encoder &e);



#endif /* MP3ENCODER_HPP_ */
