#include "human.h"
#include <string>
#include <iostream>

using std::string;
using std::cin;
using std::cout;
using std::endl;

Move Human::getMove(ChessRules& position) {
    string in;
    Move mv;
    cout << "Enter your move: ";
    while(cin >> in) {
        if(mv.NaturalIn(&position, in.c_str())) {
            return mv;
        }
        cout << "Enter your move: ";
    }
    return mv; // should never be hit
}
