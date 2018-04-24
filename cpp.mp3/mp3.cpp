/*
 * mp3.cpp
 *
 *  Created on: 23 Apr 2018
 *      Author: julianporter
 */

#include "MP3File.hpp"
#include <fstream>

int main(int argc,char *argv[]) {
	try {
		mp3::MP3::verbose=true;
		std::string infile(argv[1]);
		std::ifstream mp(infile,std::ifstream::binary);
		mp3::MP3File mpFile(mp);
		mpFile.parse();
		std::cout << "Duration is " << mpFile.duration() << std::endl;
	}
	catch(std::exception &e) {
			std::cerr << e.what() << std::endl;
		}
		return 0;
}



