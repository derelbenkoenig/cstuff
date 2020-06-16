#include <stdlib.h>

static int foo(int i)
{
	return foo(i + 1);
}

int main(int argc, char *argv[])
{
	int k = foo(0);
	return 0;
}
