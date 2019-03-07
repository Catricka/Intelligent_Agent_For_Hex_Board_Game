

#include "MonteCarlo.h"
#include <ctime>
#include <cstdlib>

using namespace std;

/*
 * Calculates milliseconds elapsed between two clock_t instances.
 */
double diffclock(clock_t clock1, clock_t clock2) {
    double diffticks = clock1 - clock2;
    double diffms = (diffticks * 1000) / CLOCKS_PER_SEC;
    return diffms;
}

/* Sets the level of difficulty of the AI */
void MonteCarlo::setLevel(int i){
	NUMGAMES = i;
}
/* Returns the best move as integer on the board 
 */
int MonteCarlo::getBestMove() {

    int bestResult = 0;
    int bestMove = 0;
    srand(time(NULL));

    //clock_t begin = clock();
    for (int i = 1; i <= (SIZE * SIZE); i++) {
        State S = HB->getState();
        if (S.get_hex_colour(i) == State::BLANK) {
            S.set_hex_colour(i, State::COMPUTER);

            int currentResult = this->numberOfWins(S);
            if (currentResult > bestResult) {
                bestResult = currentResult;
                bestMove = i;
            }
        }
    }
    //clock_t end = clock();
    //cout << "Time elapsed: " << double(diffclock(end, begin)) << " ms" << endl;

    return bestMove;
}

/* Returns the number of games won from a given state. */
int MonteCarlo::numberOfWins(State S) {

    int result = 0;
    S.fillMoves();
    for (int i = 0; i < NUMGAMES; i++) {
        result += GameResult(S, State::HUMAN);
    }
    return result;

}

/* Simulates NUMGAMES games on a given state for a given beginning player. 
 */
int MonteCarlo::GameResult(State S, State::Player player) {

    S.shuffleMoves();

    while (!S.movesEmpty()) {
        int randomMove = S.getNextMove();
        S.set_hex_colour(randomMove, player);
        player = (player == State::HUMAN ? State::COMPUTER : State::HUMAN);
    }

    State::Player winner = HB->hasWon(S);

    if (winner == State::COMPUTER) {
        return 1;
    } else if (winner == State::HUMAN) {
        return 0;
    }
}

