/*
 * WAVFile.cpp
 *
 *  Created on: 30 Nov 2017
 *      Author: julianporter
 */

#include "WAVFile.hpp"



namespace pylame { namespace pcm {


DataFormat WAVFile::convertFormat(const uint16_t value) {
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
			throw MP3Error("Unknown WAV data format : ",value);
	}	
};

std::string WAVFile::FormHeader() const { return "RIFF"; };

FormMetaData::TypeMap WAVFile::FormTypes() const {
	return {{"WAVE",FileType::WAV}};
};


void WAVFile::infoChunk(const std::shared_ptr<DataChunk> &chunk) {
	if(chunk->size()!=16) throw MP3Error("Format block length wrong");
	auto itc=chunk->iterator();



	auto fmtCh=itc.nextPair();
	format=convertFormat(fmtCh.first);
	nChannels=fmtCh.second;

	sampleRate=itc.nextInt();
	auto byteRate=itc.nextInt();

	auto alignBits=itc.nextPair();
	bitsPerSample=alignBits.second;
	if((bitsPerSample&7) != 0) throw MP3Error("Bad bits per sample: ",bitsPerSample);

	//std::cout << "nCh=" << nChannels << " rate=" << byteRate << " sRate=" << sampleRate << " BPS=" << bitsPerSample << std::endl;
	bytesPerSample=bitsPerSample/8;
	if(alignBits.first!= bytesPerSample*nChannels) throw MP3Error("Block align check failed");
	if(byteRate != sampleRate*bytesPerSample*nChannels) throw MP3Error("Byte rate check failed");
};

void WAVFile::soundChunk(const std::shared_ptr<DataChunk> &chunk) {
	dataSize=chunk->size();
	nSamples=dataSize/(nChannels*bytesPerSample);
}



WAVFile::WAVFile(const data_t &file_) : PCMFile(file_) {
	Iterator32 it(file,Endianness::LittleEndian);
	parse(it,"FMT ","DATA");
}

WAVFile::WAVFile(std::istream &stream) : PCMFile(stream) {
	Iterator32 it(file,Endianness::LittleEndian);
	parse(it,"FMT ","DATA");
}

std::pair<long,long> WAVFile::clip() {
	long b=1<<(8*bytesPerSample);
	return std::make_pair<long,long>(-b,b-1);
}


PCMData WAVFile::bytes() {
	if(format!=DataFormat::PCM) throw MP3Error("Can only enumerate PCM files");
	
	auto data=form["DATA"];
	auto it=data->iterator();
	return PCMData(nChannels,nSamples,it);
}

bool WAVFile::isInstance(const data_t &d) {
		try {
			WAVFile w(d);
			return true;
		}
		catch(...) {
			return false;
		}
	};
bool WAVFile::isInstance(std::istream &stream) {
		try {
			WAVFile w(stream);
			return true;
		}
		catch(...) {
			return false;
		}
	};

}}

std::istream & operator >> (std::istream &i,pylame::pcm::WAVFile &w) {
	w=pylame::pcm::WAVFile(i);
	return i;
}


