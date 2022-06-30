#include "engine.h"

using std::unique_ptr;

Engine::Engine() {}

double Engine::evaluatePosition(ChessPosition& position) {
    // TODO: implement positional advantages

    static const int numSquares = 64;

    double eval = 0;
    for (int i = 0; i < numSquares; i++) {
        switch(position->squares[i]) {
            case 'r':
                eval -= 5;
                break;
            case 'n':
                eval -= 3;
                break;
            case 'b':
                eval -= 3;
                break;
            case 'q':
                eval -= 9;
                break;
            case 'k':
                eval -= 1000;
                break;
            case 'p':
                eval -= 1;
                break;
            case 'R':
                eval += 5;
                break;
            case 'N':
                eval += 3;
                break;
            case 'B':
                eval += 3;
                break;
            case 'Q':
                eval += 9;
                break;
            case 'K':
                eval += 1000;
                break;
            case 'P':
                eval += 1;
                break;
        }
    }
    return eval;
}

Move Engine::getMove(ChessRules& position) {
    // TODO: implement minimax function
    unique_ptr<MoveEval> moveToMake = minimax(position);
    return moveToMake->move;
}

unique_ptr<MoveEval> Engine::minimax(ChessRules& position) {
    // TODO

    // use position. PushMove(Move) and PopMove(Move) to make and undo moves
    // use GenLegalMoveList(std::vector<Move>&) to get all legal moves
    return nullptr;
}