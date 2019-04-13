//
// Created by Manor on 4/12/2019.
//

/*	smash.c
main file. This file contains the main function of smash
*/

#include "commands.h"
#include "signals.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <cstring>
#include <signal.h>
#include <iostream>

#define MAX_LINE_SIZE 80
#define MAX_ARGS 20

char lineSize[MAX_LINE_SIZE];

/*
function name: main
Description: main function of smash.
 get command from user and calls command functions
*/
int main(int argc, char *argv[])
{
    char cmdString[MAX_LINE_SIZE];

    struct sigaction act;
    act.sa_handler=&signal_handler;
    sigset_t mask_set;
    sigfillset(&mask_set);
    act.sa_mask=mask_set;
    sigaction(SIGINT, &act, NULL);
    sigaction(SIGTSTP, &act, NULL);

    while (1)
    {
        printf("smash > ");

        fgets(lineSize, MAX_LINE_SIZE, stdin);
        strcpy(cmdString, lineSize);
        cmdString[strlen(lineSize)-1]='\0';

        // perform a complicated Command
        if(!ExeComp(cmdString)) continue;

        // background command
        if(!BgCmd(lineSize, cmdString)) continue;

        // built in commands
        ExeCmd(lineSize, cmdString);

        /* initialize for next line read*/
        lineSize[0]='\0';
        cmdString[0]='\0';
    }
    return 0;
}



