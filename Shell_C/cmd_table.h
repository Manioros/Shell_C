#ifndef CMD_TABLE_H
#define CMD_TABLE_H

#define SIZE 100

typedef struct cmd_item{
	char *argv[50];
	int argc;
	char *outfile;
	char *infile;
	char *writemode;
	int runbg;
	//int pipefd[2];
} cmd;

//typedef struct cmd_item cmd;

cmd *cmd_table[SIZE];

int cmd_size; /*poso exw valei mesa size ths domhs*/


void init_comand_table();

cmd* new_cmd(); //char **argv,int argc, char *outfile,char *infile,int runbg);

void insert_cmd(cmd *c);
cmd* get_cmd(int i);
#endif
