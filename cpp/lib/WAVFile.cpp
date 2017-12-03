/*
 * WAVFile.cpp
 *
 *  Created on: 30 Nov 2017
 *      Author: julianporter
 */

#include "WAVFile.hpp"

#include <stdexcept>






WAVFile::DataFormat WAVFile::convertFormat(const uint16_t value) {
	switch(value) {
		case 1:
		return DataFormat::PCM;
		case 3:
		return DataFormat::IEEEFloat;
		case 6:
		return DataFormat::ALaw;
		case 7:
		return DataFormat::ULaw;
		default:
		throw MP3Error("Unknown WAV data format");
	}	
};

void WAVFile::parseHeader() {
	Iterator32 it(file);
	auto riff=it.nextString();
	if (riff!="RIFF") throw MP3Error("RIFF tag not present");
	nBytes=it.nextInt();
	auto wave=it.nextString();
	if (wave!="WAVE") throw MP3Error("WAVE tag not present");
	auto fmt=it.nextString();
	if (fmt!="fmt ") throw MP3Error("FMT tag not present");
	auto formatLength=it.nextInt();
	if (formatLength!=16) throw MP3Error("Format block length wrong");
	
	
	auto fmtCh=it.nextPair();
	format=WAVFile::convertFormat(fmtCh.first);
	nChannels=fmtCh.second;
	
	sampleRate=it.nextInt();
	auto byteRate=it.nextInt();
	auto alignBits=it.nextPair();
	bitsPerSample=alignBits.second;
	if((bitsPerSample&7) != 0) throw MP3Error("Bad bits per sample");
	bytesPerSample=bitsPerSample/8;
	if(alignBits.first!= bytesPerSample*nChannels) throw MP3Error("Block align check failed");
	if(byteRate != sampleRate*bytesPerSample*nChannels) throw MP3Error("Byte rate check failed");
	
	auto d=it.nextString();
	if (d!="data") throw MP3Error("DATA tag not present");
	dataSize=it.nextInt();
	nSamples=dataSize/(nChannels*bytesPerSample);
	
}

WAVFile::WAVFile(const Iterator32::data_t &file_) : PCMFile(), file(file_) {
	parseHeader();
}

WAVFile::WAVFile(std::ifstream &stream) {
	stream.seekg (0, stream.end);
    auto length = stream.tellg();
    stream.seekg (0, stream.beg);
    
    file=Iterator32::data_t(length,0);
    auto c=file.data();
    int pos=0;
    while(pos<length) {
    	stream.read(c+pos,1024);
    	pos+=stream.gcount();
    }
    stream.close();
    
    parseHeader();
}

std::pair<long,long> WAVFile::clip() {
	long b=1<<(8*bytesPerSample);
	return std::make_pair<long,long>(-b,b-1);
}


PCMFile::Data WAVFile::bytes() {
	if(format!=DataFormat::PCM) throw MP3Error("Can only enumerate PCM files");
	
	Iterator32 it(file);
	it.skip(11);	// skip over header
	
	return PCMFile::Data(nChannels,nSamples,it);
}

std::ifstream & operator >> (std::ifstream &i,WAVFile &w) {
	w=WAVFile(i);
	return i;
}

std::ostream & operator << (std::ostream &o,const WAVFile &w) {
	o << "Size            " << w.nBytes << std::endl;
	o << "N Channels      " << w.nChannels  << std::endl;
	o << "Sample rate     " << w.sampleRate << std::endl;
	o << "Bits per sample " << w.bitsPerSample << std::endl;
	o << "Data size       " << w.dataSize << std::endl;
	o << "N Samples       " << w.nSamples << std::endl;
	return o;
}
