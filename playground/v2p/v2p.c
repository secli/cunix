/*
 * A utility to find out the PFN of a virtual page,
 * making use of the /proc/[pid]/pagemap.
 * Please refer to [0]
 * [0]: https://www.kernel.org/doc/Documentation/vm/pagemap.txt
 */

#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <err.h>

#define GET_PFN(x)		((x) & 0x07fffffffffffff)
#define GET_SWAP_TY(x)		((x) & 0x1f)
#define GET_SWAP_NR(x)		((x) & 0x07fffffffffffe0)
#define IS_SOFT_DIRTY(x)	(((x) & (1l << 55)) != 0)
#define IS_EXCLUSIVE(x)		(((x) & (1l << 56)) != 0)
#define IS_ZERO(x)		((x) & (0xf << 57))
#define IS_BACKED(x)		(((x) & (1l << 61)) != 0)
#define IS_SWAPPED(x)		(((x) & (1l << 62)) != 0)
#define IS_PRESENT(x)		(((x) & (1l << 63)) != 0)

#define MAX_FILE


int main(int argc, const char **argv)
{
	pid_t pid;
	unsigned long va;
	int pgsize;
	int pagemap_fd;
	unsigned long pfn, offset;
	char filename[32];

	if (getuid() != 0)
		errx(1, "CAP_SYS_ADMIN is required!");
	if (argc == 2) {
		pid = getpid();
		va = strtoul(argv[1], NULL, 16);
	} else if (argc == 3) {
		pid = strtoll(argv[1], NULL, 10);
		va = strtoul(argv[2], NULL, 16);
	} else
		errx(1, "Usage: %s [pid] virtual address", argv[0]);

	pgsize = getpagesize();

	snprintf(filename, sizeof(filename), "/proc/%d/pagemap", pid);

	if ((pagemap_fd = open(filename, O_RDONLY)) == -1)
		err(1, "open pagemap failed.");

	offset = va / pgsize * 8;	
	if (lseek(pagemap_fd, offset, SEEK_SET) == -1)
		err(1, "lseek failed.");
	if (read(pagemap_fd, &pfn, 8) != 8)
		err(1, "read failed.");

	close(pagemap_fd);
	printf("   8 bytes: %016lx\n", pfn);
	printf("       pid: %d\n", pid);
	printf("        va: 0x%016lx\n", va);
	if (IS_SWAPPED(pfn)) {
		printf("   swap nr: 0x%016lx\n", GET_SWAP_NR(pfn));
		printf(" swap type: 0x%016lx\n", GET_SWAP_TY(pfn));
	}
	if (IS_PRESENT(pfn))
		printf("        pa: 0x%016lx\n", GET_PFN(pfn) * pgsize);
	printf("soft dirty: %d\n", IS_SOFT_DIRTY(pfn));
	printf(" exclusive: %d\n", IS_EXCLUSIVE(pfn));
	printf("    backed: %d\n", IS_BACKED(pfn));
	printf("   swapped: %d\n", IS_SWAPPED(pfn));
	printf("   present: %d\n", IS_PRESENT(pfn));
	exit(0);
}
