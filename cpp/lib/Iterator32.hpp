/*
 * Iterator32.hpp
 *
 *  Created on: 1 Dec 2017
 *      Author: julianporter
 */

#ifndef ITERATOR32_HPP_
#define ITERATOR32_HPP_

#include "enums.hpp"


class Iterator32 {
public:
private:
	data_t::const_iterator end;
	data_t::const_iterator it;
	
	union Converter {
		uint32_t u32;
		uint16_t u16[2];
		char bytes[4];	
	};
	Converter convertNext();
public:
	Iterator32(const data_t &data) : end(data.end()), it(data.begin()) {};
	virtual ~Iterator32() = default;
		
	uint32_t nextInt() ;
	std::string nextString() ;
	std::pair<uint16_t,uint16_t> nextPair() ;
		
	void skip(unsigned n);
	bool finished() const { return it==end; };
};

#endif /* ITERATOR32_HPP_ */
