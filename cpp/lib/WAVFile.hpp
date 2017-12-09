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
#include <map>

namespace pylame { namespace pcm {

class WAVFile : public PCMFile {
friend std::ostream & operator << (std::ostream &o,const WAVFile &w);

private:
	data_t file;
	void parseHeader();
	std::map<PCMParameter,Parameter> parameters;
	
protected:

	
	DataFormat format;
	static DataFormat convertFormat(const uint16_t);
	virtual void set(const PCMParameter &name ,const uint32_t arg);
	virtual void set(const PCMParameter &name ,const long double arg);
	virtual void set(const PCMParameter &name ,const std::string & arg);

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
