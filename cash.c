#include <unistd.h>
#include <signal.h>
#include <stdio.h>

static pid_t scrn;

void
cash_50(int sig) {
    printf("50 rubles paid\n");
	kill(scrn, sig);
	return;
}

void
cash_100(int sig) {
    printf("100 rubles paid\n");	
    kill(scrn, sig);
    return;
}

void
p_state(int sig) {
    printf("===%d=== Bill acceptor: ready\n", getpid());
    return;
}

int 
main(void){
	signal(SIGUSR1, cash_50);
	signal(SIGUSR2, cash_100);
	signal(SIGTERM, SIG_DFL);
	signal(SIGINT, p_state);
	read(0, &scrn, sizeof(pid_t));
	while (1) {
		pause();
	}
}