/*
 * PCMData.cpp
 *
 *  Created on: 16 Jan 2018
 *      Author: julianporter
 */



#include "PCMData.hpp"
#include <cmath>

using namespace pylame::pcm;



Channels<float> PCMData::asFloat(bool boost) {
		switch(format) {
		case pylame::SampleFormat::Float32:
			return channels<float>(boost);
			break;
		case pylame::SampleFormat::Int16:
			return channels<int16_t>(boost).asFloat();
			break;
		case pylame::SampleFormat::Int32:
			return channels<int32_t>(boost).asFloat();
			break;
		default:
			throw MP3Error("Cannot process data type");
		}
	};
