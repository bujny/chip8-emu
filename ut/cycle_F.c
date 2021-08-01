#include "common.h"

static void test_LD_vx_DT(void **state)
{
    struct chip8* emu = (struct chip8*)*state;

    emu->V[0xA] = 0xA;
    emu->delay = 0xB;
    write_opcode_to_memory(emu, 0xFA07);

    runCycle(emu);

    assert_int_equal(emu->PC, PROGRAM_START_LOCATION + NEXT_INSTRUCTION);
    assert_int_equal(emu->V[0xA], 0xB);
}

static void test_LD_vx_K(void **state)
{
    struct chip8* emu = (struct chip8*)*state;

    emu->key[0x1] = 1;
    write_opcode_to_memory(emu, 0xFA0A);

    runCycle(emu);

    assert_int_equal(emu->PC, PROGRAM_START_LOCATION + NEXT_INSTRUCTION);
    assert_int_equal(emu->V[0xA], 0x2);
}

static void test_LD_vx_K_stump(void **state)
{
    struct chip8* emu = (struct chip8*)*state;

    write_opcode_to_memory(emu, 0xFA0A);

    runCycle(emu);

    assert_int_equal(emu->PC, PROGRAM_START_LOCATION);
    assert_int_equal(emu->V[0xA], 0x0);
}

static void test_LD_DT_vx(void **state)
{
    struct chip8* emu = (struct chip8*)*state;

    emu->V[0xA] = 0x13;
    write_opcode_to_memory(emu, 0xFA15);

    runCycle(emu);

    assert_int_equal(emu->PC, PROGRAM_START_LOCATION + NEXT_INSTRUCTION);
    assert_int_equal(emu->V[0xA], 0x13);
    assert_int_equal(emu->delay, 0x13);
}

static void test_LD_ST_vx(void **state)
{
    struct chip8* emu = (struct chip8*)*state;

    emu->V[0xA] = 0x14;
    write_opcode_to_memory(emu, 0xFA18);

    runCycle(emu);

    assert_int_equal(emu->PC, PROGRAM_START_LOCATION + NEXT_INSTRUCTION);
    assert_int_equal(emu->V[0xA], 0x14);
    assert_int_equal(emu->sound, 0x14);
}

static void test_ADD_I_vx(void **state)
{
    struct chip8* emu = (struct chip8*)*state;

    emu->I = 0x3;
    emu->V[0xA] = 0x14;
    write_opcode_to_memory(emu, 0xFA1E);

    runCycle(emu);

    assert_int_equal(emu->PC, PROGRAM_START_LOCATION + NEXT_INSTRUCTION);
    assert_int_equal(emu->V[0xA], 0x14);
    assert_int_equal(emu->I, 0x17);
}

static void test_ADD_F_vx(void **state)
{
    struct chip8* emu = (struct chip8*)*state;

    write_opcode_to_memory(emu, 0xFA29);

    runCycle(emu);

    assert_int_equal(emu->PC, PROGRAM_START_LOCATION);
    assert_int_equal(emu->V[0xA], 0x00);
}

static void test_LD_B_vx(void **state)
{
    struct chip8* emu = (struct chip8*)*state;

    emu->I = 256;
    emu->V[0xA] = 123;
    write_opcode_to_memory(emu, 0xFA33);

    runCycle(emu);

    assert_int_equal(emu->PC, PROGRAM_START_LOCATION + NEXT_INSTRUCTION);
    assert_int_equal(emu->V[0xA], 123);
    assert_int_equal(emu->memory[256], 1);
    assert_int_equal(emu->memory[257], 2);
    assert_int_equal(emu->memory[258], 3);
}

static void test_LD_I_vx(void **state)
{
    struct chip8* emu = (struct chip8*)*state;

    emu->I = 256;
    for (int i = 0; i < REG_QUANTITY; i++)
    {
        emu->V[i] = 123 + i;
    }
    
    write_opcode_to_memory(emu, 0xFA55);

    runCycle(emu);

    assert_int_equal(emu->PC, PROGRAM_START_LOCATION + NEXT_INSTRUCTION);
    for (int i = 0; i < REG_QUANTITY; i++)
    {
        assert_int_equal(emu->memory[256 + i], 123 + i);
    }
}

static void test_LD_vx_I(void **state)
{
    struct chip8* emu = (struct chip8*)*state;

    emu->I = 123;
    for (int i = 0; i < REG_QUANTITY; i++)
    {
        emu->memory[123 + i] = 73 + i;
    }
    
    write_opcode_to_memory(emu, 0xFA65);

    runCycle(emu);

    assert_int_equal(emu->PC, PROGRAM_START_LOCATION + NEXT_INSTRUCTION);
    for (int i = 0; i < REG_QUANTITY; i++)
    {
        assert_int_equal(emu->V[i], 73 + i);
    }
}

static void incorrect_opcode_F(void **state)
{
    struct chip8* emu = (struct chip8*)*state;

    write_opcode_to_memory(emu, 0xFA77);

    runCycle(emu);

    assert_int_equal(emu->V[0xA], 0);
    assert_int_equal(emu->V[0xF], 0);
    assert_int_equal(emu->PC, PROGRAM_START_LOCATION);
}

int main()
{
    const struct CMUnitTest cycle_F[] = {
        cmocka_unit_test_setup_teardown(test_LD_vx_DT, setup_emu, teardown_emu),
        cmocka_unit_test_setup_teardown(test_LD_vx_K, setup_emu, teardown_emu),
        cmocka_unit_test_setup_teardown(test_LD_vx_K_stump, setup_emu, teardown_emu),
        cmocka_unit_test_setup_teardown(test_LD_DT_vx, setup_emu, teardown_emu),
        cmocka_unit_test_setup_teardown(test_LD_ST_vx, setup_emu, teardown_emu),
        cmocka_unit_test_setup_teardown(test_ADD_I_vx, setup_emu, teardown_emu),
        cmocka_unit_test_setup_teardown(test_ADD_F_vx, setup_emu, teardown_emu),
        cmocka_unit_test_setup_teardown(test_LD_B_vx, setup_emu, teardown_emu),
        cmocka_unit_test_setup_teardown(test_LD_I_vx, setup_emu, teardown_emu),
        cmocka_unit_test_setup_teardown(test_LD_vx_I, setup_emu, teardown_emu),
        cmocka_unit_test_setup_teardown(incorrect_opcode_F, setup_emu, teardown_emu),
    };

    return cmocka_run_group_tests(cycle_F, NULL, NULL);
}
