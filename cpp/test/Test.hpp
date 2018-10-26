/*
 * Test.hpp
 *
 *  Created on: 21 Oct 2018
 *      Author: julianporter
 */

#ifndef TEST_HPP_
#define TEST_HPP_

#include "MP3File.hpp"
#include <iostream>

namespace mp3 {

struct MP3TestResult {
public:
	MPEGSpecification spec;
	double duration;
	size_t nsamples;
	unsigned bitrate;
	unsigned samplerate;
	unsigned nframes;
	unsigned nCRC;

	bool goodSampleRate;

	bool isGood() const;

	std::string message;
	std::vector<std::string> log;

	MP3TestResult(const std::string &m=""):
		spec(), duration(0), nsamples(0),bitrate(0),samplerate(0),nframes(0),nCRC(0),goodSampleRate(false), message(m), log() {};
	MP3TestResult(const MP3File &file,const bool verbose=false);
	MP3TestResult(const MP3TestResult &) = default;
	virtual ~MP3TestResult() = default;
	MP3TestResult &operator=(const MP3TestResult &) = default;

};


class MP3Test {
private:
	std::string fname;
	std::shared_ptr<MP3TestResult> result;
	std::string error;




public:
	MP3Test(const std::string &name) : fname(name), result(), error() {};
	virtual ~MP3Test() = default;

	void parse(const bool verbose=false);
	std::shared_ptr<MP3TestResult> operator()() const { return result; }
	bool isGood() const { return result->isGood(); }
	std::string message() const { return result->message; }
};

} /* namespace mp3 */

std::ostream & operator<<(std::ostream &o,const mp3::MP3TestResult &r);



#endif /* TEST_HPP_ */
