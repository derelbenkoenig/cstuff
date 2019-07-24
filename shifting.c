#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

int main(int argc, char *argv[])
{
	unsigned short int un_int = 0xFAEE;
	signed short int si_int = 0xFAEE;
	printf("Unsigned [%04x]\nSigned [%04x]\n", un_int, si_int);
}
