#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <err.h>
#include <sys/wait.h>

int main()
{
	int child;
	int pipefd[2];
	int msg = 0;
	int nap = 2;
	int round = 0x100a;

	if (pipe(pipefd) == -1)
		err(1, "cannot create a pipe.");
	printf("[+] pipe created: %d and %d\n", pipefd[0], pipefd[1]);
	fcntl(pipefd[0], F_SETPIPE_SZ, 0x1000);
	fcntl(pipefd[1], F_SETPIPE_SZ, 0x1000);

	child = fork();
	if (child == -1)
		err(1, "cannot spawn a child.");

	if (child == 0) {
		/* consumer */
		close(pipefd[1]);
		for (unsigned int i = 0; i < round; i++) {
			if ((i % 1024) == 0) {
				puts("Consumer is put to sleep.");
				sleep(nap);
			}
			read(pipefd[0], &msg, sizeof(msg));
			printf("C got %d.\n", msg);
		}

	} else {
		/* producer */
		close(pipefd[0]);
		for (unsigned int i = 0; i < round; i++) {
			write(pipefd[1], &msg, sizeof(msg));
			printf("P put %d.\n", msg);
			msg++;
		}
		int status;
		waitpid(child, &status, 0);
	}
}
