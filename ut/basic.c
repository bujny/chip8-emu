#include "common.h"

static void test_init(void **state)
{
    struct chip8 emu;

    uint16_t empty_stack[STACK_SIZE] = {0};
    uint8_t empty_V[REG_QUANTITY] = {0};
    uint8_t empty_memory[MEMORY_SIZE] = {0};
    uint8_t empty_key[KEYPAD_SIZE] = {0};

    init(&emu);

    assert_int_equal(emu.PC, 512);
    assert_int_equal(emu.I, 0);
    assert_int_equal(emu.SP, 0);
    assert_memory_equal(emu.stack, empty_stack, STACK_SIZE);
    assert_memory_equal(emu.V, empty_V, REG_QUANTITY);
    assert_memory_equal(emu.memory, empty_memory, MEMORY_SIZE);
    assert_memory_equal(emu.key, empty_key, KEYPAD_SIZE);
}

int main()
{
    const struct CMUnitTest cycle_basic[] = {
        cmocka_unit_test_teardown(test_init, teardown_emu),
    };

    return cmocka_run_group_tests(cycle_basic, NULL, NULL);
}
