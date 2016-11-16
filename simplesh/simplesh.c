#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
 
#include <sys/types.h>
#include <sys/wait.h>

#include <termios.h>
 
char* args[512];
pid_t pid;

#define BUF_SIZE	1024 
 
#define READ  0
#define WRITE 1

char line[BUF_SIZE];
int n = 0; 
 
int command(int input, int first, int last){
	int pipe_descriptors[2];

	pipe( pipe_descriptors );	
	pid = fork();
	if (pid == 0){
		if (first == 1 && last == 0 && input == 0){
			dup2( pipe_descriptors[WRITE], STDOUT_FILENO );
		}else if (first == 0 && last == 0 && input != 0){
			dup2(input, STDIN_FILENO);
			dup2(pipe_descriptors[WRITE], STDOUT_FILENO);
		}else{
			dup2( input, STDIN_FILENO );
		}
		if (execvp( args[0], args) == -1){
			_exit(EXIT_FAILURE); 
		}
	}
	if (input != 0){
		close(input);
	}
	close(pipe_descriptors[WRITE]);
	if (last == 1){
		close(pipe_descriptors[READ]);
	}
	return pipe_descriptors[READ];
}

void wait_all(int n){
	int i;
	for (i = 0; i < n; i++){
		wait(NULL); 
	}
}

char* skip_spaces(char* s){
	while (isspace(*s)) ++s;
	return s;
}

void split(char* cmd){
	cmd = skip_spaces(cmd);
	char* next = strchr(cmd, ' ');
	int i = 0;
	while(next != NULL){
		next[0] = '\0';
		args[i] = cmd;
		++i;
		cmd = skip_spaces(next + 1);
		next = strchr(cmd, ' ');
	}
	if (cmd[0] != '\0'){
		args[i] = cmd;
		next = strchr(cmd, '\n');
		next[0] = '\0';
		++i; 
	}
 
	args[i] = NULL;
}

int run(char* cmd, int input, int first, int last){
	split(cmd);
	if (args[0] != NULL){
		if (strcmp(args[0], "exit") == 0){
			exit(0);
		}
		n += 1;
		return command(input, first, last);
	}
	return 0;
}

void sig_handler(int i){ 
	kill(pid, SIGINT);
	exit(EXIT_SUCCESS); 
}
 
int main(){
	int rd;
	const char *str_help = "simplesh: enter 'exit' or send EOF for finish\n";
	const char *str_echo = "$> ";
	char *tmp;
	struct sigaction sa;
	sa.sa_handler = sig_handler;
	sigaction(SIGINT, &sa, 0);
	
	write(STDOUT_FILENO, str_help, strlen(str_help) );

	while (1) {
		write(STDOUT_FILENO, str_echo, strlen(str_echo));
		fflush(NULL);
		memset(line, 0, BUF_SIZE);
		rd = 0;		
		tmp = line;
		read(STDIN_FILENO, tmp, 1);
		if(*tmp == '\x0') break;
		while (*tmp != '\n'){ 	
			rd ++;		// считано символов
			tmp++;                        
			read(STDIN_FILENO, tmp, 1);   
		}
		
		int input = 0; 
		int first = 1; 
 
		char* cmd = line;
		char* next = strchr(cmd, '|'); 
		while (next != NULL) {
			*next = '\0';
			input = run(cmd, input, first, 0);
			cmd = next + 1;
			next = strchr(cmd, '|');
			first = 0; 
		}
		input = run(cmd, input, first, 1);
		wait_all(n);
		n = 0;	
	}
	return 0;
}
