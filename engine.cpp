#include "engine.h"
#include <vector>

using std::vector;

uint32_t Engine::m_depth = 5;

constexpr int rookVal = 5;
constexpr int knightVal = 3;
constexpr int bishopVal = 3;
constexpr int queenVal = 9;
constexpr int kingVal = 10000;
constexpr int pawnVal = 1;

Engine::Engine() {}

double Engine::evaluatePosition(ChessPosition& position) {
    // TODO: implement positional advantages

    constexpr int numSquares = 64;

    double eval = 0;
    for (int i = 0; i < numSquares; i++) {
        switch(position.squares[i]) {
            case 'r':
                eval -= rookVal;
                break;
            case 'n':
                eval -= knightVal;
                break;
            case 'b':
                eval -= bishopVal;
                break;
            case 'q':
                eval -= queenVal;
                break;
            case 'k':
                eval -= kingVal;
                break;
            case 'p':
                eval -= pawnVal;
                break;
            case 'R':
                eval += rookVal;
                break;
            case 'N':
                eval += knightVal;
                break;
            case 'B':
                eval += bishopVal;
                break;
            case 'Q':
                eval += queenVal;
                break;
            case 'K':
                eval += kingVal;
                break;
            case 'P':
                eval += pawnVal;
                break;
        }
    }
    return eval;
}

Move Engine::getMove(ChessRules& position) {
    MoveEval moveToMake = minimax(position, m_depth);
    return moveToMake.move;
}

MoveEval Engine::minimax(ChessRules& position, uint32_t depth) {
    if(depth == 0) {
        MoveEval ret;
        ret.eval = evaluatePosition(position);
        return ret;
    }

    MoveEval optimalMove;

    return optimalMove; // temporary

    // check for Mate/Draw
    // TERMINAL score_terminal;
    // position.Evaluate(score_terminal);
    // switch (score_terminal) {
    //     case TERMINAL_WCHECKMATE:
    //         return true;
    //         break;
    //     case TERMINAL_WSTALEMATE:
    //         return true;
    //         break;
    //     case TERMINAL_BCHECKMATE:
    //         return true;
    //         break;
    //     case TERMINAL_BSTALEMATE:
    //         return true;
    //         break;
    // }

    // vector<Move> moves;
    // position.GenLegalMoveList(moves);



    // TODO

    // use position. PushMove(Move) and PopMove(Move) to make and undo moves
    // use GenLegalMoveList(std::vector<Move>&) to get all legal moves

}