#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

#define MEMORY_MAX (1 << 16)
uint16_t memory[MEMORY_MAX];

typedef enum {
  REG_0 = 0,
  REG_1,
  REG_2,
  REG_3,
  REG_4,
  REG_5,
  REG_6,
  REG_7,
  REG_PC,
  REG_COND, // stores condition P,Z,N
  REG_NUM
} Registers;
uint16_t reg[REG_NUM];

typedef enum {
  OP_BR = 0, // branch
  OP_ADD,    // add
  OP_LD,     // load
  OP_ST,     // store
  OP_JSR,    // jump register
  OP_AND,    // bitwise and
  OP_LDR,    // load register
  OP_STR,    // store register
  OP_RTI,    // unused
  OP_NOT,    // bitwise not
  OP_LDI,    // load indirect
  OP_STI,    // store indirecdsr
  OP_JMP,    // jmp
  OP_RES,    // reserved(unused)
  OP_LEA,    // load effetive address
  OP_TRAP,   // execute trap
} Opcodes;
typedef enum {
  GETC = 0x20,
  OUT,
  PUTS,
  IN,
  PUTSP,
  HALT,
} Trap_Functions;
// just by knowing this cond i can implement all comparison op.
typedef enum {
  FL_POS = 1 << 0, // positive flag
  FL_ZRO = 1 << 1, // zero flag
  FL_NEG = 1 << 2  // negative flag
} Flags;
typedef enum {
  MR_KBSR = 0xFE00, // keyboard status reg,last bit indicate that new char was
                    // received.
  MR_KBDR = 0xFE02  // kbd data register bit[0:7] contain last char typed.
} Device_register;

// bit_count is bit to extend from
uint16_t sign_extend16(uint16_t x, int bit_count) {
  if ((x >> (bit_count - 1)) &
      1) {                      // check if last bit is set => num is negative
    x |= (0xFFFF << bit_count); // set last bit_count bit to became negative
  }
  return x;
}
// trivial
uint16_t swap16(uint16_t x) { return (x << 8) | (x >> 8); }
// check if register val is...
void update_flags(uint16_t r) {
  if (reg[r] == 0) {
    reg[REG_COND] = FL_ZRO;
  } else if (reg[r] >> 15) { // msb is 1 => neg
    reg[REG_COND] = FL_NEG;
  } else {
    reg[REG_COND] = FL_POS;
  }
}
void read_image_file(FILE *file) {
  uint16_t origin;
  // first 2bytes of file are origin address
  fread(&origin, sizeof(origin), 1, file);
  origin = swap16(origin);

  // read rest of file - origin
  uint16_t max_read = MEMORY_MAX - origin;
  uint16_t *start = memory + origin;
  size_t bytes_read = fread(start, sizeof(uint16_t), max_read, file);

  // swap to le
  while (bytes_read-- > 0) {
    *start = swap16(*start);
    ++start;
  }
}
int read_image(const char *image_path) {
  FILE *file = fopen(image_path, "rb");
  if (!file) {
    return 0;
  };
  read_image_file(file);
  fclose(file);
  return 1;
}
uint16_t check_key_press() {
  fd_set readfds;
  FD_ZERO(&readfds);
  FD_SET(STDIN_FILENO, &readfds);

  struct timeval timeout;
  timeout.tv_sec = 0;
  timeout.tv_usec = 0;
  return select(1, &readfds, NULL, NULL, &timeout) != 0;
}
void mem_write(uint16_t address, uint16_t val) { memory[address] = val; }
uint16_t mem_read(uint16_t address) {
  if (address == MR_KBSR) {
    if (check_key_press()) {
      memory[MR_KBSR] = 1 << 15;
      memory[MR_KBDR] = getchar();
    } else {
      memory[MR_KBSR] = 0;
    }
  }
  return memory[address];
}
struct termios original_tio;

void disable_input_buffering() {
  tcgetattr(STDIN_FILENO, &original_tio);
  struct termios new_tio = original_tio;
  new_tio.c_lflag &= ~ICANON & ~ECHO;
  tcsetattr(STDIN_FILENO, TCSANOW, &new_tio);
}

void restore_input_buffering() {
  tcsetattr(STDIN_FILENO, TCSANOW, &original_tio);
}
void handle_interrupt(int signal) {
  restore_input_buffering();
  printf("\n");
  exit(-2);
}
int main(int argc, char **argv) {
  setvbuf(stdout, NULL, _IONBF, 0);
  signal(SIGINT, handle_interrupt);
  disable_input_buffering();
  if (argc < 2) {
    puts("usage: lc3 [image-file] ...");
    exit(2);
  }
  for (int i = 1; i < argc; ++i) {
    if (!read_image(argv[i])) {
      printf("failed to load image: %s\n", argv[i]);
      exit(1);
    }
    restore_input_buffering();
  }
  reg[REG_COND] = FL_ZRO;
  enum {
    PC_START = 0x3000
  }; // start here because ideally before there should be code for trap function
  reg[REG_PC] = PC_START;

  int running = 1;
  while (running) {
    uint16_t instr =
        mem_read(reg[REG_PC]++); // cisc, fixed size for instruction D:
    uint16_t op = instr >> 12;
    switch (op) {
    case OP_ADD: {
      uint16_t dr = (instr >> 9) & 0x7; // get only first 3 bit
      uint16_t sr0 = (instr >> 6) & 0x7;
      uint16_t imm_flag = (instr >> 5) & 1;
      if (imm_flag) {
        uint16_t imm5 = sign_extend16(instr & 0x1F, 5); // & 0b11111 first 5 bit
        reg[dr] = reg[sr0] + imm5;
      } else {
        uint16_t sr1 = instr & 0x7;
        reg[dr] = reg[sr0] + reg[sr1];
      }
      update_flags(dr);
      break;
    }
    case OP_AND: {
      uint16_t dr = (instr >> 9) & 0x7; // get only first 3 bit
      uint16_t sr0 = (instr >> 6) & 0x7;
      uint16_t imm_flag = (instr >> 5) & 1;
      if (imm_flag) {
        uint16_t imm5 = sign_extend16(instr & 0x1F, 5); // & 0b11111 first 5 bit
        reg[dr] = reg[sr0] & imm5;
      } else {
        uint16_t sr1 = instr & 0x7;
        reg[dr] = reg[sr0] & reg[sr1];
      }
      update_flags(dr);
      break;
    }
    case OP_NOT: {
      uint16_t dr = (instr >> 9) & 0x7;
      uint16_t sr = (instr >> 6) & 0x7;
      reg[dr] = ~reg[sr];
      update_flags(dr);
      break;
    }
    case OP_BR: { // conditional branch
      uint16_t pos_test = (instr >> 9);
      if (pos_test & reg[REG_COND]) { // HACK to avoid comparison.how does it
        // work. zero,and neg flag are in pos 10 and
        // 11. R_COND could be b1,b10,b110
        // i am basically testing & 1 if R_COND = pos,& 10 if R_COND = zero, so
        // testing the 10th(or second) bit, same reasoning for neg.
        uint16_t pc_offset = sign_extend16(instr & 0x1FF, 9); // 0b111111111
        reg[REG_PC] += pc_offset;
      }
      break;
    };
    case OP_JMP: {
      // handle also RET, for the fact that work with R7
      uint16_t base_reg =
          (instr >> 6) & 0x7; // this map to the register, there are 8 reg (0-7)
      reg[REG_PC] = reg[base_reg];
      break;
    }
    case OP_JSR:
      reg[REG_7] = reg[REG_PC];
      uint16_t flag = (instr >> 11) & 1;
      if (flag) { // JSR
        uint16_t pc_offset =
            sign_extend16(instr & 0x7FF, 11); // & 0b11111111111
        reg[REG_PC] += pc_offset;
      } else { // JSRR
        uint16_t base_reg = reg[REG_PC] = (instr >> 6) & 0x7;
        reg[REG_PC] = reg[base_reg];
      }
      break;
    case OP_LD: {
      uint16_t dr = (instr >> 9) & 0x7;
      uint16_t pc_offset = sign_extend16(instr & 0x1FF, 9);
      reg[dr] = mem_read(reg[REG_PC] + pc_offset);
      update_flags(dr);
      break;
    }
    case OP_LDI: {
      uint16_t dr = (instr >> 9) & 0x7;
      uint16_t pc_offset = sign_extend16(instr & 0b111111111, 9); // 0x1FF
      reg[dr] = mem_read(mem_read(reg[REG_PC] + pc_offset));
      update_flags(dr);
      break;
    }
    case OP_LDR: {
      uint16_t dr = (instr >> 9) & 0x7;
      uint16_t base_r = (instr >> 6) & 0x7;
      uint16_t offset = sign_extend16(instr & 0x3F, 6);
      reg[dr] = mem_read(reg[base_r] + offset);
      update_flags(dr);
      break;
    }
    case OP_LEA: {
      uint16_t dr = (instr >> 9) & 0x7;
      uint16_t pc_offset = sign_extend16(instr & 0x1FF, 9);
      reg[dr] = reg[REG_PC] + pc_offset;
      update_flags(dr);
      break;
    }
    case OP_ST: {
      uint16_t sr = (instr >> 9) & 0x7;
      uint16_t pc_offset = sign_extend16(instr & 0x1FF, 9);
      mem_write(reg[REG_PC] + pc_offset, reg[sr]);
      break;
    }
    case OP_STI: {
      uint16_t sr = (instr >> 9) & 0x7;
      uint16_t pc_offset = sign_extend16(instr & 0x1FF, 9);
      mem_write(mem_read(reg[REG_PC] + pc_offset), reg[sr]);
      break;
    }
    case OP_STR: {
      uint16_t sr = (instr >> 9) & 0x7;
      uint16_t base_r = (instr >> 6) & 0x7;
      uint16_t offset = sign_extend16(instr & 0x3F, 6);
      mem_write(reg[base_r] + offset, reg[sr]);
      break;
    }
    case OP_TRAP: {
      reg[REG_7] = reg[REG_PC];
      uint16_t trapvec = sign_extend16(instr & 0xFF, 8);
      switch (trapvec) {
      case GETC:
        reg[REG_0] = getchar();
        update_flags(REG_0);
        break;
      case PUTS: {
        for (uint16_t *c = memory + reg[REG_0]; *c != 0; ++c) {
          putchar((char)*c);
        }
        break;
      }
      case OUT: {
        putchar((char)reg[REG_0]);
        break;
      }
      case IN:
        printf("Enter char > ");
        char c = getchar();
        putchar(c);
        reg[REG_0] = c;
        update_flags(REG_0);
        break;
      case PUTSP: {
        for (uint16_t *c = memory + reg[REG_0]; *c != 0; ++c) {
          uint8_t first = *c & 0xFF;
          uint8_t second = *c & 0xFF00;
          putchar((char)first);
          putchar((char)second);
        }
        break;
      }
      case HALT:
        puts("Halting dude.");
        running = 0;
        break;
      }
      break;
    }
    case OP_RES:
    case OP_RTI:
    default:
      // bad opcode
      break;
    }
  }
}
