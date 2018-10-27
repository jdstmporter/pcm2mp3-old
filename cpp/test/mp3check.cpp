/*
 * mp3.cpp
 *
 *  Created on: 23 Apr 2018
 *      Author: julianporter
 */

#include <iostream>
#include <fstream>
#include "mp3info.hpp"
#include "ArgParseStandalone.h"

int main(int argc,char *argv[]) {
	ArgParse::ArgParser parser("MP3 file verification and analysis");
	bool verbose = false;
	std::string infile = "";

	parser.AddArgument("-v/--verbose","Produce verbose output",&verbose,ArgParse::Argument::Optional);
	parser.AddArgument("-i/--infile","File to analyse",&infile,ArgParse::Argument::Optional);

	if(parser.ParseArgs(argc,argv)<0) {
		std::cerr << "Cannot parse arguments correctly" << std::endl;
		return -1;
	}
	if(parser.HelpPrinted()) return 0;



	try {
		std::cout << "Analysing " << infile;
		if(verbose) std::cout << " with verbose output";
		std::cout << std::endl;

		mp3::MP3Test test(infile);
		test.parse(verbose);
		auto result=test();
		std::cout << *result << std::endl;
	}
	catch(std::exception &e) {
			std::cerr << "ERROR: " << e.what() << std::endl;
		}
		return 0;
}



