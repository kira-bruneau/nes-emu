#include <glib.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "cpu.h"
#include "opcode.h"

void cpu_reset(CPU * cpu) {
  cpu->pc = 0xC000;
  cpu->sp = 0xFD;
  cpu->a = 0;
  cpu->x = 0;
  cpu->y = 0;
  cpu->status = 0;
  cpu->e = 1;
}

CPU * cpu_new(Memory * mem) {
  CPU * cpu = g_malloc(sizeof(CPU));
  cpu->mem = mem;
  cpu_reset(cpu);
  return cpu;
}

////////////////////////////////////////////////////////////////////////////////

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
  Action action = instruction_action[instruction];
  AdressingMode mode = opcode_addressing_mode[opcode];

  switch (mode) {
  case ADDRMODE_0:
    action(cpu, 0);
    break;
  case ADDRMODE_1:
    action(cpu, cpu->pc++);
    break;
  case ADDRMODE_2:
    action(cpu, cpu_next_memory(cpu));
    break;
  case ADDRMODE_3:
    action(cpu, cpu_next_memory16(cpu));
    break;
  case ADDRMODE_8:
    action(cpu, cpu_next_memory(cpu) + cpu->pc);
    break;
  case ADDRMODE_B:
    action(cpu, cpu_next_memory(cpu) + cpu->x);
    break;
  case ADDRMODE_C:
    action(cpu, cpu_next_memory(cpu) + cpu->y);
    break;
  case ADDRMODE_9:
    action(cpu, cpu_next_memory16(cpu) + cpu->x);
    break;
  case ADDRMODE_A:
    action(cpu, cpu_next_memory16(cpu) + cpu->y);
    break;
  case ADDRMODE_4:
    action(cpu, memory_read16(cpu->mem, cpu_next_memory16(cpu)));
    break;
  case ADDRMODE_5:
    action(cpu, memory_read(cpu->mem, cpu_next_memory(cpu)) + cpu->y);
    break;
  case ADDRMODE_6:
    action(cpu, memory_read(cpu->mem, cpu_next_memory(cpu) + cpu->x));
    break;
  case ADDRMODE_7:
    action(cpu, memory_read(cpu->mem, cpu_next_memory(cpu) + cpu->y));
    break;
  }
}

void cpu_debug_instr(CPU * cpu, char * buffer) {
  int i = 0;

  uint16_t pc = cpu->pc;
  byte sp = cpu->sp;
  byte a = cpu->a;
  byte x = cpu->x;
  byte y = cpu->y;

  i += sprintf(buffer + i, "%04X  %02X ", pc, memory_read(cpu->mem, pc));

  byte opcode = cpu_next_memory(cpu);
  Instruction instruction = opcode_instruction[opcode];
  const char * name = instruction_name[instruction];
  AdressingMode mode = opcode_addressing_mode[opcode];

  switch (mode) {
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
    i += sprintf(buffer + i, "%02X %02X  %s $%04X                       ", memory_read(cpu->mem, pc + 1), memory_read(cpu->mem, pc + 2), name, memory_read16(cpu->mem, pc + 1));
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

  i += sprintf(buffer + i, "A:%02X X:%02X Y:%02X SP:%02X\n", a, x, y, sp);
  cpu->pc = pc;
}

void cpu_test_automated(CPU * cpu) {
  FILE * fp = fopen("sub-nestest.log", "r");

  int lineno = 1;
  char test[128], debug[128];
  while (fgets(test, ARRAY_LENGTH(test), fp) != NULL) {
    cpu_debug_instr(cpu, debug);

    if (strcmp(debug, test) != 0) {
      printf("Test Failed (line %i):\n  Expected: %s  Obtained: %s", lineno, test, debug);
      break;
    }

    cpu_next_instr(cpu);
    lineno += 1;
  }

  fclose(fp);
}

void cpu_test_interactive(CPU * cpu) {
  char buffer[16];
  printf("> ");
  while(fgets(buffer, ARRAY_LENGTH(buffer), stdin) != NULL) {
    if (strncmp(buffer, "g", 1) == 0) {
      int addr = strtol(buffer + 1, NULL, 0);
      cpu->pc = addr;
      printf("pc = $%04X\n", addr);
    } else if (strncmp(buffer, "n", 1) == 0 || strlen(buffer) == 1) {
      char debug[128];
      cpu_debug_instr(cpu, debug);
      printf("%s", debug);
      cpu_next_instr(cpu);
    } else if (strncmp(buffer, "a", 1) == 0) {
      cpu_test_automated(cpu);
    } else if (strncmp(buffer, "q", 1) == 0) {
      break;
    } else {
      printf("%s\n", "Invalid command");
    }

    printf("> ");
  }
}
