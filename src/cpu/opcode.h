#ifndef OPCODE_H
#define OPCODE_H

#include "instruction.h"
#include "addressing-mode.h"
#include "util.h"

const byte opcode_instruction[] = {
  INSTR_BRK , INSTR_ORA , INSTR_STP , INSTR_SLO ,
  INSTR_NOP , INSTR_ORA , INSTR_ASL , INSTR_SLO ,
  INSTR_PHP , INSTR_ORA , INSTR_ASL , INSTR_ANC ,
  INSTR_NOP , INSTR_ORA , INSTR_ASL , INSTR_SLO ,
  INSTR_BPL , INSTR_ORA , INSTR_STP , INSTR_SLO ,
  INSTR_NOP , INSTR_ORA , INSTR_ASL , INSTR_SLO ,
  INSTR_CLC , INSTR_ORA , INSTR_NOP , INSTR_SLO ,
  INSTR_NOP , INSTR_ORA , INSTR_ASL , INSTR_SLO ,
  INSTR_JSR , INSTR_AND , INSTR_STP , INSTR_RLA ,
  INSTR_BIT , INSTR_AND , INSTR_ROL , INSTR_RLA ,
  INSTR_PLP , INSTR_AND , INSTR_ROL , INSTR_ANC ,
  INSTR_BIT , INSTR_AND , INSTR_ROL , INSTR_RLA ,
  INSTR_BMI , INSTR_AND , INSTR_STP , INSTR_RLA ,
  INSTR_NOP , INSTR_AND , INSTR_ROL , INSTR_RLA ,
  INSTR_SEC , INSTR_AND , INSTR_NOP , INSTR_RLA ,
  INSTR_NOP , INSTR_AND , INSTR_ROL , INSTR_RLA ,
  INSTR_RTI , INSTR_EOR , INSTR_STP , INSTR_SRE ,
  INSTR_NOP , INSTR_EOR , INSTR_LSR , INSTR_SRE ,
  INSTR_PHA , INSTR_EOR , INSTR_LSR , INSTR_ALR ,
  INSTR_JMP , INSTR_EOR , INSTR_LSR , INSTR_SRE ,
  INSTR_BVC , INSTR_EOR , INSTR_STP , INSTR_SRE ,
  INSTR_NOP , INSTR_EOR , INSTR_LSR , INSTR_SRE ,
  INSTR_CLI , INSTR_EOR , INSTR_NOP , INSTR_SRE ,
  INSTR_NOP , INSTR_EOR , INSTR_LSR , INSTR_SRE ,
  INSTR_RTS , INSTR_ADC , INSTR_STP , INSTR_RRA ,
  INSTR_NOP , INSTR_ADC , INSTR_ROR , INSTR_RRA ,
  INSTR_PLA , INSTR_ADC , INSTR_ROR , INSTR_ARR ,
  INSTR_JMP , INSTR_ADC , INSTR_ROR , INSTR_RRA ,
  INSTR_BVS , INSTR_ADC , INSTR_STP , INSTR_RRA ,
  INSTR_NOP , INSTR_ADC , INSTR_ROR , INSTR_RRA ,
  INSTR_SEI , INSTR_ADC , INSTR_NOP , INSTR_RRA ,
  INSTR_NOP , INSTR_ADC , INSTR_ROR , INSTR_RRA ,
  INSTR_NOP , INSTR_STA , INSTR_NOP , INSTR_SAX ,
  INSTR_STY , INSTR_STA , INSTR_STX , INSTR_SAX ,
  INSTR_DEY , INSTR_NOP , INSTR_TXA , INSTR_XAA ,
  INSTR_STY , INSTR_STA , INSTR_STX , INSTR_SAX ,
  INSTR_BCC , INSTR_STA , INSTR_STP , INSTR_AHX ,
  INSTR_STY , INSTR_STA , INSTR_STX , INSTR_SAX ,
  INSTR_TYA , INSTR_STA , INSTR_TXS , INSTR_TAS ,
  INSTR_SHY , INSTR_STA , INSTR_SHX , INSTR_AHX ,
  INSTR_LDY , INSTR_LDA , INSTR_LDX , INSTR_LAX ,
  INSTR_LDY , INSTR_LDA , INSTR_LDX , INSTR_LAX ,
  INSTR_TAY , INSTR_LDA , INSTR_TAX , INSTR_LAX ,
  INSTR_LDY , INSTR_LDA , INSTR_LDX , INSTR_LAX ,
  INSTR_BCS , INSTR_LDA , INSTR_STP , INSTR_LAX ,
  INSTR_LDY , INSTR_LDA , INSTR_LDX , INSTR_LAX ,
  INSTR_CLV , INSTR_LDA , INSTR_TSX , INSTR_LAS ,
  INSTR_LDY , INSTR_LDA , INSTR_LDX , INSTR_LAX ,
  INSTR_CPY , INSTR_CMP , INSTR_NOP , INSTR_DCP ,
  INSTR_CPY , INSTR_CMP , INSTR_DEC , INSTR_DCP ,
  INSTR_INY , INSTR_CMP , INSTR_DEX , INSTR_AXS ,
  INSTR_CPY , INSTR_CMP , INSTR_DEC , INSTR_DCP ,
  INSTR_BNE , INSTR_CMP , INSTR_STP , INSTR_DCP ,
  INSTR_NOP , INSTR_CMP , INSTR_DEC , INSTR_DCP ,
  INSTR_CLD , INSTR_CMP , INSTR_NOP , INSTR_DCP ,
  INSTR_NOP , INSTR_CMP , INSTR_DEC , INSTR_DCP ,
  INSTR_CPX , INSTR_SBC , INSTR_NOP , INSTR_ISC ,
  INSTR_CPX , INSTR_SBC , INSTR_INC , INSTR_ISC ,
  INSTR_INX , INSTR_SBC , INSTR_NOP , INSTR_SBC ,
  INSTR_CPX , INSTR_SBC , INSTR_INC , INSTR_ISC ,
  INSTR_BEQ , INSTR_SBC , INSTR_STP , INSTR_ISC ,
  INSTR_NOP , INSTR_SBC , INSTR_INC , INSTR_ISC ,
  INSTR_SED , INSTR_SBC , INSTR_NOP , INSTR_ISC ,
  INSTR_NOP , INSTR_SBC , INSTR_INC , INSTR_ISC
};

const byte opcode_addressing_mode[] = {
  ADDR_IMPLIED     , ADDR_INDEXED_INDIRECT , ADDR_IMPLIED     , ADDR_INDEXED_INDIRECT ,
  ADDR_ZERO_PAGE   , ADDR_ZERO_PAGE        , ADDR_ZERO_PAGE   , ADDR_ZERO_PAGE        ,
  ADDR_IMPLIED     , ADDR_IMMEDIATE        , ADDR_ACCUMULATOR , ADDR_IMMEDIATE        ,
  ADDR_ABSOLUTE    , ADDR_ABSOLUTE         , ADDR_ABSOLUTE    , ADDR_ABSOLUTE         ,
  ADDR_RELATIVE    , ADDR_INDIRECT_INDEXED , ADDR_IMPLIED     , ADDR_INDIRECT_INDEXED ,
  ADDR_ZERO_PAGE_X , ADDR_ZERO_PAGE_X      , ADDR_ZERO_PAGE_X , ADDR_ZERO_PAGE_X      ,
  ADDR_IMPLIED     , ADDR_ABSOLUTE_Y       , ADDR_IMPLIED     , ADDR_ABSOLUTE_Y       ,
  ADDR_ABSOLUTE_X  , ADDR_ABSOLUTE_X       , ADDR_ABSOLUTE_X  , ADDR_ABSOLUTE_X       ,
  ADDR_ABSOLUTE    , ADDR_INDEXED_INDIRECT , ADDR_IMPLIED     , ADDR_INDEXED_INDIRECT ,
  ADDR_ZERO_PAGE   , ADDR_ZERO_PAGE        , ADDR_ZERO_PAGE   , ADDR_ZERO_PAGE        ,
  ADDR_IMPLIED     , ADDR_IMMEDIATE        , ADDR_ACCUMULATOR , ADDR_IMMEDIATE        ,
  ADDR_ABSOLUTE    , ADDR_ABSOLUTE         , ADDR_ABSOLUTE    , ADDR_ABSOLUTE         ,
  ADDR_RELATIVE    , ADDR_INDIRECT_INDEXED , ADDR_IMPLIED     , ADDR_INDIRECT_INDEXED ,
  ADDR_ZERO_PAGE_X , ADDR_ZERO_PAGE_X      , ADDR_ZERO_PAGE_X , ADDR_ZERO_PAGE_X      ,
  ADDR_IMPLIED     , ADDR_ABSOLUTE_Y       , ADDR_IMPLIED     , ADDR_ABSOLUTE_Y       ,
  ADDR_ABSOLUTE_X  , ADDR_ABSOLUTE_X       , ADDR_ABSOLUTE_X  , ADDR_ABSOLUTE_X       ,
  ADDR_IMPLIED     , ADDR_INDEXED_INDIRECT , ADDR_IMPLIED     , ADDR_INDEXED_INDIRECT ,
  ADDR_ZERO_PAGE   , ADDR_ZERO_PAGE        , ADDR_ZERO_PAGE   , ADDR_ZERO_PAGE        ,
  ADDR_IMPLIED     , ADDR_IMMEDIATE        , ADDR_ACCUMULATOR , ADDR_IMMEDIATE        ,
  ADDR_ABSOLUTE    , ADDR_ABSOLUTE         , ADDR_ABSOLUTE    , ADDR_ABSOLUTE         ,
  ADDR_RELATIVE    , ADDR_INDIRECT_INDEXED , ADDR_IMPLIED     , ADDR_INDIRECT_INDEXED ,
  ADDR_ZERO_PAGE_X , ADDR_ZERO_PAGE_X      , ADDR_ZERO_PAGE_X , ADDR_ZERO_PAGE_X      ,
  ADDR_IMPLIED     , ADDR_ABSOLUTE_Y       , ADDR_IMPLIED     , ADDR_ABSOLUTE_Y       ,
  ADDR_ABSOLUTE_X  , ADDR_ABSOLUTE_X       , ADDR_ABSOLUTE_X  , ADDR_ABSOLUTE_X       ,
  ADDR_IMPLIED     , ADDR_INDEXED_INDIRECT , ADDR_IMPLIED     , ADDR_INDEXED_INDIRECT ,
  ADDR_ZERO_PAGE   , ADDR_ZERO_PAGE        , ADDR_ZERO_PAGE   , ADDR_ZERO_PAGE        ,
  ADDR_IMPLIED     , ADDR_IMMEDIATE        , ADDR_ACCUMULATOR , ADDR_IMMEDIATE        ,
  ADDR_INDIRECT    , ADDR_ABSOLUTE         , ADDR_ABSOLUTE    , ADDR_ABSOLUTE         ,
  ADDR_RELATIVE    , ADDR_INDIRECT_INDEXED , ADDR_IMPLIED     , ADDR_INDIRECT_INDEXED ,
  ADDR_ZERO_PAGE_X , ADDR_ZERO_PAGE_X      , ADDR_ZERO_PAGE_X , ADDR_ZERO_PAGE_X      ,
  ADDR_IMPLIED     , ADDR_ABSOLUTE_Y       , ADDR_IMPLIED     , ADDR_ABSOLUTE_Y       ,
  ADDR_ABSOLUTE_X  , ADDR_ABSOLUTE_X       , ADDR_ABSOLUTE_X  , ADDR_ABSOLUTE_X       ,
  ADDR_IMMEDIATE   , ADDR_INDEXED_INDIRECT , ADDR_IMMEDIATE   , ADDR_INDEXED_INDIRECT ,
  ADDR_ZERO_PAGE   , ADDR_ZERO_PAGE        , ADDR_ZERO_PAGE   , ADDR_ZERO_PAGE        ,
  ADDR_IMPLIED     , ADDR_IMMEDIATE        , ADDR_IMPLIED     , ADDR_IMMEDIATE        ,
  ADDR_ABSOLUTE    , ADDR_ABSOLUTE         , ADDR_ABSOLUTE    , ADDR_ABSOLUTE         ,
  ADDR_RELATIVE    , ADDR_INDIRECT_INDEXED , ADDR_IMPLIED     , ADDR_INDIRECT_INDEXED ,
  ADDR_ZERO_PAGE_X , ADDR_ZERO_PAGE_X      , ADDR_ZERO_PAGE_Y , ADDR_ZERO_PAGE_Y      ,
  ADDR_IMPLIED     , ADDR_ABSOLUTE_Y       , ADDR_IMPLIED     , ADDR_ABSOLUTE_Y       ,
  ADDR_ABSOLUTE_X  , ADDR_ABSOLUTE_X       , ADDR_ABSOLUTE_Y  , ADDR_ABSOLUTE_Y       ,
  ADDR_IMMEDIATE   , ADDR_INDEXED_INDIRECT , ADDR_IMMEDIATE   , ADDR_INDEXED_INDIRECT ,
  ADDR_ZERO_PAGE   , ADDR_ZERO_PAGE        , ADDR_ZERO_PAGE   , ADDR_ZERO_PAGE        ,
  ADDR_IMPLIED     , ADDR_IMMEDIATE        , ADDR_IMPLIED     , ADDR_IMMEDIATE        ,
  ADDR_ABSOLUTE    , ADDR_ABSOLUTE         , ADDR_ABSOLUTE    , ADDR_ABSOLUTE         ,
  ADDR_RELATIVE    , ADDR_INDIRECT_INDEXED , ADDR_IMPLIED     , ADDR_INDIRECT_INDEXED ,
  ADDR_ZERO_PAGE_X , ADDR_ZERO_PAGE_X      , ADDR_ZERO_PAGE_Y , ADDR_ZERO_PAGE_Y      ,
  ADDR_IMPLIED     , ADDR_ABSOLUTE_Y       , ADDR_IMPLIED     , ADDR_ABSOLUTE_Y       ,
  ADDR_ABSOLUTE_X  , ADDR_ABSOLUTE_X       , ADDR_ABSOLUTE_Y  , ADDR_ABSOLUTE_Y       ,
  ADDR_IMMEDIATE   , ADDR_INDEXED_INDIRECT , ADDR_IMMEDIATE   , ADDR_INDEXED_INDIRECT ,
  ADDR_ZERO_PAGE   , ADDR_ZERO_PAGE        , ADDR_ZERO_PAGE   , ADDR_ZERO_PAGE        ,
  ADDR_IMPLIED     , ADDR_IMMEDIATE        , ADDR_IMPLIED     , ADDR_IMMEDIATE        ,
  ADDR_ABSOLUTE    , ADDR_ABSOLUTE         , ADDR_ABSOLUTE    , ADDR_ABSOLUTE         ,
  ADDR_RELATIVE    , ADDR_INDIRECT_INDEXED , ADDR_IMPLIED     , ADDR_INDIRECT_INDEXED ,
  ADDR_ZERO_PAGE_X , ADDR_ZERO_PAGE_X      , ADDR_ZERO_PAGE_X , ADDR_ZERO_PAGE_X      ,
  ADDR_IMPLIED     , ADDR_ABSOLUTE_Y       , ADDR_IMPLIED     , ADDR_ABSOLUTE_Y       ,
  ADDR_ABSOLUTE_X  , ADDR_ABSOLUTE_X       , ADDR_ABSOLUTE_X  , ADDR_ABSOLUTE_X       ,
  ADDR_IMMEDIATE   , ADDR_INDEXED_INDIRECT , ADDR_IMMEDIATE   , ADDR_INDEXED_INDIRECT ,
  ADDR_ZERO_PAGE   , ADDR_ZERO_PAGE        , ADDR_ZERO_PAGE   , ADDR_ZERO_PAGE        ,
  ADDR_IMPLIED     , ADDR_IMMEDIATE        , ADDR_IMPLIED     , ADDR_IMMEDIATE        ,
  ADDR_ABSOLUTE    , ADDR_ABSOLUTE         , ADDR_ABSOLUTE    , ADDR_ABSOLUTE         ,
  ADDR_RELATIVE    , ADDR_INDIRECT_INDEXED , ADDR_IMPLIED     , ADDR_INDIRECT_INDEXED ,
  ADDR_ZERO_PAGE_X , ADDR_ZERO_PAGE_X      , ADDR_ZERO_PAGE_X , ADDR_ZERO_PAGE_X      ,
  ADDR_IMPLIED     , ADDR_ABSOLUTE_Y       , ADDR_IMPLIED     , ADDR_ABSOLUTE_Y       ,
  ADDR_ABSOLUTE_X  , ADDR_ABSOLUTE_X       , ADDR_ABSOLUTE_X  , ADDR_ABSOLUTE_X
};

const byte opcode_cycles[] = {
  7, 6, 2, 8, 3, 3, 5, 5, 3, 2, 2, 2, 4, 4, 6, 6,
  2, 5, 2, 8, 4, 4, 6, 6, 2, 4, 2, 7, 4, 4, 7, 7,
  6, 6, 2, 8, 3, 3, 5, 5, 4, 2, 2, 2, 4, 4, 6, 6,
  2, 5, 2, 8, 4, 4, 6, 6, 2, 4, 2, 7, 4, 4, 7, 7,
  6, 6, 2, 8, 3, 3, 5, 5, 3, 2, 2, 2, 3, 4, 6, 6,
  2, 5, 2, 8, 4, 4, 6, 6, 2, 4, 2, 7, 4, 4, 7, 7,
  6, 6, 2, 8, 3, 3, 5, 5, 4, 2, 2, 2, 5, 4, 6, 6,
  2, 5, 2, 8, 4, 4, 6, 6, 2, 4, 2, 7, 4, 4, 7, 7,
  2, 6, 2, 6, 3, 3, 3, 3, 2, 2, 2, 2, 4, 4, 4, 4,
  2, 6, 2, 6, 4, 4, 4, 4, 2, 5, 2, 5, 5, 5, 5, 5,
  2, 6, 2, 6, 3, 3, 3, 3, 2, 2, 2, 2, 4, 4, 4, 4,
  2, 5, 2, 5, 4, 4, 4, 4, 2, 4, 2, 4, 4, 4, 4, 4,
  2, 6, 2, 8, 3, 3, 5, 5, 2, 2, 2, 2, 4, 4, 6, 6,
  2, 5, 2, 8, 4, 4, 6, 6, 2, 4, 2, 7, 4, 4, 7, 7,
  2, 6, 2, 8, 3, 3, 5, 5, 2, 2, 2, 2, 4, 4, 6, 6,
  2, 5, 2, 8, 4, 4, 6, 6, 2, 4, 2, 7, 4, 4, 7, 7
};

const byte opcode_page_cross_cycles[] = {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  1, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0
};

#endif
