/*
 * Iterator32.cpp
 *
 *  Created on: 1 Dec 2017
 *      Author: julianporter
 */

#include "Iterator32.hpp"
#include "enums.hpp"

Iterator32::Converter Iterator32::convertNext() {
	Iterator32::Converter c;
	for(auto i=0;i<4;i++) {
		if(it==end) throw MP3Error("Overrun end of file");
		c.bytes[i]=*it;
		it++; 
	}
	return c;
}


uint32_t Iterator32::nextInt() {
	return convertNext().u32;
}

std::string Iterator32::nextString()  {
	char c[5]={0,0,0,0,0};
	for(auto i=0;i<4;i++) {
		if(it==end) throw MP3Error("Overrun end of file");
		c[i]=char(*it++); 
	}
	return std::string(c);
}

std::pair<uint16_t,uint16_t> Iterator32::nextPair()  {
	auto a=convertNext().u16;
	return std::make_pair(a[0],a[1]);
}

void Iterator32::skip(unsigned n) {
	it+=(4*n);
}