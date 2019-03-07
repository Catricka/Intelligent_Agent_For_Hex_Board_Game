

#include "State.h"
#include <algorithm>
using namespace std;

/* Sets the colour of a given vertex. */
void State::set_hex_colour(int vertex, Player c) {
    piece[vertex] = c;
    turns++;
}

/* Gets the colour of a given vertex. */
State::Player State::get_hex_colour(int vertex) {
    return piece[vertex];
}
/* Sets the number of turns played */
void State::setTurns(int turns) {
    this->turns = turns;
}

/* Gets the number of turns played */
int State::getTurns() {
    return turns;
}

/* Returns the next move */
int State::getNextMove() {
    int move = moves.back();
    moves.pop_back();
    return move;
}

/* Fills the moves vector with all possible moves */
void State::fillMoves() {
    for (int i = 1; i <= (11 * 11); i++) {
        if (get_hex_colour(i) == State::BLANK) {
            moves.push_back(i);
        }
    }
}

/* Shuffles the moves vector */
void State::shuffleMoves() {
    std::random_shuffle(moves.begin(), moves.end());
}

/* Returns true if there are no more moves left */
bool State::movesEmpty() {
    return moves.empty();
}