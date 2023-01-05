/*
 * handlers.c
 *
 *  Created on: Nov 1, 2022
 *      Author: Andre
 */

#include "print.h"
#include <stdbool.h>
#include "stm32l4xx.h"
#include "GPIO.h"
#include "demo.h"
#include "next.h"
#include "LED.h"
#include "systick.h"
#include "midi.h"

int flag = 0;
int switched = 1;
int down = 0;
int check = 0;
int presses = 0;
int play = 1;
int local = 0;


void EXTI15_10_IRQHandler(){
	GPIO_PinState state = HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin);
	HAL_GPIO_TogglePin(B1_GPIO_Port, B1_Pin);
	while(state != switched){
		state = HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin);
	}
	if(state == 1){
		switched = 0;
		remote_off();
		local = 1;
		prints("\n\r***MANUAL OVERRIDE MODE ACTIVE***\n\r");
	}
	if(state == 0){
		switched = 1;
		remote_on();
		local = 0;
		LED_Off();
		unpause();
		prints("\n\r***REMOTE MODE ACTIVE***\n\r");
	}
	HAL_GPIO_EXTI_IRQHandler(B1_Pin);
}

void EXTI9_5_IRQHandler(){
	if(local == 1){
		GPIO_PinState state = HAL_GPIO_ReadPin(S1_GPIO_Port, S1_Pin);
		if(state == 1){
			if(presses == 0){
				one_second = 0;
				counter = 0;
			}
			presses++;
			stopper = 0;
			second_counter = 0;
			HAL_GPIO_EXTI_IRQHandler(S1_Pin);
		}
		if(state == 0){
			if(presses >= 2 && one_second == 0){
				presses = 0;
				a_song = local_song(player);
				size = local_song_size(player);
				parse_meta(a_song, size);
				if(player == 5){
					player = 0;
				}
				else{
					player++;
				}
			}
			else if(stopper == 1){
				LED_Off();
				unpause();
				play = 1;
				presses = 0;
				one_second = 0;
				second_counter = 0;
				counter = 0;
			}
			else{
				LED_On();
				if(one_second == 1){
					one_second = 0;
					presses = 0;
				}
				if(play == 1){
					unpause();
					LED_On();
					play = 0;
					local_play_song();
				}
				else{
					pause();
					play = 1;
				}
			}
			HAL_GPIO_EXTI_IRQHandler(S1_Pin);
		}
	}
	HAL_GPIO_EXTI_IRQHandler(S1_Pin);
}
