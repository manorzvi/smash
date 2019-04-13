//
// Created by Manor on 4/12/2019.
//

#ifndef _COMMANDS_H
#define _COMMANDS_H

#include "Job.h"
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

#define MAX_LINE_SIZE 80
#define MAX_ARG 20
#define MAX_HISTORY 50
#define MAX_JOBS 100
#define NO_FG_COMMAND -1

using std::string;
using namespace std;

//std::vector<Job>    jobs;
//int                 current_fg_command_pid = NO_FG_COMMAND;
//string              current_fg_command     = "";
//std::vector<string> history;

int ExeComp(char* cmdString);
int BgCmd(char* lineSize, char* cmdString);
int ExeCmd(char* lineSize, char* cmdString);
void ExeExternal(char *args[MAX_ARG], char* cmdString);

void update_history(string last_command);
void clean_finished_jobs();

#endif



