/*
 * enums.cpp
 *
 *  Created on: 30 Nov 2017
 *      Author: julianporter
 */


#include "enums.hpp"

std::ostream & operator<<(std::ostream &o,const MP3Error &ex) {
	o << "MP3Error : " << ex.message;
	return o;
}



