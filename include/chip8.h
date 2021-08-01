#ifndef CHIP8_H
#define CHIP8_H

#define MEMORY_SIZE             4096
#define REG_QUANTITY            16
#define STACK_SIZE              16
#define PROGRAM_START_LOCATION  512
#define KEYPAD_SIZE             16

#define CARRY      1
#define NOT_BORROW 1 
#define NO_FLAG    0
#define FLAG_REG   0xF

#define NEXT_INSTRUCTION 2
#define SKIP_INSTRUCTION 4

#define KEY_DOWN 1
#define KEY_UP 0
#define NO_KEY_PRESSED -1


struct chip8 {
    /* Whole Chip8 memory */
    uint8_t memory[MEMORY_SIZE];
    /* General purpose registers */
    uint8_t V[REG_QUANTITY];
    /* Address register */
    uint16_t I;
    /* Delay timer */
    uint8_t delay;
    /* Sound timer */
    uint8_t sound;
    /* Program counter */
    uint16_t PC;
    /* Stack pointer */
    uint8_t SP;
    uint16_t stack[STACK_SIZE];
    /* keyboard */
    uint8_t key[KEYPAD_SIZE];
};

void runCycle(struct chip8* emu);

void init(struct chip8* emu);

#endif /* CHIP8_H */
