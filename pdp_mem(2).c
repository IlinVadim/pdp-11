#include"pdp_mem.h"
#include <stdio.h>
#define MEMSIZE 65536
union
{
	byte b[MEMSIZE];
	word w[MEMSIZE/2];
} mem;

word regs[8] = {0,0,0,0,0,0,0,0};
word psw = 0;

word get_word(int addr)
{
	if (addr < 0)
		return regs[-addr-1];
	else
		return mem.w[addr>>1];  
}
byte get_byte(int addr)
{
	if (addr < 0)	
		return regs[-addr-1];
	else
		return mem.b[addr];
}
void put_word(int addr, word data)
{
	if (addr < 0)
		regs[-addr-1] = data;
	else
		mem.w[addr>>1] = data;
}
void put_byte(int addr, byte data)
{
	if (addr < 0)
	{
		regs[-addr-1]&=0xFF00;
		regs[-addr-1]|=data;
	}
	else
		mem.b[addr] = data;
}

int load(const char *filename, word * pstart)
{
	word off, sz;
	int i;
	FILE * f = fopen(filename,"r");
	if (f == NULL)
		goto err;
	if (fscanf(f,"%04hx %04hx",&off,&sz)!= 2)
		goto err;
	for (i=0;i<sz;i++)
	{
		byte b;
		if (fscanf(f,"%02hhx",&b)!=1)
			goto err;
		put_byte(i+off,b);
	}
	*pstart = off;
	return 1;
err:	
	if (f)
		fclose(f);
	return 0;
}

void pdp_dump(void)
{
	int i;
	printf("Regs:\tR0\tR1\tR2\tR3\tR4\tR5\tSP\tPC\tFlags\n");
	printf("Values:");
	for (i=0;i<8;i++)
		printf("\t%04x",regs[i]);
	printf("\t%c%c%c%c\n",(psw&NF)?'N':'-',(psw&ZF)?'Z':'-',(psw&VF)?'V':'-',(psw&CF)?'C':'-');
	printf("\n");
}
