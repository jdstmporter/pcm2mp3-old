/*
 * main.cpp
 *
 *  Created on: 30 Nov 2017
 *      Author: julianporter
 */
 
#include "lamer.hpp"


#define MODE WAV

 int main(int argc,char *argv[]) {
 	
	 	try {
/*#if MODE == AIFF
 		std::cout << "Loading test.aiff" << std::endl;
 		std::ifstream aiff("test.aiff",std::ifstream::binary);
		pylame::Transcode transcoder(aiff,5,48);
		std::cout << "Writing to testaiff.mp3" << std::endl;
		std::ofstream out("testaiff.mp3",std::ofstream::binary);
		out << transcoder;
		out.close();
		std::cout << "Completed" << std::endl;
#else*/
		std::cout << "Loading vp.wav" << std::endl;
 		std::ifstream wav("vp.wav",std::ifstream::binary);
		//std::cout << aiff;
		pylame::Transcode transcoder(wav,5,8);
		std::cout << "Writing to vpwav.mp3" << std::endl;
		std::ofstream out("vpwav.mp3",std::ofstream::binary);
		out << transcoder;
		out.close();
		std::cout << "Completed" << std::endl;
//#endif
	}
	catch(std::exception &e) {
		std::cerr << e.what() << std::endl;
	}
	return 0;
 }




