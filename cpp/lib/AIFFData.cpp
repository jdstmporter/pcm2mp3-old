/*
 * AIFFData.cpp
 *
 *  Created on: 6 Dec 2017
 *      Author: julianporter
 */
 
#include "AIFFData.hpp"
#include <algorithm>
 
 using namespace pylame::pcm::aiff;
 
 std::ostream & operator<<(std::ostream &o,const Chunk &c) {
	o << "Chunk of type '" << c.kind() << "' and length " << c.size();
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
 		auto idx=it.nextString();
 		auto n=it.nextInt();
 		data_t d(n,0);
 		it.getN(n,d.data());
 		Chunk c(idx,d);
 		chunks.insert(std::make_pair(idx,c));
 		return true;
 	}
 	catch(...) {
 		return false;
 	}

 }



 void Form::walk() {
 	auto s=it.nextString();
 	if(s!="FORM") throw MP3Error("File is not AIFF - missing FORM");
 	len=it.nextInt();
 	auto t=it.nextString();
 	std::cout << "Length = " <<  std::hex << len << std::dec << std::endl;
 	std::cout << "Type string = '" << t << "'" << std::endl;
 	if(t=="AIFF") fileType=Type::AIFF;
 	else if(t=="AIFC") fileType=Type::AIFC;
 	else throw MP3Error("Unrecognised file type");

 	while(nextChunk()) {};
 	std::for_each(chunks.begin(),chunks.end(),[](auto c) { std::cout << c.second << std::endl; });
 }





