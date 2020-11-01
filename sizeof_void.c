#include <stdlib.h>
#include <stdio.h>

int main (int argc, char **argv)
{
	void *foo;
	foo = malloc(30);
	printf("sizeof void is %d\n", sizeof(*foo));
	free(foo);
	return 0;
}
