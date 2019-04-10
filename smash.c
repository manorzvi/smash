/*	smash.c
main file. This file contains the main function of smash
*******************************************************************/
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include "commands.h"
#include "signals.h"
#include "jobs.h"

#define MAX_LINE_SIZE 80
#define MAXARGS 20

Job jobs[MAXJOBS]; //Handle background jobs

char lineSize[MAX_LINE_SIZE];
char* L_Fg_Cmd;

//**************************************************************************************
// function name: main
// Description: main function of smash. get command from user and calls command functions
//**************************************************************************************
int main(int argc, char *argv[])  {

    printf("Welcome to SMASH\n----------------\n");

    char cmdString[MAX_LINE_SIZE];

    if (getcwd(lastCWD, sizeof(lastCWD)) == NULL) {
        printf("smash error: > Failed on getcwd.");
        exit(1);
    }

    printf("[DEBUG] - Current location: %s\n",lastCWD);

    // Init all Jobs to non-valid.
    initJobsArray(jobs);
    initHistory(history);
    history_idx = 0;

    printf("[DEBUG] - %d\n",history_idx);

    history_idx++;

    printf("[DEBUG] - %d\n",history_idx);

    //signal declaretions
    //NOTE: the signal handlers and the function/s that sets the handler should be found in siganls.c
    /* add your code here */

    /************************************/
    //NOTE: the signal handlers and the function/s that sets the handler should be found in siganls.c
    //set your signal handlers here
    /* add your code here */

    /************************************/

    /************************************/
    // Init globals


//    L_Fg_Cmd = (char *) malloc(sizeof(char) * (MAX_LINE_SIZE + 1));
//

//
//    if (L_Fg_Cmd == NULL)
//        printf("smash error: > Failed on L_fg_Cmd memory allocation.");
//        exit(-1);
//
//TODO: Failed on memory allocation above. don't know why. Yet haven't used L_Fg_Cmd
//
//    L_Fg_Cmd[0] = '\0';

    while (1) {
        printf("smash > ");
        fgets(lineSize, MAX_LINE_SIZE, stdin);
        strcpy(cmdString, lineSize);
        cmdString[strlen(lineSize) - 1] = '\0';

        // perform a complicated Command
        if (!ExeComp(lineSize)) continue;

        // background command
        if (!BgCmd(lineSize, jobs)) continue;

        // built in commands
        ExeCmd(jobs, lineSize, cmdString, &history_idx);

        /* initialize for next line read*/
        lineSize[0] = '\0';
        cmdString[0] = '\0';
    }
    return 0;
}

