// Minimax algorithm to determine evaluation of given position
// in 2-player game of perfect information
int minimax(game*, int);
/* Return codes:
 * INT_MAX: Forced win for white
 * INT_MIN: Forced win for black
 * 		>0: In favour of white
 * 		<0: In favour of black
 * 		=0: Even position
 */

// Maximum function
int max(int, int);

// Minimum function
int min(int, int);

// Raw play against computer
void play_against_comp(game*, int);









/*
RUINS OF TTT CODE
// Generate tree, calculate time, and print outcome
node* tree_generation(int isDebug);

// Play game against perfect opponent
void play_against_cpu(game* ttt, node* nd, int is_CPU_P1);
*/