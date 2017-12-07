/*
 * PCMFile.cpp
 *
 *  Created on: 30 Nov 2017
 *      Author: julianporter
 */

#include "PCMFile.hpp"

namespace pylame { namespace pcm {

PCMData::PCMData(const unsigned nChannels_,const unsigned nSamples_,Iterator32 &it) :
	nChannels(nChannels_), nSamples(nSamples_) {
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



}}

std::ostream & operator << (std::ostream &o,const pylame::pcm::PCMFile &w) {
	o << "Size             " << w.size() << std::endl;
	o << "N Channels       " << w.nChans()  << std::endl;
	o << "Sample rate      " << w.samplesPerSecond() << std::endl;
	o << "Bits per sample  " << w.sampleSize() << std::endl;
	o << "Bytes per sample " << w.sampleSizeInBytes() << std::endl;
	o << "Data size        " << w.dSize() << std::endl;
	o << "N Samples        " << w.samplesPerChannel() << std::endl;
	return o;
}
