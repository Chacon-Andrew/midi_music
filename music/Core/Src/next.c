/*
 * next.c
 *
 *  Created on: Oct 20, 2022
 *      Author: Andre
 */
#include "print.h"
#include "stdint.h"
#include "little_endian.h"
#include "stdio.h"
#include "string.h"

unsigned char* name;
unsigned char* tempo;
unsigned char* copyright;
unsigned char* current_song;
unsigned char current_name[256];
unsigned char current_copy[256];
unsigned char current_tempo[256];

void parse_song_info(unsigned char* current_song, int index, unsigned char *current){
	int length = current_song[index];
	index++;

	for(int i = 0; i < length; i++){
		current[i] = current_song[index];
		index++;
	}
	current[length] = '\0';
}

void parse_meta(unsigned char* c_song, unsigned int size){
	uint8_t valid = 1;
	uint32_t real_tempo;
	current_song = c_song;
	for(int i = 0; i < size; i++){
		if(current_song[i] == 0xFF){
			if(current_song[i+1] == 0x03 && valid == 1){
				parse_song_info(current_song, i + 2, current_name);
				name = current_name;
				valid = 0;
			}
			else if(current_song[i+1] == 0x51){
				parse_song_info(current_song, i + 2, current_tempo);
				tempo = current_tempo;
				real_tempo = convert_to_uint24(tempo);
			}
			else if(current_song[i+1] == 0x02){
				parse_song_info(current_song, i + 2, current_copy);
				copyright = current_copy;
			}
		}
	}
	prints("Title: %s\r\n", name);
	if(copyright[0] == '\0'){
		prints("Copyright: NA\r\n");
	}
	else{
		prints("Copyright: %s\r\n", copyright);
	}

	prints("Tempo: %d\r\n", real_tempo);
	memset(name, 0, 256);
	memset(copyright, 0, 256);
	memset(tempo, 0, 256);
}
