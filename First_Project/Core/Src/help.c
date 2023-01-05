/*
 * help.c
 *
 *  Created on: Oct 19, 2022
 *      Author: Andre
 */


#include "help.h"
#include "print.h"

void help_me(){
	prints("***REMOTE LED CONTROL MENU***\r\n");
	prints("Available User Commands\r\n");
	prints("NEXT - Show next song info\r\n");
	prints("PLAY - Play the song (LED on)\r\n");
	prints("PAUSE - Pause the song (LED flash)\r\n");
	prints("STOP - Stop the song (LED off)\r\n");
}
