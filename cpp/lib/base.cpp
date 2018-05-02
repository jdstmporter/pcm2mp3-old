/*
 * enums.cpp
 *
 *  Created on: 30 Nov 2017
 *      Author: julianporter
 */


#include "base.hpp"

using namespace pylame;

#include <cstdlib>
#include <locale>
#include <map>

std::ostream & operator<<(std::ostream &o,const MP3Error &ex) {
	o << "MP3Error : " << ex.what();
	return o;
}

static const std::map<FileType,std::string> typeNames={
		{FileType::WAV, "WAV"},
		{FileType::AIFF, "AIFF"},
		{FileType::AIFC, "AIFC"},
		{FileType::Other, "Other"}
};
static const std::map<SampleFormat,std::string> sampleFormatNames={
		{SampleFormat::Int16, "Int16"},
		{SampleFormat::Int32, "Int32"},
		{SampleFormat::Float32, "Float32"},
		{SampleFormat::Unknown, "Other"}
};

static const std::map<DataFormat,std::string> dataFormatNames={
		{DataFormat::PCM, "PCM"},
		{DataFormat::IEEEFloat, "IEEEFloat"},
		{DataFormat::ALaw, "A-law"},
		{DataFormat::ULaw, "U-law"},
		{DataFormat::Unknown, "Other"}
};

std::ostream & operator<<(std::ostream &o,const FileType &type) {
	o << typeNames.at(type);
	return o;
}
std::ostream & operator<<(std::ostream &o,const SampleFormat &fmt) {
	o << sampleFormatNames.at(fmt);
	return o;
}
std::ostream & operator<<(std::ostream &o,const DataFormat &fmt) {
	o << dataFormatNames.at(fmt);
	return o;
}

std::locale l;
char up(const char c) { return std::toupper(c,l); };
char down(const char c) { return std::tolower(c,l); };

std::string pylame::toUpper(const std::string &s) {
	std::vector<char> bytes(s.size(),0);
	std::transform(s.begin(),s.end(),bytes.begin(), up);
	return std::move(std::string(bytes.begin(),bytes.end()));
}
std::string pylame::toLower(const std::string &s) {
	std::vector<char> bytes(s.size(),0);
	std::transform(s.begin(),s.end(),bytes.begin(), down);
	return std::move(std::string(bytes.begin(),bytes.end()));
}
