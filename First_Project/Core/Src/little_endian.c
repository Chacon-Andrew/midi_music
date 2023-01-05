/*
 * little_endian.c
 *
 *  Created on: Oct 10, 2022
 *      Author: Andre
 */


#include "little_endian.h"
#include "stdint.h"

uint16_t convert_to_uint16(unsigned char* p_value){
	uint8_t first = p_value[0];
	uint8_t second = p_value[1];
	return (first << 8) + second;
}

uint32_t convert_to_uint24(unsigned char* ptr){
	return (uint32_t)((ptr[0] << 16) + (ptr[1] << 8) + ptr[2]);
}

uint32_t convert_to_uint32(unsigned char* p_value){
	uint8_t first = p_value[0];
	uint8_t second = p_value[1];
	uint8_t third = p_value[2];
	uint8_t fourth = p_value[3];
	return (first << 24) + (second >> 16) + (third >> 8) + fourth;
}
