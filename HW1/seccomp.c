#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/audit.h>
#include <linux/filter.h>
#include <linux/seccomp.h>
#include <sys/prctl.h>
#include <sys/mman.h>

int
main(int argc, char **argv)
{
	int ret;
	void *buf;

	printf("1. Init seccomp\n");

	ret = prctl(PR_SET_SECCOMP, SECCOMP_MODE_STRICT);

	printf("2. Printf test\n");
	printf("3. Trying to alloc more memory\n");
	buf = mmap(NULL, 1000, PROT_READ | PROT_WRITE, 0, 0, 0);
	printf("4. After malloc\n");

	return 0;
}
