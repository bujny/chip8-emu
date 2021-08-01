#include "common.h"

static void test_SKP_vx_skip(void **state)
{
    struct chip8* emu = (struct chip8*)*state;

    emu->key[0xA] = KEY_DOWN;
    emu->V[0xA] = 0xA;
    write_opcode_to_memory(emu, 0xEA9E);

    runCycle(emu);

    assert_int_equal(emu->PC, PROGRAM_START_LOCATION + SKIP_INSTRUCTION);
}

static void test_SKP_vx_no_skip(void **state)
{
    struct chip8* emu = (struct chip8*)*state;

    emu->V[0xA] = 0xB;
    write_opcode_to_memory(emu, 0xEA9E);

    runCycle(emu);

    assert_int_equal(emu->PC, PROGRAM_START_LOCATION + NEXT_INSTRUCTION);
}

static void test_SKNP_vx_skip(void **state)
{
    struct chip8* emu = (struct chip8*)*state;

    emu->V[0xA] = 0xB;
    write_opcode_to_memory(emu, 0xEAA1);

    runCycle(emu);

    assert_int_equal(emu->PC, PROGRAM_START_LOCATION + SKIP_INSTRUCTION);
}

static void test_SKNP_vx_no_skip(void **state)
{
    struct chip8* emu = (struct chip8*)*state;

    emu->key[0xA] = KEY_DOWN;
    emu->V[0xA] = 0xA;
    write_opcode_to_memory(emu, 0xEAA1);

    runCycle(emu);

    assert_int_equal(emu->PC, PROGRAM_START_LOCATION + NEXT_INSTRUCTION);
}

static void incorrect_opcode_E(void**state)
{
    struct chip8* emu = (struct chip8*)*state;

    write_opcode_to_memory(emu, 0xEA11);

    runCycle(emu);

    assert_int_equal(emu->V[0xA], 0);
    assert_int_equal(emu->V[0xF], 0);
    assert_int_equal(emu->PC, PROGRAM_START_LOCATION);
}

int main()
{
    const struct CMUnitTest cycle_E[] = {
        cmocka_unit_test_setup_teardown(test_SKP_vx_skip, setup_emu, teardown_emu),
        cmocka_unit_test_setup_teardown(test_SKP_vx_no_skip, setup_emu, teardown_emu),
        cmocka_unit_test_setup_teardown(test_SKNP_vx_skip, setup_emu, teardown_emu),
        cmocka_unit_test_setup_teardown(test_SKNP_vx_no_skip, setup_emu, teardown_emu),
        cmocka_unit_test_setup_teardown(incorrect_opcode_E, setup_emu, teardown_emu),
    };

    return cmocka_run_group_tests(cycle_E, NULL, NULL);
}