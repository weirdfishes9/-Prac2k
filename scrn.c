#include <unistd.h>
#include <signal.h>
#include <stdio.h>

static pid_t food;

int sum = 0, state = 1;

enum
{
	PRICE = 160,
	NOTE = 50,
	DNOTE = 100,
};

void
give_food(int sig)
{
	if (state == 2) {
		sum -= PRICE;
		if (sum <= PRICE) {
	    	state = 1;
	    }
        printf("Current amount of money: %d\n", sum);
        kill(food, SIGUSR1);
    } else {
    	printf("Not enough money. Your balance: %d\n", sum);
    	kill(food, SIGUSR2);
    }   	
    return;
}

void
cash_50(int sig) {
	sum += NOTE;
	printf("Current amount of money: %d\n", sum);
	if (state == 3){
		if (sum >= PRICE) {
		    state = 2;
	    }
        give_food(SIGFPE);
	} else if (sum >= PRICE) {
		state = 2;
	}
	return;
}

void
cash_100(int sig) {
	sum += DNOTE;
	printf("Current amount of money: %d\n", sum);
	if (state == 3){
		if (sum >= PRICE) {
		    state = 2;
	    }
        give_food(SIGFPE);
	} else if (sum >= PRICE) {
		state = 2;
	}
	return;
}

void
trouble(int sig) {
	state = 3;
	return;
}

void
no_trouble(int sig)
{
    if (sum >= PRICE) {
		state = 2;
	} else {
		state =1;
	}
	return;
}
void
p_state(int sig) {
	if (state == 2) {
        printf("===%d=== Screen: ready. Balance: %d\n", getpid(), sum);
    } else if (state == 1) {
    	printf("===%d=== Screen: Not enough money. Balance: %d\n", getpid(), sum);
    } else {
    	printf("===%d=== Screen: Wating money. Balance: %d\n", getpid(), sum);
    }
    return;
}

int 
main(void){
	signal(SIGUSR1, cash_50);
	signal(SIGUSR2, cash_100);
	signal(SIGFPE, give_food);
	signal(SIGALRM, trouble);
	signal(SIGPIPE, no_trouble);
	signal(SIGTERM, SIG_DFL);
	signal(SIGINT, p_state);
	read(0, &food, sizeof(pid_t));
	while (1) {
		pause();
	}
}