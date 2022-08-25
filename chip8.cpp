//
// Created by Carson on 8/24/2022.
//
#include "chip8.h"
unsigned char fontset[80] =
        {
                0xF0, 0x90, 0x90, 0x90, 0xF0, //0
                0x20, 0x60, 0x20, 0x20, 0x70, //1
                0xF0, 0x10, 0xF0, 0x80, 0xF0, //2
                0xF0, 0x10, 0xF0, 0x10, 0xF0, //3
                0x90, 0x90, 0xF0, 0x10, 0x10, //4
                0xF0, 0x80, 0xF0, 0x10, 0xF0, //5
                0xF0, 0x80, 0xF0, 0x90, 0xF0, //6
                0xF0, 0x10, 0x20, 0x40, 0x40, //7
                0xF0, 0x90, 0xF0, 0x90, 0xF0, //8
                0xF0, 0x90, 0xF0, 0x10, 0xF0, //9
                0xF0, 0x90, 0xF0, 0x90, 0x90, //A
                0xE0, 0x90, 0xE0, 0x90, 0xE0, //B
                0xF0, 0x80, 0x80, 0x80, 0xF0, //C
                0xE0, 0x90, 0x90, 0x90, 0xE0, //D
                0xF0, 0x80, 0xF0, 0x80, 0xF0, //E
                0xF0, 0x80, 0xF0, 0x80, 0x80  //F
        };
chip8::chip8() {
    pc = 0;
}
chip8::~chip8() {}
void chip8::loadFile(char* name) {
    FILE* f = fopen("test.ch8", "rb");
    if (f == nullptr) {
        cout << "Error: couldn't open file" << endl;
        exit(1);
    }
    fseek(f, 0, SEEK_END);
    int fsize = ftell(f);
    fseek(f, 0, SEEK_SET);
    buffer = (unsigned char*)malloc(4096);
    fread(buffer, fsize, 1, f);
    fclose(f);
    for (int j = 3896; j < 4096; j++) {
        *(buffer+j) = fontset[j];
    }
}
void chip8::emulateCycle() {
    int num = (*(buffer+pc) << 8) | (*(buffer+pc+1));
    cout << hex << num << endl;
    switch(num&0xF000) {
        //0
        case 0x0:
            switch (num & 0x0F00) {
                case 0:
                    switch (num & 0x000F) {
                        case 0x0:
                            //00E0 - Clears the Screen
                            for (int i = 0; i < 2048; i++) {
                                gfx[i] = 0;
                            }
                            drawFlag = true;
                            pc += 2;
                            break;
                        case 0xE:
                            //00EE - Exit subroutine
                            pc = routines.top();
                            routines.pop();
                            pc += 2;
                            break;
                    }
                    break;
            }
            break;

            //1
        case 0x1000:
            //1NNN - Jump NNN
            pc = num & 0x0FFF - 0x0200;
            break;

            //2
        case 0x2000:
            routines.push(pc);
            pc = num&0x0FFF - 0x0200;
            break;

            //3
        case 0x3000:
            //3XNN - if vx != NN then
            if ((V[(num >> 8) & 0x000F]) == (num & 0x00FF)) {
                pc += 4;
            } else {
                pc += 2;
            }
            break;

            //4
        case 0x4000:
            //4XNN - if vx != NN then
            if ((V[(num >> 8) & 0x000F]) == (num & 0x00FF)) {
                pc += 2;
            } else {
                pc += 4;
            }
            break;

            //5
        case 0x5000:
            //5XY0 - if vx != vy then
            if ((V[(num >> 8) & 0x000F]) == (V[(num >> 4) & 0x000F])) {
                pc += 4;
            } else {
                pc += 2;
            }
            break;

            //6
        case 0x6000:
            //6XNN - vx := NN
            V[(num >> 8) & 0x000F] = num&0x00FF;
            pc += 2;
            break;

            //7
        case 0x7000:
            //7XNN - vx += NN
            V[(num >> 8) & 0x000F] += num & 0x00FF;
            pc += 2;
            break;

            //8
        case 0x8000:
            switch (num & 0x000F) {
                case 0x0:
                    //8XY0 - vx := vy
                    V[(num >> 8) & 0x000F] = V[(num >> 4) & 0x000F];
                    pc += 2;
                    break;
                case 0x1:
                    //8XY1 - vx |= vy
                    V[(num >> 8) & 0x000F] = V[(num >> 8) & 0x000F] | V[(num >> 4) & 0x000F];
                    pc += 2;
                    break;
                case 0x2:
                    //8XY2 - vx &= vy
                    V[(num >> 8) & 0x000F] = V[(num >> 8) & 0x000F] & V[(num >> 4) & 0x000F];
                    pc += 2;
                    break;
                case 0x3:
                    //8XY3 - vx ^= vy
                    V[(num >> 8) & 0x000F] = V[(num >> 8) & 0x000F] ^ V[(num >> 4) & 0x000F];
                    pc += 2;
                    break;
                case 0x4:
                    V[(num >> 8) & 0x000F] = V[(num >> 8) & 0x000F] + V[(num >> 4) & 0x000F];
                    if ((V[(num >> 8) & 0x000F] + V[(num >> 4) & 0x000F]) > 255) {
                        V[0xF] = 1;
                    } else {
                        V[0xF] = 0;
                    }
                    pc += 2;
                    break;
                case 0x5:
                    //8XY5 - vx -= vy (vf = 0 on borrow)
                    V[(num >> 8) & 0x000F] = V[(num >> 8) & 0x000F] - V[(num >> 4) & 0x000F];
                    if (V[(num >> 8) & 0x000F] > V[(num >> 4) & 0x000F]) {
                        V[0xF] = 1;
                    } else {
                        V[0xF] = 0;
                    }
                    pc += 2;
                    break;
                case 0x6:
                    //8XY6 - vx >>= vy (vf = old least sig bit)
                    V[0xF] = num & 0x0001;
                    V[(num >> 8) & 0x000F] = V[(num >> 8) & 0x000F] >> 1;
                    pc += 2;
                    break;
                case 0x7:
                    V[(num >> 8) & 0x000F] = V[(num >> 4) & 0x000F] - V[(num >> 8) & 0x000F];
                    if (V[(num >> 4) & 0x000F] > V[(num >> 8) & 0x000F]) {
                        V[0xF] = 1;
                    } else {
                        V[0xF] = 0;
                    }
                    pc += 2;
                    break;
                case 0xE:
                    // WARNING
                    V[0xF] = (num & 0x8000) >> 15;
                    V[(num >> 8) & 0x000F] = V[(num >> 8) & 0x000F] << 1;
                    pc += 2;
                    break;
            }
            break;

            //9
        case 0x9000:
            if ((V[(num >> 8) & 0x000F]) == (V[(num >> 4) & 0x000F])) {
                pc += 2;
            } else {
                pc += 4;
            }
            break;

            //A
        case 0xA000:
            //ANNN - i := NNN
            i = num&0x0FFF;
            pc += 2;
            break;

            //B
        case 0xB000:
            //BNNN - jump to NNN + v0
            pc = num & 0x00FF + V[0];
            break;

            //C
        case 0xC000: {
          //CXNN - vx := random NN (random number 0-255 AND NN)
            random_device rd;
            mt19937 gen(rd());
            uniform_int_distribution<> distr(0, 255);
            V[(num >> 8) & 0x000F] = distr(gen) & (num & 0x00FF);
            pc += 2;
            break;
    }
            //D
        case 0xD000: {
            unsigned short x = V[(num >> 8)&0x000F] % 64;
            unsigned short y = V[(num >> 4)&0x000F] % 32;
            unsigned short height = num & 0x000F;
            unsigned short pixel;

            V[0xF] = 0;
            for (int row = 0; row < height; row++)
            {
                    pixel = *(buffer + (i - 0x0200) + row);
                for(int pix = 0; pix < 8; pix++)
                {
                    if((pixel & (0x80 >> pix)) != 0)
                    {
                        if(gfx[(x + pix + ((y + row) * 64))] == 1)
                        {
                            V[0xF] = 1;
                        }
                        gfx[x + pix + ((y + row) * 64)] ^= 1;
                    }
                }
            }

            drawFlag = true;
            pc += 2;
            break;
        }
            //E
        case 0xE000:
            switch(num&0x00FF) {
                case 0x9E:
                    //EX9E - if vx -key then (is a key pressed?)
                    if (key[V[(num >> 8) & 0x000F]] != 0) {
                        pc += 4;
                    }
                    else {
                        pc += 2;
                    }
                    break;
                case 0xA1:
                    //EXA1 - if vx key then (is a key not pressed?)
                    if (key[V[(num >> 8) & 0x000F]] != 0) {
                        pc += 2;
                    }
                    else {
                        pc += 4;
                    }
                    break;
            }
            break;

            //F
        case 0xF000:
            switch(num&0x00FF) {
                case 0x7:
                    V[(num >> 8) & 0x000F] = delay;
                    pc += 2;
                    break;
                case 0xA: {
                    //FX0A - vx := key (wait for a keypress)
                    bool pressed = false;
                    for (int i = 0; i < 16; i++) {
                        if (key[i] != 0) {
                            V[(num >> 8) & 0x000F] = i;
                            pressed = true;
                        }
                    }
                    if (!pressed) {
                        return;
                    }
                    pc += 2;
                    break;
                }
                case 0x15:
                    delay = V[(num >> 8) & 0x000F];
                    pc += 2;
                    break;
                case 0x18:
                    sound = V[(num >> 8) & 0x000F];
                    pc += 2;
                    break;
                case 0x1E:
                    i += V[(num >> 8) & 0x000F];
                    pc += 2;
                    break;
                case 0x29:
                    //FX29 - i := hex vx (set i to a hex character)
                    i = 4096+(V[(num >> 8) & 0x000F] * 0x5);
                    pc += 2;
                    break;
                case 0x33:
                    //FF33 - bcd vx (decode vx into BCD)
                    *(buffer+(i&0x00FF)) = V[(num >> 8) & 0x000F] / 100;
                    *(buffer+(i&0x00FF)+1) = (V[(num >> 8) & 0x000F] / 10) % 10;
                    *(buffer+(i&0x00FF)+2) = V[(num >> 8) & 0x000F] % 10;
                    pc += 2;
                    break;
                case 0x55:
                    //FX55 - save vx (save v0-vx to i through (i+x))
                    for (int j = 0; j <= ((num >> 8) & 0x000F); j++) {
                        *(buffer+(i&0x00FF)+j) = V[j];
                    }
                    pc += 2;
                    break;
                case 0x65:
                    //FX65 - load vx (load v0-vx from i through (i+x))
                    for (int j = 0; j <= ((num >> 8) & 0x000F); j++) {
                        V[j] =  *(buffer+(i&0x00FF)+j);
                    }
                    pc += 2;
                    break;
            }
            break;
    }
    // Update timers
    if (delay > 0)
        --delay;

    if (sound > 0)
        if(sound == 1);
    --sound;
}