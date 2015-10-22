#include <stdio.h>

#include "cpu.h"
#include "util.h"

// Manual: http://users.telenet.be/kim1-6502/6502/proman.html
// Opcodes: http://wiki.nesdev.com/w/index.php/CPU_unofficial_opcodes

// Adressing types:
//   Immediate
//   Absolute
//   Zero Page
//   Absolute,X
//   Absolute,Y
//   Zero Page,X
//   Indexed Indirect
//   Indirect Indexed

// Instructions that modify the flags:
// c: ADC, ASL, CLC, CMP, CPX, CPY, LSR, PLP, ROL, RTI, SBC, SEC
// z: ADC, AND, ASL, BIT, CMP, CPY, CPX, DEC, DEX, DEY, EOR, INC, INX, INY, LDA, LDX, LDY, LSR, ORA, PLA, PLP, ROL, RTI, SBC, TAX, TAY, TXA, TYA
// i: BRK, CLI, PLP, RTI, SEI
// d: CLD, PLP, RTI, SED
// b: NONE
// e: NOT USED
// v: ADC, BIT, CLV, PLP, RTI, SBC
// n: ADC, AND, ASL, BIT, CMP, CPY, CPX, DEC, DEX, DEY, EOR, INC, INX, INY, LDA, LDX, LDY, LSR, ORA, PLA, PLP, ROL, BIT, SBC, TAX, TAY, TSX, TXA, TYA

/*
 * Stack operations
 */
static void cpu_push(CPU * cpu, byte value) {
  memory_write(cpu->mem, cpu->sp, value);
  cpu->sp -= 1;
}

static void cpu_push16(CPU * cpu, uint16_t value) {
  memory_write16(cpu->mem, cpu->sp - 1, value);
  cpu->sp -= 2;
}

static byte cpu_pull(CPU * cpu) {
  byte value = memory_read(cpu->mem, cpu->sp + 1);
  cpu->sp += 1;
  return value;
}

static uint16_t cpu_pull16(CPU * cpu) {
  uint16_t value = memory_read16(cpu->mem, cpu->sp + 1);
  cpu->sp += 2;
  return value;
}

/**
 * Value operations
 */
static void cpu_zn(CPU * cpu, byte value) {
  cpu->z = value == 0;
  cpu->n = (value >> 7) & 1;
}

static void cpu_compare(CPU * cpu, byte a, byte b) {
  byte result = a - b;
  cpu->c = b <= a;
  cpu_zn(cpu, result);
}

/*
 * Official instructions
 */
void cpu_adc(CPU * cpu, uint16_t addr) {
  uint16_t result = cpu->a + memory_read(cpu->mem, addr) + cpu->c;
  cpu->c = result > 0xFF;
  cpu->v = 0; // Unfinished
  cpu->a = result;
}

void cpu_and(CPU * cpu, uint16_t addr) {
  byte result = cpu->a & memory_read(cpu->mem, addr);
  cpu_zn(cpu, result);
  cpu->a = result;
}

void cpu_asl(CPU * cpu, uint16_t addr) {
  printf(" - STUB");
}

void cpu_bcc(CPU * cpu, uint16_t addr) {
  if (!cpu->c) {
    cpu->pc = addr;
  }
}

void cpu_bcs(CPU * cpu, uint16_t addr) {
  if (cpu->c) {
    cpu->pc = addr;
  }
}

void cpu_beq(CPU * cpu, uint16_t addr) {
  if (cpu->z) {
    cpu->pc = addr;
  }
}

void cpu_bit(CPU * cpu, uint16_t addr) {
  byte value = memory_read(cpu->mem, addr);
  cpu->z = value & cpu->a;
  cpu->v = (value >> 6) & 1;
  cpu->n = (value >> 7) & 1;
}

void cpu_bmi(CPU * cpu, uint16_t addr) {
  if (cpu->n) {
    cpu->pc = addr;
  }
}

void cpu_bne(CPU * cpu, uint16_t addr) {
  if (!cpu->z) {
    cpu->pc = addr;
  }
}

void cpu_bpl(CPU * cpu, uint16_t addr) {
  if (!cpu->n) {
    cpu->pc = addr;
  }
}

void cpu_bvc(CPU * cpu, uint16_t addr) {
  if (!cpu->v) {
    cpu->pc = addr;
  }
}

void cpu_bvs(CPU * cpu, uint16_t addr) {
  if (cpu->v) {
    cpu->pc = addr;
  }
}

void cpu_clc(CPU * cpu, uint16_t addr) {
  cpu->c = 0;
}

void cpu_cld(CPU * cpu, uint16_t addr) {
  cpu->d = 0;
}

void cpu_cli(CPU * cpu, uint16_t addr) {
  cpu->i = 0;
}

void cpu_clv(CPU * cpu, uint16_t addr) {
  cpu->v = 0;
}

void cpu_cmp(CPU * cpu, uint16_t addr) {
  byte value = memory_read(cpu->mem, addr);
  cpu_compare(cpu, cpu->a, value);
}

void cpu_cpx(CPU * cpu, uint16_t addr) {
  byte value = memory_read(cpu->mem, addr);
  cpu_compare(cpu, cpu->x, value);
}

void cpu_cpy(CPU * cpu, uint16_t addr) {
  byte value = memory_read(cpu->mem, addr);
  cpu_compare(cpu, cpu->y, value);
}

void cpu_dec(CPU * cpu, uint16_t addr) {
  byte result = memory_read(cpu->mem, addr) - 1;
  cpu_zn(cpu, result);
  memory_write(cpu->mem, addr, result);
}

void cpu_dex(CPU * cpu, uint16_t addr) {
  byte result = cpu->x - 1;
  cpu_zn(cpu, result);
  cpu->x = result;
}

void cpu_dey(CPU * cpu, uint16_t addr) {
  byte result = cpu->y - 1;
  cpu_zn(cpu, result);
  cpu->y = result;
}

void cpu_eor(CPU * cpu, uint16_t addr) {
  byte result = cpu->a ^ memory_read(cpu->mem, addr);
  cpu_zn(cpu, result);
  cpu->a = result;
}

void cpu_inc(CPU * cpu, uint16_t addr) {
  byte result = memory_read(cpu->mem, addr) + 1;
  cpu_zn(cpu, result);
  memory_write(cpu->mem, addr, result);
}

void cpu_inx(CPU * cpu, uint16_t addr) {
  byte result = cpu->x + 1;
  cpu_zn(cpu, result);
  cpu->x = result;
}

void cpu_iny(CPU * cpu, uint16_t addr) {
  byte result = cpu->y + 1;
  cpu_zn(cpu, result);
  cpu->y = result;
}

void cpu_jmp(CPU * cpu, uint16_t addr) {
  cpu->pc = addr;
}

void cpu_jsr(CPU * cpu, uint16_t addr) {
  cpu_push16(cpu, cpu->pc);
  cpu->pc = addr;
}

void cpu_lda(CPU * cpu, uint16_t addr) {
  byte value = memory_read(cpu->mem, addr);
  cpu_zn(cpu, value);
  cpu->a = value;
}

void cpu_ldx(CPU * cpu, uint16_t addr) {
  byte value = memory_read(cpu->mem, addr);
  cpu_zn(cpu, value);
  cpu->x = value;
}

void cpu_ldy(CPU * cpu, uint16_t addr) {
  byte value = memory_read(cpu->mem, addr);
  cpu_zn(cpu, value);
  cpu->y = value;
}

void cpu_lsr(CPU * cpu, uint16_t addr) {
  printf(" - STUB");
}

void cpu_nop(CPU * cpu, uint16_t addr) {
}

void cpu_ora(CPU * cpu, uint16_t addr) {
  byte result = cpu->a | memory_read(cpu->mem, addr);
  cpu_zn(cpu, result);
  cpu->a = result;
}

void cpu_pha(CPU * cpu, uint16_t addr) {
  cpu_push(cpu, cpu->a);
}

void cpu_php(CPU * cpu, uint16_t addr) {
  cpu_push(cpu, cpu->status);
}

void cpu_pla(CPU * cpu, uint16_t addr) {
  byte value = cpu_pull(cpu);
  cpu_zn(cpu, value);
  cpu->a = value;
}

void cpu_plp(CPU * cpu, uint16_t addr) {
  cpu->status = cpu_pull(cpu);
}

void cpu_rol(CPU * cpu, uint16_t addr) {
  printf(" - STUB");
}

void cpu_ror(CPU * cpu, uint16_t addr) {
  printf(" - STUB");
}

void cpu_rti(CPU * cpu, uint16_t addr) {
  cpu->status = cpu_pull(cpu);
  cpu->pc = cpu_pull16(cpu);
}

void cpu_rts(CPU * cpu, uint16_t addr) {
  cpu->pc = cpu_pull16(cpu);
}

void cpu_sbc(CPU * cpu, uint16_t addr) {
  uint16_t result = cpu->a - memory_read(cpu->mem, addr) - cpu->c;
  cpu->c = result > 0xFF;
  cpu->v = 0; // Unfinished
  cpu->a = result;
}

void cpu_sec(CPU * cpu, uint16_t addr) {
  cpu->c = 1;
}

void cpu_sed(CPU * cpu, uint16_t addr) {
  cpu->d = 1;
}

void cpu_sei(CPU * cpu, uint16_t addr) {
  cpu->i = 1;
}

void cpu_sta(CPU * cpu, uint16_t addr) {
  memory_write(cpu->mem, addr, cpu->a);
}

void cpu_stx(CPU * cpu, uint16_t addr) {
  memory_write(cpu->mem, addr, cpu->x);
}

void cpu_sty(CPU * cpu, uint16_t addr) {
  memory_write(cpu->mem, addr, cpu->y);
}

void cpu_tax(CPU * cpu, uint16_t addr) {
  byte value = cpu->a;
  cpu_zn(cpu, value);
  cpu->x = value;
}

void cpu_tay(CPU * cpu, uint16_t addr) {
  byte value = cpu->a;
  cpu_zn(cpu, value);
  cpu->y = value;
}

void cpu_tsx(CPU * cpu, uint16_t addr) {
  byte value = cpu->sp;
  cpu_zn(cpu, value);
  cpu->x = value;
}

void cpu_txa(CPU * cpu, uint16_t addr) {
  byte value = cpu->x;
  cpu_zn(cpu, value);
  cpu->a = value;
}

void cpu_txs(CPU * cpu, uint16_t addr) {
  byte value = cpu->x;
  cpu_zn(cpu, value);
  cpu->sp = value;
}

void cpu_tya(CPU * cpu, uint16_t addr) {
  byte value = cpu->y;
  cpu_zn(cpu, value);
  cpu->a = value;
}

void cpu_brk(CPU * cpu, uint16_t addr) {
  cpu_push16(cpu, cpu->pc);
  cpu_php(cpu, 0);
  cpu_sei(cpu, 0);
  cpu->pc = memory_read(cpu->mem, 0xFFFE);
}

/*
 * Unoffical instructions
 */
void cpu_ahx(CPU * cpu, uint16_t addr) {
  printf(" - STUB");
}

void cpu_alr(CPU * cpu, uint16_t addr) {
  printf(" - STUB");
}

void cpu_anc(CPU * cpu, uint16_t addr) {
  printf(" - STUB");
}

void cpu_arr(CPU * cpu, uint16_t addr) {
  printf(" - STUB");
}

void cpu_axs(CPU * cpu, uint16_t addr) {
  printf(" - STUB");
}

void cpu_dcp(CPU * cpu, uint16_t addr) {
  printf(" - STUB");
}

void cpu_isc(CPU * cpu, uint16_t addr) {
  printf(" - STUB");
}

void cpu_las(CPU * cpu, uint16_t addr) {
  printf(" - STUB");
}

void cpu_lax(CPU * cpu, uint16_t addr) {
  printf(" - STUB");
}

void cpu_rla(CPU * cpu, uint16_t addr) {
  printf(" - STUB");
}

void cpu_rra(CPU * cpu, uint16_t addr) {
  printf(" - STUB");
}

void cpu_sax(CPU * cpu, uint16_t addr) {
  printf(" - STUB");
}

void cpu_shx(CPU * cpu, uint16_t addr) {
  printf(" - STUB");
}

void cpu_shy(CPU * cpu, uint16_t addr) {
  printf(" - STUB");
}

void cpu_slo(CPU * cpu, uint16_t addr) {
  printf(" - STUB");
}

void cpu_sre(CPU * cpu, uint16_t addr) {
  printf(" - STUB");
}

void cpu_stp(CPU * cpu, uint16_t addr) {
  printf(" - STUB");
}

void cpu_tas(CPU * cpu, uint16_t addr) {
  printf(" - STUB");
}

void cpu_xaa(CPU * cpu, uint16_t addr) {
  printf(" - STUB");
}
