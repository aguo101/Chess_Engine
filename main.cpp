#include "play.h"
#include "human.h"
#include "engine.h"
#include <iostream>

using std::cout;
using std::endl;

int main() {
    
    cout << "New Game" << endl;

    Play* play = new Play(new Human(), new Engine());

    // Update loop
    while(play->move() == false) {}
}