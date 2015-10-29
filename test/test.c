#include <stdio.h>
#include <string.h>

#include "util.h"

const char * opcode_str[] = {
  "BRK","ORA","STP","SLO","NOP","ORA","ASL","SLO","PHP","ORA","ASL","ANC","NOP","ORA","ASL","SLO","BPL","ORA","STP","SLO","NOP","ORA","ASL","SLO","CLC","ORA","NOP","SLO","NOP","ORA","ASL","SLO",
  "JSR","AND","STP","RLA","BIT","AND","ROL","RLA","PLP","AND","ROL","ANC","BIT","AND","ROL","RLA","BMI","AND","STP","RLA","NOP","AND","ROL","RLA","SEC","AND","NOP","RLA","NOP","AND","ROL","RLA",
  "RTI","EOR","STP","SRE","NOP","EOR","LSR","SRE","PHA","EOR","LSR","ALR","JMP","EOR","LSR","SRE","BVC","EOR","STP","SRE","NOP","EOR","LSR","SRE","CLI","EOR","NOP","SRE","NOP","EOR","LSR","SRE",
  "RTS","ADC","STP","RRA","NOP","ADC","ROR","RRA","PLA","ADC","ROR","ARR","JMP","ADC","ROR","RRA","BVS","ADC","STP","RRA","NOP","ADC","ROR","RRA","SEI","ADC","NOP","RRA","NOP","ADC","ROR","RRA",
  "NOP","STA","NOP","SAX","STY","STA","STX","SAX","DEY","NOP","TXA","XAA","STY","STA","STX","SAX","BCC","STA","STP","AHX","STY","STA","STX","SAX","TYA","STA","TXS","TAS","SHY","STA","SHX","AHX",
  "LDY","LDA","LDX","LAX","LDY","LDA","LDX","LAX","TAY","LDA","TAX","LAX","LDY","LDA","LDX","LAX","BCS","LDA","STP","LAX","LDY","LDA","LDX","LAX","CLV","LDA","TSX","LAS","LDY","LDA","LDX","LAX",
  "CPY","CMP","NOP","DCP","CPY","CMP","DEC","DCP","INY","CMP","DEX","AXS","CPY","CMP","DEC","DCP","BNE","CMP","STP","DCP","NOP","CMP","DEC","DCP","CLD","CMP","NOP","DCP","NOP","CMP","DEC","DCP",
  "CPX","SBC","NOP","ISC","CPX","SBC","INC","ISC","INX","SBC","NOP","SBC","CPX","SBC","INC","ISC","BEQ","SBC","STP","ISC","NOP","SBC","INC","ISC","SED","SBC","NOP","ISC","NOP","SBC","INC","ISC"
};

const char * opcode_address_mode_str[] = {
   "" ,"(d,x)", "" ,"(d,x)","d","d","d","d","","#i","","#i", "a" ,"a","a","a","*+d","(d),y","","(d),y","d,x","d,x","d,x","d,x","","a,y","","a,y","a,x","a,x","a,x","a,x",
  "a" ,"(d,x)", "" ,"(d,x)","d","d","d","d","","#i","","#i", "a" ,"a","a","a","*+d","(d),y","","(d),y","d,x","d,x","d,x","d,x","","a,y","","a,y","a,x","a,x","a,x","a,x",
   "" ,"(d,x)", "" ,"(d,x)","d","d","d","d","","#i","","#i", "a" ,"a","a","a","*+d","(d),y","","(d),y","d,x","d,x","d,x","d,x","","a,y","","a,y","a,x","a,x","a,x","a,x",
   "" ,"(d,x)", "" ,"(d,x)","d","d","d","d","","#i","","#i","(a)","a","a","a","*+d","(d),y","","(d),y","d,x","d,x","d,x","d,x","","a,y","","a,y","a,x","a,x","a,x","a,x",
  "#i","(d,x)","#i","(d,x)","d","d","d","d","","#i","","#i", "a" ,"a","a","a","*+d","(d),y","","(d),y","d,x","d,x","d,y","d,y","","a,y","","a,y","a,x","a,x","a,y","a,y",
  "#i","(d,x)","#i","(d,x)","d","d","d","d","","#i","","#i", "a" ,"a","a","a","*+d","(d),y","","(d),y","d,x","d,x","d,y","d,y","","a,y","","a,y","a,x","a,x","a,y","a,y",
  "#i","(d,x)","#i","(d,x)","d","d","d","d","","#i","","#i", "a" ,"a","a","a","*+d","(d),y","","(d),y","d,x","d,x","d,x","d,x","","a,y","","a,y","a,x","a,x","a,x","a,x",
  "#i","(d,x)","#i","(d,x)","d","d","d","d","","#i","","#i", "a" ,"a","a","a","*+d","(d),y","","(d),y","d,x","d,x","d,x","d,x","","a,y","","a,y","a,x","a,x","a,x","a,x",
};

void addr_to_opcode() {
  int index;
  while (true) {
    scanf("%X", &index);
    if (index < 0x00 || index > 0xff) {
      printf("Invalid input\n");
    } else {
      printf("%s %s\n", opcode_str[index], opcode_address_mode_str[index]);
    }
  }
}

void opcode_to_addr() {
  char buffer[255];
  while (fgets(buffer, ARRAY_LENGTH(buffer), stdin) != NULL) {
    byte i;
    for (i = 0; i < 0xFF; ++i) {
      if (strncmp(buffer, opcode_str[i], 3) == 0) {
        printf("0x%02X: %s\n", i, opcode_address_mode_str[i]);
      }
    }
  }
}

void section(int section) {
  section = section % 4;

  int i;
  for (i = section; i < 0xFF; i += 4) {
    printf("%s\n", opcode_str[i]);
  }
  printf("\n");
}

int main(void) {
  section(0);
  section(1);
  section(2);
  section(3);
}
