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
public:

	struct Data {
		unsigned nChannels;
		unsigned nSamples;
		std::shared_ptr<short> left;
		std::shared_ptr<short> right;
		
		Data(const unsigned nChannels_,const unsigned nSamples_,Iterator32 &it);
	};

	PCMFile() : nChannels(), sampleRate(), bytesPerSample(), nSamples() {};
	virtual ~PCMFile() = default;

	virtual int bitRate() { return nChannels*sampleRate*bytesPerSample*8; };
	MPEG_mode mp3Mode() { return nChannels==1 ? MONO : JOINT_STEREO; };
	virtual unsigned samplesPerSecond() const { return sampleRate; };
	
	virtual Data bytes() { throw MP3Error("Not implemented"); };
	virtual unsigned samplesPerChannel() const { return nSamples; };
	virtual unsigned short nChans() const { return nChannels; };
	virtual unsigned size() const { throw MP3Error("Not implemented"); };
	virtual unsigned dSize() const { throw MP3Error("Not implemented"); };
	virtual unsigned sampleSize() const { return 8*bytesPerSample; };
	
};

}}



#endif /* PCMFILE_HPP_ */
