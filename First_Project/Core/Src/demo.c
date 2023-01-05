/*
 * demo.c
 *
 *  Created on: Feb 14, 2021
 *      Author: larry kiser
 *  Updated on: Sept 6, 2021
 *      Author: Mitesh Parikh
 */

#include <stdio.h>
#include <string.h>

// Custom Include files
#include "LED.h"
#include "UART.h"
#include "demo.h"
#include "stm32l4xx.h"
#include "systick.h"
#include "print.h"
#include "help.h"
#include "next.h"
#include "GPIO.h"
#include "tone.h"
#include "adc.h"
#include "dac.h"
#include "midi.h"
#include "music.h"

#define TIMEOUT (7999999)

//static int clock;
//static char* time;
static uint8_t indexer = 0;
uint32_t counter = 0;
uint8_t player = 0;
static volatile uint8_t one_second_elapsed = 0;
volatile uint8_t one_second = 0;
volatile uint8_t stopper;
uint8_t blink = 0;
uint32_t second_counter = 0;
uint8_t remote = 1;
uint32_t volatile delay_counter = 0;
unsigned char* a_song = 0;
uint32_t size = 0;
static uint8_t p = 0;

//static void demo_of_printf_scanf(){
//	char rxByte;
//	char *prompt = "Give Red LED control input (Y = On, N = off):\r\n" ;
//	char *LED_off = "LED is Off\r\n\r\n" ;
//	char *LED_on = "LED is On\r\n\r\n" ;
//
//	USART_Write( USART2, (uint8_t *)prompt, strlen( prompt ) ) ;
//	scanf ("%c", &rxByte);
//
//	if (rxByte == 'N' || rxByte == 'n'){
//		LED_Off();
//		USART_Write( USART2, (uint8_t *)LED_off, strlen( LED_off ) ) ;
//	}
//	else if (rxByte == 'Y' || rxByte == 'y'){
//		LED_On();
//		USART_Write( USART2, (uint8_t *)LED_on, strlen( LED_on ) ) ;
//	}
//}


// This function is to create a delay by consuming CPU cycle on counter
//static void delay_loop( int value )
//{
//	// spin loop consuming CPU to spend time.
//	for (int i = 0; i < value; i++)
//		;
//}

// This function is to Initialize SysTick registers
static void init_systick()
{
	// Use the SysTick global structure pointer to do the following in this
	// exact order with separate lines for each step:
	//
	// Disable SysTick by clearing the CTRL (CSR) register.
	systick_t* p_systick = get_systick_registers();
	p_systick->SYST_CSR &= ~(0xFFFFFFF);
	// Set the LOAD (RVR) to 8 million to give us a 100 milliseconds timer.
	p_systick->SYST_RVR = 799;
	// Set the clock source bit in the CTRL (CSR) to the internal clock.
	p_systick->SYST_CSR |= 1 << 2;
	// Set the enable bit in the CTRL (CSR) to start the timer.
	p_systick->SYST_CSR |= 1 << 0;

	// Set the interrupt bit to be enabled
	p_systick->SYST_CSR |= 1 << 1;
}

// This fuction is to create delay using SysTick timer counter
//static void delay_systick(int second_scalar)
//{
//	// Using the SysTick global structure pointer do the following:
//	systick_t* p_systick = get_systick_registers();
//	// Create a for loop that loops 10 times
//	while(!(p_systick->SYST_CSR & (1 << 16)));
//	// Inside that for loop check the COUNTFLAG bit in the CTRL (CSR)
//	// register in a loop. When that bit is set exit this inner loop
//	// to do another pass in the outer loop of 10.
//}

// This function is to print counter on UART port and toggle LED
//static void demo_of_UART_print(int counter){
//	prints("Counter = %d\n\r", counter);
//
//	delay_systick(2) ;
//
//	// Toggle LED
//	LED_Toggle();
//}

//static void Register__print(char reg_name[], int value){
//
//	prints("%s = %d\n\r", reg_name, value);
//
//
//}

//char* format_time () {
//	unsigned char hours = clock / 3600 % 24;
//	unsigned char minutes = clock / 60 % 60;
//	unsigned char seconds = clock % 60;
//	prints("%02d:%02d:%02d\r", hours, minutes, seconds);
//	return time;
//}
//
//void print_next_second () {
//	format_time();
//	delay_systick(1);
//	clock++;
//}
//
//void run_simple_clock() {
//	clock = 8 * 3600;
//	time = malloc (9);
//	while(1) {
//		print_next_second();
//	}
//}

/*
 * This will pause or have the light flashing
 * from the interrupt
 */
void pause(){
	blink = 1;
}

/*
 * This will unpause or have the light flashing
 * from the interrupt
 */
void unpause(){
	blink = 0;
}

/*
 * This will help do timer counting for various
 * uses
 */
void SysTick_Handler() {
		counter++;
		second_counter++;
		tone_counter++;
		delay_counter++;
	if(counter == 100000){
		one_second_elapsed = 1;
		one_second = 1;
		counter = 0;
	}
	if(second_counter == 100000){
		stopper = 1;
	}
}

/*
 * This will turn the remote access from putty
 */
void remote_on(){
	remote = 1;
}

/*
 * This will set the mode into more local for a super user
 */
void remote_off(){
	remote = 0;
}

/*
 * This will get the song itself for the interrupts
 */
unsigned char * local_song(int player){
	return songs[player].p_song;
}

/*
 * This will get the size of the song for the interrupts
 */
uint32_t local_song_size(int player){
	return songs[player].size;
}

/*
 * This will play the song from the interrupt
 */
void local_play_song(){
	p = 1;
}

/*
 * This function is the head where it will
 * run the application and getting all of the
 * commands typed as well as handling any interrupts
 */
void run_project(){
	one_second = 0;
	uint8_t letter = 0;
	char buffer[6] = {0};
	help_me();
	prints("\n\r***REMOTE MODE ACTIVE***\n\r");
	GPIO_Init();
	while(1){
		letter = USART_Read_nonblocking( USART2 );
		if(p == 1){
			play_song(a_song, size);
			p = 0;
		}
		if((blink == 1) && (one_second_elapsed == 1)){
			LED_Toggle();
			one_second_elapsed = 0;
		}
		if(remote == 1){
			if(letter == 13){
				indexer = 0;
				prints("%c\n", letter);
				if(strcmp(buffer, "HELP") == 0){
					help_me();
				}
				else if(strcmp(buffer, "PLAY") == 0){
					blink = 0;
					LED_On();
					if(a_song != 0){
						play_song(a_song, size);
					}
				}
				else if(strcmp(buffer, "PAUSE") == 0){
					blink = 1;
				}
				else if(strcmp(buffer, "NEXT") == 0){
					a_song = songs[player].p_song;
					size = songs[player].size;
					parse_meta(a_song, size);
					if(player == 5){
						player = 0;
					}
					else{
						player++;
					}
				}
				else if(strcmp(buffer, "STOP") == 0){
					blink = 0;
					LED_Off();
				}
				else{
					prints("Invalid Command\r\n");
				}
				memset(buffer, 0, sizeof(buffer));
				}
			else if(letter == 127){
				if(indexer > 0){
					indexer--;
					buffer[indexer] = 0;
					prints("%c", letter);
				}
			}
			else if((indexer < 5) && (letter != 0)){
				buffer[indexer] = letter;
				prints("%c", letter);
				indexer++;
			}
		}
	}
}

void Error_Handler(){
	//do nothing
}

void run_demo(){

	init_systick();
	DAC_Init ();
	DAC_Start ();
	run_project();
}
