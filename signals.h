//
// Created by Manor on 4/12/2019.
//

#ifndef _SIGS_H
#define _SIGS_H

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <cstring>
#include <signal.h>
#include <vector>
#include <iostream>

#define NO_FG_COMMAND -1
using namespace std;

void signal_handler(int sig_num);

bool send_signal(int sig_num, int job_pid);

string signal_dictionary(int sig_num);

#endif