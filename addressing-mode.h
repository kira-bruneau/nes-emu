#ifndef ADRESSING_MODE
#define ADRESSING_MODE

typedef enum AdressingMode {
  ADDRMODE_0,
  ADDRMODE_1,
  ADDRMODE_2,
  ADDRMODE_3,
  ADDRMODE_4,
  ADDRMODE_5,
  ADDRMODE_6,
  ADDRMODE_7,
  ADDRMODE_8,
  ADDRMODE_9,
  ADDRMODE_A,
  ADDRMODE_B,
  ADDRMODE_C,
} AdressingMode;

const char * addressing_mode_string[] = {
  ""
  "#i"
  "d"
  "a"
  "(a)"
  "(d),y"
  "(d,x)"
  "(d,y)"
  "*+d"
  "a,x"
  "a,y"
  "d,x"
  "d,y"
};

#endif
