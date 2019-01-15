/*********************************************************
* FILE: minimax.c
*
* DESC: Minimax algorithm used for anti-pawn game.
*		Uses alpha-beta pruning.
*
* AUTHOR: Michael Lindner-D'Addario
***********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <limits.h> // INT_MAX, INT_MIN
#include <time.h>
#include "ap_game.h"
#include "minimax.h"

// See header file for function descriptions //

// RETURN PRINCIPLE VARIATION //

int minimax(game* ap, int depth, int alpha, int beta){

	// Check if leaf node
	if (depth == 0 || (!ap->isActive))
		return heuristic_eval(ap);

	// Consider all possible moves
	int num_moves, value;
	int *row_i, *col_i, *row_f, *col_f;
	num_moves = crude_moves(ap, &row_i, &col_i, &row_f, &col_f);

	// White to move
	if (ap->ply % 2){

		value = INT_MIN;

		for (int k = 0; k < num_moves; k++){

			// Ignore illegal moves
			if(make_move(ap, row_i[k], col_i[k], row_f[k], col_f[k]) < 0)
				continue;

			// Pick best move
			value = max(value, minimax(ap, depth-1, alpha, beta));
			alpha = max(alpha, value);
			
			// Rollback move
			rollback(ap, row_i[k], col_i[k], row_f[k], col_f[k]);

			// Pruning
			if (alpha >= beta)
				break;
		}
	}
	// Black to move
	else{

		value = INT_MAX;

		for (int k = 0; k < num_moves; k++){

			// Ignore illegal moves
			if(make_move(ap, row_i[k], col_i[k], row_f[k], col_f[k]) < 0)
				continue;

			// Pick best move
			value = min(value, minimax(ap, depth-1, alpha, beta));
			beta = min(beta, value);
			
			// Rollback move
			rollback(ap, row_i[k], col_i[k], row_f[k], col_f[k]);

			// Pruning
			if (alpha >= beta)
				break;
		}
	}
	free(row_i);
	free(col_i);
	free(row_f);
	free(col_f);
	return value;
}

int max(int a, int b){

	if (a > b)
		return a;
	return b;
}

int min(int a, int b){

	if (a < b)
		return a;
	return b;
}

void play_against_comp(game* ap, int human_P1){

	// Create game if start
	if (ap == NULL){
		ap = game_init();
	}

	print_game(ap);

	// Ensure game not over
	if (!ap->isActive)
		return;

	int row_i_me, col_i_me, row_f_me, col_f_me;

	// Player turn
	if (human_P1){

		printf("Its the players turn!\n");
		printf("Please input initial row, col, ending row, col:\n");
		scanf("%d %d %d %d", &row_i_me, &col_i_me, &row_f_me, &col_f_me);

		while(make_move(ap, row_i_me, col_i_me, row_f_me, col_f_me) < 0){
			printf("Please input a valid move:\n");
			scanf("%d %d %d %d", &row_i_me, &col_i_me, &row_f_me, &col_f_me);
		}
	}
	// CPU turn
	else{

		// Force computer as black
		if (ap->ply % 2){
			printf("Computer must play bLack. Closing.\n");
			return;
		}

		printf("Its the CPUs turn!\n");

		// Find all moves
		int num_moves;
		int *row_i, *col_i, *row_f, *col_f;
		num_moves = crude_moves(ap, &row_i, &col_i, &row_f, &col_f);

		int value = INT_MAX;
		int temp;

		for (int k = 0; k < num_moves; k++){

			// Ignore illegal moves
			if (make_move(ap, row_i[k], col_i[k], row_f[k], col_f[k]) < 0)
				continue;

			// Try out move
			temp = minimax(ap, MAX_DEPTH, INT_MIN, INT_MAX);

			// Find best move
			if (temp < value){

				value = temp;
				row_i_me = row_i[k];
				col_i_me = col_i[k];
				row_f_me = row_f[k];
				col_f_me = col_f[k];
			}

			// Rollback move and repeat
			rollback(ap, row_i[k], col_i[k], row_f[k], col_f[k]);
		}
		// Commit to best move
		printf("%d %d %d %d\n", row_i_me, col_i_me, row_f_me, col_f_me);
		make_move(ap, row_i_me, col_i_me, row_f_me, col_f_me);
	}
	play_against_comp(ap, !human_P1);
	return;
}

void time_benchmark(){

	// Make game
	game* ap = game_init();

	// Time the algorithm
	clock_t start = clock(), diff;
	printf("Minimax result is: %d\n", minimax(ap, MAX_DEPTH, INT_MIN, INT_MAX));
	diff = clock() - start;
	int msec = diff * 1000 / CLOCKS_PER_SEC;

	// Print output
	printf("Board size: %d\n", DIMENSION);
	printf("Max depth: %d\n", MAX_DEPTH);
	printf("Time taken: %d hours %d minutes %d seconds %d milliseconds\n", 
		msec/3600000, msec/60000, msec/1000, msec%1000);

	// Care for the environment
	free(ap);

	return;
}
