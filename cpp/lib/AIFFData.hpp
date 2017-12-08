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
#include <algorithm>
#include "enum.hpp"
#include "PCMFile.hpp"





namespace pylame { namespace pcm {

	class AIFFFile;
namespace aiff {

	ENUM(ChunkKind,short,SSND,COMM,FVER,Other)
	ENUM(FormKind,short,AIFF,AIFC,Other)
	ENUM(CompressionKind,uint32_t,NONE,SOWT,FL32,FL64,ALAW,ULAW,Other)

	template<typename E>
	E named(const std::string &name,bool caseIndependent=false) {
		try {
				const char *c=name.c_str();
				return (caseIndependent) ? E::_from_string_nocase(c) : E::_from_string(c);
			}
			catch(...) {
				return E::Other;
			}
	};
	template<typename E>
	std::string nameOf(const E &kind) {
		return std::string(kind._to_string());
	};
	



	class Chunk : public ParameterSet {
	private:
		const static uint32_t AIFCVersion1TimeStamp=0xa2805140;

		ChunkKind ID;
		data_t data;
		std::map<PCMParameter,Parameter> parameters;
	protected:
		virtual Parameter get(const PCMParameter &name) const{ return parameters.at(name); };
		virtual void set(const PCMParameter &name ,const uint32_t arg);
		virtual void set(const PCMParameter &name ,const long double arg);
		virtual void set(const PCMParameter &name ,const std::string & arg);


	public:
		Chunk() : ID(ChunkKind::Other), data(),  parameters() {};
		Chunk(const ChunkKind &c,const data_t data_) : ID(c), data(data_), parameters() {};
		Chunk(const std::string &name,const data_t &data_) : Chunk(named<ChunkKind>(name),data) {};
		Chunk(const Chunk &) = default;
	virtual ~Chunk() = default;
	Chunk & operator=(const Chunk &) = default;
	
	unsigned size() const { return data.size(); };
	Iterator32 iterator() const { return Iterator32(data,Iterator32::Endianness::BigEndian); };
	ChunkKind kind() const { return ID; };

	virtual bool has(const PCMParameter &name) const { return parameters.find(name)!=parameters.end(); };

	void parse(const FormKind &fileType);
};







class Form : public ParameterSet {
private:

	Iterator32 it;
	FormKind fileType;
	std::multimap<ChunkKind,Chunk> chunks;
	
	bool nextChunk();
	unsigned len;
	
protected:
	virtual Parameter get(const PCMParameter &name) const;

public:
	Form() : it(), fileType(FormKind::Other), chunks(), len() {};
	Form(const data_t &data) : it(data,Iterator32::Endianness::BigEndian), fileType(FormKind::Other), chunks(), len() {};
	Form(Iterator32 &ptr) : it(ptr), fileType(FormKind::Other), chunks(), len() {};
	virtual ~Form() = default;
	


	void walk();
	
	
	unsigned size() const { return chunks.size(); };
	unsigned fileSize() const { return len; }
	virtual bool has(const ChunkKind &key) const { return chunks.count(key)>0; };
	std::vector<Chunk> all(const ChunkKind &) const;
	Chunk first(const ChunkKind &) const;



};


	
}}}

std::ostream & operator<<(std::ostream &o,const pylame::pcm::aiff::ChunkKind &kind);
std::ostream & operator<<(std::ostream &o,const pylame::pcm::aiff::Chunk &c);





#endif /* AIFFDATA_HPP_ */
