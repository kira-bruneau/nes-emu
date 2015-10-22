#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include "cpu.h"

typedef enum Instruction {
  INSTR_ADC, INSTR_AHX, INSTR_ALR, INSTR_ANC,
  INSTR_AND, INSTR_ARR, INSTR_ASL, INSTR_AXS,
  INSTR_BCC, INSTR_BCS, INSTR_BEQ, INSTR_BIT,
  INSTR_BMI, INSTR_BNE, INSTR_BPL, INSTR_BRK,
  INSTR_BVC, INSTR_BVS, INSTR_CLC, INSTR_CLD,
  INSTR_CLI, INSTR_CLV, INSTR_CMP, INSTR_CPX,
  INSTR_CPY, INSTR_DCP, INSTR_DEC, INSTR_DEX,
  INSTR_DEY, INSTR_EOR, INSTR_INC, INSTR_INX,
  INSTR_INY, INSTR_ISC, INSTR_JMP, INSTR_JSR,
  INSTR_LAS, INSTR_LAX, INSTR_LDA, INSTR_LDX,
  INSTR_LDY, INSTR_LSR, INSTR_NOP, INSTR_ORA,
  INSTR_PHA, INSTR_PHP, INSTR_PLA, INSTR_PLP,
  INSTR_RLA, INSTR_ROL, INSTR_ROR, INSTR_RRA,
  INSTR_RTI, INSTR_RTS, INSTR_SAX, INSTR_SBC,
  INSTR_SEC, INSTR_SED, INSTR_SEI, INSTR_SHX,
  INSTR_SHY, INSTR_SLO, INSTR_SRE, INSTR_STA,
  INSTR_STP, INSTR_STX, INSTR_STY, INSTR_TAS,
  INSTR_TAX, INSTR_TAY, INSTR_TSX, INSTR_TXA,
  INSTR_TXS, INSTR_TYA, INSTR_XAA
} Instruction;

const char * instruction_string[] = {
  "ADC", "AHX", "ALR", "ANC",
  "AND", "ARR", "ASL", "AXS",
  "BCC", "BCS", "BEQ", "BIT",
  "BMI", "BNE", "BPL", "BRK",
  "BVC", "BVS", "CLC", "CLD",
  "CLI", "CLV", "CMP", "CPX",
  "CPY", "DCP", "DEC", "DEX",
  "DEY", "EOR", "INC", "INX",
  "INY", "ISC", "JMP", "JSR",
  "LAS", "LAX", "LDA", "LDX",
  "LDY", "LSR", "NOP", "ORA",
  "PHA", "PHP", "PLA", "PLP",
  "RLA", "ROL", "ROR", "RRA",
  "RTI", "RTS", "SAX", "SBC",
  "SEC", "SED", "SEI", "SHX",
  "SHY", "SLO", "SRE", "STA",
  "STP", "STX", "STY", "TAS",
  "TAX", "TAY", "TSX", "TXA",
  "TXS", "TYA", "XAA"
};

/*
 * Official instructions
 */
void cpu_adc(CPU * cpu, uint16_t addr);
void cpu_and(CPU * cpu, uint16_t addr);
void cpu_asl(CPU * cpu, uint16_t addr);
void cpu_bcc(CPU * cpu, uint16_t addr);
void cpu_bcs(CPU * cpu, uint16_t addr);
void cpu_beq(CPU * cpu, uint16_t addr);
void cpu_bit(CPU * cpu, uint16_t addr);
void cpu_bmi(CPU * cpu, uint16_t addr);
void cpu_bne(CPU * cpu, uint16_t addr);
void cpu_bpl(CPU * cpu, uint16_t addr);
void cpu_brk(CPU * cpu, uint16_t addr);
void cpu_bvc(CPU * cpu, uint16_t addr);
void cpu_bvs(CPU * cpu, uint16_t addr);
void cpu_clc(CPU * cpu, uint16_t addr);
void cpu_cld(CPU * cpu, uint16_t addr);
void cpu_cli(CPU * cpu, uint16_t addr);
void cpu_clv(CPU * cpu, uint16_t addr);
void cpu_cmp(CPU * cpu, uint16_t addr);
void cpu_cpx(CPU * cpu, uint16_t addr);
void cpu_cpy(CPU * cpu, uint16_t addr);
void cpu_dec(CPU * cpu, uint16_t addr);
void cpu_dex(CPU * cpu, uint16_t addr);
void cpu_dey(CPU * cpu, uint16_t addr);
void cpu_eor(CPU * cpu, uint16_t addr);
void cpu_inc(CPU * cpu, uint16_t addr);
void cpu_inx(CPU * cpu, uint16_t addr);
void cpu_iny(CPU * cpu, uint16_t addr);
void cpu_jmp(CPU * cpu, uint16_t addr);
void cpu_jsr(CPU * cpu, uint16_t addr);
void cpu_lda(CPU * cpu, uint16_t addr);
void cpu_ldx(CPU * cpu, uint16_t addr);
void cpu_ldy(CPU * cpu, uint16_t addr);
void cpu_lsr(CPU * cpu, uint16_t addr);
void cpu_nop(CPU * cpu, uint16_t addr);
void cpu_ora(CPU * cpu, uint16_t addr);
void cpu_pha(CPU * cpu, uint16_t addr);
void cpu_php(CPU * cpu, uint16_t addr);
void cpu_pla(CPU * cpu, uint16_t addr);
void cpu_plp(CPU * cpu, uint16_t addr);
void cpu_rol(CPU * cpu, uint16_t addr);
void cpu_ror(CPU * cpu, uint16_t addr);
void cpu_rti(CPU * cpu, uint16_t addr);
void cpu_rts(CPU * cpu, uint16_t addr);
void cpu_sbc(CPU * cpu, uint16_t addr);
void cpu_sec(CPU * cpu, uint16_t addr);
void cpu_sed(CPU * cpu, uint16_t addr);
void cpu_sei(CPU * cpu, uint16_t addr);
void cpu_shx(CPU * cpu, uint16_t addr);
void cpu_shy(CPU * cpu, uint16_t addr);
void cpu_sta(CPU * cpu, uint16_t addr);
void cpu_stp(CPU * cpu, uint16_t addr);
void cpu_stx(CPU * cpu, uint16_t addr);
void cpu_sty(CPU * cpu, uint16_t addr);
void cpu_tax(CPU * cpu, uint16_t addr);
void cpu_tay(CPU * cpu, uint16_t addr);
void cpu_tsx(CPU * cpu, uint16_t addr);
void cpu_txa(CPU * cpu, uint16_t addr);
void cpu_txs(CPU * cpu, uint16_t addr);
void cpu_tya(CPU * cpu, uint16_t addr);

/*
 * Unoffical instructions
 */
void cpu_ahx(CPU * cpu, uint16_t addr);
void cpu_alr(CPU * cpu, uint16_t addr);
void cpu_anc(CPU * cpu, uint16_t addr);
void cpu_arr(CPU * cpu, uint16_t addr);
void cpu_axs(CPU * cpu, uint16_t addr);
void cpu_dcp(CPU * cpu, uint16_t addr);
void cpu_isc(CPU * cpu, uint16_t addr);
void cpu_las(CPU * cpu, uint16_t addr);
void cpu_lax(CPU * cpu, uint16_t addr);
void cpu_rla(CPU * cpu, uint16_t addr);
void cpu_rra(CPU * cpu, uint16_t addr);
void cpu_sax(CPU * cpu, uint16_t addr);
void cpu_slo(CPU * cpu, uint16_t addr);
void cpu_sre(CPU * cpu, uint16_t addr);
void cpu_tas(CPU * cpu, uint16_t addr);
void cpu_xaa(CPU * cpu, uint16_t addr);

typedef void (*Action)(CPU * cpu, uint16_t addr);
const Action instruction_action[] = {
  cpu_adc, cpu_ahx, cpu_alr, cpu_anc,
  cpu_and, cpu_arr, cpu_asl, cpu_axs,
  cpu_bcc, cpu_bcs, cpu_beq, cpu_bit,
  cpu_bmi, cpu_bne, cpu_bpl, cpu_brk,
  cpu_bvc, cpu_bvs, cpu_clc, cpu_cld,
  cpu_cli, cpu_clv, cpu_cmp, cpu_cpx,
  cpu_cpy, cpu_dcp, cpu_dec, cpu_dex,
  cpu_dey, cpu_eor, cpu_inc, cpu_inx,
  cpu_iny, cpu_isc, cpu_jmp, cpu_jsr,
  cpu_las, cpu_lax, cpu_lda, cpu_ldx,
  cpu_ldy, cpu_lsr, cpu_nop, cpu_ora,
  cpu_pha, cpu_php, cpu_pla, cpu_plp,
  cpu_rla, cpu_rol, cpu_ror, cpu_rra,
  cpu_rti, cpu_rts, cpu_sax, cpu_sbc,
  cpu_sec, cpu_sed, cpu_sei, cpu_shx,
  cpu_shy, cpu_slo, cpu_sre, cpu_sta,
  cpu_stp, cpu_stx, cpu_sty, cpu_tas,
  cpu_tax, cpu_tay, cpu_tsx, cpu_txa,
  cpu_txs, cpu_tya, cpu_xaa
};

#endif
