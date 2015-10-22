#include <glib.h>
#include <stdio.h>

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

/**
 * Testing
 */

void cpu_debug_instr(CPU * cpu) {
  // Snapshot registers
  uint16_t pc = cpu->pc;
  byte sp = cpu->sp;
  byte a = cpu->a;
  byte x = cpu->x;
  byte y = cpu->y;

  byte opcode = pc;
  Instruction instruction = opcode_instruction[opcode];
  Action action = instruction_action[instruction];

  const char * name = instruction_name[instruction];
  AdressingMode mode = opcode_addressing_mode[opcode];
  switch (mode) {
  case ADDRMODE_0:
    action(cpu, 0);
    break;
  case ADDRMODE_1:
    printf("%02X     %s #$%02X                        ", memory_read(cpu->mem, pc + 1), name, memory_read(cpu->mem, pc + 1));
    break;
  case ADDRMODE_2:
    printf("%02X     %s $%02X = %02X                    ", memory_read(cpu->mem, pc + 1), name, memory_read(cpu->mem, pc + 1), memory_read(cpu->mem, memory_read(cpu->mem, pc + 1)));
    break;
  case ADDRMODE_3:
    printf("%02X %02X  %s $%04X                       ", memory_read(cpu->mem, pc + 1), memory_read(cpu->mem, pc + 2), name, memory_read16(cpu->mem, pc + 1));
    break;
  case ADDRMODE_8:
    printf("%02X     %s $%02X                       ", memory_read(cpu->mem, pc + 1), name, memory_read(cpu->mem, pc + 1) + pc + 2);
    break;
  case ADDRMODE_B:
    printf("%s                             ", name);
    break;
  case ADDRMODE_C:
    printf("%s                             ", name);
    break;
  case ADDRMODE_9:
    printf("%s                             ", name);
    break;
  case ADDRMODE_A:
    printf("%s                             ", name);
    break;
  case ADDRMODE_4:
    printf("%s                             ", name);
    break;
  case ADDRMODE_5:
    printf("%s                             ", name);
    break;
  case ADDRMODE_6:
    printf("%s                             ", name);
    break;
  case ADDRMODE_7:
    printf("%s                             ", name);
    break;
  }

  printf("A:%02X X:%02X Y:%02X SP:%02X", a, x, y, sp);
}

void cpu_test_interactive(CPU * cpu) {
  // Number of instructions to perform automatically
  int i;
  for (i = 0; i < 0; ++i) {
    cpu_next_instr(cpu);
    printf("\n");
  }

  char buffer[256];
  while(fgets(buffer, ARRAY_LENGTH(buffer), stdin) != NULL) {
    cpu_debug_instr(cpu);
    cpu_next_instr(cpu);
  }
}

void cpu_test_automated(CPU * cpu) {
  // Stub
}
