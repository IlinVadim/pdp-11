#include"pdp_mem.h"
#include"pdp_process.h"
#include<stdio.h>
int main(int argc, char ** argv)
{
	const char * fname = (argc == 1)?"Input.o":argv[1]; 
	word start;
	if (!load(fname, &start))
	{
		printf("Error loading program\n");
		return 1;
	}
	PC = start;
	while (do_cmd())
		pdp_dump();
	return 0;
}
