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

static void test_jp(void **state)
{
    struct chip8* emu = (struct chip8*)*state;

    write_opcode_to_memory(emu, 0x12BC); // 700

    runCycle(emu);

    assert_int_equal(emu->PC, 0x2BC);
}

static void test_call(void **state)
{
    struct chip8* emu = (struct chip8*)*state;

    write_opcode_to_memory(emu, 0x22BC); // 700

    runCycle(emu);
    assert_int_equal(emu->PC, 0x2BC);
    assert_int_equal(emu->SP, 1);
    assert_int_equal(emu->stack[emu->SP], PROGRAM_START_LOCATION);
}

static void test_se_skip(void **state)
{
    struct chip8* emu = (struct chip8*)*state;

    emu->V[0xA] = 0x01;
    write_opcode_to_memory(emu, 0x3A01);

    runCycle(emu);

    assert_int_equal(emu->PC, PROGRAM_START_LOCATION + SKIP_INSTRUCTION);
}

static void test_se_no_skip(void **state)
{
    struct chip8* emu = (struct chip8*)*state;

    write_opcode_to_memory(emu, 0x3A01);

    runCycle(emu);

    assert_int_equal(emu->PC, PROGRAM_START_LOCATION + NEXT_INSTRUCTION);
}

static void test_sne_skip(void **state)
{
    struct chip8* emu = (struct chip8*)*state;

    write_opcode_to_memory(emu, 0x4A01); 

    runCycle(emu);

    assert_int_equal(emu->PC, PROGRAM_START_LOCATION + SKIP_INSTRUCTION);
}

static void test_sne_no_skip(void **state)
{
    struct chip8* emu = (struct chip8*)*state;

    emu->V[0xA] = 0x01;
    write_opcode_to_memory(emu, 0x4A01);

    runCycle(emu);

    assert_int_equal(emu->PC, PROGRAM_START_LOCATION + NEXT_INSTRUCTION);
}

static void test_se_vxvy_skip(void **state)
{
    struct chip8* emu = (struct chip8*)*state;

    emu->V[0xA] = 0x01;
    emu->V[0xB] = 0x01;
    write_opcode_to_memory(emu, 0x5AB0);

    runCycle(emu);

    assert_int_equal(emu->PC, PROGRAM_START_LOCATION + SKIP_INSTRUCTION);
}

static void test_se_vxvy_no_skip(void **state)
{
    struct chip8* emu = (struct chip8*)*state;

    emu->V[0xA] = 0x01;
    write_opcode_to_memory(emu, 0x5AB0);

    runCycle(emu);

    assert_int_equal(emu->PC, PROGRAM_START_LOCATION + NEXT_INSTRUCTION);
}

static void test_ld_vxbyte(void **state)
{
    struct chip8* emu = (struct chip8*)*state;

    write_opcode_to_memory(emu, 0x6A11);

    runCycle(emu);

    assert_int_equal(emu->PC, PROGRAM_START_LOCATION + NEXT_INSTRUCTION);
    assert_int_equal(emu->V[0xA], 0x11);
}

static void test_add_vxbyte(void **state)
{
    struct chip8* emu = (struct chip8*)*state;

    emu->V[0xA] = 0x01;
    write_opcode_to_memory(emu, 0x7A11);

    runCycle(emu);

    assert_int_equal(emu->PC, PROGRAM_START_LOCATION + NEXT_INSTRUCTION);
    assert_int_equal(emu->V[0xA], 0x12);
}

static void test_add_vxbyte_wrap(void **state)
{
    struct chip8* emu = (struct chip8*)*state;

    emu->V[0xA] = 0xFF;
    write_opcode_to_memory(emu, 0x7A11);

    runCycle(emu);

    assert_int_equal(emu->PC, PROGRAM_START_LOCATION + NEXT_INSTRUCTION);
    assert_int_equal(emu->V[0xA], 0x10);
    assert_int_equal(emu->V[0xF], 0x00);
}

int main()
{
    const struct CMUnitTest cycle_0_7[] = {
        cmocka_unit_test_setup_teardown(test_cls, setup_emu, teardown_emu),
        cmocka_unit_test_setup_teardown(test_jp, setup_emu, teardown_emu),
        cmocka_unit_test_setup_teardown(test_call, setup_emu, teardown_emu),
        cmocka_unit_test_setup_teardown(test_se_skip, setup_emu, teardown_emu),
        cmocka_unit_test_setup_teardown(test_se_no_skip, setup_emu, teardown_emu),
        cmocka_unit_test_setup_teardown(test_sne_skip, setup_emu, teardown_emu),
        cmocka_unit_test_setup_teardown(test_sne_no_skip, setup_emu, teardown_emu),
        cmocka_unit_test_setup_teardown(test_se_vxvy_skip, setup_emu, teardown_emu),
        cmocka_unit_test_setup_teardown(test_se_vxvy_no_skip, setup_emu, teardown_emu),
        cmocka_unit_test_setup_teardown(test_ld_vxbyte, setup_emu, teardown_emu),
        cmocka_unit_test_setup_teardown(test_add_vxbyte, setup_emu, teardown_emu),
        cmocka_unit_test_setup_teardown(test_add_vxbyte_wrap, setup_emu, teardown_emu),
    };

    return cmocka_run_group_tests(cycle_0_7, NULL, NULL);
}