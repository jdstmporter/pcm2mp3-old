/*
 * PCMFile.hpp
 *
 *  Created on: 30 Nov 2017
 *      Author: julianporter
 */

#ifndef PCMFILE_HPP_
#define PCMFILE_HPP_

#include "Iterator32.hpp"
#include <lame/lame.h>
#include "base.hpp"
#include "property.hpp"



namespace pylame { namespace pcm {

struct PCMData {
		unsigned nChannels;
		unsigned nSamples;
		std::shared_ptr<short> left;
		std::shared_ptr<short> right;

		PCMData(const unsigned nChannels_,const unsigned nSamples_,Iterator32 &it);
		virtual ~PCMData() = default;
	};

class PCMFile : public ParameterSet {
protected:
	enum class DataFormat : uint16_t {
		PCM = 1,
		IEEEFloat = 3,
		ALaw = 6,
		ULaw = 7
	};
	unsigned fileSize;

public:
	PCMFile() :   fileSize() {};
	virtual ~PCMFile() = default;

	virtual unsigned bitRate() { return nChannels()*sampleRate()*bytesPerSample()*8; };
	MPEG_mode mp3Mode() { return nChannels()==1 ? MONO : JOINT_STEREO; };
	virtual unsigned sampleRate() const { return PARAM_GET(SampleRate,uint32_t); };
	
	virtual PCMData bytes() { throw MP3Error("Not implemented"); };
	virtual unsigned nSamples() const { return PARAM_GET(NumberOfSamples,uint32_t); };
	unsigned nChannels() const { return PARAM_GET(NumberOfChannels,uint32_t); };
	virtual unsigned size() const { return fileSize; };
	virtual unsigned dataSize() const { return PARAM_GET(DataSize,uint32_t); };
	virtual unsigned bitsPerSample() const { return PARAM_GET(BitsPerSample,uint32_t); };
	virtual unsigned bytesPerSample() const { return (bitsPerSample()+7)/8; };

	bool wholeBytes() const { return (bitsPerSample()%8)==0; };



};

using file_t = std::shared_ptr<PCMFile>;

}}

std::ostream & operator<<(std::ostream &o,const pylame::pcm::PCMFile &f);





#endif /* PCMFILE_HPP_ */
