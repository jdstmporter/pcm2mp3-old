/*
 * DataChunk.cpp
 *
 *  Created on: 21 Dec 2017
 *      Author: julianporter
 */
#include "DataChunk.hpp"

std::ostream & operator<<(std::ostream &o,const pylame::pcm::DataChunk &c) {
	o << "Chunk " << c.kind() << " of length " << c.size();

	return o;
}

void pylame::pcm::DataChunk::print() const {
	unsigned index=0;
	for(auto it=data.begin();it!=data.end();it++) {
		std::cout << std::dec << std::setw(2) << (int)*it << " ";
		if((index%8)==7) std::cout << std::dec << std::endl;
		index++;
		if(index==16) return;
	}
}

