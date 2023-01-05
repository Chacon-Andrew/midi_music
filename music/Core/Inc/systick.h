/*
 * systick.h
 *
 *  Created on: Sep 13, 2022
 *      Author: Andre
 */

#ifndef INC_SYSTICK_H_
#define INC_SYSTICK_H_

#define SYST (0xE000E010)

typedef struct {
	unsigned int SYST_CSR;
	unsigned int SYST_RVR;
	unsigned int SYST_CVR;
	unsigned int SYST_CALIB;
} systick_t;

systick_t* get_systick_registers();

#endif /* INC_SYSTICK_H_ */
