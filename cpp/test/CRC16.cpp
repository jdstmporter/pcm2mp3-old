/*
 * CRC16.cpp
 *
 *  Created on: 21 Oct 2018
 *      Author: julianporter
 */

#include "CRC16.hpp"

namespace pylame {
namespace mp3 {

uint32_t CRC16::step(const uint32_t x) const {
	auto y = (x&0x10000) ? x^poly : x;
	return y>>1;
}

CRC16::CRC16(const CRC16Polynomial p) : value(0), poly(static_cast<uint32_t>(p)) {
	for(unsigned i=0;i<256;i++) {
		auto x=i<<9;
		for(auto j=0;j<8;j++) x=step(x);
		table[i]=(uint16_t)x;
	}
}

void CRC16::operator()(const char c) {
	auto idx=(value>>8) & (uint16_t)c;
	value=table[idx]^(value<<8);
}



} /* namespace mp3 */
} /* namespace pylame */
