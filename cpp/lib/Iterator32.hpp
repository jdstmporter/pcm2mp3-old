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
#include <iomanip>

namespace pylame { namespace pcm {


class Iterator32 {
private:
	const_iterator_t end;
	const_iterator_t it;

	bool isLittleEndian;

	template <typename N>
	N wrap(N n) {
		return isLittleEndian ? n : swap(n);
	};
public:
	Iterator32() : end(), it(), isLittleEndian(true) {};
	Iterator32(const data_t &data,const Endianness &e) : end(data.end()), it(data.begin()), isLittleEndian(e==Endianness::LittleEndian) {};
	Iterator32(const Iterator32 &o) : end(o.end), it(o.it), isLittleEndian(o.isLittleEndian) {};
	virtual ~Iterator32() = default;
	Iterator32 & operator=(const Iterator32 & o) = default;

	Endianness endianness() const { return isLittleEndian ? Endianness::LittleEndian : Endianness::BigEndian; };

	Converter32 convertNext();
	Converter64 convertNext64();
		
	uint32_t nextInt() { return wrap(convertNext().u32); };
	uint64_t nextInt64() { return convertNext64().u64; };
	float nextFloat() { return convertNext().f; };
	double nextDouble() { return convertNext64().d; };
	long double nextLongDouble();
	std::string nextString();
	std::pair<uint16_t,uint16_t> nextPair();


	void skip(const int n) { it+=(4*n); };
	bool finished() const { return it==end; };


	void getN(const unsigned n,char *data);
	std::pair<const_iterator_t,const_iterator_t> getN(const unsigned n);
	char get();



	unsigned size() const { return end-it; }
};

}}



#endif /* ITERATOR32_HPP_ */
