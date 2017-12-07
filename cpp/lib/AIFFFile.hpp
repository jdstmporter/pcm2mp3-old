/*
 * AIFFFile.hpp
 *
 *  Created on: 5 Dec 2017
 *      Author: julianporter
 */

#ifndef AIFFFILE_HPP_
#define AIFFFILE_HPP_


#include "PCMFile.hpp"
#include "Iterator32.hpp"
#include "base.hpp"
#include "AIFFData.hpp"

namespace pylame { namespace pcm {






class AIFFFile: public PCMFile {

private:
	data_t file;
	aiff::Form form;
	Iterator32 iterator;
	unsigned offset;
	unsigned blocksize;
	
	void commChunk(const aiff::Chunk &f);
	void soundChunk(const aiff::Chunk &f);
	
	void parseHeader();

	
protected:
	
	
	
public:
	AIFFFile(const data_t &file_);
	AIFFFile(std::istream &);
	virtual ~AIFFFile() = default;

	virtual PCMData bytes();

	static bool isInstance(const data_t &d);
	static bool isInstance(std::istream &stream) ;
};

}}




#endif /* AIFFFILE_HPP_ */
