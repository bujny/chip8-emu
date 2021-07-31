#ifndef COMMON_H
#define COMMON_H

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <cmocka.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "chip8.h"

int setup_emu(void **state);
int teardown_emu(void **state);
void write_opcode_to_memory(struct chip8* emu, uint16_t opcode);

#endif /* COMMON_H */
