/*
 * AIFFFile.cpp
 *
 *  Created on: 5 Dec 2017
 *      Author: julianporter
 */

#include "AIFFFile.hpp"
#include <algorithm>
#include "Conversions.hpp"


std::ostream & operator<<(std::ostream &o,const Chunk &c) {
	o << "Chunk of type '" << c.ID << "' and length " << c.size();
	return o;
}




std::vector<Chunk> Form::operator[](const std::string &ID) {
	auto begin=chunks.lower_bound(ID);
	auto end=chunks.upper_bound(ID);
	if(begin==end) throw MP3Error("No instances of chunk kind found");
	std::vector<Chunk> c;
	for(auto ic=begin;ic!=end;ic++) c.push_back(ic->second);
	return c;
}

bool Form::nextChunk() {
	try {
		auto ID=it.nextString();
		auto n=swap32(it.nextInt());
		data_t d(n,0);
		it.getN(n,d.data());
		Chunk c(ID,d);
		chunks.insert(std::make_pair(ID,c));
		return true;
	}
	catch(...) {
		return false;
	}
	
}



void Form::walk() {
	auto s=it.nextString();
	if(s!="FORM") throw MP3Error("File is not AIFF - missing FORM");
	auto len=swap32(it.nextInt());
	auto t=it.nextString();
	std::cout << "Length = " <<  std::hex << len << std::dec << std::endl;
	std::cout << "Type string = '" << t << "'" << std::endl;
	if(t=="AIFF") fileType=Type::AIFF;
	else if(t=="AIFC") fileType=Type::AIFC;
	else throw MP3Error("Unrecognised file type");
	
	while(nextChunk()) {};
	std::for_each(chunks.begin(),chunks.end(),[](auto c) { std::cout << c.second << std::endl; });
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
    Iterator32 it(file);
    Form f(it);
    f.walk();
    
    auto common=f["COMM"];
    if(common.size()!=1) throw MP3Error("Anomalous AIFF file with multiple COMM chunks");
    auto comm=*common.begin();
    std::cout << "Processing COMM block" << std::endl;
    commChunk(comm);
    
    auto ssnds=f["SSND"];
    if(ssnds.size()!=1) throw MP3Error("Anomalous AIFF file with multiple SSND chunks");
	auto ssnd=*ssnds.begin();
	 std::cout << "PROCESSING SSND CHUNK" << std::endl;
    soundChunk(ssnd);
    
    
    
}

void AIFFFile::commChunk(const Chunk &comm) {
	

    auto itc=comm.iterator();
    
    auto p1=swap16(itc->nextPair());
    auto p2=swap16(itc->nextPair());
    nChannels=(unsigned)p1.first;
    nSamples=swap32(p1.second,p2.first);
    auto sSize=(unsigned)p2.second;
    
    bytesPerSample=(sSize+7)/8;
    sampleRate=(unsigned)itc->nextLongDouble();
    
    std::cout << "nChannels = " << nChannels << std::endl;
    std::cout << "nSamples = " << nSamples << std::endl;
    std::cout << "Size = " << sSize << std::endl;
    std::cout << "Bytes per sample = " << bytesPerSample << std::endl;
    std::cout << "Rate = " << sampleRate << std::endl;
    
    
}

void AIFFFile::soundChunk(const Chunk &ssnd) {
	 
	 	auto len=ssnd.size();
	 	iterator=ssnd.iterator();
	 	auto offset=swap32(iterator->nextInt());
	 	auto blockSize=swap32(iterator->nextInt());
	 	
	 	std::cout << "nBytes = " << len << std::endl;
    	std::cout << "offset = " << offset << std::endl;
    	std::cout << "blockSize = " << blockSize << std::endl;
    	std::cout << "iterator size = " << iterator->size() << std::endl;
}