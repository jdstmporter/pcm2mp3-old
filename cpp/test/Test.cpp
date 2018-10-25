/*
 * Test.cpp
 *
 *  Created on: 21 Oct 2018
 *      Author: julianporter
 */

#include "Test.hpp"
#include <fstream>
#include <sstream>



namespace mp3 {


inline std::string _(const MP3Frame &f) {
	std::stringstream s;
	s << f;
	return s.str();
}

TestResult::TestResult(const MP3File &file,const bool verbose) :
		spec(file.initial.Version(),file.initial.Layer(),file.initial.Mode()),
		duration(file.duration()),
		nsamples(file.size()),
		bitrate(1000*file.bitrate()),
		samplerate(file.samplerate()),
		nframes(file.nframes()),
		goodSampleRate(bitrate>samplerate),
		log(verbose ? file.size() : 0) {


	message=isGood() ? "PASSED" : "FAILED";
	if(verbose) {
		std::transform(file.begin(),file.end(),log.begin(), [](auto f) { return _(f); });
	}
}


bool TestResult::isGood() const {
	return spec.isGood() && goodSampleRate;
}

void Test::parse(const bool verbose) {
	try {
		std::ifstream mp(fname,std::ifstream::binary);
		if(mp.fail()) throw std::runtime_error("Cannot open file");

		MP3::verbose=verbose;
		MP3File file(mp);
		file.parse();
		result=std::make_shared<TestResult>(file,verbose);
	}
	catch(std::exception &e) {
		result=std::make_shared<TestResult>(e.what());
	}

}
} /* namespace mp3 */

inline std::string _(const bool b) { return b ? "PASS" : "FAIL"; }

std::ostream & operator<<(std::ostream &o,const mp3::TestResult &r) {
	o << "Header : " << r.spec << " (" << _(r.spec.isGood()) << ")" << std::endl
			<< "Byte rate: " << r.bitrate << " Sample rate: " << r.samplerate << " (" << _(r.goodSampleRate)
			<< ") Duration: " << r.duration << std::endl
			<< "RESULT (" << _(r.isGood()) <<") : " << r.message << std::endl;
	return o;
}





