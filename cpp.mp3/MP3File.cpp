/*
 * MP3File.cpp
 *
 *  Created on: 22 Apr 2018
 *      Author: julianporter
 */

#include "MP3File.hpp"
#include <numeric>




namespace mp3 {



std::vector<char> MP3File::readBinaryFile(std::istream &stream) {
	stream.seekg (0, stream.end);
	auto length = stream.tellg();
	stream.seekg (0, stream.beg);

	std::vector<char> file(length,0);
	auto c=file.data();
	int pos=0;
	while(pos<length) {
	   stream.read(c+pos,1024);
	   pos+=stream.gcount();
	}
	return file;
}

MP3File::MP3File(std::istream &stream) : mp3(readBinaryFile(stream)), frames(), initial(mp3,0) {
	offset=initial.match();
	frames.push_back(initial);
}


void MP3File::parse() {
	MP3Header head=initial.Header();
	MP3ValidFrame validator(head);

	while(offset<mp3.size()-MP3::MinimumFrameSize) {
		try {
			MP3Frame f(mp3,offset);
			offset=f.match(validator);
			frames.push_back(f);
			if(MP3::verbose) std::cout << f << std::endl;
		}
		catch(...) {
			offset+=MP3::FrameHeaderSize;
		}
	}
}

size_t MP3File::size() const {
	return std::accumulate(begin(),end(),0,[](size_t s,auto frame) { return s+frame.size(); });
}
double MP3File::duration() const {
	return double(size())/(125.0*double(initial.BitRate()));
}


} /* namespace mp3 */


std::ostream & operator<<(std::ostream &o,const mp3::MP3File &f) {
	std::for_each(f.begin(),f.end(),[&o](auto frame) { o << frame << std::endl; });
	return o;
}