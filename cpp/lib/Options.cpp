/*
 * Options.cpp
 *
 *  Created on: 19 Jan 2018
 *      Author: julianporter
 */

#include "Options.hpp"

namespace pylame {

#include <algorithm>





MP3Settings & MP3Settings::operator=(const MP3Settings &other) {
		options=other.options;
		quality_=other.quality_;
		rate_=other.rate_;
		return *this;
	}
MP3Settings & MP3Settings::operator=(MP3Settings &&other) {
	options=std::move(other.options);
	quality_=other.quality_;
	rate_=other.rate_;
	return *this;
}

bool MP3Settings::operator[](const Option o) const {
	return std::find(options.begin(),options.end(),o)!=options.end();
}

} /* namespace pylame */
