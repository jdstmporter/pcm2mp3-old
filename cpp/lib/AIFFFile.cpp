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
    //nBytesInFile=form.fileSize();
    
}





PCMData AIFFFile::bytes() {
	if(offset!=0 || blocksize!=0) throw MP3Error("Cannot enumerate blocked data sets");
	if(!wholeBytes()) throw MP3Error("Cannot enumerate non-integral byte samples");

	aiff::Chunk ssnd=form.first(aiff::ChunkKind::SSND);
	auto it=ssnd.iterator();
	it.skip(2);
	return PCMData(nChannels(),nSamples(),it);
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


