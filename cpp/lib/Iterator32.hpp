/*
 * Iterator32.hpp
 *
 *  Created on: 1 Dec 2017
 *      Author: julianporter
 */

#ifndef ITERATOR32_HPP_
#define ITERATOR32_HPP_

#include "base.hpp"
#include "Conversions.hpp"

namespace pylame { namespace pcm {

class Iterator32 {
public:
private:
	data_t::const_iterator end;
	data_t::const_iterator it;
	

	Converter32 convertNext();
	Converter64 convertNext64();
public:
	Iterator32(const data_t &data) : end(data.end()), it(data.begin()) {};
	Iterator32(const Iterator32 &o) : end(o.end), it(o.it) {};
	virtual ~Iterator32() = default;
	Iterator32 & operator=(const Iterator32 & o) = default;
		
	uint32_t nextInt() { return convertNext().u32; };
	uint64_t nextInt64() { return convertNext64().u64; };
	float nextFloat() { return convertNext().f; };
	double nextDouble() { return convertNext64().d; };
	long double nextLongDouble();
	std::string nextString() ;
	std::pair<uint16_t,uint16_t> nextPair() ;


	void skip(const unsigned n) { it+=(4*n); };
	bool finished() const { return it==end; };


	void getN(const unsigned n,char *d);
	char get();



	unsigned size() const { return end-it; }
};

}}



#endif /* ITERATOR32_HPP_ */
