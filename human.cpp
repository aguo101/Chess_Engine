#include "human.h"
#include <string>
#include <iostream>

using std::string;
using std::cin;

Move Human::getMove(ChessRules& position) {
    string in;
    Move mv;
    while(cin >> in) {
        if(mv.NaturalIn(&position, in.c_str())) {
            return mv;
        }
    }
}