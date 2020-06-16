#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
	int a = 5;
	long *b;

	b = (long *) &a;
	*b = 300l;
	return 0;
}
