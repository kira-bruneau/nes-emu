#include <glib.h>
#include <stdio.h>

#include "cpu.h"

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

void cpu_eval_clc(CPU * cpu) {
  printf("CLC\n");
  cpu_write_c(cpu, 0);
}

// TODO: Unfinished
void cpu_eval_jsr(CPU * cpu) {
  uint16_t addr = cpu_next_16(cpu);
  printf("JSR $%04X\n", addr);
  cpu_write_pc(cpu, addr);
}

void cpu_eval_sec(CPU * cpu) {
  printf("SEC\n");
  cpu_write_c(cpu, 1);
}

void cpu_eval_jmp(CPU * cpu) {
  uint16_t addr = cpu_next_16(cpu);
  printf("JMP $%04X\n", addr);
  cpu_write_pc(cpu, addr);
}

void cpu_eval_stx(CPU * cpu) {
  byte addr = cpu_next_8(cpu);
  byte val = cpu_read_x(cpu);
  printf("STX $%02X = %02X\n", addr, val);
  memory_write(cpu->mem, addr, val);
}

void cpu_eval_bcc(CPU * cpu) {
  byte offset = cpu_next_8(cpu);
  uint16_t addr = cpu_read_pc(cpu) + offset;
  printf("BCC $%04X\n", addr);
  if (!cpu_read_c(cpu)) {
    cpu_write_pc(cpu, addr);
  }
}

void cpu_eval_ldx(CPU * cpu) {
  byte val = cpu_next_8(cpu);
  printf("LDX #$%02X\n", val);
  cpu_write_x(cpu, val);
  cpu_write_z(cpu, val == 0);
}

void cpu_eval_lda(CPU * cpu) {
  byte val = cpu_next_8(cpu);
  printf("LDA #$%02X\n", val);
  cpu_write_a(cpu, val);
  cpu_write_z(cpu, val == 0);
}

void cpu_eval_bcs(CPU * cpu) {
  byte offset = cpu_next_8(cpu);
  uint16_t addr = cpu_read_pc(cpu) + offset;
  printf("BCS $%04X\n", addr);
  if (cpu_read_c(cpu)) {
    cpu_write_pc(cpu, addr);
  }
}

void cpu_eval_nop(CPU * cpu) {
  printf("NOP\n");
}

void cpu_next_instr(CPU * cpu) {
  printf("%04X  ", cpu_read_pc(cpu));
  byte opcode = cpu_next_8(cpu);
  switch (opcode) {
  case 0x18:
    cpu_eval_clc(cpu);
    break;
  case 0x20:
    cpu_eval_jsr(cpu);
    break;
  case 0x38:
    cpu_eval_sec(cpu);
    break;
  case 0x4C:
    cpu_eval_jmp(cpu);
    break;
  case 0x86:
    cpu_eval_stx(cpu);
    break;
  case 0x90:
    cpu_eval_bcc(cpu);
    break;
  case 0xA2:
    cpu_eval_ldx(cpu);
    break;
  case 0xA9:
    cpu_eval_lda(cpu);
    break;
  case 0xB0:
    cpu_eval_bcs(cpu);
    break;
  case 0xEA:
    cpu_eval_nop(cpu);
    break;
  default:
    fprintf(stderr, "Unknown opcode %X\n", opcode);
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
