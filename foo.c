#include <stdlib.h>
#include <stdio.h>

enum foo {
	ZEBRA,
	ELEFANT,
	DOGE
};

int main(int argc, char *argv[])
{
	enum foo some_foo = ZEBRA;
	printf("ZEBRA is %d\n", some_foo);
	return 0;
}
