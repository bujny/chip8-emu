#include "common.h"

static void test_SNE_vx_vy_skip(void **state)
{
    struct chip8* emu = (struct chip8*)*state;

    emu->V[0xB] = 7;
    write_opcode_to_memory(emu, 0x9AB0);

    runCycle(emu);

    assert_int_equal(emu->PC, PROGRAM_START_LOCATION + SKIP_INSTRUCTION);
}

static void test_SNE_vx_vy_no_skip(void **state)
{
    struct chip8* emu = (struct chip8*)*state;

    emu->V[0xA] = 7;
    emu->V[0xB] = 7;
    write_opcode_to_memory(emu, 0x9AB0);

    runCycle(emu);

    assert_int_equal(emu->PC, PROGRAM_START_LOCATION + NEXT_INSTRUCTION);
}

static void test_LDI_I_addr(void **state)
{
    struct chip8* emu = (struct chip8*)*state;

    write_opcode_to_memory(emu, 0xA2BC); /* 0x2BC == 700 */

    runCycle(emu);

    assert_int_equal(emu->I, 0x2BC);
    assert_int_equal(emu->PC, PROGRAM_START_LOCATION + NEXT_INSTRUCTION);
}

static void test_JP_v0_addr(void **state)
{
    struct chip8* emu = (struct chip8*)*state;

    emu->V[0x0] = 0x10;
    write_opcode_to_memory(emu, 0xB2BC); /* 0x2BC == 700 */

    runCycle(emu);

    assert_int_equal(emu->PC, 0x2CC);
}

static void test_RND_vX_byte(void **state)
{
    struct chip8* emu = (struct chip8*)*state;

    write_opcode_to_memory(emu, 0xCAFF);

    runCycle(emu);

    assert_int_equal(emu->PC, PROGRAM_START_LOCATION + NEXT_INSTRUCTION);
    assert_in_range(emu->V[0xA], 0, UINT8_MAX);
}

static void test_RND_vX_byte_zero(void **state)
{
    struct chip8* emu = (struct chip8*)*state;

    write_opcode_to_memory(emu, 0xCA00);

    runCycle(emu);

    assert_int_equal(emu->PC, PROGRAM_START_LOCATION + NEXT_INSTRUCTION);
    assert_int_equal(emu->V[0xA], 0);
}

static void test_DRW_vx_vy_nibble(void **state)
{
    struct chip8* emu = (struct chip8*)*state;

    write_opcode_to_memory(emu, 0xDAB3);

    runCycle(emu);

    assert_int_equal(emu->PC, PROGRAM_START_LOCATION + NEXT_INSTRUCTION);
    assert_int_equal(emu->V[0xA], 0);
    assert_int_equal(emu->V[0xB], 0);
}

int main()
{
    const struct CMUnitTest cycle_9_D[] = {
        cmocka_unit_test_setup_teardown(test_SNE_vx_vy_skip, setup_emu, teardown_emu),
        cmocka_unit_test_setup_teardown(test_SNE_vx_vy_no_skip, setup_emu, teardown_emu),
        cmocka_unit_test_setup_teardown(test_LDI_I_addr, setup_emu, teardown_emu),
        cmocka_unit_test_setup_teardown(test_JP_v0_addr, setup_emu, teardown_emu),
        cmocka_unit_test_setup_teardown(test_RND_vX_byte, setup_emu, teardown_emu),
        cmocka_unit_test_setup_teardown(test_RND_vX_byte_zero, setup_emu, teardown_emu),
        cmocka_unit_test_setup_teardown(test_DRW_vx_vy_nibble, setup_emu, teardown_emu),
    };

    return cmocka_run_group_tests(cycle_9_D, NULL, NULL);
}
