/*
 * AIFFData.cpp
 *
 *  Created on: 6 Dec 2017
 *      Author: julianporter
 */
 
#include "AIFFData.hpp"
#include "AIFFFile.hpp"
#include <map>
 
using namespace pylame::pcm::aiff;
using namespace pylame;

_CLASS_FUNCS_DEF(ChunkKind,ChunkKind::SSND,ChunkKind::COMM,ChunkKind::FVER,ChunkKind::Other)
_CLASS_FUNCS_DEF(FormKind,FormKind::AIFF,FormKind::AIFC,FormKind::Other)
_CLASS_FUNCS_DEF(CompressionKind,NONE,SOWT,FL32,FL64,ALAW,ULAW,Other)
//const std::vector<item_t> ChunkKind::all=std::vector<item_t>{ChunkKind::SSND,ChunkKind::COMM,ChunkKind::FVER,ChunkKind::Other};
//item_t ChunkKind::named(const std::string &name) {
//	auto it=std::find_if(all.begin(),all.end(),[&name](auto &v) { return v.str()==name; });
//	return (it==all.end()) ? Other : *it;
//}


void Chunk::set(const PCMParameter &name ,const uint32_t arg) {
	Parameter p(name,arg);
	parameters[p.name]=p;
}
void Chunk::set(const PCMParameter &name ,const long double arg) {
	Parameter p(name,arg);
	parameters[p.name]=p;
}
void Chunk::set(const PCMParameter &name ,const std::string & arg) {
	Parameter p(name,arg);
	parameters[p.name]=p;
}


void Chunk::parse(const FormKind::value_t &fileType) {
	auto it=iterator();
	switch(ID) {
	case ChunkKind::COMM: {
		auto p1=it.nextPair();
		auto p2=it.nextPair();
		PARAM_SET(NumberOfChannels,uint32_t(p1.first));
		PARAM_SET(NumberOfSamples,swap(p1.second,p2.first));
		PARAM_SET(BitsPerSample,(uint32_t)p2.second);
		PARAM_SET(SampleRate,it.nextLongDouble());
		if(fileType==FormKind::AIFC) {
			auto compression=CompressionKind::named(it.nextString(),true);
			PARAM_SET(Compression,(unsigned)compression);
		}
		else PARAM_SET(Compression,CompressionKind::NONE);
	}
	break;
	case ChunkKind::SSND: {
		uint32_t d=size()-8;
		PARAM_SET(DataSize,d);
		PARAM_SET(Offset,it.nextInt());
		PARAM_SET(BlockSize,it.nextInt());
		if(it.size()!=d) throw MP3Error("Data size error");
	}
	break;
	case ChunkKind::FVER: {
		auto ts=it.nextInt();
		if(ts!=Chunk::AIFCVersion1TimeStamp) throw MP3Error("Invalid AIFC version timestamp");
		PARAM_SET(TimeStamp,ts);
	}
	break;
	default:
		break;
	}
}


std::ostream & operator<<(std::ostream &o,const ChunkKind::value_t &kind) {
	o << kind.str();
	return o;
}

 std::ostream & operator<<(std::ostream &o,const Chunk &c) {
 	o << "Chunk of type '" << c.kind() << "' and length " << c.size();
 	return o;
 }

 std::vector<Chunk> Form::all(const ChunkKind::value_t &ID) const {
 	auto begin=chunks.lower_bound(ID);
 	auto end=chunks.upper_bound(ID);
 	if(begin==end) throw MP3Error("No instances of chunk kind found");
 	std::vector<Chunk> c;
 	for(auto ic=begin;ic!=end;ic++) c.push_back(ic->second);
 	return c;
 }

 Chunk Form::first(const ChunkKind::value_t &kind) const {
	 auto chunks=all(kind);
	 if(chunks.size()!=1) throw MP3Error("Anomalous AIFF file with multiple chunks");
	 return *chunks.begin();
 }

 bool Form::nextChunk() {
 	try {
 		auto idx=ChunkKind::named(it.nextString(),false);
 		auto n=it.nextInt();
 		data_t d(n,0);
 		it.getN(n,d.data());
 		Chunk c(idx,d);
 		c.parse(fileType());
 		chunks.insert(std::make_pair(idx,c));
 		return true;
 	}
 	catch(...) {
 		return false;
 	}

 }

 void Form::walk() {
 	auto s=it.nextString();
 	if(s!="FORM") throw MP3Error("File is not AIFF / AIFC - missing FORM");
 	file_s=it.nextInt();
 	auto t=it.nextString();
 	std::cout << "Length = " <<  std::hex << len << std::dec << std::endl;
 	std::cout << "Type string = '" << t << "'" << std::endl;
 	try {
 		fileType();
 	}
 	catch(...) {
 		throw MP3Error("Unrecognised file type");
 	}

 	while(nextChunk()) {};
 }

 pylame::pcm::Parameter Form::get(const PCMParameter &name) const {
	for(auto it=ChunkKind::cbegin();it!=ChunkKind::cend();it++) {
		auto chunk=first(*it);
		if(chunk.has(name)) { return chunk[name]; }
	}
	throw MP3Error("No such parameter");
 }





