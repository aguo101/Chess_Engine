#include "engine.h"
#include <stdlib.h>
#include <thread>
#include <future>

using std::max;
using std::min;
using std::swap;
using std::async;
using std::future;

uint32_t Engine::m_depth = 7;

constexpr double rookVal = 5;
constexpr double knightVal = 3;
constexpr double bishopVal = 3;
constexpr double queenVal = 9;
constexpr double kingVal = 10000;
constexpr double pawnVal = 1;

vector<double> Engine::knightMap = {
    -0.30,-0.20,-0.20,-0.20,-0.20,-0.20,-0.20,-0.30,
    -0.20, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00,-0.20,
    -0.20, 0.00, 0.20, 0.20, 0.20, 0.20, 0.00,-0.20,
    -0.20, 0.00, 0.20, 0.30, 0.30, 0.20, 0.00,-0.20,
    -0.20, 0.00, 0.20, 0.30, 0.30, 0.20, 0.00,-0.20,
    -0.20, 0.00, 0.20, 0.20, 0.20, 0.20, 0.00,-0.20,
    -0.20, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00,-0.20,
    -0.30,-0.20,-0.20,-0.20,-0.20,-0.20,-0.20,-0.30
};

// first and last row shouldn't be necessary
vector<double> Engine::pawnMap = {queenVal,0.3,0.2,0.1,0.0,-0.1,-0.2,-queenVal};

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
                eval -= knightMap[i];
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
                eval -= pawnMap[i / 8];
                break;
            case 'R':
                eval += rookVal;
                break;
            case 'N':
                eval += knightVal;
                eval += knightMap[i];
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
                eval += pawnMap[i / 8];
                break;
        }
    }
    return eval;
}

Move Engine::getMove(ChessRules& position) {

    static constexpr size_t maxThreads = 100;

    vector<Move> moves;
    position.GenLegalMoveList(moves);
    
    size_t numThreads = moves.size(); //min(maxThreads, moves.size());
    vector<future<MoveEval>> threads(numThreads);

    vector<vector<Move>> movesForThreads(numThreads);
    int movesPerThread = moves.size() / numThreads;
    for(int i = 0; i < numThreads - 1; i++) {
        movesForThreads[i] = vector<Move>(moves.begin() + i * movesPerThread, moves.begin() + (i + 1) * movesPerThread);
    }
    movesForThreads[numThreads - 1] = vector<Move>(moves.begin() + (numThreads - 1) * movesPerThread, moves.end()); 
    
    for(int i = 0; i < numThreads; i++) {
        threads[i] = async(std::launch::async, [this, &position, &movesForThreads](int index) {
            ChessRules copy(position);
            return minimaxThread(copy, movesForThreads[index], m_depth, -kingVal, kingVal);
        }, i);
    }
    
    MoveEval moveToMake;
    bool whiteToPlay = position.WhiteToPlay();
    moveToMake.eval = startingEval(whiteToPlay);

    for(int i = 0; i < numThreads; i++) {
        MoveEval temp = threads[i].get();
        
        if(whiteToPlay && temp.eval > moveToMake.eval) {
            moveToMake = temp;
        }
        else if(!whiteToPlay && temp.eval < moveToMake.eval) {
            moveToMake = temp;
        }
    }
    
    // MoveEval moveToMake = minimax(position, m_depth, -kingVal, kingVal); send a cached eval too
    return moveToMake.move;
}

MoveEval Engine::minimax(ChessRules& position, uint32_t depth, double alpha, double beta, 
                         unordered_map<unsigned short, pair<uint32_t, double>>& cachedEvals) {

    CompressedPosition compressed;
    unsigned short positionHash = position.Compress(compressed);
    if(cachedEvals.count(positionHash) > 0 && cachedEvals[positionHash].first >= depth) {
        MoveEval ret;
        ret.eval = cachedEvals[positionHash].second;
        return ret;
    }

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
            optimalMove.eval = -kingVal - depth;
            return optimalMove;
        case TERMINAL_WSTALEMATE:
            optimalMove.eval = 0;
            return optimalMove;
        case TERMINAL_BCHECKMATE:
            optimalMove.eval = kingVal + depth;
            return optimalMove;
            break;
        case TERMINAL_BSTALEMATE:
            optimalMove.eval = 0;
            return optimalMove;
    }

    vector<Move> moves;
    position.GenLegalMoveList(moves);

    bool whiteToPlay = position.WhiteToPlay();
    optimalMove.eval = startingEval(whiteToPlay);

    // heuristic: evaluate all captures first
    int swapPos = 0;
    for(Move& mv : moves) {
        char square = position.squares[mv.dst];
        char offset = !whiteToPlay * 32;
        if(square >= 'a' + offset && square <= 'z' + offset) {
            swap(mv, moves[swapPos]);
            swapPos++;
        }
    }

    MoveEval tempMove;
    for(Move& mv : moves) {
        position.PushMove(mv);
        tempMove = minimax(position, depth - 1, alpha, beta, cachedEvals);
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

    // only cache the position if we have enough depth left
    static int minCacheDepth = m_depth - 5;
    if(depth >= minCacheDepth) {
        cachedEvals[positionHash] = pair<uint32_t, double>(depth, optimalMove.eval);
    }

    return optimalMove;
}

MoveEval Engine::minimaxThread(ChessRules& position, vector<Move>& moves, uint32_t depth, double alpha, double beta) {

    MoveEval optimal;
    bool whiteToPlay = position.WhiteToPlay();
    optimal.eval = startingEval(whiteToPlay);

    unordered_map<unsigned short, pair<uint32_t, double>> cachedEvals;

    for(Move mv : moves) {
        position.PushMove(mv);
        MoveEval temp = minimax(position, depth - 1, alpha, beta, cachedEvals);
        if(whiteToPlay && temp.eval > optimal.eval) {
            optimal = temp;
            optimal.move = mv;
            alpha = max(alpha, optimal.eval);
        }
        else if(!whiteToPlay && temp.eval < optimal.eval) {
            optimal = temp;
            optimal.move = mv;
            beta = min(beta, optimal.eval);
        }
    }

    return optimal;
}

double Engine::startingEval(bool whiteToPlay) {
    return (whiteToPlay * -2 + 1) * kingVal * 10;
}