#include <unistd.h>
#include <signal.h>
#include <stdio.h>

static pid_t scrn;

int state = 2;

void
delivery(int sig) {
	state = 2;
	printf("Food delivery\n");
	return;
}

void
trouble(int sig) {
	if (state == 1) {
        printf("Give more money!\n");
	}
	state = 1;
	alarm(5);
	kill(scrn, SIGALRM);
	return;
}

void
alrm(int sig) {
    if (state == 1) {
    	state =2;
    	printf("Give more money!\n");
    } 
    kill(scrn, SIGPIPE);
    return;
}

void
p_state(int sig) {
	if (state == 2) {
        printf("===%d=== Food delivery: ready\n", getpid());
    } else {
    	int time = alarm(0);
    	printf("===%d=== Food delivery: Waiting money. Alarm through %d sec.\n", getpid(), time);
    	alarm(time);
    }
    return;
}

int 
main(void) {
	signal(SIGUSR1, delivery);
	signal(SIGALRM, alrm);
	signal(SIGUSR2, trouble);
	signal(SIGTERM, SIG_DFL);
	signal(SIGINT, p_state);
	read(0, &scrn, sizeof(pid_t));
	while (1) {
		pause();
	}
}