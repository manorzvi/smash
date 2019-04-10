#ifndef _COMMANDS_H
#define _COMMANDS_H
#include <unistd.h> 
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "jobs.h"

#define MAX_LINE_SIZE 80
#define MAX_ARG 20
#define FALSE 1
#define TRUE 0
#define HISTORY_SIZE 50

//typedef enum { FALSE , TRUE } bool;

char  lastCWD[MAX_LINE_SIZE];

char* history[HISTORY_SIZE];
int history_idx;

int ExeComp(char* lineSize);
int BgCmd(char* lineSize, void* jobs);
int ExeCmd(void* jobs, char* lineSize, char* cmdString, int* history_idx);
void ExeExternal(char *args[MAX_ARG], char* cmdString);
#endif

