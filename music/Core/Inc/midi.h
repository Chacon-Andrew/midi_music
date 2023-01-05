/*
 * midi.h
 *
 *  Created on: Nov 19, 2022
 *      Author: Andre
 */

#ifndef INC_MIDI_H_
#define INC_MIDI_H_

#include "stdint.h"
//This is the note struct to show the information needed to play the note
typedef struct {
	uint32_t delay;
	uint8_t note;
	uint8_t velocity;
} note;

uint32_t parse_meta_event(int index);
uint32_t parse_midi_event(int index, uint8_t on_or_off);
void parse_header();
void parse_song(uint32_t size);
uint32_t play_song(unsigned char* c_song, uint32_t size);



#endif /* INC_MIDI_H_ */
