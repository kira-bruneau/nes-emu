#include <glib.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "cpu.h"
#include "memory.h"
#include "opcode.h"
#include "util.h"

/**
 * References:
 * Manual: http://users.telenet.be/kim1-6502/6502/proman.html
 * NesDev: http://wiki.nesdev.com/w/index.php/CPU
 */

struct CPU {
  uint16_t pc;
  byte sp;
  byte a, x, y;

  union {
    byte status;
    struct {
      byte c : 1;
      byte z : 1;
      byte i : 1;
      byte d : 1;
      byte b : 1; // undefined
      byte e : 1; // undefined
      byte v : 1;
      byte n : 1;
    };
  };

  Memory * mem;
};

CPU * cpu_new(Memory * mem) {
  CPU * cpu = g_malloc(sizeof(CPU));
  cpu->mem = mem;
  cpu_reset(cpu);
  return cpu;
}

void cpu_reset(CPU * cpu) {
  cpu->pc = 0xC000; // ? when not nestest
  cpu->sp = 0xFD;
  cpu->a = 0;
  cpu->x = 0;
  cpu->y = 0;
  cpu->status = 0x24; // 0x34 when not nestest
}

// Obtain the next byte in memory and increment PC
byte cpu_next_memory(CPU * cpu) {
  byte addr = memory_read(cpu->mem, cpu->pc);
  cpu->pc += 1;
  return addr;
}

// Obtain the next two bytes in memory and increment PC
uint16_t cpu_next_memory16(CPU * cpu) {
  uint16_t addr = memory_read16(cpu->mem, cpu->pc);
  cpu->pc += 2;
  return addr;
}

// Evaluate the next instruction in the program
void cpu_next_instr(CPU * cpu) {
  byte opcode = cpu_next_memory(cpu);
  Instruction instruction = opcode_instruction[opcode];

  Address addr = {
    .val = 0,
    .null = false
  };

  switch (opcode_addressing_mode[opcode]) {
  case ADDRMODE_0:
    addr.null = true;
    break;
  case ADDRMODE_1:
    addr.val = cpu->pc++;
    break;
  case ADDRMODE_2:
    addr.val = cpu_next_memory(cpu);
    break;
  case ADDRMODE_3:
    addr.val = cpu_next_memory16(cpu);
    break;
  case ADDRMODE_8:
    addr.val = cpu_next_memory(cpu) + cpu->pc;
    break;
  case ADDRMODE_B:
    addr.val = cpu_next_memory(cpu) + cpu->x;
    break;
  case ADDRMODE_C:
    addr.val = cpu_next_memory(cpu) + cpu->y;
    break;
  case ADDRMODE_9:
    addr.val = cpu_next_memory16(cpu) + cpu->x;
    break;
  case ADDRMODE_A:
    addr.val = cpu_next_memory16(cpu) + cpu->y;
    break;
  case ADDRMODE_4:
    addr.val = memory_read16(cpu->mem, cpu_next_memory16(cpu));
    break;
  case ADDRMODE_5:
    addr.val = memory_read(cpu->mem, cpu_next_memory(cpu)) + cpu->y;
    break;
  case ADDRMODE_6:
    addr.val = memory_read(cpu->mem, cpu_next_memory(cpu) + cpu->x);
    break;
  case ADDRMODE_7:
    addr.val = memory_read(cpu->mem, cpu_next_memory(cpu) + cpu->y);
    break;
  }

  instruction_action[instruction](cpu, addr);
}

/*
 * Stack operations
 */
static void cpu_push(CPU * cpu, byte val) {
  memory_write(cpu->mem, cpu->sp, val);
  cpu->sp -= 1;
}

static void cpu_push16(CPU * cpu, uint16_t val) {
  memory_write16(cpu->mem, cpu->sp - 1, val);
  cpu->sp -= 2;
}

static byte cpu_pull(CPU * cpu) {
  byte val = memory_read(cpu->mem, cpu->sp + 1);
  cpu->sp += 1;
  return val;
}

static uint16_t cpu_pull16(CPU * cpu) {
  uint16_t val = memory_read16(cpu->mem, cpu->sp + 1);
  cpu->sp += 2;
  return val;
}

/**
 * Value operations
 */
static void cpu_zn(CPU * cpu, byte val) {
  cpu->z = val == 0;
  cpu->n = val >> 7 & 1;
}

static void cpu_compare(CPU * cpu, byte a, byte b) {
  byte result = a - b;
  cpu->c = b <= a;
  cpu_zn(cpu, result);
}

/**
 * Pure shift functions
 */
static byte asl(byte val) {
  return val;
}

static byte lsr(byte val) {
  return val;
}

static byte rol(byte val) {
  return val;
}

static byte ror(byte val) {
  return val;
}

/*
 * Official instructions
 */
void cpu_adc(CPU * cpu, Address addr) {
  byte a = cpu->a;
  byte b = memory_read(cpu->mem, addr.val);

  uint16_t result = a + b + cpu->c;
  cpu->a = result;
  cpu->c = result > 0xFF;
  cpu->v = !((a^b) >> 7 & 1) && ((result^a) >> 7 & 1);
  cpu_zn(cpu, result);
}

void cpu_and(CPU * cpu, Address addr) {
  byte result = cpu->a & memory_read(cpu->mem, addr.val);
  cpu->a = result;
  cpu_zn(cpu, result);
}

void cpu_asl(CPU * cpu, Address addr) {
  byte result;

  if (addr.null) {
    result = asl(cpu->a);
    cpu->a = result;
  } else {
    result = asl(memory_read(cpu->mem, addr.val));
    memory_write(cpu->mem, addr.val, result);
  }

  cpu_zn(cpu, result);
}

void cpu_bcc(CPU * cpu, Address addr) {
  if (!cpu->c) {
    cpu->pc = addr.val;
  }
}

void cpu_bcs(CPU * cpu, Address addr) {
  if (cpu->c) {
    cpu->pc = addr.val;
  }
}

void cpu_beq(CPU * cpu, Address addr) {
  if (cpu->z) {
    cpu->pc = addr.val;
  }
}

void cpu_bit(CPU * cpu, Address addr) {
  byte val = memory_read(cpu->mem, addr.val);
  cpu->z = (val & cpu->a) == 0;
  cpu->v = val >> 6 & 1;
  cpu->n = val >> 7 & 1;
}

void cpu_bmi(CPU * cpu, Address addr) {
  if (cpu->n) {
    cpu->pc = addr.val;
  }
}

void cpu_bne(CPU * cpu, Address addr) {
  if (!cpu->z) {
    cpu->pc = addr.val;
  }
}

void cpu_bpl(CPU * cpu, Address addr) {
  if (!cpu->n) {
    cpu->pc = addr.val;
  }
}

void cpu_brk(CPU * cpu, Address addr) {
  static const Address interrupt_addr = {
    .val = 0xFFFE,
    .null = false
  };

  cpu_jsr(cpu, interrupt_addr);
  cpu_php(cpu, addr);
  cpu_sei(cpu, addr);
}

void cpu_bvc(CPU * cpu, Address addr) {
  if (!cpu->v) {
    cpu->pc = addr.val;
  }
}

void cpu_bvs(CPU * cpu, Address addr) {
  if (cpu->v) {
    cpu->pc = addr.val;
  }
}

void cpu_clc(CPU * cpu, Address addr) {
  cpu->c = 0;
}

void cpu_cld(CPU * cpu, Address addr) {
  cpu->d = 0;
}

void cpu_cli(CPU * cpu, Address addr) {
  cpu->i = 0;
}

void cpu_clv(CPU * cpu, Address addr) {
  cpu->v = 0;
}

void cpu_cmp(CPU * cpu, Address addr) {
  byte val = memory_read(cpu->mem, addr.val);
  cpu_compare(cpu, cpu->a, val);
}

void cpu_cpx(CPU * cpu, Address addr) {
  byte val = memory_read(cpu->mem, addr.val);
  cpu_compare(cpu, cpu->x, val);
}

void cpu_cpy(CPU * cpu, Address addr) {
  byte val = memory_read(cpu->mem, addr.val);
  cpu_compare(cpu, cpu->y, val);
}

void cpu_dec(CPU * cpu, Address addr) {
  byte result = memory_read(cpu->mem, addr.val) - 1;
  memory_write(cpu->mem, addr.val, result);
  cpu_zn(cpu, result);
}

void cpu_dex(CPU * cpu, Address addr) {
  byte result = cpu->x - 1;
  cpu->x = result;
  cpu_zn(cpu, result);
}

void cpu_dey(CPU * cpu, Address addr) {
  byte result = cpu->y - 1;
  cpu->y = result;
  cpu_zn(cpu, result);
}

void cpu_eor(CPU * cpu, Address addr) {
  byte result = cpu->a ^ memory_read(cpu->mem, addr.val);
  cpu->a = result;
  cpu_zn(cpu, result);
}

void cpu_inc(CPU * cpu, Address addr) {
  byte result = memory_read(cpu->mem, addr.val) + 1;
  memory_write(cpu->mem, addr.val, result);
  cpu_zn(cpu, result);
}

void cpu_inx(CPU * cpu, Address addr) {
  byte result = cpu->x + 1;
  cpu->x = result;
  cpu_zn(cpu, result);
}

void cpu_iny(CPU * cpu, Address addr) {
  byte result = cpu->y + 1;
  cpu->y = result;
  cpu_zn(cpu, result);
}

void cpu_jmp(CPU * cpu, Address addr) {
  cpu->pc = addr.val;
}

void cpu_jsr(CPU * cpu, Address addr) {
  cpu_push16(cpu, cpu->pc);
  cpu->pc = addr.val;
}

void cpu_lda(CPU * cpu, Address addr) {
  byte val = memory_read(cpu->mem, addr.val);
  cpu->a = val;
  cpu_zn(cpu, val);
}

void cpu_ldx(CPU * cpu, Address addr) {
  byte val = memory_read(cpu->mem, addr.val);
  cpu->x = val;
  cpu_zn(cpu, val);
}

void cpu_ldy(CPU * cpu, Address addr) {
  byte val = memory_read(cpu->mem, addr.val);
  cpu->y = val;
  cpu_zn(cpu, val);
}

void cpu_lsr(CPU * cpu, Address addr) {
  byte result;

  if (addr.null) {
    result = lsr(cpu->a);
    cpu->a = result;
  } else {
    result = lsr(memory_read(cpu->mem, addr.val));
    memory_write(cpu->mem, addr.val, result);
  }

  cpu_zn(cpu, result);
}

void cpu_nop(CPU * cpu, Address addr) {

}

void cpu_ora(CPU * cpu, Address addr) {
  byte result = cpu->a | memory_read(cpu->mem, addr.val);
  cpu->a = result;
  cpu_zn(cpu, result);
}

void cpu_pha(CPU * cpu, Address addr) {
  cpu_push(cpu, cpu->a);
}

void cpu_php(CPU * cpu, Address addr) {
  cpu_push(cpu, cpu->status | 0x30);
}

void cpu_pla(CPU * cpu, Address addr) {
  byte val = cpu_pull(cpu);
  cpu->a = val;
  cpu_zn(cpu, val);
}

void cpu_plp(CPU * cpu, Address addr) {
  cpu->status = (cpu_pull(cpu) & ~0x30) | (cpu->status & 0x30);
}

void cpu_rol(CPU * cpu, Address addr) {
  byte result;

  if (addr.null) {
    result = rol(cpu->a);
    cpu->a = result;
  } else {
    result = rol(memory_read(cpu->mem, addr.val));
    memory_write(cpu->mem, addr.val, result);
  }

  cpu_zn(cpu, result);
}

void cpu_ror(CPU * cpu, Address addr) {
  byte result;

  if (addr.null) {
    result = ror(cpu->a);
    cpu->a = result;
  } else {
    result = ror(memory_read(cpu->mem, addr.val));
    memory_write(cpu->mem, addr.val, result);
  }

  cpu_zn(cpu, result);
}

void cpu_rti(CPU * cpu, Address addr) {
  cpu_plp(cpu, addr);
  cpu_rts(cpu, addr);
}

void cpu_rts(CPU * cpu, Address addr) {
  cpu->pc = cpu_pull16(cpu);
}

void cpu_sbc(CPU * cpu, Address addr) {
  byte a = cpu->a;
  byte b = memory_read(cpu->mem, addr.val);

  int16_t result = a - b - (1 - cpu->c);
  cpu->a = result;
  cpu->c = result >= 0x00;
  cpu->v = ((a^b) >> 7 & 1) && ((result^a) >> 7 & 1);
  cpu_zn(cpu, result);
}

void cpu_sec(CPU * cpu, Address addr) {
  cpu->c = 1;
}

void cpu_sed(CPU * cpu, Address addr) {
  cpu->d = 1;
}

void cpu_sei(CPU * cpu, Address addr) {
  cpu->i = 1;
}

void cpu_sta(CPU * cpu, Address addr) {
  memory_write(cpu->mem, addr.val, cpu->a);
}

void cpu_stx(CPU * cpu, Address addr) {
  memory_write(cpu->mem, addr.val, cpu->x);
}

void cpu_sty(CPU * cpu, Address addr) {
  memory_write(cpu->mem, addr.val, cpu->y);
}

void cpu_tax(CPU * cpu, Address addr) {
  byte val = cpu->a;
  cpu->x = val;
  cpu_zn(cpu, val);
}

void cpu_tay(CPU * cpu, Address addr) {
  byte val = cpu->a;
  cpu->y = val;
  cpu_zn(cpu, val);
}

void cpu_tsx(CPU * cpu, Address addr) {
  byte val = cpu->sp;
  cpu->x = val;
  cpu_zn(cpu, val);
}

void cpu_txa(CPU * cpu, Address addr) {
  byte val = cpu->x;
  cpu->a = val;
  cpu_zn(cpu, val);
}

void cpu_txs(CPU * cpu, Address addr) {
  byte val = cpu->x;
  cpu->sp = val;
}

void cpu_tya(CPU * cpu, Address addr) {
  byte val = cpu->y;
  cpu->a = val;
  cpu_zn(cpu, val);
}

/*
 * Unoffical instructions
 */
void cpu_ahx(CPU * cpu, Address addr) {
  printf("STUB\n");
}

void cpu_alr(CPU * cpu, Address addr) {
  printf("STUB\n");
}

void cpu_anc(CPU * cpu, Address addr) {
  printf("STUB\n");
}

void cpu_arr(CPU * cpu, Address addr) {
  printf("STUB\n");
}

void cpu_axs(CPU * cpu, Address addr) {
  printf("STUB\n");
}

void cpu_dcp(CPU * cpu, Address addr) {
  printf("STUB\n");
}

void cpu_isc(CPU * cpu, Address addr) {
  printf("STUB\n");
}

void cpu_las(CPU * cpu, Address addr) {
  printf("STUB\n");
}

void cpu_lax(CPU * cpu, Address addr) {
  printf("STUB\n");
}

void cpu_rla(CPU * cpu, Address addr) {
  printf("STUB\n");
}

void cpu_rra(CPU * cpu, Address addr) {
  printf("STUB\n");
}

void cpu_sax(CPU * cpu, Address addr) {
  printf("STUB\n");
}

void cpu_shx(CPU * cpu, Address addr) {
  printf("STUB\n");
}

void cpu_shy(CPU * cpu, Address addr) {
  printf("STUB\n");
}

void cpu_slo(CPU * cpu, Address addr) {
  printf("STUB\n");
}

void cpu_sre(CPU * cpu, Address addr) {
  printf("STUB\n");
}

void cpu_stp(CPU * cpu, Address addr) {
  printf("STUB\n");
}

void cpu_tas(CPU * cpu, Address addr) {
  printf("STUB\n");
}

void cpu_xaa(CPU * cpu, Address addr) {
  printf("STUB\n");
}

/**
 * Debugging
 */
void cpu_debug_instr(CPU * cpu, char * buffer) {
  int i = 0;

  uint16_t pc = cpu->pc;
  byte sp = cpu->sp;
  byte a = cpu->a;
  byte x = cpu->x;
  byte y = cpu->y;
  byte status = cpu->status;

  i += sprintf(buffer + i, "%04X  %02X ", pc, memory_read(cpu->mem, pc));

  byte opcode = memory_read(cpu->mem, pc);
  Instruction instruction = opcode_instruction[opcode];
  const char * name = instruction_name[instruction];

  switch (opcode_addressing_mode[opcode]) {
  case ADDRMODE_0:
    i += sprintf(buffer + i, "       %s                             ", name);
    break;
  case ADDRMODE_1:
    i += sprintf(buffer + i, "%02X     %s #$%02X                        ", memory_read(cpu->mem, pc + 1), name, memory_read(cpu->mem, pc + 1));
    break;
  case ADDRMODE_2:
    i += sprintf(buffer + i, "%02X     %s $%02X = %02X                    ", memory_read(cpu->mem, pc + 1), name, memory_read(cpu->mem, pc + 1), memory_read(cpu->mem, memory_read(cpu->mem, pc + 1)));
    break;
  case ADDRMODE_3:
    // Do not resolve address when jumping
    if (instruction == INSTR_JMP || instruction == INSTR_JSR) {
      i += sprintf(buffer + i, "%02X %02X  %s $%04X                       ", memory_read(cpu->mem, pc + 1), memory_read(cpu->mem, pc + 2), name, memory_read16(cpu->mem, pc + 1));
    } else {
      i += sprintf(buffer + i, "%02X %02X  %s $%04X = %02X                  ", memory_read(cpu->mem, pc + 1), memory_read(cpu->mem, pc + 2), name, memory_read16(cpu->mem, pc + 1), memory_read(cpu->mem, memory_read16(cpu->mem, pc + 1)));
    }
    break;
  case ADDRMODE_8:
    i += sprintf(buffer + i, "%02X     %s $%02X                       ", memory_read(cpu->mem, pc + 1), name, memory_read(cpu->mem, pc + 1) + pc + 2);
    break;
  case ADDRMODE_B:
    i += sprintf(buffer + i, "       %s                             ", name);
    break;
  case ADDRMODE_C:
    i += sprintf(buffer + i, "       %s                             ", name);
    break;
  case ADDRMODE_9:
    i += sprintf(buffer + i, "       %s                             ", name);
    break;
  case ADDRMODE_A:
    i += sprintf(buffer + i, "       %s                             ", name);
    break;
  case ADDRMODE_4:
    i += sprintf(buffer + i, "       %s                             ", name);
    break;
  case ADDRMODE_5:
    i += sprintf(buffer + i, "       %s                             ", name);
    break;
  case ADDRMODE_6:
    i += sprintf(buffer + i, "       %s                             ", name);
    break;
  case ADDRMODE_7:
    i += sprintf(buffer + i, "       %s                             ", name);
    break;
  }

  i += sprintf(buffer + i, "A:%02X X:%02X Y:%02X P:%02X SP:%02X\n", a, x, y, status, sp);
}

bool cpu_debug_next(CPU * cpu, const char * buffer) {
  char * ptr;
  int num = strtol(buffer, &ptr, 0);
  if (ptr == buffer) {
    num = 1;
  }

  while (num > 0) {
    char debug[128];
    cpu_debug_instr(cpu, debug);
    printf("%s", debug);
    cpu_next_instr(cpu);
    num -= 1;
  }

  return true;
}

bool cpu_debug_goto(CPU * cpu, const char * buffer) {
  char * ptr;
  int addr = strtol(buffer, &ptr, 0);
  if (ptr == buffer) {
    printf("Expected address\n");
    return true;
  }

  cpu->pc = addr;
  printf("PC = $%04X\n", addr);
  return true;
}

bool cpu_debug_reset(CPU * cpu, const char * buffer) {
  cpu_reset(cpu);
  memory_reset(cpu->mem);
  printf("Reset NES\n");
  return true;
}

bool cpu_debug_test(CPU * cpu, const char * buffer) {
  char * ptr;
  int tolerance = strtol(buffer, &ptr, 0);
  if (ptr == buffer) {
    tolerance = 1;
  }

  cpu_debug_reset(cpu, buffer);
  printf("\n");

  FILE * fp = fopen("sub-nestest.log", "r");

  int lineno = 1;
  char test[128], debug[128];
  while (tolerance != 0 && fgets(test, ARRAY_LENGTH(test), fp) != NULL) {
    cpu_debug_instr(cpu, debug);
    printf("%s", debug);

    if (strcmp(debug, test) != 0) {
      printf("\nTest Failed (line %i):\nExpected: %sObtained: %s", lineno, test, debug);
      if (--tolerance != 0) {
        printf("\n");
      }
    }

    cpu_next_instr(cpu);
    lineno += 1;
  }

  fclose(fp);

  printf("\n");
  cpu_debug_reset(cpu, buffer);
  return true;
}

bool cpu_debug_quit(CPU * cpu, const char * buffer) {
  return false;
}

const struct {
  const char * name;
  bool (*action)(CPU * cpu, const char * buffer);
} cpu_debug_commands[] = {
  {"next", cpu_debug_next},
  {"goto", cpu_debug_goto},
  {"reset", cpu_debug_reset},
  {"rs", cpu_debug_reset},
  {"test", cpu_debug_test},
  {"quit", cpu_debug_quit},
  {"exit", cpu_debug_quit}
};

void cpu_debug(CPU * cpu) {
  char buffer[256];

  printf("> ");
  while(fgets(buffer, ARRAY_LENGTH(buffer), stdin) != NULL) {
    size_t len;
    char * params = strchr(buffer, ' ');
    if (!params) {
      len = strlen(buffer) - 1;
      params = buffer + len;
    } else {
      len = params - buffer;
    }

    size_t i;
    bool valid = false;
    bool quit = false;
    for (i = 0; i < ARRAY_LENGTH(cpu_debug_commands); ++i) {
      if (strncmp(buffer, cpu_debug_commands[i].name, len) == 0) {
        quit = !cpu_debug_commands[i].action(cpu, params);
        valid = true;
        break;
      }
    }

    if (quit) {
      break;
    }

    if (!valid) {
      printf("Invalid command\n");
    }

    printf("> ");
  }
}
