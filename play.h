#ifndef CHESS_PLAY
#define CHESS_PLAY

#include "thc.h"
#include "player.h"

using namespace thc;

class Play {
    public:
    Play();
    ~Play();
    Play(Player* w, Player* b);
    bool move();
    void display();
    
    private:
    ChessRules* board;
    Player* white;
    Player* black;
};

#endif