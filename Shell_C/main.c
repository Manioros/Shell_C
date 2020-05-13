#define BUFFER_SIZE 1000
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctype.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>
#include <stdio.h>
#include <dirent.h>
#include "cmd_table.h"

void print_prompt();
char *read_line(void);
int parse_line(char* line); /* epistrefei poses entoles periexei h grammh */
int getch(void);

void print(char *s);
void printn() ;
void printd(int d) ;
void printchar(char s);
void handle_sigint(int sig);

pid_t pid=1;  /* fork result if >0, is parent pid */
    

int main (){
	char *line;
	int i, j;
	int total;
	cmd* p;     /* trexousa entolh */
	cmd *q;     /* prohgoumenh entolh */
	FILE *out_fp, *in_fp;
	int pipefd[2]; 
	int lastpipefd;
	int prev_pipe_fd;
	int status;
	int exec_result;
    char cur_path[BUFFER_SIZE];
        	
	init_comand_table();
    signal(SIGINT, handle_sigint); 
    signal(SIGTSTP, handle_sigint); 
    signal(SIGCHLD, handle_sigint);	
    signal(SIGSTOP, handle_sigint);	
    
    while (1){
		/*print prompt*/
        getcwd(cur_path, BUFFER_SIZE); 						/* print prompt */
        printf("mysh:%s:%s> ", getlogin(), cur_path); 
		
		/*read line*/
		line = read_line();
		
       /* elegxos gia kenh entolh */
	    if (strcmp(line,"")==0) {
	        continue;
	    }
		
		/*parse line*/
		total = parse_line(line);
        if (total==0) continue;

		/*execute commands*/

		for (i=0; i<total; i++) {
            p = get_cmd(i);
            /* print("executing ");printd(i); print(p->argv[0]);printn(); */

		    if (strcmp(p->argv[0], "exit")==0) {
		        print("mysh exiting...\n");
		        exit(EXIT_SUCCESS);
		    }
            	
            /* oles oi paused diergasies synexizoun */
		    if (strcmp(p->argv[0], "fg")==0) {
		        if (pid>1) {    /* 1 einai h arxiikh timh */
	                kill(pid, SIGCONT);
    		        pause();
		        }
		        continue;
		    }            		

            if (total>1)
                pipe(pipefd);
            		
            pid = fork();            
            if (pid==0) {  /* child process */

                /* anakatefthinsi eksodou */
                if (p->outfile!=NULL) {
                    out_fp = fopen(p->outfile, p->writemode);
                    if (out_fp==NULL) {
                        print("cannot open outpout file\n");
                    }
                    dup2(fileno(out_fp), STDOUT_FILENO);
                }
                
                /* anakatefthinsi eisodou  */
                if (p->infile!=NULL) {
                    in_fp = fopen(p->infile, "r");
                    if (in_fp==NULL) {
                        print("cannot open inpout file\n");
                    }
                    dup2(fileno(in_fp), STDIN_FILENO);
                }                
                
				/* xeirismos pipes */
				if ((i+1)<total) { 
					if(dup2(pipefd[1], STDOUT_FILENO)==-1) 
						fprintf(stderr, "pipe %d %s %d\n", i, strerror(errno), errno);
				}
				if (i>0) {		/* diabaze apo to prohgoumeno input, ektos apo thn prwth entolh */
					if(dup2(lastpipefd, STDIN_FILENO)==-1) 
						fprintf(stderr, "pipe %d %s %d\n", i, strerror(errno), errno);
				}
					
                exec_result = execvp(p->argv[0], p->argv);
                if (exec_result==-1) {
                    fprintf(stderr, "%s\n", strerror(errno));
                }
                exit(errno);
            }
            else if (pid>0) {  /* parent process */
	            
				lastpipefd = pipefd[0]; 
				close(pipefd[1]);	  
                
                if (p->runbg) {
                    fprintf(stderr, "[%d] %s\n", pid, p->argv[0]);
                }
                else {
                    pause();
                    //wait(&status);
                    /*
                    while (waitpid(-1,&status,WNOHANG)==0) {
                        sleep(1);
                    }
                    */
                }
                

                
            }
            
            else {
                print("mysh: cannot start process\n");
            }
		  
	    } /* for i*/

	} /* while 1 */
	return 0;
}







void print(char *s){
	fprintf(stderr, "%s",s);
}

void printn() {
  fprintf(stderr, "\n");
}

void printd(int d) {
	fprintf(stderr, "%d",d);	
}

void printchar(char s){
	fprintf(stderr, "%c\n",s);
}

void printcmd(cmd *p) {
	


}

/* reads from keypress, doesn't echo */
/* phgh: https://stackoverflow.com/questions/3276546/how-to-implement-getch-function-of-c-in-linux */
int getch(void)
{
    struct termios oldattr, newattr;
    int ch;
    tcgetattr( STDIN_FILENO, &oldattr );
    newattr = oldattr;
    newattr.c_lflag &= ~( ICANON | ECHO );
    tcsetattr( STDIN_FILENO, TCSANOW, &newattr );
    ch = getchar();
    tcsetattr( STDIN_FILENO, TCSANOW, &oldattr );
    return ch;
}

int getche(void)
{
    struct termios oldattr, newattr;
    int ch;
    tcgetattr( STDIN_FILENO, &oldattr );
    newattr = oldattr;
    newattr.c_lflag &= ~( ICANON );
    tcsetattr( STDIN_FILENO, TCSANOW, &newattr );
    ch = getchar();
    tcsetattr( STDIN_FILENO, TCSANOW, &oldattr );
    return ch;
}






char *read_line(void)
{
  int i = 0;
  char *buffer = malloc(sizeof(char) * BUFFER_SIZE);
  int c;
  DIR *dir;
  struct dirent *ent;
  
  /* anoikse to directory */
  /*
  dir = opendir ("c:\\src\\");
  closedir (dir);
  */

  if (!buffer) {
    print("read_line: allocation error\n");
    exit(EXIT_FAILURE);
  }

  while (1) {
    c = getchar();
    /* c = getch();  */

    if (c == EOF || c == '\n') {    // mexri telos grammhs 
      buffer[i] = '\0';
      break;
    } 
    else if (c=='\t') {
        printf("TAB ");
    }
    else {
      buffer[i] = c;
    }
    i++;

    /* buffer gemise */
    if (i >= BUFFER_SIZE) {
		print("read_line: buffer full\n");
        break;

    }
   
  }
  return buffer;
}



int parse_line(char* line) {
	int status = 1;
	char c;
	int i=0;
	int j;
	int sum;
	char *commands[SIZE];
	char *argument;
	int start;
	int counter, counter_arg;
	int size;
	int inword = 0;
	cmd *ptr_cmd;
	
	/* spasimo grammhs se string entolwn (sto commands) */
	cmd_size=0;
	counter=0;
	start = 0;
	size = strlen(line);
	for (i=0; i<size; i++) {
		c=line[i];
		if (c=='|'){
			line[i] = '\0';
			commands[counter++] = line+start;
			start = i+1;
		}

	}
	commands[counter++] = line+start;

	/* spasimo entolhs se arguments (kathe entolh einai h commands[i] */
	for (i=0; i<counter; i++) {
		ptr_cmd = new_cmd();
		counter_arg=0;
		size = strlen(commands[i])+1;
		j=0;
		while(isspace(commands[i][j])) {
			j++;	
		}
		start=j;
		inword=1;
		while (j<size) {
			c = commands[i][j];
			if ((isspace(c) || (c=='\0')) && inword){
				inword=0;
				commands[i][j] ='\0';
				ptr_cmd->argv[counter_arg++] = commands[i] + start;
				j++;
				while(isspace(commands[i][j])) { 
					j++;					
				}
				start = j;
			}
			else { 
				inword = 1;
				j++;
			}

		}
		ptr_cmd->argc = counter_arg;
		insert_cmd(ptr_cmd);
	}



	/*  elegxos gia special arguments  > , >> , < , &  */
	for (i=0; i<counter; i++) {
    	ptr_cmd = get_cmd(i);
    	/*
		print("command # "); printd(i); printn();
		print("#args: "); printd(ptr_cmd->argc); printn();
		*/
		/* print("command: "); print(commands[i]);printn();*/
		
		sum=ptr_cmd->argc;
		for (j=0;j<sum; j++ ){
			argument = ptr_cmd->argv[j];
			/*printd(j);print(argument); printd(strlen(argument));printn();*/
			
			if (strcmp(argument,"&")==0) {
				ptr_cmd->argv[j] = NULL;
				ptr_cmd->runbg=1;
				ptr_cmd->argc -= 1;
			}
			else if(strcmp(argument,">")==0) {
				ptr_cmd->argv[j] = NULL;
				if (ptr_cmd->argv[j+1] != NULL) {
					ptr_cmd->outfile = ptr_cmd->argv[j+1];
					ptr_cmd->writemode = strdup("w");
					ptr_cmd->argc -= 2;
				}
				else {
					print("Invalid syntax on output redirection");
					exit(EXIT_FAILURE);
				} 
			}
			else if(strcmp(argument,">>")==0) {
				ptr_cmd->argv[j] = NULL;
				if (ptr_cmd->argv[j+1] != NULL) {
					ptr_cmd->outfile = ptr_cmd->argv[j+1];
					ptr_cmd->writemode = strdup("a");
					ptr_cmd->argc -= 2;
				}
				else {
					print("Invalid syntax on output redirection");
					exit(EXIT_FAILURE);
				} 
			}				
			else if(strcmp(argument,"<")==0) {
				ptr_cmd->argv[j] = NULL;
				if (ptr_cmd->argv[j+1] != NULL) {
					ptr_cmd->infile = ptr_cmd->argv[j+1];
					ptr_cmd->argc -= 2;
				}
				else {
					print("Invalid syntax on input redirection");
					exit(EXIT_FAILURE);
				} 
			}
			/* else {
				ptr_cmd->argc += 1;
			}	*/
			/*-----*/
			/*----*/
		}	
		
		/*
		
        for (j=0;j<ptr_cmd->argc; j++ ){
            print("argv["); printd(j); print("]:");
            if (ptr_cmd->argv[j]) { print(ptr_cmd->argv[j]); } printn();			
        }		
		print("argc:"); printd(ptr_cmd->argc); 
		print("\tinfile: "); 
        if (ptr_cmd->infile) { print(ptr_cmd->infile); } else { print("STDIN"); } 
		print("\toutfile: ");
        if (ptr_cmd->outfile) { print(ptr_cmd->outfile); } else { print("STDOUT"); }  
		print("\tmode: "); 
        if (ptr_cmd->writemode) { print(ptr_cmd->writemode); } else { print("-"); }  
		print("\trunbg"); printd(ptr_cmd->runbg);
		printn();		
		*/
	}
	return counter;
}


// Handler for SIGINT, caused by 
// Ctrl-C at keyboard 
void handle_sigint(int sig) 
{ 
	int status;

    switch (sig) {
        case SIGINT:
            printf("Ctrl-C\n");
            if (pid==0) {
                exit(0);
            }
            break;
            
        case SIGTSTP:
            printf("Ctrl-Z\n");
            
            /* to paidi na peirmenei signal gia na synexisei */
            /*
            if (pid==0) {   
                pause();
            }
            */
            break;
            
        case SIGCHLD:
            if (pid>0) {    /* o pateras elexgei katastash paidiwn */
                waitpid(-1, &status, WNOHANG);
                
                /*
                if (WIFEXITED(status)) {
                    printf("exited, status=%d\n", WEXITSTATUS(status));
                } else if (WIFSIGNALED(status)) {
                    printf("killed by signal %d\n", WTERMSIG(status));
                } else if (WIFSTOPPED(status)) {
                    printf("stopped by signal %d\n", WSTOPSIG(status));
                } else if (WIFCONTINUED(status)) {
                    printf("continued\n");
                }
                */
                
            }
            break;
            
        case SIGSTOP:
            printf("SIGSTOP\n");
            break;
            
        case SIGCONT:
            printf("SIGCONT\n");
            break;

        default:
            printf("Signal %d\n", sig); 
    }
} 





