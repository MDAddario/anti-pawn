/*********************************************************
* FILE: main.c
*
* DESC: The main file where you setup a game of anti-pawn
*		versus the computer. 
*
* AUTHOR: Michael Lindner-D'Addario
* DATE: January 13 2019
***********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "ap_game.h"
#include "minimax.h"

int main(){

	// Play as white
	play_against_comp(NULL, 1);

	return 0;
}