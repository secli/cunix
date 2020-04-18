#define _GNU_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <sched.h>
#include <err.h>


int child(void *dummy)
{
	sleep(2);
	printf(" [child]: ppid: %d, pid: %d\n", getppid(), getpid());
	return 0;
}

int main()
{
	static char child_stack[0x1000 * 0x1000];
	int ret = 0;
	ret = clone(child, child_stack + sizeof(child_stack), CLONE_FILES|CLONE_FS|CLONE_IO|CLONE_PARENT|CLONE_VM|CLONE_SIGHAND|CLONE_SYSVSEM|CLONE_VFORK, NULL);
	if (ret == -1)
		err(1, "clone failed");
	printf("[parent]: ppid: %d, pid: %d\n", getppid(), getpid());
	return 0;

}
