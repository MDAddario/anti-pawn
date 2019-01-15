// Minimax algorithm to determine evaluation of given position
// in 2-player game of perfect information. Alpha beta pruning.
int minimax(game*, int, int, int);
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

// Witness two computers fight
void computer_brawl(game*);

// See how long algorithm takes
void time_benchmark();
