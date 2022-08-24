//
// Created by RealL on 8/24/2022.
//
#include <iostream>
using namespace std;
int main() {
    File* f = fopen("test.bin", "rb");
    if (f == nullptr) {
        cout << "Error: Couldn't open file" << endl;
        exit(1);
    }

    fseek(f, 0L, SEEK_END);
    int fsize = ftell(f);
    fseek(f, 0L, SEEK_SET);
    unsigned char* buffer = malloc(fsize);
    fread(buffer, fsize, 1, f);
    fclose(f);
    int pc = 0;

    //While not at end
    while(pc < fsize) {

        //Fetch - Get opcodes



        //Decode - Turn opcodes into instructions (use switch statements, look at first bit)

        //Execute - Execute on hardware
    }


    return 0;
}
