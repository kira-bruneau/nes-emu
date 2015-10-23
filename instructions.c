#include <stdio.h>

#include "cpu.h"
#include "instruction.h"
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
void cpu_adc(CPU * cpu, Address addr) {
  byte a = cpu->a;
  byte b = memory_read(cpu->mem, addr.val);
  uint16_t result = a - b - cpu->c;
  cpu->c = result > 0xFF;
  cpu->v = (a^b)&0x80 && ((a^result)&0x80) != 0;
  cpu_zn(cpu, result);
  cpu->a = result;
}

void cpu_and(CPU * cpu, Address addr) {
  byte result = cpu->a & memory_read(cpu->mem, addr.val);
  cpu_zn(cpu, result);
  cpu->a = result;
}

void cpu_asl(CPU * cpu, Address addr) {
  printf("STUB\n");
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
  byte value = memory_read(cpu->mem, addr.val);
  cpu->z = value & cpu->a;
  cpu->v = (value >> 6) & 1;
  cpu->n = (value >> 7) & 1;
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
  byte value = memory_read(cpu->mem, addr.val);
  cpu_compare(cpu, cpu->a, value);
}

void cpu_cpx(CPU * cpu, Address addr) {
  byte value = memory_read(cpu->mem, addr.val);
  cpu_compare(cpu, cpu->x, value);
}

void cpu_cpy(CPU * cpu, Address addr) {
  byte value = memory_read(cpu->mem, addr.val);
  cpu_compare(cpu, cpu->y, value);
}

void cpu_dec(CPU * cpu, Address addr) {
  byte result = memory_read(cpu->mem, addr.val) - 1;
  cpu_zn(cpu, result);
  memory_write(cpu->mem, addr.val, result);
}

void cpu_dex(CPU * cpu, Address addr) {
  byte result = cpu->x - 1;
  cpu_zn(cpu, result);
  cpu->x = result;
}

void cpu_dey(CPU * cpu, Address addr) {
  byte result = cpu->y - 1;
  cpu_zn(cpu, result);
  cpu->y = result;
}

void cpu_eor(CPU * cpu, Address addr) {
  byte result = cpu->a ^ memory_read(cpu->mem, addr.val);
  cpu_zn(cpu, result);
  cpu->a = result;
}

void cpu_inc(CPU * cpu, Address addr) {
  byte result = memory_read(cpu->mem, addr.val) + 1;
  cpu_zn(cpu, result);
  memory_write(cpu->mem, addr.val, result);
}

void cpu_inx(CPU * cpu, Address addr) {
  byte result = cpu->x + 1;
  cpu_zn(cpu, result);
  cpu->x = result;
}

void cpu_iny(CPU * cpu, Address addr) {
  byte result = cpu->y + 1;
  cpu_zn(cpu, result);
  cpu->y = result;
}

void cpu_jmp(CPU * cpu, Address addr) {
  cpu->pc = addr.val;
}

void cpu_jsr(CPU * cpu, Address addr) {
  cpu_push16(cpu, cpu->pc);
  cpu->pc = addr.val;
}

void cpu_lda(CPU * cpu, Address addr) {
  byte value = memory_read(cpu->mem, addr.val);
  cpu_zn(cpu, value);
  cpu->a = value;
}

void cpu_ldx(CPU * cpu, Address addr) {
  byte value = memory_read(cpu->mem, addr.val);
  cpu_zn(cpu, value);
  cpu->x = value;
}

void cpu_ldy(CPU * cpu, Address addr) {
  byte value = memory_read(cpu->mem, addr.val);
  cpu_zn(cpu, value);
  cpu->y = value;
}

void cpu_lsr(CPU * cpu, Address addr) {
  printf("STUB\n");
}

void cpu_nop(CPU * cpu, Address addr) {
}

void cpu_ora(CPU * cpu, Address addr) {
  byte result = cpu->a | memory_read(cpu->mem, addr.val);
  cpu_zn(cpu, result);
  cpu->a = result;
}

void cpu_pha(CPU * cpu, Address addr) {
  cpu_push(cpu, cpu->a);
}

void cpu_php(CPU * cpu, Address addr) {
  cpu_push(cpu, cpu->status);
}

void cpu_pla(CPU * cpu, Address addr) {
  byte value = cpu_pull(cpu);
  cpu_zn(cpu, value);
  cpu->a = value;
}

void cpu_plp(CPU * cpu, Address addr) {
  cpu->status = cpu_pull(cpu);
}

void cpu_rol(CPU * cpu, Address addr) {
  printf("STUB\n");
}

void cpu_ror(CPU * cpu, Address addr) {
  printf("STUB\n");
}

void cpu_rti(CPU * cpu, Address addr) {
  cpu->status = cpu_pull(cpu);
  cpu->pc = cpu_pull16(cpu);
}

void cpu_rts(CPU * cpu, Address addr) {
  cpu->pc = cpu_pull16(cpu);
}

void cpu_sbc(CPU * cpu, Address addr) {
  byte a = cpu->a;
  byte b = memory_read(cpu->mem, addr.val);
  uint16_t result = a - b - cpu->c;
  cpu->c = result > 0xFF;
  cpu->v = (a^b)&0x80 && ((a^result)&0x80) != 0;
  cpu_zn(cpu, result);
  cpu->a = result;
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
  byte value = cpu->a;
  cpu_zn(cpu, value);
  cpu->x = value;
}

void cpu_tay(CPU * cpu, Address addr) {
  byte value = cpu->a;
  cpu_zn(cpu, value);
  cpu->y = value;
}

void cpu_tsx(CPU * cpu, Address addr) {
  byte value = cpu->sp;
  cpu_zn(cpu, value);
  cpu->x = value;
}

void cpu_txa(CPU * cpu, Address addr) {
  byte value = cpu->x;
  cpu_zn(cpu, value);
  cpu->a = value;
}

void cpu_txs(CPU * cpu, Address addr) {
  byte value = cpu->x;
  cpu_zn(cpu, value);
  cpu->sp = value;
}

void cpu_tya(CPU * cpu, Address addr) {
  byte value = cpu->y;
  cpu_zn(cpu, value);
  cpu->a = value;
}

void cpu_brk(CPU * cpu, Address addr) {
  cpu_push16(cpu, cpu->pc);
  cpu_php(cpu, addr);
  cpu_sei(cpu, addr);
  cpu->pc = memory_read(cpu->mem, 0xFFFE);
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
