//
// Created by RealL on 8/24/2022.
//

#include "chip8.h"

using namespace std;
int main(int argc, char* args[]) {

        chip8 game = chip8();
        game.loadFile(args[1]);
        game.emulateCycle();

        //Decode - Turn opcodes into instructions (use switch statements, look at first bit)

        //Execute - Execute on hardware

    cout << "hi " << endl;



    return 0;
}
