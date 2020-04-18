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

set RESUID all to ruid
 current UIDs:
ruid:   1000
euid:   1000
suid:   1000

try to reset IDs
 current UIDs:
ruid:   1000
euid:   1000
suid:   1000
$ sudo chown root:root a.out
$ sudo chmod a+s a.out
$ ./a.out
original UIDs:
ruid:   1000
euid:   0
suid:   0

set RESUID all to ruid
 current UIDs:
ruid:   1000
euid:   1000
suid:   1000

try to reset IDs
a.out: setresuid failed: Operation not permitted
 *
 *
 */

int main()
{
	uid_t ruid, euid, suid;
	uid_t orig_ruid, orig_euid, orig_suid;
	if (getresuid(&ruid, &euid, &suid) == -1) err(1, "getresuid failed");
	orig_ruid = ruid;
	orig_euid = euid;
	orig_suid = suid;
	puts("original UIDs:");
	printf("ruid:\t%d\neuid:\t%d\nsuid:\t%d\n", ruid, euid, suid);

	puts("\nset RESUID all to ruid");
	if (setresuid(ruid, ruid, ruid) == -1) err(1, "setresuid failed");
	if (getresuid(&ruid, &euid, &suid) == -1) err(1, "getresuid failed");
	puts(" current UIDs:");
	printf("ruid:\t%d\neuid:\t%d\nsuid:\t%d\n", ruid, euid, suid);

	puts("\ntry to reset IDs");
	if (setresuid(orig_ruid, orig_euid, orig_suid) == -1) err(1, "setresuid failed");
	if (getresuid(&ruid, &euid, &suid) == -1) err(1, "getresuid failed");
	puts(" current UIDs:");
	printf("ruid:\t%d\neuid:\t%d\nsuid:\t%d\n", ruid, euid, suid);

	return 0;
}
