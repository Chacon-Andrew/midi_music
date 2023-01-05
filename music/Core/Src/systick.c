/*
 * systick.c
 *
 *  Created on: Sep 21, 2022
 *      Author: Andre
 */

#include "systick.h"

systick_t* get_systick_registers(){
	return (systick_t*)SYST;
}
