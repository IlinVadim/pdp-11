#include"pdp_mem.h"
#include<stdio.h>
#include<stdlib.h>
/* 15 14 13 12 11 10 09 08 07 06 05 04 03 02 01 00 
   [ CMD     ] [MOD 1 ] [ Reg1 ] [MOD 2 ] [ Reg2 ]
*/
#define GET_BIG_CMD(word) ((word)>>12)
#define ARG1_MODE(word) (((word)>>9)&7)
#define ARG1_REG(word) (((word)>>6)&7)
#define ARG2_MODE(word) (((word)>>3)&7)
#define ARG2_REG(word) ((word)&7)
#define SETF(f,expr) {\
	if ((expr))\
		psw|=(f);\
	else\
		psw&=~(f);\
}
void print_oct(word x)
{
	int t;
	for (t = 15;t>=0;t-=3)
		putchar('0'+((x>>t)&7));
}

static int get_addr(word mode, word reg, int sz)
{
	switch (mode)
	{
		case 0: printf("R%d",reg); 
			return -reg-1;
		case 1: printf("(R%d)", reg);
			return regs[reg];
		case 2: { 
			 	int v = regs[reg];
				if (reg == 7)
				{
					printf("#");
					print_oct(get_word(PC));
				}
				else
					printf("(R%d)+",reg);
				regs[reg]+=sz;
				return v;
			}
		case 3: {
				int v = regs[reg];
				if (reg == 7)
				{
					printf("@#");
					print_oct(get_word(PC));
				}
				else
					printf("@(R%d)+",reg);

				regs[reg]+=sz;
				return get_word(v);
			}
		case 4:	printf("-(R%d)",reg);
			return regs[reg]-=sz;
		case 5:	printf("@-(R%d)",reg);
			return get_word(regs[reg]-=sz);
		case 6: {
				word w = get_word(PC);	
				if (reg == 7)
					print_oct(w);
				else
				{
					print_oct(w);
					printf("(R%d)",reg);
				}
				PC+=2;
				return (w+regs[reg])&0xFFFF;
			}
		case 7: {
				word w = get_word(PC);
				if (reg == 7)
				{
					printf("@");
					print_oct(w);
				}
				else
				{
					printf("@");
					print_oct(w);
					printf("(R%d)",reg);
				}

				return get_word((w+regs[reg])&0xFFFF);
			}
		default: return -65536;
	}
	return -65536;
}


static int is_neg(word x)
{
	return (x&0x8000)?1:0;
}

static int sgn(word x)
{
	return is_neg(x)?-1:(x!=0);
}

static void not_impl()
{
	printf("NOT IMPLEMENTED\n");
	exit(1);
}

void print_cmd_oct(word cmd, word save_PC)
{
	printf("\t|");
	print_oct(cmd);
	for (;save_PC != PC;save_PC+=2)
	{
		printf("\t");
		print_oct(get_word(save_PC));
	}
	printf("\n");
}

static void do_mov(word cmd)
{
	word src, save_PC = PC;
	int dst_addr;
	printf("mov\t");
	src = get_word(get_addr(ARG1_MODE(cmd), ARG1_REG(cmd),2));
	printf(",\t");
	dst_addr = get_addr(ARG2_MODE(cmd),ARG2_REG(cmd),2);
	put_word(dst_addr,src);
	print_cmd_oct(cmd, save_PC);
	SETF(VF,0);
	SETF(ZF,src == 0);
	SETF(NF, is_neg(src));
}

static void do_add(word cmd)
{
	word src, save_PC = PC;
	int dst_addr;
	printf("add\t");
	src = get_word(get_addr(ARG1_MODE(cmd), ARG1_REG(cmd),2));
	printf(",\t");
	dst_addr = get_addr(ARG2_MODE(cmd),ARG2_REG(cmd),2);
	word dst = get_word(dst_addr);
	uint32_t r1 = src+dst;
	word res = (word)r1;
	put_word(dst_addr, res);
	print_cmd_oct(cmd, save_PC);
	SETF(ZF,  res == 0 );
	SETF(CF,  r1 != res );
	SETF(VF,  sgn(src) == sgn(dst) && sgn(src) != sgn(res) );
	SETF(NF,  is_neg(res) );
}

static void do_halt(word cmd)
{
	word save_PC = PC;
	printf("halt");
	print_cmd_oct(cmd, save_PC);
}

int do_cmd(void)
{
	word cmd = get_word(PC);
	PC+=2;
	switch(GET_BIG_CMD(cmd))
	{
		case 01: do_mov(cmd); 
			 return 1;
		case 06: do_add(cmd);
			 return 1;
		case 02: 
		case 03:
		case 04:
		case 05:
		case 011:
		case 012:
		case 013:
		case 014:
		case 015:
		case 016:
			not_impl();
	}
	if (cmd == 0 )
	{
		do_halt(cmd);
		return 0;
	}
	not_impl();
}
