#include"pdp_mem.h"
#include"pdp_process.h"
#include<stdio.h>
int main(int argc, char ** argv)
{
	const char * fname = (argc == 1)?"Input.o":argv[1]; 
	word start;
	int i;
	if (!load(fname, &start))
	{
		printf("Error loading program\n");
		return 1;
	}
	PC = start;
	while (do_cmd())
		;
	printf("Finished.\nRegs:\tR0\tR1\tR2\tR3\tR4\tR5\tSP\tPC\tFlags\n");
	printf("Values:");
	for (i=0;i<8;i++)
		printf("\t%04x",regs[i]);
	printf("\t%c%c%c%c\n",(psw&NF)?'N':'-',(psw&ZF)?'Z':'-',(psw&VF)?'V':'-',(psw&CF)?'C':'-');
	return 0;
}
