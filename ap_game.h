/* DESCRIPTION OF ANTI-PAWN GAME
 *
 * Anti-pawn is a simple game with the goal of getting one of
 * your pieces to the other end of the board. Each player starts
 * with DIMENSION pawns all lined up on their side of the board.
 * Players take turns moving their anti-pawns, either moving a
 * pawn forward, diagonally by 1 square, or they can capture an
 * opposing pawn that is placed directly in front of theirs. The
 * game ends immediately once a pawn reaches the final rank. If
 * a player cannot make a move, they lose.
 *
 *
 * STARTING BOARD LAYOUT (DIMENSION = 4)
 *
 * [ BLACK BLACK BLACK BLACK]
 *
 * [ ----- ----- ----- -----]
 *
 * [ ----- ----- ----- -----]
 *
 * [ WHITE WHITE WHITE WHITE]
 *
 */

// Struct for anti pawn game
typedef struct ap_game_struct{

	int ply;
	int winner;
	int isActive;
	int white_array[DIMENSION][DIMENSION];
	int black_array[DIMENSION][DIMENSION];

} game;

// Initialize game object
game* game_init();

// Print the board to terminal
void print_game(game*);

// Perform a move and return outcome code, does not increment ply
int make_move(game*, int, int, int, int);
/* Return codes:
 * -3: Illegal move (game inactive)
 * -2: Illegal move (out of bounds)
 * -1: Illegal move (against rules)
 *  0: Legal move, game continues
 *  1: Player 1 wins (white wins)
 *  2: Player 2 wins (black wins)
 */

// Check to see if game has ended, increment ply, update game state
int check_end(game*);
/* Return codes:
 *  0: Game continues
 *  1: Player 1 wins (white wins)
 *  2: Player 2 wins (black wins)
 */

// Revoke a move, decrement ply, revert game state
void rollback(game*, int, int, int, int);

// Returns number of potential moves, and associated arrays of moves
int crude_moves(game*, int**, int**, int**, int**);
/* Return code:
 * Number of possible moves
 */

// Returns a heuristic evaluation of the given position
int heuristic_eval(game*);
/* Return codes:
 * INT_MAX: White victory
 * INT_MIN: Black victory
 * 		>0: In favour of white
 * 		<0: In favour of black
 * 		=0: Even position
 */
