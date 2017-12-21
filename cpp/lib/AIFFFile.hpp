/*
 * AIFFFile.hpp
 *
 *  Created on: 5 Dec 2017
 *      Author: julianporter
 */

#ifndef AIFFFILE_HPP_
#define AIFFFILE_HPP_

#include "base.hpp"
#include "PCMFile.hpp"
#include "Iterator32.hpp"

namespace pylame { namespace pcm {






class AIFFFile: public PCMFile {
	using Iterator=Iterator32<Endianness::BigEndian>;
	using Form=DataForm<Endianness::BigEndian>;
	using Chunk=DataChunk<Endianness::BigEndian>;
private:
	unsigned offset;
	unsigned blocksize;
	Form form;
	
	void commChunk(const Chunk &f);
	void soundChunk(const Chunk &f);
	
	void parseForm();


	
public:
	AIFFFile(const data_t &file_);
	AIFFFile(std::istream &);
	virtual ~AIFFFile() = default;

	virtual PCMData bytes();
	virtual FileType fileType() const { return form.fileType(); };
	static bool isInstance(const data_t &d);
	static bool isInstance(std::istream &stream) ;
};

}}




#endif /* AIFFFILE_HPP_ */
