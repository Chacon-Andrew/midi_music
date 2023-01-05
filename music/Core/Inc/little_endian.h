/*
 * little_endian.h
 *
 *  Created on: Oct 10, 2022
 *      Author: Andre
 */

#ifndef INC_LITTLE_ENDIAN_H_
#define INC_LITTLE_ENDIAN_H_

#include "stdint.h"


uint16_t convert_to_uint16(unsigned char* p_value);
uint32_t convert_to_uint24(unsigned char* ptr);
uint32_t convert_to_uint32(unsigned char* p_value);


#endif /* INC_LITTLE_ENDIAN_H_ */
