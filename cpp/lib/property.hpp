/*
 * property.hpp
 *
 *  Created on: 7 Dec 2017
 *      Author: julianporter
 */

#ifndef PROPERTY_HPP_
#define PROPERTY_HPP_

#include <iostream>
#include <type_traits>
#include "base.hpp"

namespace pylame {
namespace pcm {

enum class PCMParameter {
	SampleRate,
	NumberOfSamples,
	NumberOfChannels,
	DataSize,
	BitsPerSample,
	Compression,
	Offset,
	BlockSize,
	TimeStamp
};

struct Parameter {
	PCMParameter name;
	uint32_t u32;
	std::string s;
	long double fl;

		Parameter() : name(), u32(), s(), fl() {};
		Parameter(const PCMParameter &name_,const uint32_t uint) : name(name_), u32(uint), s(), fl(0) {};
		Parameter(const PCMParameter &name_,const std::string &s_) : name(name_), u32(0), s(s_), fl(0) {};
		Parameter(const PCMParameter &name_,const long double fl_) : name(name_), u32(0), s(), fl(fl_) {};

		template <typename T>
		T value() { throw MP3Error("Invalid parameter type"); };

		operator uint32_t() { return u32; };
		operator long double() { return fl; };
		operator std::string() { return s; };
	};

template <> long double Parameter::value<long double>() { return fl; };
template <> uint32_t Parameter::value<uint32_t>() { return u32; };
template <> std::string Parameter::value<std::string>() { return s; };

class ParameterSet {
protected:
	virtual Parameter get(const PCMParameter &name) const  { throw MP3Error("Not implemented"); };
	virtual void set(const PCMParameter &name,uint32_t) { throw MP3Error("Not implemented"); };
	virtual void set(const PCMParameter &name,long double) { throw MP3Error("Not implemented"); };
	virtual void set(const PCMParameter &name,const std::string &) { throw MP3Error("Not implemented"); };


public:
	ParameterSet() = default;
	virtual ~ParameterSet() = default;
	Parameter operator[](const PCMParameter &name) const { return get(name); }
	virtual bool has(const PCMParameter &name) const;
};

#define PARAM_GET(name,type) (type)get(PCMParameter::name)
#define PARAM_SET(name,value) set(PCMParameter::name,value)

} /* namespace pcm */
} /* namespace pylame */

#endif /* PROPERTY_HPP_ */
