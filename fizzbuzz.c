#include <stdlib.h>
#include <stdio.h>

#define DEFAULT_UPPER_LIMIT 100
#define FIZZ_DIVISOR 3
#define BUZZ_DIVISOR 5

int main(int argc, char *argv[])
{
	long int upper_limit = DEFAULT_UPPER_LIMIT;
	char *strtol_endptr;
	if (argc > 1) {
		int arg_int = strtol(argv[1], &strtol_endptr, 0);
		if (strtol_endptr != argv[1]) {
			upper_limit = arg_int;
		}
	}
	for (int i = 1; i <= upper_limit; i++) {
		int is_fizz = i % FIZZ_DIVISOR == 0;
		int is_buzz = i % BUZZ_DIVISOR == 0;
		int fizzbuzz_flags = is_fizz | (is_buzz << 1);
		switch (fizzbuzz_flags) {
			case 0:
				printf("%d", i);
				break;
			case 1:
				printf("Fizz");
				break;
			case 3:
				printf("Fizz");
			case 2:
				printf("Buzz");
				break;
			default:
				dprintf(2, "Illegal value for flags, should be impossible\n");
				exit(1);
		}
		printf("\n");
	}
}
