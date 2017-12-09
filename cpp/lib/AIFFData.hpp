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
#include "PCMFile.hpp"
#include "Enumerated.hpp"




namespace pylame { namespace pcm {

	class AIFFFile;
namespace aiff {
ENUM_CLASS(ChunkKind,SSND,COMM,FVER,Other)

/*	struct ChunkKind {
		//using value_t=item_t;
		//using const_iterator=std::vector<item_t>::const_iterator;
		ITEM(SSND,1);
		ITEM(COMM,2);
		ITEM(FVER,3);
		ITEM(Other,4);

		__loop(ITEM,A,B,C)

		_CLASS_FUNCS
		//static constexpr value_t SSND = item_t("SSND",1);
		//static constexpr value_t COMM = item_t("COMM",2);
		//static constexpr value_t FVER = item_t("FVER",3);
		//static constexpr value_t Other = item_t("Other",4);

		//static const std::vector<item_t> all;
		//item_t static named(const std::string &name);
		//static const_iterator cbegin() { return all.cbegin(); };
		//static const_iterator cend() { return all.cend(); };
	};
*/
ENUM_CLASS(FormKind,AIFF,AIFC,Other)
ENUM_CLASS(CompressionKind,NONE,SOWT,FL32,FL64,ALAW,ULAW,Other)




	class Chunk : public ParameterSet {
	private:
		const static uint32_t AIFCVersion1TimeStamp=0xa2805140;

		ChunkKind::value_t ID;
		data_t data;
		std::map<PCMParameter,Parameter> parameters;
	protected:
		virtual Parameter get(const PCMParameter &name) const{ return parameters.at(name); };
		virtual void set(const PCMParameter &name ,const uint32_t arg);
		virtual void set(const PCMParameter &name ,const long double arg);
		virtual void set(const PCMParameter &name ,const std::string & arg);


	public:
		Chunk() : ID(ChunkKind::Other), data(),  parameters() {};
		Chunk(const ChunkKind::value_t &c,const data_t data_) : ID(c), data(data_), parameters() {};
		Chunk(const std::string &name,const data_t &data_) : Chunk(ChunkKind::named(name,false),data) {};
		Chunk(const Chunk &) = default;
	virtual ~Chunk() = default;
	Chunk & operator=(Chunk &) = default;
	
	unsigned size() const { return data.size(); };
	Iterator32 iterator() const { return Iterator32(data,Iterator32::Endianness::BigEndian); };
	ChunkKind::value_t kind() const { return ID; };

	virtual bool has(const PCMParameter &name) const { return parameters.find(name)!=parameters.end(); };

	void parse(const FormKind::value_t &fileType);
};







class Form : public ParameterSet {
private:


	Iterator32 it;
	std::string file_s;

	std::multimap<ChunkKind::value_t,Chunk> chunks;
	
	bool nextChunk();
	unsigned len;
	
	item_t fileType() { return FormKind::named(file_s,false); };

protected:
	virtual Parameter get(const PCMParameter &name) const;

public:
	Form() : it(), file_s(), chunks(), len() {};
	Form(const data_t &data) : it(data,Iterator32::Endianness::BigEndian), file_s(), chunks(), len() {};
	Form(Iterator32 &ptr) : it(ptr), file_s(), chunks(), len() {};
	virtual ~Form() = default;
	


	void walk();
	
	
	unsigned size() const { return chunks.size(); };
	unsigned fileSize() const { return len; }
	virtual bool has(const ChunkKind::value_t &key) const { return chunks.count(key)>0; };
	std::vector<Chunk> all(const ChunkKind::value_t &) const;
	Chunk first(const ChunkKind::value_t &) const;



};


	
}}}

std::ostream & operator<<(std::ostream &o,const pylame::pcm::aiff::ChunkKind &kind);
std::ostream & operator<<(std::ostream &o,const pylame::pcm::aiff::Chunk &c);





#endif /* AIFFDATA_HPP_ */
