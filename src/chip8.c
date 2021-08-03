#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "raylib.h"

#include "chip8.h"


void init(struct chip8* emu)
{
    time_t t;   
    srand((unsigned) time(&t));
    emu->PC = PROGRAM_START_LOCATION;
    emu->I = 0;
    emu->SP = 0;

    /* TODO: Clear display */

    memset(emu->stack, 0, STACK_SIZE * sizeof(uint16_t));
    memset(emu->V, 0, REG_QUANTITY * sizeof(uint8_t));
    memset(emu->memory, 0, MEMORY_SIZE * sizeof(uint8_t));
    memset(emu->key, 0, KEYPAD_SIZE * sizeof(uint8_t));
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
    uint16_t opcode = emu->memory[emu->PC] << 8 | emu->memory[emu->PC+1];

    switch(opcode & 0xF000)
    {
        case 0x0000:
        {
            switch(opcode & 0x00FF)
            {
                case 0xE0:
                {
                    /* TODO: clear the display */
                    emu->PC += 2;
                    printf("CLS \n");
                    break;
                }
                case 0xEE:
                {
                    emu->PC = emu->stack[emu->SP];
                    emu->SP--;
                    printf("RET \n");
                    break;
                }
                default:
                {
                    /* TODO */
                    printf("SYS \n");
                    break;
                }
            }
            break;
        }
        case 0x1000:
        {
            emu->PC = opcode & 0x0FFF;
            printf("JP \n");
            break;
        }
        case 0x2000:
        {
            emu->SP++;
            emu->stack[emu->SP] = emu->PC;
            emu->PC = opcode & 0x0FFF;
            printf("CALL \n");
            break;
        }
        case 0x3000:
        {
            uint8_t byte = opcode & 0x00FF;
            uint8_t vx = (opcode & 0x0F00) >> 8;
            
            emu->PC += (emu->V[vx] == byte) ? SKIP_INSTRUCTION : NEXT_INSTRUCTION;
            printf("SE Vx, byte \n");
            break;
        }
        case 0x4000:
        {
            uint8_t byte = opcode & 0x00FF;
            uint8_t vx = (opcode & 0x0F00) >> 8;
            emu->PC += (emu->V[vx] != byte) ? SKIP_INSTRUCTION : NEXT_INSTRUCTION;
            printf("SNE Vx, byte \n");
            break;
        }
        case 0x5000:
        {
            uint8_t vx = (opcode & 0x0F00) >> 8;
            uint8_t vy = (opcode & 0x00F0) >> 4;
            emu->PC += (emu->V[vx] == emu->V[vy]) ? SKIP_INSTRUCTION : NEXT_INSTRUCTION;
            printf("SE Vx, Vy \n");
            break;
        }
        case 0x6000:
        {
            uint8_t byte = opcode & 0x00FF;
            uint8_t vx = (opcode & 0x0F00) >> 8;
            emu->V[vx] = byte;
            emu->PC += NEXT_INSTRUCTION;
            printf("LD Vx, byte \n");
            break;
        }
        case 0x7000:
        {
            uint8_t byte = opcode & 0x00FF;
            uint8_t vx = (opcode & 0x0F00) >> 8;
            emu->V[vx] = emu->V[vx] + byte;
            printf("ADD Vx, byte \n");
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
                    printf("LD Vx, Vy \n");
                    emu->PC += NEXT_INSTRUCTION;
                    break;
                }
                case 0x1:
                {
                    emu->V[vx] = emu->V[vx] | emu->V[vy];
                    printf("OR Vx, Vy \n");
                    emu->PC += NEXT_INSTRUCTION;
                    break;
                }
                case 0x2:
                {
                    emu->V[vx] = emu->V[vx] & emu->V[vy];
                    printf("AND Vx, Vy \n");
                    emu->PC += NEXT_INSTRUCTION;
                    break;
                }
                case 0x3:
                {
                    emu->V[vx] = emu->V[vx] ^ emu->V[vy];
                    printf("XOR Vx, Vy \n");
                    emu->PC += NEXT_INSTRUCTION;
                    break;
                }
                case 0x4:
                {
                    uint16_t sum = emu->V[vx] + emu->V[vy];
                    emu->V[FLAG_REG] = (sum > UINT8_MAX) ? CARRY : NO_FLAG;
                    emu->V[vx] = sum & 0x00FF;
                    emu->PC += NEXT_INSTRUCTION;
                    printf("ADD Vx, Vy \n");
                    break;
                }
                case 0x5:
                {
                    emu->V[FLAG_REG] = (emu->V[vx] > emu->V[vy]) ? NOT_BORROW : NO_FLAG;
                    emu->V[vx] = emu->V[vx] - emu->V[vy];
                    emu->PC += NEXT_INSTRUCTION;
                    printf("SUB Vx, Vy \n");
                    break;
                }
                case 0x6:
                {
                    emu->V[FLAG_REG] = emu->V[vx] & 0x01;
                    emu->V[vx] = emu->V[vx] >> 1;
                    emu->PC += NEXT_INSTRUCTION;
                    printf("SHR Vx {, Vy} \n");
                    break;
                }
                case 0x7:
                {
                    emu->V[FLAG_REG] = (emu->V[vy] > emu->V[vx]) ? NOT_BORROW : NO_FLAG;
                    emu->V[vx] = emu->V[vy] - emu->V[vx];
                    emu->PC += NEXT_INSTRUCTION;
                    printf("SUBN Vx, Vy \n");
                    break;
                }
                case 0xE:
                {
                    emu->V[FLAG_REG] = (emu->V[vx] & 0x80) >> 7;
                    emu->V[vx] = emu->V[vx] << 1;
                    emu->PC += NEXT_INSTRUCTION;
                    printf("SHL Vx {, Vy} \n");
                    break;
                }
                default:
                {
                    printf("ERROR at 0x8000 \n");
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
            printf("SNE Vx, Vy \n");
            break;
        }
        case 0xA000:
        {
            emu->I = opcode & 0x0FFF;
            emu->PC += NEXT_INSTRUCTION;
            printf("LD I, addr \n");
            break;
        }
        case 0xB000:
        {
            emu->PC = (opcode & 0x0FFF) + emu->V[0];
            printf("JP V0, addr \n");
            break;
        }
        case 0xC000:
        {
            uint8_t rand_b = rand() % (UINT8_MAX + 1);
            uint8_t byte = opcode & 0x00FF;
            uint8_t vx = (opcode & 0x0F00) >> 8;
            emu->V[vx] = rand_b & byte;
            emu->PC += NEXT_INSTRUCTION;
            printf("RND Vx, byte \n");
            break;
        }
        case 0xD000:
        {
            /* TODO */
            emu->PC += NEXT_INSTRUCTION;
            printf("DRW Vx, Vy, nibble \n");
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
                    printf("SKP Vx \n");
                    break;
                }
                case 0xA1:
                {
                    emu->PC += (!emu->key[emu->V[vx]]) ? SKIP_INSTRUCTION : NEXT_INSTRUCTION;
                    printf("SKNP Vx \n");
                    break;
                }
                default:
                {
                    printf("ERROR AT 0xE000 \n");
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
                    printf("LD Vx, DT\n");
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
                    printf("LD Vx, K\n");
                    break;
                }
                case 0x15:
                {
                    emu->delay = emu->V[vx];
                    emu->PC += NEXT_INSTRUCTION;
                    printf("LD DT, Vx\n");
                    break;
                }
                case 0x18:
                {
                    emu->sound = emu->V[vx];
                    emu->PC += NEXT_INSTRUCTION;
                    printf("LD ST, Vx\n");
                    break;
                }
                case 0x1E:
                {
                    emu->I += emu->V[vx];
                    emu->PC += NEXT_INSTRUCTION;
                    printf("ADD I, Vx\n");
                    break;
                }
                case 0x29:
                {
                    /* TODO */
                    printf("LD F, Vx\n");
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
                    printf("LD B, Vx\n");
                    break;
                }
                case 0x55:
                {
                    for (int i = 0; i < REG_QUANTITY; i++)
                    {
                        emu->memory[emu->I + i] = emu->V[i];
                    }
                    emu->PC += NEXT_INSTRUCTION;
                    printf("LD [I], Vx");
                    break;
                }
                case 0x65:
                {
                    for (int i = 0; i < REG_QUANTITY; i++)
                    {
                        emu->V[i] = emu->memory[emu->I + i];
                    }
                    emu->PC += NEXT_INSTRUCTION;
                    printf("LD Vx, [I]");
                    break;
                }
                default:
                {
                    printf("ERROR AT 0xF000 \n");
                    break;
                }
            }
            break;
        }
        default:
        {
            /* Unreachable */
            printf("ERROR \n");
            break;
        }
    }
 
  // Update timers
}

#if !defined(BUILD_STATIC_LIB)
int main()
{
    printf("Welp, hello.\n");
    struct chip8 emulator;

    init(&emulator);

    // load program

    while(1)
    {
        runCycle(&emulator);

        // handle graphics

        // handle input
    }
    
    return 0;
}
#endif /* !BUILD_STATIC_LIB */

