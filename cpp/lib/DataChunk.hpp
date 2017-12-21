/*
 * DataChunk.hpp
 *
 *  Created on: 20 Dec 2017
 *      Author: julianporter
 */

#ifndef DATACHUNK_HPP_
#define DATACHUNK_HPP_

#include "base.hpp"
#include "Iterator32.hpp"
#include <map>
#include <algorithm>
#include "PCMFile.hpp"

namespace pylame {
namespace pcm {



	template<Endianness E>
	class DataChunk {
private:
	std::string ID;
	data_t data;
	
		
public:
	DataChunk() : ID(), data() {};
	DataChunk(const std::string &name,const data_t &data_) : ID(name), data(data_){};
	DataChunk(const DataChunk &) = default;
	virtual ~DataChunk() = default;
	DataChunk & operator=(const DataChunk &) = default;
	
	unsigned size() const { return data.size(); };
	Iterator32<E> iterator() const { return Iterator32<E>(data); };
	std::string kind() const { return ID; };
};




template<Endianness E>
	class DataForm {
	private:

		Iterator32<E> it;
		FileType type;
		std::multimap<std::string,DataChunk<E>> chunks;
		unsigned len;

		bool nextChunk() {
			try {
				auto idx=it.nextString();
				auto n=it.nextInt();
				data_t d(n,0);
				it.getN(n,d.data());
				DataChunk<E> c(idx,d);
				chunks.insert(std::make_pair(idx,c));
				return true;
			}
			catch(...) {
				 return false;
			}
		};

	public:

		DataForm() : it(), type(FileType::Other), chunks(), len() {};
		DataForm(const data_t &data) : it(data,E), type(FileType::Other), chunks(), len() {};
		DataForm(Iterator32<E> &ptr) : it(ptr), type(FileType::Other), chunks(), len() {};
		virtual ~DataForm() = default;

		void walk() {
			auto header=it.nextString();
			len=it.nextInt();
			auto t=it.nextString();
			std::cout << "Form header = '" << header << "'" << std::endl;
			std::cout << "Length = " <<  std::hex << len << std::dec << std::endl;
			std::cout << "Type string = '" << t << "'" << std::endl;
			if(header=="RIFF") {
				type=FileType::RIFF;
				if (t!="WAVE") throw MP3Error("WAVE tag not present in RIFF header");
			}
			else if(header=="FORM") {
				if(t=="AIFF") type=FileType::AIFF;
				else if(t=="AIFC") type=FileType::AIFC;
				else throw MP3Error("Unrecognised file type in FORM header");
			}
			else throw MP3Error("File is of unknown format");
			while(nextChunk()) {};
			std::for_each(chunks.begin(),chunks.end(),[](auto c) { std::cout << c.second << std::endl; });
		}

		FileType fileType() const { return type; };
		bool isWAV() const { return type==FileType::RIFF; };
		bool isAIFC() const { return type==FileType::AIFC; };
		bool isAIFF() const { return type==FileType::AIFF; };
		unsigned size() const { return chunks.size(); };
		unsigned bytesInFile() const { return len; };
		bool has(const std::string &key) const { return chunks.count(key)>0; };
		std::vector<DataChunk<E>> operator[](const std::string &ID) {
		 	auto begin=chunks.lower_bound(ID);
		 	auto end=chunks.upper_bound(ID);
		 	if(begin==end) throw MP3Error("No instances of chunk kind found");
		 	std::vector<DataChunk<E>> c;
		 	for(auto ic=begin;ic!=end;ic++) c.push_back(ic->second);
		 	return c;
		 };

	};






} /* namespace pcm */
} /* namespace pylame */

template<pylame::Endianness E>
std::ostream & operator<<(std::ostream &o,const pylame::pcm::DataChunk<E> &c) {
	o << "Chunk of type '" << c.kind() << "' and length " << c.size();
	return o;
}

#endif /* DATACHUNK_HPP_ */
