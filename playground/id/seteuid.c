#define _GNU_SOURCE
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <err.h>
#include <signal.h>
#include <stdio.h>
#include <fcntl.h>
#include <sched.h>
#include <stddef.h>
#include <stdarg.h>
#include <pwd.h>
#include <sys/prctl.h>
#include <sys/wait.h>
#include <sys/ptrace.h>
#include <sys/user.h>
#include <sys/syscall.h>
#include <sys/stat.h>

/*
 *
 *
$ ./a.out 
original UIDs:
ruid:   1000
euid:   1000
suid:   1000

set EUID to 967 only
a.out: setresuid failed: Operation not permitted
$ sudo chown root:root a.out 
$ sudo chmod a+s a.out      
$ ./a.out 
original UIDs:
ruid:   1000
euid:   0
suid:   0

set EUID to 967 only
 current UIDs:
ruid:   1000
euid:   967
suid:   0

reset EUID to 0 only
 current UIDs:
ruid:   1000
euid:   0
suid:   0 
 *
 *
 */

int main()
{
	uid_t ruid, euid, suid;

	if (getresuid(&ruid, &euid, &suid) == -1) err(1, "getresuid failed");
	puts("original UIDs:");
	printf("ruid:\t%d\neuid:\t%d\nsuid:\t%d\n", ruid, euid, suid);

	puts("\nset EUID to 967 only");
	if (seteuid(967) == -1) err(1, "setresuid failed");
	if (getresuid(&ruid, &euid, &suid) == -1) err(1, "getresuid failed");
	puts(" current UIDs:");
	printf("ruid:\t%d\neuid:\t%d\nsuid:\t%d\n", ruid, euid, suid);

	puts("\nreset EUID to 0 only");
	if (seteuid(0) == -1) err(1, "setresuid failed");
	if (getresuid(&ruid, &euid, &suid) == -1) err(1, "getresuid failed");
	puts(" current UIDs:");
	printf("ruid:\t%d\neuid:\t%d\nsuid:\t%d\n", ruid, euid, suid);
	return 0;
}
