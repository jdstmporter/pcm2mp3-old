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

using pair_t=std::pair<uint16_t,uint16_t>;

class Iterator32 {
private:
	const_iterator_t end;
	const_iterator_t it;
	Endianness endian;

	template <typename N>
	N wrap(N n) {
		return (endian==Endianness::LittleEndian) ? n : swap(n);
	};

protected:

	uint32_t nextInt() { return wrap(convertNext().u32); };
	uint64_t nextInt64() { return convertNext64().u64; };
	float nextFloat() { return convertNext().f; };
	double nextDouble() { return convertNext64().d; };
	long double nextLongDouble();
	std::string nextString();
	pair_t nextPair();

public:
	Iterator32() : end(), it(), endian(Endianness::LittleEndian) {};
	Iterator32(const data_t &data,const Endianness &e) : end(data.end()), it(data.begin()), endian(e) {};
	Iterator32(const Iterator32 &o) : end(o.end), it(o.it), endian(o.endian) {};
	Iterator32(const_iterator_t it_,const_iterator_t end_,Endianness endian_) : end(end_), it(it_), endian(endian_) {};
	virtual ~Iterator32() = default;
	Iterator32 & operator=(const Iterator32 & o) = default;

	Endianness endianness() const { return endian; };

	Converter32 convertNext();
	Converter64 convertNext64();

	template<typename T>
	T next() { throw MP3Error("Not defined"); };

	template<typename T>
	std::pair<T,T> nextPairOf() { throw MP3Error("Not defined"); };
		
	Iterator32 fix(const unsigned n) const {
		return Iterator32(it,it+n,endian);
	};

	void skip(const int n) { it+=(4*n); };
	bool finished() const { return it==end; };


	void getN(const unsigned n,char *data);
	data_t getN(const unsigned n);
	char get();

	unsigned size() const { return end-it; }
};


}}



#endif /* ITERATOR32_HPP_ */
