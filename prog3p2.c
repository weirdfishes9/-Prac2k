#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <sys/wait.h>

static pid_t cash, scrn, button, food;

void
pwr_off(int sig)
{
	kill(cash, SIGTERM);
	kill(scrn, SIGTERM);
	kill(button, SIGTERM);
	kill(food, SIGTERM);
	while (wait(0) != -1);
	printf("All processes are completed. Power off\n");
	_exit(0);
}

void
p_state(int sig){
	return;	
}

int 
main(int argc, char **argv)
{
	int fd1[2], fd2[2], fd3[2], fd4[2];
	char enter;
	if (pipe(fd1) == -1 || pipe(fd2) == -1 || pipe(fd3) == -1 
	        || pipe(fd4) == -1) {
        perror(0);
        return 0;
	}
	if ((cash = fork()) == 0) {
		dup2(fd1[0], 0);
		close(fd1[1]);
		execl("cash.out", "cash.out", (char *)0);
		perror(0);
		kill(getppid(), SIGTERM);
		_exit(1);
	}
	if (cash == -1) {
		perror(0);
		kill(getpid(), SIGTERM);
	}
	if ((scrn = fork()) == 0) {
		dup2(fd2[0], 0);
		close(fd2[1]);
		execl("scrn.out", "scrn.out", (char *)0);
		perror(0);
		kill(getppid(), SIGTERM);
		_exit(1);
	}
	if (scrn == -1) {
		perror(0);
		kill(getpid(), SIGTERM);
	}
	if ((button = fork()) == 0) {
		dup2(fd3[0], 0);
		close(fd3[1]);
		execl("button.out", "button.out", (char *)0);
		perror(0);
		kill(getppid(), SIGTERM);
		_exit(1);
	}
	if (button == -1) {
		perror(0);
		kill(getpid(), SIGTERM);
	}
	if ((food = fork()) == 0) {
		dup2(fd4[0], 0);
		close(fd4[1]);
		execl("food.out", "food.out", (char *)0);
		perror(0);
		kill(getppid(), SIGTERM);
	    _exit(1);
	}
	if (food == -1) {
		perror(0);
		kill(getpid(), SIGTERM);
	}
	signal(SIGTERM, pwr_off);
	signal(SIGINT, p_state);
    write(fd1[1], &scrn, sizeof(pid_t));
    write(fd2[1], &food, sizeof(pid_t));
    write(fd3[1], &scrn, sizeof(pid_t));
    write(fd4[1], &scrn, sizeof(pid_t));
    close(fd1[1]);
    close(fd2[1]);
    close(fd3[1]);
    close(fd4[1]);
    close(fd1[0]);
    close(fd2[0]);
    close(fd3[0]);
    close(fd4[0]);
    printf("Press 'f' to pay 50 rubles\n");
    printf("Press 'u' to pay 100 rubles\n");
    printf("Press 'c' to get food\n");
    printf("Press 'k' to end\n");
    while (1) {
    	enter = getchar();
    	if (enter == 'f') {   //50 rubles
    		kill(cash, SIGUSR1);
    	}
        if (enter == 'u') {   //100 rubles
        	kill(cash, SIGUSR2);
        }
        if (enter == 'c') {    // give me food
        	kill(button, SIGUSR1);
        }
        if (enter == 'k') {
        	kill(getpid(), SIGTERM); 
        }
    }
    return 0;
}






