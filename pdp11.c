#include <stdio.h>
#include <assert.h>

typedef unsigned char byte;
typedef unsigned short int word;
typedef word Adress;

#define MEMSIZE (64*1024)

byte mem[MEMSIZE];

void b_write(Adress adr, byte b);
byte b_read(Adress adr);
void w_write(Adress adr, word w);
word w_read(Adress adr);

void test_mem() {
	// пишем слово, читаем слово
	Adress a = 2;
	word w0 = 0x0c0b;
	w_write(a, w0);
	word wres1 = w_read(k);
	printf("%04hx = %04hx\n", w0, wres1);
	assert(w0 = wres1);
	
	// пишем 2 байта, читаем 1 слово 
	Adress k1 = 4;
	byte b1 = 0xaa;
	byte b0 = 0x0c;
	word w = 0xcb0a;
	b_write(k1, b0);
	b_write(k1+1, b1);
	word wres2 = w_read(k1);
	printf("ww/br \k %04hx = %02hhx%02hhx\n", wres2, b1, b0);
	assert(wres2 == w);
}
