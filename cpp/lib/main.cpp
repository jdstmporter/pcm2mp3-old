/*
 * main.cpp
 *
 *  Created on: 30 Nov 2017
 *      Author: julianporter
 */
 
 #include "enums.hpp"
 #include "AIFFFile.hpp"
 #include "MP3Encoder.hpp"
 #include <iostream>
 #include <fstream>
 
 
 int main(int argc,char *argv[]) {
 	
 	try {
 		std::cout << "Loading test.aiff" << std::endl;
 		std::ifstream aiffFile("test.aiff",std::ifstream::binary);
		AIFFFile aiff(aiffFile);
		/*std::cout << aiff;
		std::cout << "Creating LAME object" << std::endl;
		MP3Encoder mp3(&wav,Quality::Good_Fast,BitRate::High);
		std::cout << "About to transcode" << std::endl;
		mp3.transcode();
		std::cout << "Writing to test.mp3" << std::endl;
		std::ofstream out("test.mp3",std::ofstream::binary);
		out << mp3;
		out.close();*/
		std::cout << "Completed" << std::endl;
	}
	catch(std::exception &e) {
		std::cerr << e.what() << std::endl;
	}
	return 0;
 }




