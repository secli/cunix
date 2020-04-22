/*
 * A utility to break down a virtual address into
 * page talbe offsets.
 */

#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <err.h>

#define PGD_OFFSET(x)	(((x) & 0x0000ff8000000000) >> 39)
#define PUD_OFFSET(x)	(((x) & 0x0000007fc0000000) >> 30)
#define PMD_OFFSET(x)	(((x) & 0x000000003fe00000) >> 21)
#define PTE_OFFSET(x)	(((x) & 0x00000000001ff000) >> 12)
#define PG_OFFSET(x)	(((x) & 0x0000000000000fff) >> 0 )

int main(int argc, const char **argv)
{
	unsigned long va;
	unsigned int pgd_offset, pud_offset, pmd_offset, pte_offset,
		      pg_offset;

	if (argc == 2)
		va = strtoul(argv[1], NULL, 16);
	else if (argc == 3) {
		; /* different levels */
	} else
		errx(1, "Usage: %s [pid] virtual address", argv[0]);

	pgd_offset = PGD_OFFSET(va);
	pud_offset = PUD_OFFSET(va);
	pmd_offset = PMD_OFFSET(va);
	pte_offset = PTE_OFFSET(va);
	pg_offset  = PG_OFFSET(va); 

	printf("         va: 0x%016lx\n", va);
	printf(" pgd offset: 0x%x\t%u\n", pgd_offset, pgd_offset);
	printf(" pud offset: 0x%x\t%u\n", pud_offset, pud_offset);
	printf(" pmd offset: 0x%x\t%u\n", pmd_offset, pmd_offset);
	printf(" pte offset: 0x%x\t%u\n", pte_offset, pte_offset);
	printf("page offset: 0x%x\t%u\n", pg_offset, pg_offset);
}
