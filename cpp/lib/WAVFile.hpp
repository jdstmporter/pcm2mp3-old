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
friend std::ostream & operator << (std::ostream &o,const WAVFile &w);

private:
	data_t file;
	std::pair<long,long> clip();
	void parseHeader();
	
protected:

	
	DataFormat format;
	static DataFormat convertFormat(const uint16_t);
public:
	
	WAVFile(const data_t &file_);
	WAVFile(std::istream & stream);
	
	
	//WAVFile(const Mode & mode_,const SampleRate & rate_,int sampleSize) : PCMFile(mode_,rate_,sampleSize) {};
	virtual ~WAVFile() = default;

	
	virtual PCMData bytes(); // Gives interleaved data
	
	static bool isInstance(const data_t &d) {
		try {
			WAVFile w(d);
			return true;
		}
		catch(...) {
			return false;
		}
	};
	static bool isInstance(std::istream &stream) {
		try {
			WAVFile w(stream);
			return true;
		}
		catch(...) {
			return false;
		}
	};

};
}}

std::istream & operator >> (std::istream &i,pylame::pcm::WAVFile &w);

#endif /* DEBUG_WAVFILE_HPP_ */
