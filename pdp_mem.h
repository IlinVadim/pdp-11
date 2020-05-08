#ifndef _PDP_H_
#define _PDP_H_
#include<stdint.h>
typedef uint16_t word;
typedef uint8_t byte;
word get_word(int addr);
byte get_byte(int addr);
void put_word(int addr, word data);
void put_byte(int addr, byte data);
int load(const char * filename, word * pstart);
extern word regs[8];
extern word psw;
#define CF (1<<0)
#define VF (1<<1)
#define ZF (1<<2) 
#define NF (1<<3)
#define TF (1<<4)
#define R0 (regs[0])
#define R1 (regs[1])
#define R2 (regs[2])
#define R3 (regs[3])
#define R4 (regs[4])
#define R5 (regs[5])
#define SP (regs[6])
#define PC (regs[7])
#endif
