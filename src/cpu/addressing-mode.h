#ifndef ADRESSING_MODE
#define ADRESSING_MODE

typedef enum AdressingMode {
  ADDR_IMPLIED,
  ADDR_ACCUMULATOR,
  ADDR_IMMEDIATE,
  ADDR_ZERO_PAGE,
  ADDR_ABSOLUTE,
  ADDR_RELATIVE,
  ADDR_ZERO_PAGE_X,
  ADDR_ZERO_PAGE_Y,
  ADDR_ABSOLUTE_X,
  ADDR_ABSOLUTE_Y,
  ADDR_INDIRECT,
  ADDR_INDIRECT_INDEXED,
  ADDR_INDEXED_INDIRECT,
} AdressingMode;

const char * addressing_mode_string[] = {
  "",
  "A"
  "#i",
  "d",
  "a",
  "*+d",
  "d,x",
  "d,y"
  "a,x",
  "a,y",
  "(a)",
  "(d),y",
  "(d,x)",
};

#endif
