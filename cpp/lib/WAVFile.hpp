/*
 * WAVFile.hpp
 *
 *  Created on: 30 Nov 2017
 *      Author: julianporter
 */

#ifndef DEBUG_WAVFILE_HPP_
#define DEBUG_WAVFILE_HPP_

#include "PCMFile.hpp"
#include "enums.hpp"
#include "Iterator32.hpp"



class WAVFile : public PCMFile {
friend std::ostream & operator << (std::ostream &o,const WAVFile &w);

private:
	data_t file;
	std::pair<long,long> clip();
	void parseHeader();
	
protected:
	unsigned nBytes;
	unsigned bitsPerSample;
	unsigned dataSize;
	
	DataFormat format;
	static DataFormat convertFormat(const uint16_t);
public:
	
	WAVFile(const data_t &file_);
	WAVFile(std::istream & stream);
	
	
	//WAVFile(const Mode & mode_,const SampleRate & rate_,int sampleSize) : PCMFile(mode_,rate_,sampleSize) {};
	virtual ~WAVFile() = default;

	
	virtual PCMFile::Data bytes(); // Gives interleaved data
	
	
};
std::istream & operator >> (std::istream &i,WAVFile &w);
std::ostream & operator << (std::ostream &o,const WAVFile &w);

#endif /* DEBUG_WAVFILE_HPP_ */
