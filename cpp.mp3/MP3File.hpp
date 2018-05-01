/*
 * MP3File.hpp
 *
 *  Created on: 22 Apr 2018
 *      Author: julianporter
 */

#ifndef LIB_MP3FILE_HPP_
#define LIB_MP3FILE_HPP_

#include "MP3.hpp"
#include "MP3Frame.hpp"
#include <unordered_set>


namespace mp3 {



class MP3File {
public:
	using frameset_t = std::vector<MP3Frame>;
	using iterator = std::vector<MP3Frame>::const_iterator;


	static mdata_t readBinaryFile(std::istream &stream);
private:
	mdata_t mp3;
	frameset_t frames;
	MP3Frame initial;
	offset_t offset;
	std::unordered_set<unsigned> rates;

public:
	MP3File() : mp3(), frames(), initial(), offset(0), rates() {};
	MP3File(std::istream &stream) ;
	virtual ~MP3File() = default;

	void parse();
	size_t size() const;
	double duration() const;

	iterator begin() const { return frames.cbegin(); };
	iterator end() const { return frames.cend(); };
	bool isCBR() const { return rates.size()==1; };
	bool isVBR() const { return rates.size()>1; };
	unsigned bitRate() const;
};

} /* namespace mp3 */


std::ostream & operator<<(std::ostream &o,const mp3::MP3File &f);

#endif /* LIB_MP3FILE_HPP_ */
