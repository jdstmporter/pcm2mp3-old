/*
 * AIFFFile.cpp
 *
 *  Created on: 5 Dec 2017
 *      Author: julianporter
 */

#include "AIFFFile.hpp"
#include "Conversions.hpp"
#include <cstdlib>
#include <locale>
using namespace pylame::pcm;

std::string AIFFFile::FormHeader() const { return "FORM"; };

FormMetaData::TypeMap AIFFFile::FormTypes() const {
		return {
			{"AIFF",FileType::AIFF},
			{"AIFC",FileType::AIFC}
		};
};



AIFFFile::AIFFFile(const data_t &file_) : PCMFile(file_) {
	Iterator32 it(file,Endianness::BigEndian);
	parse(it,"COMM","SSND");
}



AIFFFile::AIFFFile(std::istream &stream) : PCMFile(stream) {
	Iterator32 it(file,Endianness::BigEndian);
    parse(it,"COMM","SSND");
}


void AIFFFile::infoChunk(const std::shared_ptr<DataChunk> &comm) {
	
    auto itc=comm->iterator();
    
    auto p1=itc.nextPair();
    auto p2=itc.nextPair();
    nChannels=(unsigned)p1.first;
    nSamples=swap(p1.second,p2.first);
    bitsPerSample=(unsigned)p2.second;
    sampleRate=(unsigned)itc.nextLongDouble();
    if(isAIFC()) {
    	auto f=itc.convertNext();
    	char bytes[4];
    	std::locale l;
    	for(auto i=0;i<4;i++) bytes[i]=std::tolower(f.bytes[i],l);
    	std::string fmt(bytes,4);
    	if(fmt=="none" || fmt=="sowt") format=DataFormat::PCM;
    	else if(fmt=="fl32" || fmt=="fl64") format=DataFormat::IEEEFloat;
    	else if(fmt=="ulaw") format=DataFormat::ULaw;
    	else if(fmt=="alaw") format=DataFormat::ALaw;
    	else MP3Error("Unknown AIFC data format");
    }
    else format=DataFormat::PCM;
}

void AIFFFile::soundChunk(const std::shared_ptr<DataChunk> &ssnd) {
	 	dataSize=ssnd->size()-8;
	 	auto it=ssnd->iterator();
	 	offset=it.nextInt();
	 	blocksize=it.nextInt();
	 	if(it.size()!=dataSize) throw MP3Error("Data size error");
}

PCMData AIFFFile::bytes() {
	if(format!=DataFormat::PCM) throw MP3Error("Can only enumerate PCM files");
	if(offset!=0 || blocksize!=0) throw MP3Error("Cannot enumerate blocked data sets");
	auto qr=std::div(bitsPerSample,8);
	if(qr.rem!=0) throw MP3Error("Cannot enumerate non-integral byte samples");
	bytesPerSample=qr.quot;

	std::shared_ptr<DataChunk> ssnd=form["SSND"];
	auto it=ssnd->iterator();
	it.skip(2);
	return PCMData(nChannels,nSamples,it);
}


bool AIFFFile::isInstance(const data_t &d) {
			try {
				AIFFFile w(d);
				return true;
			}
			catch(...) {
				return false;
			}
		};
bool AIFFFile::isInstance(std::istream &stream) {
			try {
				AIFFFile w(stream);
				return true;
			}
			catch(std::exception &e) {
				std::cout << e.what() << std::endl;
				return false;
			}
		};


