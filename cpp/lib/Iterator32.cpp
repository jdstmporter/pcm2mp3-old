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
	data_t Iterator32::getN(const unsigned n) {
		data_t d(it,it+n);
		it+=n;
		return std::move(d);
	}
		char Iterator32::get() {
			if(it==end) throw MP3Error("Overrun end of file");
			return *it++;
		}

		template<>
		uint32_t Iterator32::next<uint32_t>() { return wrap(convertNext().u32); };
		template<>
		uint64_t Iterator32::next<uint64_t>() { return convertNext64().u64; };
		template<>
		float Iterator32::next<float>() { return convertNext().f; };
		template<>
		double Iterator32::next<double>() { return convertNext64().d; };
		template<>
		long double Iterator32::next<long double>() { return nextLongDouble(); };
		template<>
		std::string Iterator32::next<std::string>() { return nextString(); };
		template<>
		pair_t Iterator32::next<pair_t>() { return nextPair(); };

		template<>
		std::pair<int16_t,int16_t> Iterator32::nextPairOf() { return next<pair_t>(); }
		template<>
		std::pair<int32_t,int32_t> Iterator32::nextPairOf() { return std::make_pair(next<uint32_t>(),next<uint32_t>()); }
		template<>
		std::pair<float,float> Iterator32::nextPairOf() { return std::make_pair(next<float>(),next<float>()); }

}}
