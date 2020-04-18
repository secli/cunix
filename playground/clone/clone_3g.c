#define _GNU_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <sched.h>
#include <err.h>
#include <sys/types.h>
#include <sys/wait.h>

int grandchild(void *dummy)
{
	sleep(2);
	printf("[grandchild]: ppid: %d, pid: %d\n", getppid(), getpid());
}

int main()
{
	printf("    [parent]: ppid: %d, pid: %d\n", getppid(), getpid());
	pid_t child = fork();
	if (child == -1) err(1, "fork failed");

	static char child_stack[0x1000 * 0x1000];
	int ret = 0;
	if (child == 0) {
		printf("     [child]: ppid: %d, pid: %d\n", getppid(), getpid());
		ret = clone(grandchild, child_stack + sizeof(child_stack), CLONE_FILES|CLONE_FS|CLONE_IO|CLONE_VM|CLONE_SIGHAND|CLONE_SYSVSEM|CLONE_VFORK, NULL);
		if (ret == -1) err(1, "clone failed");
		return 0;
	}
	int status;
	wait(&status);
	return 0;

}
