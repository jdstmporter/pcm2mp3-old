/*
 * AIFFFile.hpp
 *
 *  Created on: 5 Dec 2017
 *      Author: julianporter
 */

#ifndef AIFFFILE_HPP_
#define AIFFFILE_HPP_


#include "enums.hpp"
#include "PCMFile.hpp"
#include "Iterator32.hpp"
#include <map>
#include <memory>


class Chunk {
friend std::ostream & operator<<(std::ostream &o,const Chunk &c);
private:
	std::string ID;
	data_t data;
	
		
public:
	Chunk(const std::string &name,const data_t &data_) : ID(name), data(data_) {};
	Chunk(const Chunk &) = default;
	virtual ~Chunk() = default;
	Chunk & operator=(const Chunk &) = default;
	
	unsigned size() const { return data.size(); };
	std::shared_ptr<Iterator32> iterator() const { return std::make_shared<Iterator32>(data); };

};



std::ostream & operator<<(std::ostream &o,const Chunk &c);

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
	
	
public:
	
	Form(Iterator32 &ptr) : it(ptr), fileType(Type::Other), chunks() {};
	virtual ~Form() = default;
	
	void walk();
	
	
	unsigned size() const { return chunks.size(); };
	
	bool has(const std::string &key) const { return chunks.count(key)>0; };
	std::vector<Chunk> operator[](const std::string &);


};





class AIFFFile: public PCMFile {

private:
	data_t file;
	std::shared_ptr<Iterator32> iterator;
	
	void commChunk(const Chunk &f);
	void soundChunk(const Chunk &f);
	
	

	
protected:
	
	
	
public:
	AIFFFile(std::istream &);
	virtual ~AIFFFile() = default;
	
	void process();
	
};

#endif /* AIFFFILE_HPP_ */
