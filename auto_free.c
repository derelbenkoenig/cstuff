#include <stdlib.h>
#include <stdio.h>

static void print_it_out(int *number);

int main(int argc, char *argv[])
{
	for (int i = 0 ; i < 10; i++) {
		__attribute__((cleanup(print_it_out))) int k;
		k = i;
		printf("I declared k to be %d\n", k);
	}
}

static void print_it_out(int *number)
{
	printf("calling cleanup function for %d whose address is %p\n", *number, number);
}
