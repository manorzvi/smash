//
// Created by Manor on 4/12/2019.
//

#ifndef _JOB_H
#define _JOB_H

#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <signal.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <sys/types.h>
#include <sys/wait.h>
#include <iostream>

using std::string;
using namespace std;

class Job {
public:

    Job(int pid, string command, bool suspended, time_t creation_time);

    int get_pid() const;
    string get_command_name() const;
    bool is_suspended() const;
    time_t get_creation_time() const;
    void print_job(int id) const;
    void unsuspend();
    void suspend();

private:

    int pid_;
    string command_;
    time_t creation_time_;
    bool suspended_;
};


#endif //_JOB_H
