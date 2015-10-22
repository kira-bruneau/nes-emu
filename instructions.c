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

// v: ADC, BIT, CLV, PLP, RTI, SBC
// n: ADC, AND, ASL, BIT, CMP, CPY, CPX, DEC, DEX, DEY, EOR, INC, INX, INY, LDA, LDX, LDY, LSR, ORA, PLA, PLP, ROL, BIT, SBC, TAX, TAY, TSX, TXA, TYA

/*
 * Official instructions
 */
void cpu_adc(CPU * cpu, uint16_t addr) {
  printf(" - STUB");
}

void cpu_and(CPU * cpu, uint16_t addr) {
  printf(" - STUB");
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
  cpu->n = (value >> 7) & 1;
  cpu->v = (value >> 6) & 1;
  cpu->z = value & cpu->a;
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

void cpu_brk(CPU * cpu, uint16_t addr) {
  printf(" - STUB");
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
  printf(" - STUB");
}

void cpu_cmp(CPU * cpu, uint16_t addr) {
  printf(" - STUB");
}

void cpu_cpx(CPU * cpu, uint16_t addr) {
  printf(" - STUB");
}

void cpu_cpy(CPU * cpu, uint16_t addr) {
  printf(" - STUB");
}

void cpu_dec(CPU * cpu, uint16_t addr) {
  printf(" - STUB");
}

void cpu_dex(CPU * cpu, uint16_t addr) {
  printf(" - STUB");
}

void cpu_dey(CPU * cpu, uint16_t addr) {
  printf(" - STUB");
}

void cpu_eor(CPU * cpu, uint16_t addr) {
  printf(" - STUB");
}

void cpu_inc(CPU * cpu, uint16_t addr) {
  printf(" - STUB");
}

void cpu_inx(CPU * cpu, uint16_t addr) {
  printf(" - STUB");
}

void cpu_iny(CPU * cpu, uint16_t addr) {
  printf(" - STUB");
}

void cpu_jmp(CPU * cpu, uint16_t addr) {
  cpu->pc = addr;
}

void cpu_jsr(CPU * cpu, uint16_t addr) {
  memory_write16(cpu->mem, cpu->sp - 1, cpu->pc);
  cpu->sp -= 2;
  cpu->pc = addr;
}

void cpu_lda(CPU * cpu, uint16_t addr) {
  byte value = memory_read(cpu->mem, addr);
  cpu->a = value;
  cpu->z = value == 0;
  cpu->n = (value >> 7) & 1;
}

void cpu_ldx(CPU * cpu, uint16_t addr) {
  byte value = memory_read(cpu->mem, addr);
  cpu->x = value;
  cpu->z = value == 0;
  cpu->n = (value >> 7) & 1;
}

void cpu_ldy(CPU * cpu, uint16_t addr) {
  byte value = memory_read(cpu->mem, addr);
  cpu->y = value;
  cpu->z = value == 0;
  cpu->n = (value >> 7) & 1;
}

void cpu_lsr(CPU * cpu, uint16_t addr) {
  printf(" - STUB");
}

void cpu_nop(CPU * cpu, uint16_t addr) {
}

void cpu_ora(CPU * cpu, uint16_t addr) {
  printf(" - STUB");
}

void cpu_pha(CPU * cpu, uint16_t addr) {
  printf(" - STUB");
}

void cpu_php(CPU * cpu, uint16_t addr) {
  printf(" - STUB");
}

void cpu_pla(CPU * cpu, uint16_t addr) {
  printf(" - STUB");
}

void cpu_plp(CPU * cpu, uint16_t addr) {
  printf(" - STUB");
}

void cpu_rol(CPU * cpu, uint16_t addr) {
  printf(" - STUB");
}

void cpu_ror(CPU * cpu, uint16_t addr) {
  printf(" - STUB");
}

void cpu_rti(CPU * cpu, uint16_t addr) {
  printf(" - STUB");
}

void cpu_rts(CPU * cpu, uint16_t addr) {
  cpu->pc = memory_read16(cpu->mem, cpu->sp + 1);
  cpu->sp += 2;
}

void cpu_sbc(CPU * cpu, uint16_t addr) {
  printf(" - STUB");
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

void cpu_shx(CPU * cpu, uint16_t addr) {
  printf(" - STUB");
}

void cpu_shy(CPU * cpu, uint16_t addr) {
  printf(" - STUB");
}

void cpu_sta(CPU * cpu, uint16_t addr) {
  memory_write(cpu->mem, addr, cpu->a);
}

void cpu_stp(CPU * cpu, uint16_t addr) {
  printf(" - STUB");
}

void cpu_stx(CPU * cpu, uint16_t addr) {
  memory_write(cpu->mem, addr, cpu->x);
}

void cpu_sty(CPU * cpu, uint16_t addr) {
  memory_write(cpu->mem, addr, cpu->y);
}

void cpu_tax(CPU * cpu, uint16_t addr) {
  printf(" - STUB");
}

void cpu_tay(CPU * cpu, uint16_t addr) {
  printf(" - STUB");
}

void cpu_tsx(CPU * cpu, uint16_t addr) {
  printf(" - STUB");
}

void cpu_txa(CPU * cpu, uint16_t addr) {
  printf(" - STUB");
}

void cpu_txs(CPU * cpu, uint16_t addr) {
  printf(" - STUB");
}

void cpu_tya(CPU * cpu, uint16_t addr) {
  printf(" - STUB");
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

void cpu_slo(CPU * cpu, uint16_t addr) {
  printf(" - STUB");
}

void cpu_sre(CPU * cpu, uint16_t addr) {
  printf(" - STUB");
}

void cpu_tas(CPU * cpu, uint16_t addr) {
  printf(" - STUB");
}

void cpu_xaa(CPU * cpu, uint16_t addr) {
  printf(" - STUB");
}
