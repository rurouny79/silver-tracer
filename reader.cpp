#include <iostream>
#include "silver.h"
#include <cstring>
#include <stdlib.h>
#include <stdio.h>

static FILE *logfp = NULL;

// typedef struct {
// 	unsigned int tid;
// 	char arrow;
// 	unsigned long long faddr;
// 	unsigned long long caddr;
// }__attribute__((packed)) MYDATA;

char **systemargv;

void printFname(unsigned long long fsymaddr){
    FILE* filePointer;
    int bufferLength = 255;
    char buffer[bufferLength];

    char *deli = " <>:";
    char *tok;

    filePointer = fopen(systemargv[1], "r");

    int find = 0;
    while(fgets(buffer, bufferLength, filePointer)) {
        //printf("%s\n", buffer);
        tok = strtok(buffer, deli);
        unsigned long long symret = strtol(tok, NULL, 16);

        if(symret == fsymaddr){
            tok = strtok(NULL, deli);
            printf("%s", tok);
            find = 1;
            break;
        }
    }

    if(find == 0){
        printf("NotFoundSymbol(%llx)", fsymaddr);
    }

    fclose(filePointer);
}

int main(int argc, char *argv[])
{
    systemargv = argv;
    logfp = fopen(argv[2], "r");

    int fret;

    MYDATA d;
    
    while(fread(&d, sizeof(MYDATA), 1, logfp) > 0){
        //printf("%u %d %llx %llx\n", d.tid, d.arrow, d.faddr, d.caddr);
        printf("%u %u %d ", d.tid, d.level, d.arrow);
        printFname(d.faddr);
        printf("\n");
    }

    fclose(logfp);

    return 0;
}
