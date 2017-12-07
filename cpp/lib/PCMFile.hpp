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

namespace pylame { namespace pcm {



struct PCMData {
		unsigned nChannels;
		unsigned nSamples;
		std::shared_ptr<short> left;
		std::shared_ptr<short> right;

		PCMData(const unsigned nChannels_,const unsigned nSamples_,Iterator32 &it);
		virtual ~PCMData() = default;
	};

class PCMFile {
protected:
protected:
	enum class DataFormat : uint16_t {
		PCM = 1,
		IEEEFloat = 3,
		ALaw = 6,
		ULaw = 7
	};

	unsigned short nChannels;
	unsigned sampleRate;
	unsigned bytesPerSample;
	unsigned nSamples;	// per channel
	unsigned nBytesInFile;
	unsigned dataSize;
	unsigned bitsPerSample;

public:



	PCMFile() : nChannels(), sampleRate(), bytesPerSample(), nSamples(),nBytesInFile(), dataSize(), bitsPerSample() {};
	virtual ~PCMFile() = default;

	virtual int bitRate() { return nChannels*sampleRate*bytesPerSample*8; };
	MPEG_mode mp3Mode() { return nChannels==1 ? MONO : JOINT_STEREO; };
	virtual unsigned samplesPerSecond() const { return sampleRate; };
	
	virtual PCMData bytes() { throw MP3Error("Not implemented"); };
	virtual unsigned samplesPerChannel() const { return nSamples; };
	virtual unsigned short nChans() const { return nChannels; };
	virtual unsigned size() const { return nBytesInFile; };
	virtual unsigned dSize() const { return dataSize; };
	virtual unsigned sampleSize() const { return bitsPerSample; };
	virtual unsigned sampleSizeInBytes() const { return bytesPerSample; };



};

using file_t = std::shared_ptr<PCMFile>;

}}

std::ostream & operator<<(std::ostream &o,const pylame::pcm::PCMFile &f);





#endif /* PCMFILE_HPP_ */
