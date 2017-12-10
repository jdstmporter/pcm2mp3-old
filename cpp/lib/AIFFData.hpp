/*
 * AIFFData.hpp
 *
 *  Created on: 6 Dec 2017
 *      Author: julianporter
 */

#ifndef AIFFDATA_HPP_
#define AIFFDATA_HPP_

#include "base.hpp"
#include "Iterator32.hpp"
#include <map>

namespace pylame { namespace pcm { namespace aiff {
	
	class Chunk {
private:
	std::string ID;
	data_t data;
	
		
public:
	Chunk() : ID(), data() {};
	Chunk(const std::string &name,const data_t &data_) : ID(name), data(data_) {};
	Chunk(const Chunk &) = default;
	virtual ~Chunk() = default;
	Chunk & operator=(const Chunk &) = default;
	
	unsigned size() const { return data.size(); };
	Iterator32 iterator() const { return Iterator32(data,Iterator32::Endianness::BigEndian); };
	std::string kind() const { return ID; };
};





class Form {
private:
	enum class Type {
		AIFF,
		AIFC,
		Other
	};
	Iterator32 it;
	Type fileType;
	std::multimap<std::string,Chunk> chunks;
	
	bool nextChunk();
	unsigned len;
	
public:
	Form() : it(), fileType(Type::Other), chunks(), len() {};
	Form(const data_t &data) : it(data,Iterator32::Endianness::BigEndian), fileType(Type::Other), chunks(), len() {};
	Form(Iterator32 &ptr) : it(ptr), fileType(Type::Other), chunks(), len() {};
	virtual ~Form() = default;
	
	void walk();
	
	bool isAIFC() const { return fileType==Type::AIFC; };
	unsigned size() const { return chunks.size(); };
	unsigned bytesInFile() const { return len; }
	bool has(const std::string &key) const { return chunks.count(key)>0; };
	std::vector<Chunk> operator[](const std::string &);



};


	
}}}

std::ostream & operator<<(std::ostream &o,const pylame::pcm::aiff::Chunk &c);





#endif /* AIFFDATA_HPP_ */
