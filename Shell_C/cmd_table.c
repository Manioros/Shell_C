#include <stdio.h>
#include <stdlib.h>
#include "cmd_table.h"

void init_comand_table() {
	cmd_size = 0;
}

cmd* new_cmd() { //char **argv,int argc, char *outfile,char *infile,int runbg){
	cmd *p;
	p = (cmd *)malloc(sizeof(cmd));
	if(p==NULL){
		return NULL;
	}
	//p->argv = NULL;
	p->argc=0;
	p->outfile = NULL;
	p->infile = NULL;
	p->writemode = NULL;
	p->runbg = 0;
	
	//pipe(p->pipefd);
	/*
	p->pipefd[0] = -1;
	p->pipefd[1] = -1;
	*/
	return p;
}

void insert_cmd(cmd *c){
	cmd_table [cmd_size++] = c;
}

cmd* get_cmd(int i) {
  return cmd_table[i];
}
