/*
 * PCMFile.hpp
 *
 *  Created on: 30 Nov 2017
 *      Author: julianporter
 */

#ifndef PCMFILE_HPP_
#define PCMFILE_HPP_

#include "DataChunk.hpp"
#include "Iterator32.hpp"
#include <lame/lame.h>
#include "base.hpp"

namespace pylame { namespace pcm {


struct PCMData {
		unsigned nChannels;
		unsigned nSamples;
		std::shared_ptr<short> left;
		std::shared_ptr<short> right;

		PCMData() : nChannels(), nSamples(), left(), right() {};

		template <Endianness E>
		void load(const unsigned nChannels_,const unsigned nSamples_,Iterator32<E> &it) {
			nChannels=nChannels_;
			nSamples=nSamples_;
			if(nChannels<1 || nChannels>2) throw MP3Error("Invalid number of channels");

			short *lBuffer=new short[nSamples];
			short *rBuffer=new short[nSamples];

			unsigned index=0;
			if(nChannels==1) {
				while(!it.finished()) {
					auto d=it.nextPair();
					lBuffer[index]=(short)d.first;
					rBuffer[index]=0;
					index++;
					rBuffer[index]=(short)d.second;
					rBuffer[index]=0;
					index++;
				}
			} else { 	/// Stereo
				while(!it.finished()) {
					auto d=it.nextPair();
					lBuffer[index]=(short)d.first;
					rBuffer[index]=(short)d.second;
					index++;
				}
			}
			left=std::shared_ptr<short>(lBuffer);
			right=std::shared_ptr<short>(rBuffer);
		}
		virtual ~PCMData() = default;

		template<Endianness E>
		static PCMData make(const unsigned nChannels,const unsigned nSamples,Iterator32<E> &it) {
			PCMData p;
			p.load(nChannels,nSamples,it);
			return p;
		}
	};

class PCMFile {

protected:


	unsigned short nChannels=0;
	unsigned sampleRate=0;
	unsigned bytesPerSample=0;
	unsigned nSamples=0;	// per channel
	unsigned nBytesInFile=0;
	unsigned dataSize=0;
	unsigned bitsPerSample=0;
	DataFormat format;
	data_t file;




public:

	PCMFile() : format(DataFormat::PCM), file()  {};
	PCMFile(const data_t &file_) : format(DataFormat::PCM), file(file_)  {
	};
	PCMFile(std::istream &stream) : format(DataFormat::PCM), file() {
		stream.seekg (0, stream.end);
	    auto length = stream.tellg();
	    stream.seekg (0, stream.beg);

	    file.assign(length,0);
	    auto c=file.data();
	    int pos=0;
	    while(pos<length) {
	    	stream.read(c+pos,1024);
	    	pos+=stream.gcount();
	    }

	};
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
	virtual FileType fileType() const { throw MP3Error("Not implemented"); };



};

using file_t = std::shared_ptr<PCMFile>;


}}


std::ostream & operator<<(std::ostream &o,const pylame::pcm::PCMFile &w);





#endif /* PCMFILE_HPP_ */
