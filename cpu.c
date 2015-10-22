#include <glib.h>
#include <stdio.h>

#include "cpu.h"
#include "opcode.h"

CPU * cpu_new(Memory * mem) {
  CPU * cpu = g_malloc(sizeof(CPU));
  cpu->mem = mem;
  cpu_reset(cpu);
  return cpu;
}

void cpu_reset(CPU * cpu) {
  cpu_write_pc(cpu, 0xC000);
  cpu_write_sp(cpu, 0xFD);

  cpu->a = 0;
  cpu->x = 0;
  cpu->y = 0;

  cpu->c = 0;
  cpu->z = 0;
  cpu->i = 0;
  cpu->d = 0;
  cpu->b = 0;
  cpu->v = 0;
  cpu->n = 0;
}

////////////////////////////////////////////////////////////////////////////////

void cpu_next_instr(CPU * cpu) {
  printf("%04X  %02X ", cpu->pc, memory_read(cpu->mem, cpu->pc));
  byte opcode = cpu_next_8(cpu);

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
    action(cpu, cpu_next_8(cpu));
    break;
  case ADDRMODE_3:
    printf("%02X %02X  %s $%04X", memory_read(cpu->mem, cpu->pc), memory_read(cpu->mem, cpu->pc + 1), name, memory_read16(cpu->mem, cpu->pc));
    action(cpu, cpu_next_16(cpu));
    break;
  case ADDRMODE_8:
    printf("%02X     %s $%02X", memory_read(cpu->mem, cpu->pc), name, memory_read(cpu->mem, cpu->pc) + cpu->pc + 1);
    action(cpu, cpu_next_8(cpu) + cpu->pc);
    break;
  case ADDRMODE_B:
    printf("%s", name);
    action(cpu, cpu_next_8(cpu) + cpu->x);
    break;
  case ADDRMODE_C:
    printf("%s", name);
    action(cpu, cpu_next_8(cpu) + cpu->y);
    break;
  case ADDRMODE_9:
    printf("%s", name);
    action(cpu, cpu_next_16(cpu) + cpu->x);
    break;
  case ADDRMODE_A:
    printf("%s", name);
    action(cpu, cpu_next_16(cpu) + cpu->y);
    break;
  case ADDRMODE_4:
    printf("%s", name);
    action(cpu, memory_read16(cpu->mem, cpu_next_16(cpu)));
    break;
  case ADDRMODE_5:
    printf("%s", name);
    action(cpu, memory_read(cpu->mem, cpu_next_8(cpu)) + cpu->y);
    break;
  case ADDRMODE_6:
    printf("%s", name);
    action(cpu, memory_read(cpu->mem, cpu_next_8(cpu) + cpu->x));
    break;
  case ADDRMODE_7:
    printf("%s", name);
    action(cpu, memory_read(cpu->mem, cpu_next_8(cpu) + cpu->y));
    break;
  }
}

////////////////////////////////////////////////////////////////////////////////

// Obtain and increment the next 8 bits in PC
byte cpu_next_8(CPU * cpu) {
  byte addr = memory_read(cpu->mem, cpu->pc);
  cpu->pc += 1;
  return addr;
}

// Obtain and increment the next 16 bits in PC
uint16_t cpu_next_16(CPU * cpu) {
  uint16_t addr = memory_read16(cpu->mem, cpu->pc);
  cpu->pc += 2;
  return addr;
}

////////////////////////////////////////////////////////////////////////////////

// Program counter register read & write
uint16_t cpu_read_pc(CPU * cpu) {
  return cpu->pc;
}

void cpu_write_pc(CPU * cpu, uint16_t val) {
  cpu->pc = val;
}

// SP register read & write
byte cpu_read_sp(CPU * cpu) {
  return cpu->sp;
}

void cpu_write_sp(CPU * cpu, byte val) {
  cpu->sp = val;
}

// A register read & write
byte cpu_read_a(CPU * cpu) {
  return cpu->a;
}

void cpu_write_a(CPU * cpu, byte val) {
  cpu->a = val;
}

// X register read & write
byte cpu_read_x(CPU * cpu) {
  return cpu->x;
}

void cpu_write_x(CPU * cpu, byte val) {
  cpu->x = val;
}

// Y register read & write
byte cpu_read_y(CPU * cpu) {
  return cpu->y;
}

void cpu_write_y(CPU * cpu, byte val) {
  cpu->y = val;
}

// Flag register read & write
byte cpu_read_n(CPU * cpu) {
  return cpu->n;
}

void cpu_write_n(CPU * cpu, byte val) {
  cpu->n = val;
}

byte cpu_read_v(CPU * cpu) {
  return cpu->v;
}

void cpu_write_v(CPU * cpu, byte val) {
  cpu->v = val;
}

byte cpu_read_b(CPU * cpu) {
  return cpu->b;
}

void cpu_write_b(CPU * cpu, byte val) {
  cpu->b = val;
}

byte cpu_read_d(CPU * cpu) {
  return cpu->d;
}

void cpu_write_d(CPU * cpu, byte val) {
  cpu->d = val;
}

byte cpu_read_i(CPU * cpu) {
  return cpu->i;
}

void cpu_write_i(CPU * cpu, byte val) {
  cpu->i = val;
}

byte cpu_read_z(CPU * cpu) {
  return cpu->z;
}

void cpu_write_z(CPU * cpu, byte val) {
  cpu->z = val;
}

byte cpu_read_c(CPU * cpu) {
  return cpu->c;
}

void cpu_write_c(CPU * cpu, byte val) {
  cpu->c = val;
}
