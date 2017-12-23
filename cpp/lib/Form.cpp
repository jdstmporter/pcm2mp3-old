/*
 * Form.cpp
 *
 *  Created on: 21 Dec 2017
 *      Author: julianporter
 */

#include "Form.hpp"
#include <cstdlib>
#include <locale>

namespace pylame {
namespace pcm {

bool FormMetaData::verify(const std::string &head,const TypeMap &formats) {
	try {
		//std::cout << "Got '"<< header << "' expected '" << head << "'" << std::endl;
		if(header!=head) throw MP3Error("Form header mismatch with expected header");
		auto it=formats.find(format);
		if(it==formats.end()) throw MP3Error("Form format does not match any of those available");
		type=it->second;
		return true;
	}
	catch(std::exception &e) {
		//std::cerr << e.what() << std::endl;
		type=FileType::Other;
		return false;
	}
};

bool Form::nextChunk() {
	try {
		auto f=it.convertNext();
		std::locale l;
		char bytes[4];
		for(auto i=0;i<4;i++) bytes[i]=std::toupper(f.bytes[i],l);
		std::string idx(bytes,4);
		auto n=it.nextInt();
		//data_t d(n,0);
		//for(auto i=0;i<n;i++) d[i]=it.get();
		auto c=std::make_shared<DataChunk>(idx,std::move(it.getN(n)),endian);
		chunks[idx]=c;
		return true;
	}
	catch(...) {
		return false;
	}
}

FormMetaData Form::typeCheck() {
	auto h=it.nextString();
	len=it.nextInt();
	auto t=it.nextString();
	return FormMetaData(h,t,len);
}

void Form::walk() {
	while(nextChunk()) {};
	std::for_each(chunks.begin(),chunks.end(),[](auto c) { std::cout << c.first << std::endl; });
}

std::shared_ptr<DataChunk> Form::operator[](const std::string &ID) {
	try {
		return chunks.at(ID);
	}
	catch(...) {
		throw MP3Error("No such chunk");
	}
}



} /* namespace pcm */
} /* namespace pylame */

std::ostream & operator<<(std::ostream &o,const pylame::pcm::FormMetaData &m) {
	o << "Form header = '" << m.header << "'" << std::endl;
	o << "Format      = '" << m.format << "'" << std::endl;
	o << "Length = " <<  std::hex << m.length << std::dec << std::endl;
	return o;
}

