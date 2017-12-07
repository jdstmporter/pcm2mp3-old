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
	enum class Endianness {
		BigEndian,
		LittleEndian
	};
private:
	const_iterator_t end;
	const_iterator_t it;
	bool littleEndian;
	

	Converter32 convertNext();
	Converter64 convertNext64();

	template <typename N>
	N wrap(N n) {
		return (littleEndian) ? n : swap(n);
	};
public:
	Iterator32() : end(), it(), littleEndian(true) {};
	Iterator32(const data_t &data,const Endianness &e = Endianness::LittleEndian) : end(data.end()), it(data.begin()), littleEndian(e==Endianness::LittleEndian) {};
	Iterator32(const Iterator32 &o) : end(o.end), it(o.it), littleEndian(o.littleEndian) {};
	virtual ~Iterator32() = default;
	Iterator32 & operator=(const Iterator32 & o) = default;
		
	uint32_t nextInt() { return wrap(convertNext().u32); };
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
