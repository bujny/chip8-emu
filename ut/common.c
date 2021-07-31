#include "common.h"

int setup_emu(void **state)
{
    struct chip8* emu; 
    
    emu = test_malloc(sizeof(*emu));
    emu->PC = PROGRAM_START_LOCATION;
    emu->I = 0;
    emu->SP = 0;

    memset(emu->stack, 0, STACK_SIZE * sizeof(uint16_t));
    memset(emu->V, 0, REG_QUANTITY * sizeof(uint8_t));
    memset(emu->memory, 0, MEMORY_SIZE * sizeof(uint8_t));
    memset(emu->key, 0, KEYPAD_SIZE * sizeof(uint8_t));

    *state = emu;
    
    return 0;
}


int teardown_emu(void **state)
{
    struct chip8* emu = (struct chip8*)*state;

    test_free(emu);
    *state = NULL;

    return 0;
}

void write_opcode_to_memory(struct chip8* emu, uint16_t opcode)
{
    emu->memory[PROGRAM_START_LOCATION] = (opcode & 0xFF00) >> 8;
    emu->memory[PROGRAM_START_LOCATION + 1] = opcode & 0x00FF;
}
