#include "player.h"
#include <vector>

using namespace thc;
using std::vector;

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
    MoveEval minimaxThread(ChessRules& position, vector<Move>& moves, uint32_t depth, double alpha, double beta);
    double startingEval(bool whiteToPlay);

    static uint32_t m_depth;
};