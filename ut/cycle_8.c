#include "common.h"

static void test_LD_vx_vy(void **state)
{
    struct chip8* emu = (struct chip8*)*state;

    emu->V[0xB] = 7;
    write_opcode_to_memory(emu, 0x8AB0);

    runCycle(emu);

    assert_int_equal(emu->V[0xA], 7);
    assert_int_equal(emu->V[0xB], 7);
    assert_int_equal(emu->PC, PROGRAM_START_LOCATION + NEXT_INSTRUCTION);
}

static void test_OR_vx_vy(void **state)
{
    struct chip8* emu = (struct chip8*)*state;

    emu->V[0xA] = 5; /* b'101 */
    emu->V[0xB] = 2; /* b'010 */
    write_opcode_to_memory(emu, 0x8AB1);

    runCycle(emu);

    assert_int_equal(emu->V[0xA], 7);
    assert_int_equal(emu->V[0xB], 2);
    assert_int_equal(emu->PC, PROGRAM_START_LOCATION + NEXT_INSTRUCTION);
}

static void test_AND_vx_vy(void **state)
{
    struct chip8* emu = (struct chip8*)*state;

    emu->V[0xA] = 5; /* b'101 */
    emu->V[0xB] = 2; /* b'010 */
    write_opcode_to_memory(emu, 0x8AB2);

    runCycle(emu);

    assert_int_equal(emu->V[0xA], 0);
    assert_int_equal(emu->V[0xB], 2);
    assert_int_equal(emu->PC, PROGRAM_START_LOCATION + NEXT_INSTRUCTION);
}

static void test_XOR_vx_vy(void **state)
{
    struct chip8* emu = (struct chip8*)*state;

    emu->V[0xA] = 5; /* b'101 */
    emu->V[0xB] = 3; /* b'011 */
    write_opcode_to_memory(emu, 0x8AB3);

    runCycle(emu);

    assert_int_equal(emu->V[0xA], 6);
    assert_int_equal(emu->V[0xB], 3);
    assert_int_equal(emu->PC, PROGRAM_START_LOCATION + NEXT_INSTRUCTION);
}

static void test_ADD_vx_vy_carry(void **state)
{
    struct chip8* emu = (struct chip8*)*state;

    emu->V[0xA] = UINT8_MAX;
    emu->V[0xB] = 3;
    write_opcode_to_memory(emu, 0x8AB4);

    runCycle(emu);

    assert_int_equal(emu->V[0xA], 2);
    assert_int_equal(emu->V[0xB], 3);
    assert_int_equal(emu->V[0xF], 1);
    assert_int_equal(emu->PC, PROGRAM_START_LOCATION + NEXT_INSTRUCTION);
}

static void test_ADD_vx_vy_no_carry(void **state)
{
    struct chip8* emu = (struct chip8*)*state;

    emu->V[0xA] = 2;
    emu->V[0xB] = 3;
    write_opcode_to_memory(emu, 0x8AB4);

    runCycle(emu);

    assert_int_equal(emu->V[0xA], 5);
    assert_int_equal(emu->V[0xB], 3);
    assert_int_equal(emu->V[0xF], 0);
    assert_int_equal(emu->PC, PROGRAM_START_LOCATION + NEXT_INSTRUCTION);
}

static void test_SUB_vx_vy_no_borrow(void **state)
{
    struct chip8* emu = (struct chip8*)*state;

    emu->V[0xA] = 5;
    emu->V[0xB] = 3;
    write_opcode_to_memory(emu, 0x8AB5);

    runCycle(emu);

    assert_int_equal(emu->V[0xA], 2);
    assert_int_equal(emu->V[0xB], 3);
    assert_int_equal(emu->V[0xF], 1);
    assert_int_equal(emu->PC, PROGRAM_START_LOCATION + NEXT_INSTRUCTION);
}

static void test_SUB_vx_vy_borrow(void **state)
{
    struct chip8* emu = (struct chip8*)*state;

    emu->V[0xA] = 3;
    emu->V[0xB] = 5;
    write_opcode_to_memory(emu, 0x8AB5);

    runCycle(emu);

    assert_int_equal(emu->V[0xA], 254);
    assert_int_equal(emu->V[0xB], 5);
    assert_int_equal(emu->V[0xF], 0);
    assert_int_equal(emu->PC, PROGRAM_START_LOCATION + NEXT_INSTRUCTION);
}

static void test_SHR_vx_vy_flag(void **state)
{
    struct chip8* emu = (struct chip8*)*state;

    emu->V[0xA] = 5;
    write_opcode_to_memory(emu, 0x8AB6);

    runCycle(emu);

    assert_int_equal(emu->V[0xA], 2);
    assert_int_equal(emu->V[0xF], 1);
    assert_int_equal(emu->PC, PROGRAM_START_LOCATION + NEXT_INSTRUCTION);
}

static void test_SHR_vx_vy_no_flag(void **state)
{
    struct chip8* emu = (struct chip8*)*state;

    emu->V[0xA] = 6;
    write_opcode_to_memory(emu, 0x8AB6);

    runCycle(emu);

    assert_int_equal(emu->V[0xA], 3);
    assert_int_equal(emu->V[0xF], 0);
    assert_int_equal(emu->PC, PROGRAM_START_LOCATION + NEXT_INSTRUCTION);
}

static void test_SUBN_vx_vy_borrow(void **state)
{
    struct chip8* emu = (struct chip8*)*state;

    emu->V[0xA] = 8;
    emu->V[0xB] = 6;
    write_opcode_to_memory(emu, 0x8AB7);

    runCycle(emu);

    assert_int_equal(emu->V[0xA], 254);
    assert_int_equal(emu->V[0xB], 6);
    assert_int_equal(emu->V[0xF], 0);
    assert_int_equal(emu->PC, PROGRAM_START_LOCATION + NEXT_INSTRUCTION);
}

static void test_SUBN_vx_vy_no_borrow(void **state)
{
    struct chip8* emu = (struct chip8*)*state;

    emu->V[0xA] = 6;
    emu->V[0xB] = 8;
    write_opcode_to_memory(emu, 0x8AB7);

    runCycle(emu);

    assert_int_equal(emu->V[0xA], 2);
    assert_int_equal(emu->V[0xB], 8);
    assert_int_equal(emu->V[0xF], 1);
    assert_int_equal(emu->PC, PROGRAM_START_LOCATION + NEXT_INSTRUCTION);
}

static void test_SHL_vx_vy_no_flag(void **state)
{
    struct chip8* emu = (struct chip8*)*state;

    emu->V[0xA] = 127;
    write_opcode_to_memory(emu, 0x8ABE);

    runCycle(emu);

    assert_int_equal(emu->V[0xA], 254);
    assert_int_equal(emu->V[0xF], 0);
    assert_int_equal(emu->PC, PROGRAM_START_LOCATION + NEXT_INSTRUCTION);
}

static void test_SHL_vx_vy_flag(void **state)
{
    struct chip8* emu = (struct chip8*)*state;

    emu->V[0xA] = 255;
    write_opcode_to_memory(emu, 0x8ABE);

    runCycle(emu);

    assert_int_equal(emu->V[0xA], 254);
    assert_int_equal(emu->V[0xF], 1);
    assert_int_equal(emu->PC, PROGRAM_START_LOCATION + NEXT_INSTRUCTION);
}

static void incorrect_opcode_8(void**state)
{
    struct chip8* emu = (struct chip8*)*state;

    write_opcode_to_memory(emu, 0x8ABF);

    runCycle(emu);

    assert_int_equal(emu->V[0xA], 0);
    assert_int_equal(emu->V[0xF], 0);
    assert_int_equal(emu->PC, PROGRAM_START_LOCATION);
}

int main()
{
    const struct CMUnitTest cycle_8[] = {
        cmocka_unit_test_setup_teardown(test_LD_vx_vy, setup_emu, teardown_emu),
        cmocka_unit_test_setup_teardown(test_OR_vx_vy, setup_emu, teardown_emu),
        cmocka_unit_test_setup_teardown(test_AND_vx_vy, setup_emu, teardown_emu),
        cmocka_unit_test_setup_teardown(test_XOR_vx_vy, setup_emu, teardown_emu),
        cmocka_unit_test_setup_teardown(test_ADD_vx_vy_carry, setup_emu, teardown_emu),
        cmocka_unit_test_setup_teardown(test_ADD_vx_vy_no_carry, setup_emu, teardown_emu),
        cmocka_unit_test_setup_teardown(test_SUB_vx_vy_no_borrow, setup_emu, teardown_emu),
        cmocka_unit_test_setup_teardown(test_SUB_vx_vy_borrow, setup_emu, teardown_emu),
        cmocka_unit_test_setup_teardown(test_SHR_vx_vy_flag, setup_emu, teardown_emu),
        cmocka_unit_test_setup_teardown(test_SHR_vx_vy_no_flag, setup_emu, teardown_emu),
        cmocka_unit_test_setup_teardown(test_SUBN_vx_vy_borrow, setup_emu, teardown_emu),
        cmocka_unit_test_setup_teardown(test_SUBN_vx_vy_no_borrow, setup_emu, teardown_emu),
        cmocka_unit_test_setup_teardown(test_SHL_vx_vy_no_flag, setup_emu, teardown_emu),
        cmocka_unit_test_setup_teardown(test_SHL_vx_vy_flag, setup_emu, teardown_emu),
        cmocka_unit_test_setup_teardown(incorrect_opcode_8, setup_emu, teardown_emu),
    };

    return cmocka_run_group_tests(cycle_8, NULL, NULL);
}