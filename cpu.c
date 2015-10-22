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
  printf("%04X  %02X ", cpu->pc, memory_read(cpu->mem, cpu->pc));
  byte opcode = cpu_next_memory(cpu);

  Instruction instruction = opcode_instruction[opcode];
  const char * name = instruction_name[instruction];
  Action action = instruction_action[instruction];

  AdressingMode mode = opcode_addressing_mode[opcode];
  switch (mode) {
  case ADDRMODE_0:
    printf("       %s", name);
    action(cpu, 0);
    break;
  case ADDRMODE_1:
    printf("%02X     %s #$%02X", memory_read(cpu->mem, cpu->pc), name, memory_read(cpu->mem, cpu->pc));
    action(cpu, cpu->pc++);
    break;
  case ADDRMODE_2:
    printf("%02X     %s $%02X", memory_read(cpu->mem, cpu->pc), name, memory_read(cpu->mem, cpu->pc));
    action(cpu, cpu_next_memory(cpu));
    break;
  case ADDRMODE_3:
    printf("%02X %02X  %s $%04X", memory_read(cpu->mem, cpu->pc), memory_read(cpu->mem, cpu->pc + 1), name, memory_read16(cpu->mem, cpu->pc));
    action(cpu, cpu_next_memory16(cpu));
    break;
  case ADDRMODE_8:
    printf("%02X     %s $%02X", memory_read(cpu->mem, cpu->pc), name, memory_read(cpu->mem, cpu->pc) + cpu->pc + 1);
    action(cpu, cpu_next_memory(cpu) + cpu->pc);
    break;
  case ADDRMODE_B:
    printf("%s", name);
    action(cpu, cpu_next_memory(cpu) + cpu->x);
    break;
  case ADDRMODE_C:
    printf("%s", name);
    action(cpu, cpu_next_memory(cpu) + cpu->y);
    break;
  case ADDRMODE_9:
    printf("%s", name);
    action(cpu, cpu_next_memory16(cpu) + cpu->x);
    break;
  case ADDRMODE_A:
    printf("%s", name);
    action(cpu, cpu_next_memory16(cpu) + cpu->y);
    break;
  case ADDRMODE_4:
    printf("%s", name);
    action(cpu, memory_read16(cpu->mem, cpu_next_memory16(cpu)));
    break;
  case ADDRMODE_5:
    printf("%s", name);
    action(cpu, memory_read(cpu->mem, cpu_next_memory(cpu)) + cpu->y);
    break;
  case ADDRMODE_6:
    printf("%s", name);
    action(cpu, memory_read(cpu->mem, cpu_next_memory(cpu) + cpu->x));
    break;
  case ADDRMODE_7:
    printf("%s", name);
    action(cpu, memory_read(cpu->mem, cpu_next_memory(cpu) + cpu->y));
    break;
  }
}
