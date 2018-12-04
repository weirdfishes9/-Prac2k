#include <unistd.h>
#include <signal.h>
#include <stdio.h>

static pid_t scrn;

void
get_food(int sig)
{
	printf("Food request\n");
	kill(scrn, SIGFPE);
	return;
}

void
p_state(int sig) {
    printf("===%d=== Button: ready\n", getpid());
    return;
}

int 
main(void) {
	signal(SIGUSR1, get_food);
	signal(SIGTERM, SIG_DFL);
	signal(SIGINT, p_state);
	read(0, &scrn, sizeof(pid_t));
	while (1) {
		pause();
	}
}