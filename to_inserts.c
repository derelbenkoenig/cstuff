#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define LINE_MAX 256

int main (int argc, char **argv)
{
	size_t size = LINE_MAX;
	ssize_t result;
	char *line_ptr = malloc(LINE_MAX * sizeof(char));

	for (;;) {
		char *profile_id, *attr_name, *attr_value;
		result = getline(&line_ptr, &size, stdin);
		if (result < 0) {
			break;
		}

		profile_id = strtok(line_ptr, " \n");
		if (strcmp(profile_id, "Profile") == 0) {
			continue;
		}

		attr_name = strtok(NULL, " \n");
		attr_value = strtok(NULL, " \n");
		printf("INSERT INTO acl_profile (profile_id, name, value) values ('%s', '%s', '%s');\n"
				, profile_id
				, attr_name
				, attr_value);
	}
}
