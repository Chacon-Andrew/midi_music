// SWEN 340 Demo code header file
// Larry Kiser copyright 2021



/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DEMO_H
#define __DEMO_H

extern volatile uint8_t one_second;

extern uint32_t counter;

extern volatile uint8_t stopper;

extern uint32_t second_counter;

extern uint8_t player;

extern volatile uint32_t delay_counter;

extern uint32_t time;

extern unsigned char* a_song;

extern uint32_t size;

void pause();
void unpause();
void SysTick_Handler();
void run_demo( void ) ;
void remote_on();
void remote_off();
unsigned char * local_song(int player);
uint32_t local_song_size(int player);
void local_play_song();

#endif /* __DEMO_H */
