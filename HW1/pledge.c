#include <err.h>
#include <stdio.h>
#include <unistd.h>

int
main(int argc, char **argv)
{
	int ret;

	printf("1. before first pledge\n");
	if (pledge("stdio proc", NULL))
		err(1, "pledge");

	printf("2. after first pledge\n");

	printf("3. forking\n");
	ret = fork();

	switch (ret) {
	case -1: err(1, "fork");
	case 0: printf("4. in child\n"); break;
	default: printf("4. in parent\n"); break;
	}

	if (ret > 0) {
		printf("5. re-pledging\n");
		if (pledge("stdio", NULL))
			err(1, "pledge");

		printf("6. forking again\n");
		ret = fork();
		printf("7. after fork\n");
	}

	return 0;
}
