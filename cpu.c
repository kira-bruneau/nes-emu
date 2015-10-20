#include <glib.h>
#include <stdio.h>

#include "cpu.h"

CPU * cpu_new(Memory * mem) {
  CPU * cpu = g_malloc(sizeof(CPU));
  cpu->mem = mem;
  return cpu;
}

void cpu_reset(CPU * cpu) {
  cpu->pc = memory_read(cpu->mem, 0xFFFC);
}

byte cpu_read_pc(CPU * cpu) {
  byte addr = memory_read(cpu->mem, cpu->pc);
  cpu->pc += 1;
  return addr;
}

uint16_t cpu_read_pc16(CPU * cpu) {
  uint16_t addr = memory_read16(cpu->mem, cpu->pc);
  cpu->pc += 2;
  return addr;
}

void cpu_eval_next(CPU * cpu) {
  byte opcode = cpu_read_pc(cpu);
  printf("%X", opcode);
  /* switch (opcode % 4) { */
  /* case 0: */
  /*   cpu_eval_control(opcode); */
  /* case 1: */
  /*   cpu_eval_alu(opcode); */
  /* case 2: */
  /*   cpu_eval_rmw(opcode); */
  /* case 3: */
  /*   cpu_eval_unofficial(opcode); */
  /* } */
}

/* void cpu_eval_control(byte opcode) { */

/* } */

/* void cpu_eval_alu(byte opcode) { */

/* } */

/* void cpu_eval_rmw(byte opcode) { */

/* } */

/* void cpu_eval_unofficial(byte opcode) { */

/* } */
