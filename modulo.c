#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
	for (int i = -14; i <= 14; i++) {
		printf("%d by 13 is %d, remainder %d\n", i, (i / 13), (i % 13));
	}
}
