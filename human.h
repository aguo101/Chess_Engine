#include "player.h"

using namespace thc;

class Human : public Player {
    public:
    Move getMove(ChessRules& position);
};