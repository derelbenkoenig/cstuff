#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv)
{
	ssize_t result;
	unsigned long my_long = 0x123456789ABCDEF0;
	write(1, &my_long, sizeof(long));

	return (result != sizeof(long));
}
