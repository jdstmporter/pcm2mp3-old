/*
 * CRC16.hpp
 *
 *  Created on: 21 Oct 2018
 *      Author: julianporter
 */

#ifndef CRC16_HPP_
#define CRC16_HPP_

#include <cstdint>
#include <vector>

namespace pylame {
namespace mp3 {

enum class CRC16Polynomial : uint32_t {
	CRC_16 = 0x18005,
	CCITT  = 0x11021
};

class CRC16 {
private:
	uint16_t table[256];
	uint16_t value;
	uint32_t poly;

	uint32_t step(const uint32_t) const;

public:
	CRC16(const CRC16Polynomial p);
	virtual ~CRC16() = default;

	void reset(const uint16_t init=0) noexcept { value = init; }
	void operator()(const char);
	uint16_t operator()() const { return value; }


};

} /* namespace mp3 */
} /* namespace pylame */

#endif /* CRC16_HPP_ */
