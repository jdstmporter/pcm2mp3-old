/*
 * enums.cpp
 *
 *  Created on: 30 Nov 2017
 *      Author: julianporter
 */


#include "base.hpp"

std::ostream & operator<<(std::ostream &o,const pylame::MP3Error &ex) {
	o << "MP3Error : " << ex.what();
	return o;
}



