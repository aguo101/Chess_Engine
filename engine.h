#include "player.h"

using namespace thc;

struct MoveEval {
    Move move;
    double eval;
};

class Engine : public Player {
    public:

    Engine();

    double evaluatePosition(ChessPosition& position);

    Move getMove(ChessRules& position);

    private:

    MoveEval minimax(ChessRules& position, uint32_t depth, double alpha, double beta);

    static uint32_t m_depth;
};