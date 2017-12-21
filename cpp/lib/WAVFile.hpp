/*
 * WAVFile.hpp
 *
 *  Created on: 30 Nov 2017
 *      Author: julianporter
 */

#ifndef DEBUG_WAVFILE_HPP_
#define DEBUG_WAVFILE_HPP_

#include "base.hpp"
#include "PCMFile.hpp"
#include "Iterator32.hpp"

namespace pylame { namespace pcm {

class WAVFile : public PCMFile {
	using Iterator=Iterator32<Endianness::LittleEndian>;
	using Form=DataForm<Endianness::LittleEndian>;
	using Chunk=DataChunk<Endianness::LittleEndian>;
friend std::ostream & operator << (std::ostream &o,const WAVFile &w);

private:
	Form form;
	std::pair<long,long> clip();
	void parseForm();
	void fmtChunk(const Chunk &chunk);
	void dataChunk(const Chunk &chunk);
	
protected:
	static DataFormat convertFormat(const uint16_t);
public:
	
	WAVFile(const data_t &file_);
	WAVFile(std::istream & stream);
	virtual ~WAVFile() = default;

	
	virtual PCMData bytes(); // Gives interleaved data
	virtual FileType fileType() const { return form.fileType(); };
	static bool isInstance(const data_t &d);
	static bool isInstance(std::istream &stream);

};
}}

std::istream & operator >> (std::istream &i,pylame::pcm::WAVFile &w);

#endif /* DEBUG_WAVFILE_HPP_ */
