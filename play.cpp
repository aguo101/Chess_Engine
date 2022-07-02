#include "play.h"
#include "human.h"
#include "engine.h"
#include <iostream>
#include <string>
#include <memory>

using std::cin;
using std::cout;
using std::endl;

Play::Play() {
    white = new Human();
    black = new Human();
    board = new ChessRules();
    display();
}

Play::~Play() {
    delete(white);
    delete(black);
    delete(board);
}

Play::Play(Player* w, Player* b) : white(w), black(b) {
    board = new ChessRules();
    display();
}

void Play::display() {
    cout << board->ToDebugStr().c_str() << endl;
}

bool Play::move() {
    TERMINAL score_terminal;
    Move mv;
    if(board->WhiteToPlay()) {
        mv = white->getMove(*board);
    } else {
        mv = black->getMove(*board);
    }
    board->PlayMove(mv);
    display();
    board->Evaluate(score_terminal);
    switch (score_terminal) {
        case TERMINAL_WCHECKMATE:
            cout << "Black Won" << endl;
            return true;
            break;
        case TERMINAL_WSTALEMATE:
            cout << "Draw" << endl;
            return true;
            break;
        case TERMINAL_BCHECKMATE:
            cout << "White Won" << endl;
            return true;
            break;
        case TERMINAL_BSTALEMATE:
            cout << "Draw" << endl;
            return true;
            break;
    }
    return false;
}