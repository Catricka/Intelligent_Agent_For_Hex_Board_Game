
#ifndef HEXBOARD_H
#define	HEXBOARD_H
#include "Graph.h"
#include "State.h"

const int SIZE = 11;

class HexBoard {
public:

    HexBoard();
    HexBoard(Graph &G, State S, int turns);

    void print();
    void putPiece(int x, int y);
    void reset();
    void playNext();
    void playComputer(int level);

    State getState();

    State::Player hasWon();
    State::Player hasWon(State &state);
private:
    State S;
    Graph G;
    State::Player Player;

    static inline int getNode(int row, int col);
    static inline int getCol(int x);
    static inline int getRow(int x);

    void switchPlayer();
    std::vector<int> getNeighboursOf(int i, State &state, State::Player wanted, std::vector<bool> &visited);
    void checkWon(State &state, State::Player wanted, int current, std::vector<bool> &visited, bool &won);

};

#endif	/* HEXBOARD_H */

