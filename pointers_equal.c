#include <stdlib.h>
#include <stdio.h>

int some_func(int);

int main(int argc, char *argv[])
{
	char a[5];
	printf("char a[5];\n");
	printf("a as long: %lld\n", a);
	printf("&a as long: %lld\n", &a);
	printf("(a == &a): %s\n", (a == &a) ? "true" : "false");
	printf("\n");

	printf("int some_func(int b);\n");
	printf("some_func as long: %lld\n", some_func);
	printf("&some_func as long: %lld\n", &some_func);
	printf("(some_func == &some_func): %s\n", (some_func == &some_func) ? "true" : "false");
}

int some_func(int b)
{
	return b + 1;
}
