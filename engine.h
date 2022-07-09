#include "player.h"
#include <vector>
#include <unordered_map>

using namespace thc;
using std::vector;
using std::unordered_map;
using std::pair;

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

    MoveEval minimax(ChessRules& position, uint32_t depth, double alpha, double beta,
                     unordered_map<unsigned short, pair<uint32_t, double>>& cachedEvals);
    MoveEval minimaxThread(ChessRules& position, vector<Move>& moves, uint32_t depth, double alpha, double beta);
    double startingEval(bool whiteToPlay);

    static uint32_t m_depth;
    static vector<double> knightMap;
    static vector<double> pawnMap;
};