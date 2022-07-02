#ifndef CHESS_PLAYER
#define CHESS_PLAYER

#include "thc.h"
using namespace thc;

class Player {
    public:
    Player();
    
    virtual Move getMove(ChessRules& position) {
        Move mv;
        return mv;
    }

    private:

};

#endif