#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <cmocka.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "chip8.h"

static int setup_emu(void **state)
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

static int teardown_emu(void **state)
{
    struct chip8* emu = (struct chip8*)*state;

    test_free(emu);
    *state = NULL;

    return 0;
}

static void write_opcode_to_memory(struct chip8* emu, uint16_t opcode)
{
    emu->memory[PROGRAM_START_LOCATION] = (opcode & 0xFF00) >> 8;
    emu->memory[PROGRAM_START_LOCATION + 1] = opcode & 0x00FF;
}

static void test_cls(void **state)
{
    struct chip8* emu = (struct chip8*)*state;

    write_opcode_to_memory(emu, 0x00E0);

    runCycle(emu);

    assert_int_equal(emu->PC, PROGRAM_START_LOCATION + NEXT_INSTRUCTION);
}

int main()
{
    const struct CMUnitTest cycle[] = {
        cmocka_unit_test_setup_teardown(test_cls, setup_emu, teardown_emu),
    };

    return cmocka_run_group_tests(cycle, NULL, NULL);
}