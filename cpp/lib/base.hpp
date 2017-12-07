/*
 * enums.hpp
 *
 *  Created on: 30 Nov 2017
 *      Author: julianporter
 */

#ifndef BASE_HPP_
#define BASE_HPP_

#include <cstdint>
#include <string>
#include <exception>
#include <iostream>
#include <vector>
#include <memory>

namespace pylame {

using data_t = std::vector<char>;
using cdata_t = std::vector<unsigned char>;
using iterator_t = data_t::iterator;
using const_iterator_t = data_t::const_iterator;


	class MP3Error : public std::exception {
	private:
		std::string message;
	public:
		MP3Error(const std::string &message_) noexcept : std::exception(), message(message_) {} ;
		MP3Error(const MP3Error &) = default;
		MP3Error & operator=(const MP3Error &) = default;
		virtual ~MP3Error() = default;
		
		virtual const char *what() const noexcept { return message.c_str(); };
		
		
	};
	


	enum class Mode  {
		Mono,
		Stereo
	};



}

std::ostream & operator<<(std::ostream &o,const pylame::MP3Error &ex);


	
	







#endif /* BASE_HPP_ */
