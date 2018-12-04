#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <fcntl.h>

int 
main(int argc, char **argv)
{
	int status;
	int fd[2];
	int input_file = open(argv[3], O_RDONLY);
	int temp = dup(0);
	pid_t check;


	if (argc < 6) {
        fprintf(stderr, "Wrong number of arguments\n");
        return 0;
	}
	if (input_file == -1) {
        perror("file");
        return 0;
    }
	if (dup2(input_file, 0) == -1) {
		perror(0);
		close(input_file);
		return 0;
	}


	check = fork();
    if (check == -1) {
    	perror(0);
    	close(input_file);
    	return 0;
    }
	if (check == 0) {
		execlp(argv[1], argv[1], (char *)0);
		perror("prog1");
		_exit(1);
	}
	wait(&status);
	

    if (WIFEXITED(status) == 0 || WEXITSTATUS(status) != 0) {
    	check = fork();
        if (check == -1) {
    	    perror(0);
    	    close(input_file);
        	return 0;
        }
    	if (check == 0) {
    		execlp(argv[2], argv[2], (char *)0);
    		perror("prog2");
    		_exit(1);
    	}
    }


    wait(0);
    close(input_file);
    if (dup2(temp, 0) == -1) {
        perror(0);
        close(temp);
		return 0;
	}
    close(temp);
    if (pipe(fd) == -1) {
    	perror(0);
    	return 0;
    }


    check = fork();
    if (check == -1) {
    	perror(0);
    	close(fd[1]);
    	close(fd[0]);
    	return 0;
    }
    if (check == 0) {
    	dup2(fd[1], 1);
    	close(fd[1]);
    	execlp(argv[4], argv[4], (char *)0);
    	perror("prog3");
    	_exit(1);
    }


    close(fd[1]);
    pid_t check2 = fork();
    if (check == -1) {
    	perror(0);
    	close(fd[0]);
    	return 0;
    }
    if (check2 == 0) {
    	dup2(fd[0], 0);
    	execvp(argv[5], argv + 5);
    	perror("prog4");
    	_exit(1);
    }
    while (wait(0) != -1);
}