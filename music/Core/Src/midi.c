/*
 * midi.c
 *
 *  Created on: Nov 19, 2022
 *      Author: Andre
 */
#include "midi.h"
#include "print.h"
#include "stdint.h"
#include "little_endian.h"
#include "stdio.h"
#include "string.h"
#include "demo.h"
#include "tone.h"

// This is a struct to hold all the information for number of tracks, the format, the division and length of a song
typedef struct {
	unsigned char chunk_type[4];
	uint32_t length;
	uint16_t format;
	uint16_t ntrks;
	uint16_t division;

} chunk;

chunk* pheader;
unsigned char* c_song;
static uint32_t tempo;
note notes[3000];
static uint32_t note_incre;
note current_note;
uint32_t time = 0;

/*
 * This function will parse through all of the meta
 * events in a song and will grab the tempo from
 * a specific meta event
 */
uint32_t parse_meta_event(int index){
	int length = c_song[index + 2];
	if(c_song[index + 1] == 0x51){
		index += 3;
		unsigned char t[3];
		for(int i = 0; i < length; i++){
			t[i] = c_song[index];
			index++;
		}
		tempo = convert_to_uint24(t);
	}
	else{
		index += length + 3;
	}
	return index;
}

/*
 * This function will parse through on or off events for the midi
 * mtrk events where it will calculate the variable length delay
 * and grab the note and velocity from each of these events
 */
uint32_t parse_midi_event(int index, uint8_t on_or_off){
	uint32_t delay = 0;
	if(on_or_off == 1){
		uint32_t check = (c_song[index] & 0xFF) >> 7;
		if(check == 1){
			delay += (c_song[index] & 0x0F)<<7;
			while(check == 1){
				index++;
				check = (c_song[index] & 0xFF) >> 7;
				if(check == 1){
					delay += (c_song[index] & 0x0F)<<7;
				}
			}
		}
		delay += c_song[index];
		index++;
		prints("delay: %d\r\n", delay);
		current_note.delay = delay;
		uint8_t c_note = c_song[index];
		current_note.note = c_note;
		current_note.velocity = 0;
		notes[note_incre] = current_note;
		note_incre++;
		index++;
	}
	else{
		delay = c_song[index - 1];
		index ++;
		prints("delay: %d\r\n", delay);
		uint8_t c_note = c_song[index];
		index ++;
		current_note.delay = delay;
		current_note.note = c_note;
		current_note.velocity = c_song[index];
		notes[note_incre] = current_note;
		note_incre++;
	}
	return index;
}

/*
 * This function will get the data for the length,
 * number of tracks, the division, and the division
 * of the song to fill the pheader struct
 */
void parse_header(){
	pheader = (chunk*)c_song;

	uint32_t *len = &(pheader->length);
	uint32_t length = convert_to_uint32((unsigned char*)len);
	pheader->length = length;

	uint16_t *div = &(pheader->division);
	uint16_t division = convert_to_uint16((unsigned char*)div);
	pheader->division = division;

	uint16_t *form = &(pheader->format);
	uint16_t format = convert_to_uint16((unsigned char*)form);
	pheader->format = format;

	uint16_t *nt = &(pheader->ntrks);
	uint16_t ntrks = convert_to_uint16((unsigned char*)nt);
	pheader->ntrks = ntrks;
}

/*
 * This function will go through byte by byte
 * and do the necessary parsing if it is a
 * meta event or a midi event
 */
void parse_song(uint32_t size){
	parse_header();
	note_incre = 0;
	for(int i = 0; i < size; i++){
		if(c_song[i] == 0x4d){
			i += 8;
		}
		else if(c_song[i] == 0xFF){
			int val = parse_meta_event(i);
			i = val - 1;
		}
		else{
			int k;
			switch(c_song[i] >> 4){
			case 0x9:
				k = parse_midi_event(i, 0);
				i = k;
				break;
			case 0xE:
				i += 3;
				break;
			case 0xC:
				i += 2;
				break;
			case 0xB:
				i += 3;
				break;
			case 0xD:
				i += 2;
				break;
			case 0xA:
				i += 3;
				break;
			case 0x8:
				k = parse_midi_event(i, 1);
				i = k;
			}
		}
	}
}

/*
 * This function will be used to play the song itself
 * and play a note for its duration for each note that
 * was parsed
 */
uint32_t play_song(unsigned char * song, uint32_t size){
	c_song = song;
	parse_song(size);
	time = (tempo / pheader->division) * 10;
	int index = 0;
	while(index < note_incre){
		add_tone(notes[index].note, notes[index].velocity);
		delay_counter = 0;
		while(delay_counter < (time + notes[index].delay)){
			play_tones();
		}
		remove_tone(notes[index].note);
		index++;
	}
	return note_incre;
}
