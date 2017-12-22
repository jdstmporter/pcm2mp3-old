/*
 * Iterator32.cpp
 *
 *  Created on: 21 Dec 2017
 *      Author: julianporter
 */
 #include "Iterator32.hpp"

namespace pylame { namespace pcm {

	Converter32 Iterator32::convertNext() {
		Converter32 c;
		for(auto i=0;i<4;i++) {
			if(it==end) throw MP3Error("Overrun end of file");
			c.bytes[i]=*it;
			it++;
		}
		return c;
	}
	Converter64 Iterator32::convertNext64() {
		Converter64 c;
		for(auto i=0;i<8;i++) {
			if(it==end) throw MP3Error("Overrun end of file");
			c.bytes[i]=*it;
			it++;
		}
		return c;
	}
	std::string Iterator32::nextString() {
			char c[5]={0,0,0,0,0};
			for(auto i=0;i<4;i++) {
				if(it==end) throw MP3Error("Overrun end of file");
				c[i]=char(*it++);
			}
			return std::string(c);
	}
	long double Iterator32::nextLongDouble() {
			Float80 f(it);
			return (long double)f;
		}
	std::pair<uint16_t,uint16_t> Iterator32::nextPair() {
			auto a=convertNext().u16;
			return wrap(std::make_pair(a[0],a[1]));
		}
	void Iterator32::getN(const unsigned n,char *data) {
			for(unsigned i=0;i<n;i++) {
				if(it==end) throw MP3Error("Overrun end of file");
				data[i]=*it;
				it++;
			}
	}
	std::pair<const_iterator_t,const_iterator_t> Iterator32::getN(const unsigned n) {
		auto p=std::make_pair(it,it+n);
		it+=n;
		return p;
	}
		char Iterator32::get() {
			if(it==end) throw MP3Error("Overrun end of file");
			return *it++;
		}


}}
