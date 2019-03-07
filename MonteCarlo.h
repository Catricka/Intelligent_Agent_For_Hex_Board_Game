

#ifndef MONTECARLO_H_
#define MONTECARLO_H_
#include "HexBoard.h"


class MonteCarlo {
public:

    MonteCarlo(HexBoard *HB) : HB(HB) {
    };
    int getBestMove();
    void setLevel(int i);
private:
    int GameResult(State S, State::Player player);
    int numberOfWins(State S);
    int NUMGAMES;
    HexBoard *HB;
};


#endif /* MONTECARLO_H_ */
