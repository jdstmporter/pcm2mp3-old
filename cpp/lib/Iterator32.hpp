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



template<Endianness E>
class Iterator32 {
private:
	const_iterator_t end;
	const_iterator_t it;

	template <typename N>
	N wrap(N n) {
		return (E == Endianness::LittleEndian) ? n : swap(n);
	};
public:
	Iterator32() : end(), it() {};
	Iterator32(const data_t &data) : end(data.end()), it(data.begin()) {};
	Iterator32(const Iterator32<E> &o) : end(o.end), it(o.it) {};
	virtual ~Iterator32() = default;
	Iterator32 & operator=(const Iterator32 & o) = default;

	Converter32 convertNext() {
		Converter32 c;
		for(auto i=0;i<4;i++) {
			if(it==end) throw MP3Error("Overrun end of file");
			c.bytes[i]=*it;
			it++;
		}
		return c;
	}
	Converter64 convertNext64() {
		Converter64 c;
		for(auto i=0;i<8;i++) {
			if(it==end) throw MP3Error("Overrun end of file");
			c.bytes[i]=*it;
			it++;
		}
		return c;
	}
		
	uint32_t nextInt() { return wrap(convertNext().u32); };
	uint64_t nextInt64() { return convertNext64().u64; };
	float nextFloat() { return convertNext().f; };
	double nextDouble() { return convertNext64().d; };
	long double nextLongDouble() {
		Float80 f(it);
		return (long double)f;
	}
	std::string nextString() {
		char c[5]={0,0,0,0,0};
		for(auto i=0;i<4;i++) {
			if(it==end) throw MP3Error("Overrun end of file");
			c[i]=char(*it++);
		}
		return std::string(c);
	}
	std::pair<uint16_t,uint16_t> nextPair() {
		auto a=convertNext().u16;
		return wrap(std::make_pair(a[0],a[1]));
	}


	void skip(const unsigned n) { it+=(4*n); };
	bool finished() const { return it==end; };


	void getN(const unsigned n,char *data) {
		for(unsigned i=0;i<n;i++) {
			if(it==end) throw MP3Error("Overrun end of file");
			data[i]=*it;
			it++;
		}
	}
	char get() {
		if(it==end) throw MP3Error("Overrun end of file");
		return *it++;
	}



	unsigned size() const { return end-it; }
};

}}



#endif /* ITERATOR32_HPP_ */
