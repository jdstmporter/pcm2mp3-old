/*
 * PCMFile.cpp
 *
 *  Created on: 30 Nov 2017
 *      Author: julianporter
 */

#include "PCMFile.hpp"

std::ostream & operator<<(std::ostream &o,const pylame::pcm::PCMFile &w)  {
	o << "Size             " << w.size() << std::endl;
	o << "N Channels       " << w.nChans()  << std::endl;
	o << "Sample rate      " << w.samplesPerSecond() << std::endl;
	o << "Bits per sample  " << w.sampleSize() << std::endl;
	o << "Bytes per sample " << w.sampleSizeInBytes() << std::endl;
	o << "Data size        " << w.dSize() << std::endl;
	o << "N Samples        " << w.samplesPerChannel() << std::endl;
	return o;
}
