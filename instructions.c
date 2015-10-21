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
void cpu_adc(CPU * cpu) {
  printf("stub\n");
}

void cpu_and(CPU * cpu) {
  printf("stub\n");
}

void cpu_asl(CPU * cpu) {
  printf("stub\n");
}

void cpu_bcc(CPU * cpu, uint16_t addr) {
  if (!cpu_read_c(cpu)) {
    cpu_write_pc(cpu, addr);
  }
}

void cpu_bcs(CPU * cpu, uint16_t addr) {
  if (cpu_read_c(cpu)) {
    cpu_write_pc(cpu, addr);
  }
}

void cpu_beq(CPU * cpu, uint16_t addr) {
  if (cpu_read_z(cpu)) {
    cpu_write_pc(cpu, addr);
  }
}

void cpu_bit(CPU * cpu) {
  printf("stub\n");
}

void cpu_bmi(CPU * cpu) {
  printf("stub\n");
}

void cpu_bne(CPU * cpu, uint16_t addr) {
  if (!cpu_read_z(cpu)) {
    cpu_write_pc(cpu, addr);
  }
}

void cpu_bpl(CPU * cpu) {
  printf("stub\n");
}

void cpu_brk(CPU * cpu) {
  printf("stub\n");
}

void cpu_bvc(CPU * cpu, uint16_t addr) {
  if (!cpu_read_v(cpu)) {
    cpu_write_pc(cpu, addr);
  }
}

void cpu_bvs(CPU * cpu, uint16_t addr) {
  if (cpu_read_v(cpu)) {
    cpu_write_pc(cpu, addr);
  }
}

void cpu_clc(CPU * cpu) {
  printf("stub\n");
}

void cpu_cld(CPU * cpu) {
  printf("stub\n");
}

void cpu_cli(CPU * cpu) {
  printf("stub\n");
}

void cpu_clv(CPU * cpu) {
  printf("stub\n");
}

void cpu_cmp(CPU * cpu) {
  printf("stub\n");
}

void cpu_cpx(CPU * cpu) {
  printf("stub\n");
}

void cpu_cpy(CPU * cpu) {
  printf("stub\n");
}

void cpu_dec(CPU * cpu) {
  printf("stub\n");
}

void cpu_dex(CPU * cpu) {
  printf("stub\n");
}

void cpu_dey(CPU * cpu) {
  printf("stub\n");
}

void cpu_eor(CPU * cpu) {
  printf("stub\n");
}

void cpu_inc(CPU * cpu) {
  printf("stub\n");
}

void cpu_inx(CPU * cpu) {
  printf("stub\n");
}

void cpu_iny(CPU * cpu) {
  printf("stub\n");
}

void cpu_jmp(CPU * cpu, uint16_t addr) {
  cpu_write_pc(cpu, addr);
}

void cpu_jsr(CPU * cpu) {
  printf("stub\n");
}

void cpu_lda(CPU * cpu, byte value) {
  cpu_write_a(cpu, value);
  cpu_write_z(cpu, value == 0);
  cpu_write_n(cpu, value < 0);
}

void cpu_ldx(CPU * cpu, byte value) {
  cpu_write_x(cpu, value);
  cpu_write_z(cpu, value == 0);
  cpu_write_n(cpu, value < 0);
}

void cpu_ldy(CPU * cpu, byte value) {
  cpu_write_y(cpu, value);
  cpu_write_z(cpu, value == 0);
  cpu_write_n(cpu, value < 0);
}

void cpu_lsr(CPU * cpu) {
  printf("stub\n");
}

void cpu_nop(CPU * cpu) {}

void cpu_ora(CPU * cpu) {
  printf("stub\n");
}

void cpu_pha(CPU * cpu) {
  printf("stub\n");
}

void cpu_php(CPU * cpu) {
  printf("stub\n");
}

void cpu_pla(CPU * cpu) {
  printf("stub\n");
}

void cpu_plp(CPU * cpu) {
  printf("stub\n");
}

void cpu_rol(CPU * cpu) {
  printf("stub\n");
}

void cpu_ror(CPU * cpu) {
  printf("stub\n");
}

void cpu_rti(CPU * cpu) {
  printf("stub\n");
}

void cpu_rts(CPU * cpu) {
  printf("stub\n");
}

void cpu_sbc(CPU * cpu) {
  printf("stub\n");
}

void cpu_sec(CPU * cpu) {
  cpu_write_c(cpu, 1);
}

void cpu_sed(CPU * cpu) {
  printf("stub\n");
}

void cpu_sei(CPU * cpu) {
  printf("stub\n");
}

void cpu_shx(CPU * cpu) {
  printf("stub\n");
}

void cpu_shy(CPU * cpu) {
  printf("stub\n");
}

void cpu_sta(CPU * cpu, uint16_t addr) {
  memory_write(cpu->mem, addr, cpu_read_a(cpu));
}

void cpu_stp(CPU * cpu) {
  printf("stub\n");
}

void cpu_stx(CPU * cpu, uint16_t addr) {
  memory_write(cpu->mem, addr, cpu_read_x(cpu));
}

void cpu_sty(CPU * cpu, uint16_t addr) {
  memory_write(cpu->mem, addr, cpu_read_y(cpu));
}

void cpu_tax(CPU * cpu) {
  printf("stub\n");
}

void cpu_tay(CPU * cpu) {
  printf("stub\n");
}

void cpu_tsx(CPU * cpu) {
  printf("stub\n");
}

void cpu_txa(CPU * cpu) {
  printf("stub\n");
}

void cpu_txs(CPU * cpu) {
  printf("stub\n");
}

void cpu_tya(CPU * cpu) {
  printf("stub\n");
}

/*
 * Unoffical instructions
 */
void cpu_ahx(CPU * cpu) {
  printf("stub\n");
}

void cpu_alr(CPU * cpu) {
  printf("stub\n");
}

void cpu_anc(CPU * cpu) {
  printf("stub\n");
}

void cpu_arr(CPU * cpu) {
  printf("stub\n");
}

void cpu_axs(CPU * cpu) {
  printf("stub\n");
}

void cpu_dcp(CPU * cpu) {
  printf("stub\n");
}

void cpu_isc(CPU * cpu) {
  printf("stub\n");
}

void cpu_las(CPU * cpu) {
  printf("stub\n");
}

void cpu_lax(CPU * cpu) {
  printf("stub\n");
}

void cpu_rla(CPU * cpu) {
  printf("stub\n");
}

void cpu_rra(CPU * cpu) {
  printf("stub\n");
}

void cpu_sax(CPU * cpu) {
  printf("stub\n");
}

void cpu_slo(CPU * cpu) {
  printf("stub\n");
}

void cpu_sre(CPU * cpu) {
  printf("stub\n");
}

void cpu_tas(CPU * cpu) {
  printf("stub\n");
}

void cpu_xaa(CPU * cpu) {
  printf("stub\n");
}
