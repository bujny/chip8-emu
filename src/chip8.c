#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include <unistd.h>
#include <stdarg.h>

#include "raylib.h"

#include "chip8.h"

#ifndef NO_DEBUG
    #define LOG(...) printf(__VA_ARGS__)
#else
    #define LOG(f_, ...) 
#endif 

void loadFonts(struct chip8* emu)
{
    emu->memory[0] = 0xF0; /* **** */
    emu->memory[1] = 0x90; /* *  * */
    emu->memory[2] = 0x90; /* *  * */
    emu->memory[3] = 0x90; /* *  * */
    emu->memory[4] = 0xF0; /* **** */

    emu->memory[5] = 0x20; /*   *  */
    emu->memory[6] = 0x60; /*  **  */
    emu->memory[7] = 0x20; /*   *  */
    emu->memory[8] = 0x20; /*   *  */
    emu->memory[9] = 0x70; /*  *** */

    emu->memory[10] = 0xF0; /* **** */
    emu->memory[11] = 0x10; /*    * */
    emu->memory[12] = 0xF0; /* **** */
    emu->memory[13] = 0x80; /* *    */
    emu->memory[14] = 0xF0; /* **** */

    emu->memory[15] = 0xF0; /* **** */
    emu->memory[16] = 0x10; /*    * */
    emu->memory[17] = 0xF0; /* **** */
    emu->memory[18] = 0x10; /*    * */
    emu->memory[19] = 0xF0; /* **** */

    emu->memory[20] = 0x90; /* *  * */
    emu->memory[21] = 0x90; /* *  * */
    emu->memory[22] = 0xF0; /* **** */
    emu->memory[23] = 0x10; /*    * */
    emu->memory[24] = 0x10; /*    * */

    emu->memory[25] = 0xF0; /* **** */
    emu->memory[26] = 0x80; /* *    */
    emu->memory[27] = 0xF0; /* **** */
    emu->memory[28] = 0x10; /*    * */
    emu->memory[29] = 0xF0; /* **** */

    emu->memory[30] = 0xF0; /* **** */
    emu->memory[31] = 0x80; /* *    */
    emu->memory[32] = 0xF0; /* **** */
    emu->memory[33] = 0x90; /* *  * */
    emu->memory[34] = 0xF0; /* **** */

    emu->memory[35] = 0xF0; /* **** */
    emu->memory[36] = 0x10; /*    * */
    emu->memory[37] = 0x20; /*   *  */
    emu->memory[38] = 0x40; /*  *   */
    emu->memory[39] = 0x40; /*  *   */

    emu->memory[40] = 0xF0; /* **** */
    emu->memory[41] = 0x90; /* *  * */
    emu->memory[42] = 0xF0; /* **** */
    emu->memory[43] = 0x90; /* *  * */
    emu->memory[44] = 0xF0; /* **** */

    emu->memory[45] = 0xF0; /* **** */
    emu->memory[46] = 0x90; /* *  * */
    emu->memory[47] = 0xF0; /* **** */
    emu->memory[48] = 0x10; /*    * */
    emu->memory[49] = 0xF0; /* **** */

    emu->memory[50] = 0xF0; /* **** */
    emu->memory[51] = 0x90; /* *  * */
    emu->memory[52] = 0xF0; /* **** */
    emu->memory[53] = 0x90; /* *  * */
    emu->memory[54] = 0x90; /* *  * */

    emu->memory[55] = 0xE0; /* ***  */
    emu->memory[56] = 0x90; /* *  * */
    emu->memory[57] = 0xE0; /* ***  */
    emu->memory[58] = 0x90; /* *  * */
    emu->memory[59] = 0xE0; /* ***  */

    emu->memory[60] = 0xF0; /* **** */
    emu->memory[61] = 0x80; /* *    */
    emu->memory[62] = 0x80; /* *    */
    emu->memory[63] = 0x80; /* *    */
    emu->memory[64] = 0xF0; /* **** */

    emu->memory[65] = 0xE0; /* ***  */
    emu->memory[66] = 0x90; /* *  * */
    emu->memory[67] = 0x90; /* *  * */
    emu->memory[68] = 0x90; /* *  * */
    emu->memory[69] = 0xE0; /* ***  */

    emu->memory[70] = 0xF0; /* **** */
    emu->memory[71] = 0x80; /* *    */
    emu->memory[72] = 0xF0; /* **** */
    emu->memory[73] = 0x80; /* *    */
    emu->memory[74] = 0xF0; /* **** */

    emu->memory[75] = 0xF0; /* **** */
    emu->memory[76] = 0x80; /* *    */
    emu->memory[77] = 0xF0; /* **** */
    emu->memory[78] = 0x80; /* *    */
    emu->memory[79] = 0x80; /* *    */
}

void init(struct chip8* emu)
{
    time_t t;   
    srand((unsigned) time(&t));
    emu->PC = PROGRAM_START_LOCATION;
    emu->I = 0;
    emu->SP = 0;

    for (int i = 0; i < DISPLAY_X; i++)
    {
        memset(emu->display[i], 0, DISPLAY_Y * sizeof(uint8_t));
    }

    memset(emu->stack, 0, STACK_SIZE * sizeof(uint16_t));
    memset(emu->V, 0, REG_QUANTITY * sizeof(uint8_t));
    memset(emu->memory, 0, MEMORY_SIZE * sizeof(uint8_t));
    memset(emu->key, 0, KEYPAD_SIZE * sizeof(uint8_t));

    loadFonts(emu);
}

int8_t getPressedKey(uint8_t* key)
{
    int8_t which_key_pressed = NO_KEY_PRESSED;

    for (int i = 0; i < KEYPAD_SIZE; i++)
    {   
        if (KEY_DOWN == key[i])
        {
            which_key_pressed = i + 1;
            break;
        }
    }

    return which_key_pressed;
}

void runCycle(struct chip8* emu)
{
    static int counter = 0;
    counter++;

    uint16_t opcode = emu->memory[emu->PC] << 8 | emu->memory[emu->PC+1];

    switch(opcode & 0xF000)
    {
        case 0x0000:
        {
            switch(opcode & 0x00FF)
            {
                case 0xE0:
                {
                    for (int i = 0; i < DISPLAY_X; i++)
                    {
                        memset(emu->display[i], 0, DISPLAY_Y * sizeof(uint8_t));
                    }
                    emu->PC += 2;
                    LOG("CLS \n");
                    break;
                }
                case 0xEE:
                {
                    emu->PC = emu->stack[emu->SP];
                    emu->SP--;
                    LOG("RET \n");
                    break;
                }
                default:
                {
                    /* TODO */
                    LOG("SYS \n");
                    break;
                }
            }
            break;
        }
        case 0x1000:
        {
            emu->PC = opcode & 0x0FFF;
            LOG("JP \n");
            break;
        }
        case 0x2000:
        {
            emu->SP++;
            emu->stack[emu->SP] = emu->PC + NEXT_INSTRUCTION;
            emu->PC = opcode & 0x0FFF;
            LOG("CALL \n");
            break;
        }
        case 0x3000:
        {
            uint8_t byte = opcode & 0x00FF;
            uint8_t vx = (opcode & 0x0F00) >> 8;
            
            emu->PC += (emu->V[vx] == byte) ? SKIP_INSTRUCTION : NEXT_INSTRUCTION;
            LOG("SE Vx, byte \n");
            break;
        }
        case 0x4000:
        {
            uint8_t byte = opcode & 0x00FF;
            uint8_t vx = (opcode & 0x0F00) >> 8;
            emu->PC += (emu->V[vx] != byte) ? SKIP_INSTRUCTION : NEXT_INSTRUCTION;
            LOG("SNE Vx, byte \n");
            break;
        }
        case 0x5000:
        {
            uint8_t vx = (opcode & 0x0F00) >> 8;
            uint8_t vy = (opcode & 0x00F0) >> 4;
            emu->PC += (emu->V[vx] == emu->V[vy]) ? SKIP_INSTRUCTION : NEXT_INSTRUCTION;
            LOG("SE Vx, Vy \n");
            break;
        }
        case 0x6000:
        {
            uint8_t byte = opcode & 0x00FF;
            uint8_t vx = (opcode & 0x0F00) >> 8;
            emu->V[vx] = byte;
            emu->PC += NEXT_INSTRUCTION;
            LOG("LD V[%d], byte=%d \n", vx, byte);
            break;
        }
        case 0x7000:
        {
            uint8_t byte = opcode & 0x00FF;
            uint8_t vx = (opcode & 0x0F00) >> 8;
            emu->V[vx] = emu->V[vx] + byte;
            LOG("ADD Vx, byte \n");
            emu->PC += NEXT_INSTRUCTION;
            break;
        }
        case 0x8000:
        {
            uint8_t vx = (opcode & 0x0F00) >> 8;
            uint8_t vy = (opcode & 0x00F0) >> 4;
            switch(opcode & 0xF)
            {
                case 0x0:
                {
                    emu->V[vx] = emu->V[vy];
                    LOG("LD Vx, Vy \n");
                    emu->PC += NEXT_INSTRUCTION;
                    break;
                }
                case 0x1:
                {
                    emu->V[vx] = emu->V[vx] | emu->V[vy];
                    LOG("OR Vx, Vy \n");
                    emu->PC += NEXT_INSTRUCTION;
                    break;
                }
                case 0x2:
                {
                    emu->V[vx] = emu->V[vx] & emu->V[vy];
                    LOG("AND Vx, Vy \n");
                    emu->PC += NEXT_INSTRUCTION;
                    break;
                }
                case 0x3:
                {
                    emu->V[vx] = emu->V[vx] ^ emu->V[vy];
                    LOG("XOR Vx, Vy \n");
                    emu->PC += NEXT_INSTRUCTION;
                    break;
                }
                case 0x4:
                {
                    uint16_t sum = emu->V[vx] + emu->V[vy];
                    emu->V[FLAG_REG] = (sum > UINT8_MAX) ? CARRY : NO_FLAG;
                    emu->V[vx] = sum & 0x00FF;
                    emu->PC += NEXT_INSTRUCTION;
                    LOG("ADD Vx, Vy \n");
                    break;
                }
                case 0x5:
                {
                    emu->V[FLAG_REG] = (emu->V[vx] > emu->V[vy]) ? NOT_BORROW : NO_FLAG;
                    emu->V[vx] = emu->V[vx] - emu->V[vy];
                    emu->PC += NEXT_INSTRUCTION;
                    LOG("SUB Vx, Vy \n");
                    break;
                }
                case 0x6:
                {
                    emu->V[FLAG_REG] = emu->V[vx] & 0x01;
                    emu->V[vx] = emu->V[vx] >> 1;
                    emu->PC += NEXT_INSTRUCTION;
                    LOG("SHR Vx {, Vy} \n");
                    break;
                }
                case 0x7:
                {
                    emu->V[FLAG_REG] = (emu->V[vy] > emu->V[vx]) ? NOT_BORROW : NO_FLAG;
                    emu->V[vx] = emu->V[vy] - emu->V[vx];
                    emu->PC += NEXT_INSTRUCTION;
                    LOG("SUBN Vx, Vy \n");
                    break;
                }
                case 0xE:
                {
                    emu->V[FLAG_REG] = (emu->V[vx] & 0x80) >> 7;
                    emu->V[vx] = emu->V[vx] << 1;
                    emu->PC += NEXT_INSTRUCTION;
                    LOG("SHL Vx {, Vy} \n");
                    break;
                }
                default:
                {
                    LOG("ERROR at 0x8000 \n");
                    break;
                }
            }
            break;
        }
        case 0x9000:
        {
            uint8_t vx = (opcode & 0x0F00) >> 8;
            uint8_t vy = (opcode & 0x00F0) >> 4;
            emu->PC += (emu->V[vx] != emu->V[vy]) ? SKIP_INSTRUCTION : NEXT_INSTRUCTION;
            LOG("SNE Vx, Vy \n");
            break;
        }
        case 0xA000:
        {
            emu->I = opcode & 0x0FFF;
            emu->PC += NEXT_INSTRUCTION;
            LOG("LD I, addr=%d \n", emu->I);
            break;
        }
        case 0xB000:
        {
            emu->PC = (opcode & 0x0FFF) + emu->V[0];
            LOG("JP V0, addr \n");
            break;
        }
        case 0xC000:
        {
            uint8_t rand_b = rand() % (UINT8_MAX + 1);
            uint8_t byte = opcode & 0x00FF;
            uint8_t vx = (opcode & 0x0F00) >> 8;
            emu->V[vx] = rand_b & byte;
            emu->PC += NEXT_INSTRUCTION;
            LOG("RND V[%d]=%d, byte=%d \n", vx, emu->V[vx], byte);
            break;
        }
        case 0xD000:
        {
            uint8_t xpos = emu->V[(opcode & 0x0F00) >> 8];
            uint8_t ypos = emu->V[(opcode & 0x00F0) >> 4];
            uint8_t spriteHeight = opcode & 0x000F;

            emu->V[0xF] = NO_FLAG;
            for(uint8_t line = 0; line < spriteHeight; line++)
            {
                uint8_t spriteLine = emu->memory[emu->I + line];

                for(uint8_t linePixel = 0; linePixel < 8; linePixel++)
                {
                    uint8_t pixel = (spriteLine & (0x80 >> linePixel)) >> (7 - linePixel);
                    uint8_t calcPosX = xpos + linePixel;
                    calcPosX = (calcPosX >= DISPLAY_X) ? (calcPosX % DISPLAY_X) : calcPosX;
                    uint8_t calcPosY = ypos + line;
                    calcPosY = (calcPosY >= DISPLAY_Y) ? (calcPosY % DISPLAY_Y) : calcPosY;
                    emu->display[calcPosX][calcPosY] ^= pixel;
                    if (pixel && !emu->display[calcPosX][calcPosY])
                    {
                        emu->V[FLAG_REG] = COLLISION;
                    }
                }
            }

            emu->PC += NEXT_INSTRUCTION;
            LOG("DRW V[%d]=%d, V[%d]=%d, nibble=%d \n", 
                (opcode & 0x0F00) >> 8, xpos, (opcode & 0x00F0) >> 4, ypos, spriteHeight);
            break;
        }
        case 0xE000:
        {
            uint8_t vx = (opcode & 0x0F00) >> 8;
            switch (opcode & 0x00FF)
            {
                case 0x9E:
                {
                    emu->PC += (emu->key[emu->V[vx]]) ? SKIP_INSTRUCTION : NEXT_INSTRUCTION;
                    LOG("SKP Vx \n");
                    break;
                }
                case 0xA1:
                {
                    emu->PC += (!emu->key[emu->V[vx]]) ? SKIP_INSTRUCTION : NEXT_INSTRUCTION;
                    LOG("SKNP Vx \n");
                    break;
                }
                default:
                {
                    LOG("ERROR AT 0xE000 \n");
                    break;
                }
            }
            break;
        }
        case 0xF000:
        {
            uint8_t vx = (opcode & 0x0F00) >> 8;
            switch (opcode & 0x00FF)
            {
                case 0x07:
                {
                    emu->V[vx] = emu->delay;
                    emu->PC += NEXT_INSTRUCTION;
                    LOG("LD Vx, DT\n");
                    break;
                }
                case 0x0A:
                {
                    int8_t pressedKey = getPressedKey(emu->key);
                    
                    if (pressedKey != NO_KEY_PRESSED)
                    {
                        emu->V[vx] = (uint8_t)pressedKey;
                        emu->PC += NEXT_INSTRUCTION;
                    }
                    LOG("LD Vx, K\n");
                    break;
                }
                case 0x15:
                {
                    emu->delay = emu->V[vx];
                    emu->PC += NEXT_INSTRUCTION;
                    LOG("LD DT, Vx\n");
                    break;
                }
                case 0x18:
                {
                    emu->sound = emu->V[vx];
                    emu->PC += NEXT_INSTRUCTION;
                    LOG("LD ST, Vx\n");
                    break;
                }
                case 0x1E:
                {
                    emu->I += emu->V[vx];
                    emu->PC += NEXT_INSTRUCTION;
                    LOG("ADD I, Vx\n");
                    break;
                }
                case 0x29:
                {
                    emu->I = emu->V[vx] * 5;
                    emu->PC += NEXT_INSTRUCTION;
                    LOG("LD F, V[%d] (I=%d)\n", vx, emu->I);
                    break;
                }
                case 0x33:
                {
                    uint8_t hundreds = emu->V[vx] / 100;
                    uint8_t tens = (emu->V[vx] - hundreds * 100) / 10;
                    uint8_t ones = emu->V[vx] - (hundreds * 100) - (tens * 10); 
                    emu->memory[emu->I] = hundreds;
                    emu->memory[emu->I + 1] = tens;
                    emu->memory[emu->I + 2] = ones;
                    emu->PC += NEXT_INSTRUCTION;
                    LOG("LD B, Vx, [%d|%d|%d]\n", hundreds, tens, ones);
                    break;
                }
                case 0x55:
                {
                    for (int i = 0; i < REG_QUANTITY; i++)
                    {
                        emu->memory[emu->I + i] = emu->V[i];
                    }
                    emu->PC += NEXT_INSTRUCTION;
                    LOG("LD [I], Vx\n");
                    break;
                }
                case 0x65:
                {
                    for (int i = 0; i < REG_QUANTITY; i++)
                    {
                        emu->V[i] = emu->memory[emu->I + i];
                        LOG("%d ", emu->V[i]);
                    }
                    LOG("\n");
                    emu->PC += NEXT_INSTRUCTION;
                    LOG("LD Vx, [I]\n");
                    break;
                }
                default:
                {
                    LOG("ERROR AT 0xF000 \n");
                    break;
                }
            }
            break;
        }
        default:
        {
            /* Unreachable */
            LOG("ERROR \n");
            break;
        }
    }
 
    if ((emu->delay > 0) && (counter >= 60))
    {
        emu->delay--;
    }
    if (emu->sound > 0 && (counter >= 60))
    {
        emu->sound--;
    }

    usleep(16667); /* to achieve 60 opcodes/s */
}

void handleKeyInput(struct chip8* emu)
{
    emu->key[0x1] = (uint8_t)IsKeyDown(KEY_ONE);
    emu->key[0x2] = (uint8_t)IsKeyDown(KEY_TWO);
    emu->key[0x3] = (uint8_t)IsKeyDown(KEY_THREE);
    emu->key[0xC] = (uint8_t)IsKeyDown(KEY_FOUR);

    emu->key[0x4] = (uint8_t)IsKeyDown(KEY_Q);
    emu->key[0x5] = (uint8_t)IsKeyDown(KEY_W);
    emu->key[0x6] = (uint8_t)IsKeyDown(KEY_E);
    emu->key[0xD] = (uint8_t)IsKeyDown(KEY_R);

    emu->key[0x7] = (uint8_t)IsKeyDown(KEY_A);
    emu->key[0x8] = (uint8_t)IsKeyDown(KEY_S);
    emu->key[0x9] = (uint8_t)IsKeyDown(KEY_D);
    emu->key[0xE] = (uint8_t)IsKeyDown(KEY_F);

    emu->key[0xA] = (uint8_t)IsKeyDown(KEY_Z);
    emu->key[0xB] = (uint8_t)IsKeyDown(KEY_X);
    emu->key[0x0] = (uint8_t)IsKeyDown(KEY_C);
    emu->key[0xF] = (uint8_t)IsKeyDown(KEY_V);
}

int loadProgram(struct chip8* emu, const char* name)
{
    FILE* prog = fopen(name, "rb");
    uint16_t i = 0;

    if (NULL != prog)
    {
        while(1)
        {
            if(feof(prog))
            {
                break;
            }
            fread(emu->memory + PROGRAM_START_LOCATION + i, sizeof(uint8_t), sizeof(uint8_t), prog);
            i++;
        }
        fclose(prog);
    }
    else
    {
        LOG("Error opening file: %d\n", errno);
        return -1;
    }

    return 0;
}

#if !defined(BUILD_STATIC_LIB)
int main(int argc, char **argv)
{
    if (2 != argc)
    {
        LOG("HOW TO RUN: ./chip8 name_of_prog\n");
        return -1;
    }

    LOG("Welp, hello.\n");
    struct chip8 emulator;

    init(&emulator);
    if (0 != loadProgram(&emulator, argv[1]))
    {
        return -1;
    }

    InitWindow(DISPLAY_X * DISPLAY_SCALING, DISPLAY_Y * DISPLAY_SCALING, "Chip8 emulator");

    while(!WindowShouldClose())
    {
        runCycle(&emulator);

        BeginDrawing();
            ClearBackground(RAYWHITE);
            for (int x = 0; x < 64; x++)
            {
                for(int y = 0; y < 32; y++)
                {
                    DrawRectangle(x * 10, y * 10, 10, 10, emulator.display[x][y] ? BLACK : RAYWHITE);
                }
            }
        EndDrawing();

        handleKeyInput(&emulator);
    }
    
    CloseWindow();

    return 0;
}
#endif /* !BUILD_STATIC_LIB */

