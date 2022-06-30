#include <memory>
#include "thc.h"

using namespace thc;

struct MoveEval {
    Move move;
    double eval;
};

class Engine {
    public:

    Engine();

    double evaluatePosition(ChessPosition& position);

    Move getMove(ChessRules& position);

    private:

    std::unique_ptr<MoveEval> minimax(ChessRules& position);
};