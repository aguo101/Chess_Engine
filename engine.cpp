#include "engine.h"
#include <vector>
#include <stdlib.h>

using std::vector;
using std::max;
using std::min;

uint32_t Engine::m_depth = 7;

constexpr double rookVal = 5;
constexpr double knightVal = 3;
constexpr double bishopVal = 3;
constexpr double queenVal = 9;
constexpr double kingVal = 10000;
constexpr double pawnVal = 1;

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
    MoveEval moveToMake = minimax(position, m_depth, -kingVal, kingVal);
    return moveToMake.move;
}

MoveEval Engine::minimax(ChessRules& position, uint32_t depth, double alpha, double beta) {
    if(depth == 0) {
        MoveEval ret;
        ret.eval = evaluatePosition(position);
        return ret;
    }

    MoveEval optimalMove;

    // check for Mate/Draw
    TERMINAL score_terminal;
    position.Evaluate(score_terminal);
    switch (score_terminal) {
        case TERMINAL_WCHECKMATE:
            optimalMove.eval = -kingVal;
            return optimalMove;
        case TERMINAL_WSTALEMATE:
            optimalMove.eval = 0;
            return optimalMove;
        case TERMINAL_BCHECKMATE:
            optimalMove.eval = kingVal;
            return optimalMove;
            break;
        case TERMINAL_BSTALEMATE:
            optimalMove.eval = 0;
            return optimalMove;
    }

    vector<Move> moves;
    position.GenLegalMoveList(moves);

    bool whiteToPlay = position.WhiteToPlay();
    if(whiteToPlay) {
        optimalMove.eval = -kingVal;
    } else {
        optimalMove.eval = kingVal;
    }

    MoveEval tempMove;
    for(Move& mv : moves) {
        position.PushMove(mv);
        tempMove = minimax(position, depth - 1, alpha, beta);
        tempMove.move = mv;
        position.PopMove(mv);
        if(whiteToPlay && tempMove.eval > optimalMove.eval) {
            optimalMove = tempMove;
            alpha = max(alpha, optimalMove.eval);
        }
        else if(!whiteToPlay && tempMove.eval < optimalMove.eval) {
            optimalMove = tempMove;
            beta = min(beta, optimalMove.eval);
        }
        if(beta <= alpha) {
            return optimalMove;
        }
    }

    return optimalMove;
}