pdp_emu: pdp_process.o pdp_mem.o pdp_main.o
	gcc $(CFLAGS) -o $@ $^
clean:
	rm  -f *.o pdp_emu
