/*
 * Conversions.cpp
 *
 *  Created on: 5 Dec 2017
 *      Author: julianporter
 */

#include "Conversions.hpp"
#include "Iterator32.hpp"
#include <algorithm>

Converter80::Converter80() {
	std::fill_n(bytes,10,0);
}

Converter80::Converter80(data_t::const_iterator &it) {
	for(auto i=0;i<10;i++) bytes[9-i]=*it++;
}

Converter80::Converter80(Iterator32 &it) {
	for(auto i=0;i<10;i++) bytes[9-i]=it.get();
}

Converter80::Converter80(char *p) {
	for(auto i=0;i<10;i++) bytes[9-i]=p[i];
}

uint16_t swap16(uint16_t in) {
	return (in<<8)|(in>>8);
}

std::pair<uint16_t,uint16_t> swap16(std::pair<uint16_t,uint16_t> in) {
	return std::make_pair(swap16(in.first),swap16(in.second));
}

unsigned swap32(unsigned in) {
	uint32_t top=uint32_t(swap16(uint16_t(in)))<<16;
	uint32_t bot=uint32_t(swap16(uint16_t(in>>16)));
	return (unsigned)(top|bot);
	
}

unsigned swap32(uint16_t u1,uint16_t u2) {
	uint32_t top=uint32_t(u1)<<16;
	uint32_t bot=uint32_t(u2);
	return (unsigned)(top|bot);
	
}