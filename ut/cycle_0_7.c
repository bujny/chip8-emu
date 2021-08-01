#include "common.h"

static void test_CLS(void **state)
{
    struct chip8* emu = (struct chip8*)*state;

    write_opcode_to_memory(emu, 0x00E0);

    runCycle(emu);

    assert_int_equal(emu->PC, PROGRAM_START_LOCATION + NEXT_INSTRUCTION);
}

static void test_RET(void **state)
{
    struct chip8* emu = (struct chip8*)*state;

    emu->SP = 2;
    emu->stack[2] = 530;
    write_opcode_to_memory(emu, 0x00EE);

    runCycle(emu);

    assert_int_equal(emu->PC, 530);
    assert_int_equal(emu->SP, 1);
}

static void test_SYS(void **state)
{
    struct chip8* emu = (struct chip8*)*state;

    write_opcode_to_memory(emu, 0x0033);

    runCycle(emu);

    assert_int_equal(emu->PC, PROGRAM_START_LOCATION);
}

static void test_JP(void **state)
{
    struct chip8* emu = (struct chip8*)*state;

    write_opcode_to_memory(emu, 0x12BC); // 700

    runCycle(emu);

    assert_int_equal(emu->PC, 0x2BC);
}

static void test_CALL(void **state)
{
    struct chip8* emu = (struct chip8*)*state;

    write_opcode_to_memory(emu, 0x22BC); // 700

    runCycle(emu);
    assert_int_equal(emu->PC, 0x2BC);
    assert_int_equal(emu->SP, 1);
    assert_int_equal(emu->stack[emu->SP], PROGRAM_START_LOCATION);
}

static void test_SE_skip(void **state)
{
    struct chip8* emu = (struct chip8*)*state;

    emu->V[0xA] = 0x01;
    write_opcode_to_memory(emu, 0x3A01);

    runCycle(emu);

    assert_int_equal(emu->PC, PROGRAM_START_LOCATION + SKIP_INSTRUCTION);
}

static void test_SE_no_skip(void **state)
{
    struct chip8* emu = (struct chip8*)*state;

    write_opcode_to_memory(emu, 0x3A01);

    runCycle(emu);

    assert_int_equal(emu->PC, PROGRAM_START_LOCATION + NEXT_INSTRUCTION);
}

static void test_SNE_skip(void **state)
{
    struct chip8* emu = (struct chip8*)*state;

    write_opcode_to_memory(emu, 0x4A01); 

    runCycle(emu);

    assert_int_equal(emu->PC, PROGRAM_START_LOCATION + SKIP_INSTRUCTION);
}

static void test_SNE_no_skip(void **state)
{
    struct chip8* emu = (struct chip8*)*state;

    emu->V[0xA] = 0x01;
    write_opcode_to_memory(emu, 0x4A01);

    runCycle(emu);

    assert_int_equal(emu->PC, PROGRAM_START_LOCATION + NEXT_INSTRUCTION);
}

static void test_SE_vx_vy_skip(void **state)
{
    struct chip8* emu = (struct chip8*)*state;

    emu->V[0xA] = 0x01;
    emu->V[0xB] = 0x01;
    write_opcode_to_memory(emu, 0x5AB0);

    runCycle(emu);

    assert_int_equal(emu->PC, PROGRAM_START_LOCATION + SKIP_INSTRUCTION);
}

static void test_SE_vx_vy_no_skip(void **state)
{
    struct chip8* emu = (struct chip8*)*state;

    emu->V[0xA] = 0x01;
    write_opcode_to_memory(emu, 0x5AB0);

    runCycle(emu);

    assert_int_equal(emu->PC, PROGRAM_START_LOCATION + NEXT_INSTRUCTION);
}

static void test_LD_vx_byte(void **state)
{
    struct chip8* emu = (struct chip8*)*state;

    write_opcode_to_memory(emu, 0x6A11);

    runCycle(emu);

    assert_int_equal(emu->PC, PROGRAM_START_LOCATION + NEXT_INSTRUCTION);
    assert_int_equal(emu->V[0xA], 0x11);
}

static void test_ADD_vx_byte(void **state)
{
    struct chip8* emu = (struct chip8*)*state;

    emu->V[0xA] = 0x01;
    write_opcode_to_memory(emu, 0x7A11);

    runCycle(emu);

    assert_int_equal(emu->PC, PROGRAM_START_LOCATION + NEXT_INSTRUCTION);
    assert_int_equal(emu->V[0xA], 0x12);
}

static void test_ADD_vx_byte_wrap(void **state)
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
        cmocka_unit_test_setup_teardown(test_CLS, setup_emu, teardown_emu),
        cmocka_unit_test_setup_teardown(test_RET, setup_emu, teardown_emu),
        cmocka_unit_test_setup_teardown(test_SYS, setup_emu, teardown_emu),
        cmocka_unit_test_setup_teardown(test_JP, setup_emu, teardown_emu),
        cmocka_unit_test_setup_teardown(test_CALL, setup_emu, teardown_emu),
        cmocka_unit_test_setup_teardown(test_SE_skip, setup_emu, teardown_emu),
        cmocka_unit_test_setup_teardown(test_SE_no_skip, setup_emu, teardown_emu),
        cmocka_unit_test_setup_teardown(test_SNE_skip, setup_emu, teardown_emu),
        cmocka_unit_test_setup_teardown(test_SNE_no_skip, setup_emu, teardown_emu),
        cmocka_unit_test_setup_teardown(test_SE_vx_vy_skip, setup_emu, teardown_emu),
        cmocka_unit_test_setup_teardown(test_SE_vx_vy_no_skip, setup_emu, teardown_emu),
        cmocka_unit_test_setup_teardown(test_LD_vx_byte, setup_emu, teardown_emu),
        cmocka_unit_test_setup_teardown(test_ADD_vx_byte, setup_emu, teardown_emu),
        cmocka_unit_test_setup_teardown(test_ADD_vx_byte_wrap, setup_emu, teardown_emu),
    };

    return cmocka_run_group_tests(cycle_0_7, NULL, NULL);
}
