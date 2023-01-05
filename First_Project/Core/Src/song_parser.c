/*
 * song.c
 *
 *  Created on: Oct 6, 2022
 *      Author: Andre
 */



#include "print.h"
#include "stdint.h"
#include "little_endian.h"


//void parse_song(unsigned char* song){
//	chunk* pheader = song;
//	prints("Chunk type: %c", pheader->chunk_type[0]);
//	prints("%c", pheader->chunk_type[1]);
//	prints("%c", pheader->chunk_type[2]);
//	prints("%c\r\n", pheader->chunk_type[3]);
//
//	uint8_t a = (pheader->length & 0xFF);
//	uint8_t b = (pheader->length >> 8) & 0xFF;
//	uint8_t c = (pheader->length >> 16) & 0xFF;
//	uint8_t d = (pheader->length >> 24) & 0xFF;
//	uint8_t length_bytes[4] = {a, b, c, d};
//
//	uint32_t length = convert_to_uint32(length_bytes);
//	prints("Length: %d\r\n", length);
//
//	uint8_t e = (pheader->format & 0xFF);
//	uint8_t f = (pheader->format >> 8) & 0xFF;
//	uint8_t format_bytes[2] = {e, f};
//
//	uint16_t format = convert_to_uint16(format_bytes);
//	prints("format: %d\r\n", format);
//
//	uint8_t g = (pheader->ntrks & 0xFF);
//	uint8_t h = (pheader->ntrks >> 8) & 0xFF;
//	uint8_t trks_bytes[2] = {g, h};
//
//	uint16_t ntrks = convert_to_uint16(trks_bytes);
//	prints("number of tracks: %d\r\n", ntrks);
//
//	uint8_t i = (pheader->division & 0xFF);
//	uint8_t j = (pheader->division >> 8) & 0xFF;
//	uint8_t division_bytes[2] = {i, j};
//
//	uint16_t division = convert_to_uint16(division_bytes);
//	prints("division: %d\r\n", division);
//}
