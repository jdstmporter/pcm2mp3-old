/*
 * AIFFFile.cpp
 *
 *  Created on: 5 Dec 2017
 *      Author: julianporter
 */

#include "AIFFFile.hpp"
#include <algorithm>
#include "Conversions.hpp"
#include <cstdlib>
#include <locale>
using namespace pylame::pcm;





AIFFFile::AIFFFile(const data_t &file_) : PCMFile(), file(file_) {
	parseHeader();
}



AIFFFile::AIFFFile(std::istream &stream) : file(), iterator() {
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
    parseHeader();

}

void AIFFFile::parseHeader() {
    Iterator32 it(file,Iterator32::Endianness::BigEndian);
    form=aiff::Form(it);
    form.walk();
    nBytesInFile=form.bytesInFile();
    
    auto common=form["COMM"];
    if(common.size()!=1) throw MP3Error("Anomalous AIFF file with multiple COMM chunks");
    auto comm=*common.begin();
    std::cout << "Processing COMM block" << std::endl;
    commChunk(comm);
    
    auto ssnds=form["SSND"];
    if(ssnds.size()!=1) throw MP3Error("Anomalous AIFF file with multiple SSND chunks");
	auto ssnd=*ssnds.begin();
	 std::cout << "PROCESSING SSND CHUNK" << std::endl;
    soundChunk(ssnd);
}

void AIFFFile::commChunk(const aiff::Chunk &comm) {
	

    auto itc=comm.iterator();
    
    auto p1=itc.nextPair();
    auto p2=itc.nextPair();
    nChannels=(unsigned)p1.first;
    nSamples=swap(p1.second,p2.first);
    bitsPerSample=(unsigned)p2.second;
    sampleRate=(unsigned)itc.nextLongDouble();
    if(form.isAIFC()) {
    	auto f=itc.convertNext();
    	char bytes[4];
    	std::locale l;
    	for(auto i=0;i<4;i++) bytes[i]=std::tolower(f.bytes[i],l);
    	std::string fmt(bytes);
    	if(fmt=="none" || fmt=="sowt") format==DataFormat::PCM;
    	else if(fmt=="fl32" || fmt=="fl64") format=DataFormat::IEEEFloat;
    	else if(fmt=="ulaw") format==DataFormat::ULaw;
    	else if(fmt=="alaw") format==DataFormat::ALaw;
    	else MP3Error("Unknown AIFC data format");
    }
    else format==DataFormat::PCM;
}

void AIFFFile::soundChunk(const aiff::Chunk &ssnd) {
	 
	 	dataSize=ssnd.size()-8;
	 	auto it=ssnd.iterator();
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

	aiff::Chunk ssnd=*form["SSND"].begin();
	auto it=ssnd.iterator();
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


