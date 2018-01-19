/*
 * Options.hpp
 *
 *  Created on: 19 Jan 2018
 *      Author: julianporter
 */

#ifndef OPTIONS_HPP_
#define OPTIONS_HPP_

#include <vector>
#include <initializer_list>

namespace pylame {

enum class Option {
	ReplayGain,
	Boost,
	MakeFloat
};

class MP3Settings {
private:
	std::vector<Option> options;
	unsigned quality_;
	unsigned rate_;

public:

	MP3Settings() : options(), quality_(5), rate_(8)  {};
	MP3Settings(const std::initializer_list<Option> &options_,const unsigned quality=5,const unsigned rate=8)
		: options(options_), quality_(quality), rate_(rate) {};
	MP3Settings(std::initializer_list<Option> &&options_,const unsigned quality=5,const unsigned rate=8)
		: options(options_), quality_(quality), rate_(rate) {};
	MP3Settings(const MP3Settings &other) : options(other.options), quality_(other.quality_), rate_(other.rate_) {};
	MP3Settings(MP3Settings &&other) : options(std::move(other.options)), quality_(other.quality_), rate_(other.rate_) {};
	MP3Settings & operator=(const MP3Settings &other);
	MP3Settings & operator=(MP3Settings &&other);
	virtual ~MP3Settings() = default;

	bool operator[](const Option o) const;
	int quality() const { return int(quality_); };
	int rate() const { return int(rate_); };

};

} /* namespace pylame */

#endif /* OPTIONS_HPP_ */
