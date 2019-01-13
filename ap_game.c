/*********************************************************
* FILE: ap_game.c
*
* DESC: Contains all the core logic for the anti-pawn game.
*
* AUTHOR: Michael Lindner-D'Addario
* DATE: January 13 2019
***********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <limits.h> // INT_MAX, INT_MIN
#include "ap_game.h"

// CURRENTLY NO CODE HANDLES IF A POSITION IS FROZEN //

// SEE HEADER FILE FOR FUNCTION DESCRIPTIONS //

game* game_init(){

	game* ap = (game*)malloc(sizeof(game));

	ap->ply = 1;
	ap->winner = 0;
	ap->isActive = 1;

	// Set empty squares
	for (int i = 0; i < DIMENSION - 1; i++)
		for (int j = 0; j < DIMENSION; j++){
			ap->white_array[i][j] = 0;
			ap->black_array[DIMENSION-i-1][j] = 0;
		}

	// Fill starting ranks
	for (int j = 0; j < DIMENSION; j++){
		ap->white_array[DIMENSION-1][j] = 1;
		ap->black_array[0][j] = 1;
	}
	return ap;
}

void print_game(game* ap){

	printf("\nCurrent board:\n");

	for (int i = 0; i < DIMENSION; i++){
		printf("[");

		for (int j = 0; j < DIMENSION; j++){

			if (ap->white_array[i][j])
				printf(" W ");
			else if (ap->black_array[i][j])
				printf(" B ");
			else
				printf(" . ");
		}
		printf("]\n");
	}
	printf("Current ply: \t%d\n", ap->ply);
	if (!ap->isActive)
		printf("GAME OVER\n");

	return;
}

int make_move(game* ap, int row_i, int col_i, int row_f, int col_f){

	// Check for invalid moves
	if (!ap->isActive)
		return -3;
	if (row_i < 0 || col_i < 0 || row_f < 0 || col_f < 0)
		return -2;
	if (row_i >= DIMENSION || col_i >= DIMENSION || row_f >= DIMENSION || col_f >= DIMENSION)
		return -2;

	// White turn
	if (ap->ply % 2){

		// Validate move
		if (!ap->white_array[row_i][col_i])
			return -1;
		if (row_i - row_f != 1)
			return -1;

		// Capture
		if (col_i == col_f){

			// Validate move
			if (!ap->black_array[row_f][col_f])
				return -1;

			// Perform move
			ap->white_array[row_i][col_i] = 0;
			ap->white_array[row_f][col_f] = 1;
			ap->black_array[row_f][col_f] = 0;
			return check_end(ap);
		}
		// Advance
		else if ((col_i - col_f) == 1 || (col_f - col_i) == 1){

			// Validate move
			if ((ap->white_array[row_f][col_f]) || (ap->black_array[row_f][col_f]))
				return -1;

			// Perform move
			ap->white_array[row_i][col_i] = 0;
			ap->white_array[row_f][col_f] = 1;
			return check_end(ap);
		}
		// Invalid move
		else
			return -1;
	}

	// Black turn
	else{

		// Validate move
		if (!ap->black_array[row_i][col_i])
			return -1;
		if (row_f - row_i != 1)
			return -1;

		// Capture
		if (col_i == col_f){

			// Validate move
			if (!ap->white_array[row_f][col_f])
				return -1;

			// Perform move
			ap->black_array[row_i][col_i] = 0;
			ap->black_array[row_f][col_f] = 1;
			ap->white_array[row_f][col_f] = 0;
			return check_end(ap);
		}
		// Advance
		else if ((col_i - col_f) == 1 || (col_f - col_i) == 1){

			// Validate move
			if ((ap->white_array[row_f][col_f]) || (ap->black_array[row_f][col_f]))
				return -1;

			// Perform move
			ap->black_array[row_i][col_i] = 0;
			ap->black_array[row_f][col_f] = 1;
			return check_end(ap);
		}
		// Invalid move
		else
			return -1;
	}
	// Check for game end
	return check_end(ap);
}

int check_end(game* ap){

	// Check white
	if (ap->ply % 2){

		ap->ply++;
		for (int j = 0; j < DIMENSION; j++)
			if (ap->white_array[0][j] == 1){
				ap->isActive = 0;
				ap->winner = 1;
				return 1;
			}
	}
	// Check black
	else{

		ap->ply++;
		for (int j = 0; j < DIMENSION; j++)
			if (ap->black_array[DIMENSION-1][j] == 1){
				ap->isActive = 0;
				ap->winner = 2;
				return 2;
			}
	}
	return 0;
}

void rollback(game* ap, int row_i, int col_i, int row_f, int col_f){

	ap->ply--;
	ap->winner = 0;
	ap->isActive = 1;

	// White turn
	if (ap->ply % 2){

		// Capture
		if (col_i == col_f){

			// Undo move
			ap->white_array[row_i][col_i] = 1;
			ap->white_array[row_f][col_f] = 0;
			ap->black_array[row_f][col_f] = 1;
			return;
		}
		// Advance
		else{

			// Undo move
			ap->white_array[row_i][col_i] = 1;
			ap->white_array[row_f][col_f] = 0;
			return;
		}
	}

	// Black turn
	else{

		// Capture
		if (col_i == col_f){

			// Undo move
			ap->black_array[row_i][col_i] = 1;
			ap->black_array[row_f][col_f] = 0;
			ap->white_array[row_f][col_f] = 1;
			return;
		}
		// Advance
		else{

			// Undo move
			ap->black_array[row_i][col_i] = 1;
			ap->black_array[row_f][col_f] = 0;
			return;
		}
	}
	return;
}

int crude_moves(game* ap, int** row_i, int** col_i, int** row_f, int** col_f){

	if (!ap->isActive)
		return 0;

	int num_moves = 0;

	// Every piece can do a maximum of 3 moves
	// Maximum of 2 * DIMENSION pieces
	*row_i = (int*)malloc(6 * DIMENSION * sizeof(int));
	*col_i = (int*)malloc(6 * DIMENSION * sizeof(int));
	*row_f = (int*)malloc(6 * DIMENSION * sizeof(int));
	*col_f = (int*)malloc(6 * DIMENSION * sizeof(int));

	// White turn
	if (ap->ply % 2){

		for (int i = 1; i < DIMENSION; i++)
			for (int j = 0; j < DIMENSION; j++)

				if (ap->white_array[i][j])

					for (int k = j-1; k <= j+1; k++){

						(*row_i)[num_moves] = i;
						(*col_i)[num_moves] = j;
						(*row_f)[num_moves] = i-1;
						(*col_f)[num_moves] = k;
						num_moves++;
					}
	}
	// Black turn
	else{

		for (int i = 0; i < DIMENSION-1; i++)
			for (int j = 0; j < DIMENSION; j++)

				if (ap->black_array[i][j])

					for (int k = j-1; k <= j+1; k++){

						(*row_i)[num_moves] = i;
						(*col_i)[num_moves] = j;
						(*row_f)[num_moves] = i+1;
						(*col_f)[num_moves] = k;
						num_moves++;
					}
	}
	return num_moves;
}

int heuristic_eval(game* ap){

	// Check game over
	if (!ap->isActive){

		if (ap->winner == 1)
			return INT_MAX;
		else
			return INT_MIN;
	}

	int evaluation = 0;

	// Determine value of each player's position
	for (int i = 0; i < DIMENSION; i++)
		for (int j = 0; j < DIMENSION; j++){

			if (ap->white_array[i][j])
				evaluation += (DIMENSION - i)*(DIMENSION - i);

			else if(ap->black_array[i][j])
				evaluation -= (i + 1)*(i + 1);
		}
	return evaluation;
}





/*
RUINS OF OLD TTT CODE
node* node_init(){

	node* nd = (node*)malloc(sizeof(node));

	nd->value = 0;
	nd->isTerminal = 0;
	for (int i = 0; i < DIMENSION; i++)
		for (int j = 0; j < DIMENSION; j++)
			nd->children[i][j] = NULL;

	return nd;
}
*/