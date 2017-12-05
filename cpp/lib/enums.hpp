/*
 * enums.hpp
 *
 *  Created on: 30 Nov 2017
 *      Author: julianporter
 */

#ifndef ENUMS_HPP_
#define ENUMS_HPP_

#include <cstdint>
#include <string>
#include <exception>
#include <iostream>
#include <vector>
#include <memory>

using data_t = std::vector<char>;
using cdata_t = std::vector<unsigned char>;


	class MP3Error : public std::exception {
	friend std::ostream & operator<<(std::ostream &o,const MP3Error &ex);
	private:
		std::string message;
	public:
		MP3Error(const std::string &message_) noexcept : std::exception(), message(message_) {} ;
		MP3Error(const MP3Error &) = default;
		MP3Error & operator=(const MP3Error &) = default;
		virtual ~MP3Error() = default;
		
		virtual const char *what() const noexcept { return message.c_str(); };
		
		
	};
	
	std::ostream & operator<<(std::ostream &o,const MP3Error &ex);

	enum class Mode  {
		Mono,
		Stereo
	};


	
	







#endif /* ENUMS_HPP_ */
