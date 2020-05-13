#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
 
 
void  main(int argc, char *argv[]) {
 
    char s[1000];
    int i;
    printf("argc=%d\n", argc);
    if (argc==1) {
        printf("Dwse input:");
        scanf("%s", s);
    }
    else {
        strcpy(s, argv[1]);
    }
    for (i=0; i<1000000; i++) {
        //sleep(1);
        printf("[%d] %s\n", i, s);
    }
     
}