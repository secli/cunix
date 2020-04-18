#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

inline __attribute__((always_inline)) unsigned long _rdtsc_begin()
{
        unsigned long a, d;
        asm volatile (
                "mfence\n\t"
                "xor %%rax, %%rax\n\t"
                "CPUID\n\t"
                "RDTSCP\n\t"
                "mov %%rdx, %0\n\t"
                "mov %%rax, %1\n\t"
                "mfence\n\t"
                : "=r" (d), "=r" (a) 
                :   
                : "%rax", "%rbx", "%rcx", "%rdx"
        );  
        a = (d<<32) | a;
        return a;
}

inline __attribute__((always_inline)) unsigned long _rdtsc_end()
{
        unsigned long a, d;
        asm volatile(
                "mfence\n\t"
                "RDTSCP\n\t"
                "mov %%rdx, %0\n\t"
                "mov %%rax, %1\n\t"
                "xor %%rax, %%rax\n\t"
                "CPUID\n\t"
                "mfence\n\t"
                : "=r" (d), "=r" (a)
                :
                : "%rax", "%rbx", "%rcx", "%rdx"
        );
        a = (d<<32) | a;
        return a;
}


int main()
{
	int mapfd;
	void *addr;
	unsigned long tmp, delta1, delta2;
	char a;

	mapfd = open("/etc/adjtime", O_RDONLY);
	addr = mmap(NULL, 4096, PROT_READ, MAP_PRIVATE, mapfd, 0);
	/* 1st measurement*/
	tmp = _rdtsc_begin();
	a = *(char *)addr;
	delta1 = _rdtsc_end() -tmp;
	printf("time of 1st access: %d\n", delta1);

	/* 2nd measurement*/
	tmp = _rdtsc_begin();
	a += *(char *)addr;
	delta2 = _rdtsc_end() -tmp;
	printf("time of 2nd access: %d\n", delta2);

	munmap(addr, 4096);
	close(mapfd);

	return (int) a;
}
