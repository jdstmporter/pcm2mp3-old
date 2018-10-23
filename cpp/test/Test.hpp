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

struct TestResult {
public:
	MPEGSpecification spec;
	double duration;
	size_t nsamples;
	unsigned bitrate;
	unsigned samplerate;
	unsigned samplesize;
	unsigned nframes;


	bool validSampleSize;
	bool isGood() const;

	std::string message;
	std::vector<std::string> log;

	TestResult(const std::string &m=""): spec(), duration(0), nsamples(0),bitrate(0),samplerate(0),samplesize(0), nframes(0),validSampleSize(false),message(m), log() {};
	TestResult(const MP3File &file,const bool verbose=false);
	TestResult(const TestResult &) = default;
	virtual ~TestResult() = default;
	TestResult &operator=(const TestResult &) = default;

};


class Test {
private:
	std::string fname;
	std::shared_ptr<TestResult> result;
	std::string error;




public:
	Test(const std::string &name) : fname(name), result(), error() {};
	virtual ~Test() = default;

	void parse(const bool verbose=false);
	std::shared_ptr<TestResult> operator()() const { return result; }
	bool isGood() const { return result->isGood(); }
	std::string message() const { return result->message; }
};

} /* namespace mp3 */

std::ostream & operator<<(std::ostream &o,const mp3::TestResult &r);



#endif /* TEST_HPP_ */
