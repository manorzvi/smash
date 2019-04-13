//
// Created by Manor on 4/12/2019.
//

// signals.c
// contains signal handler functions
// contains the function/s that set the signal handlers

/* Name: handler_cntlc
   Synopsis: handle the Control-C */

#include "signals.h"
#include "commands.h"
#include "Job.h"

extern int current_fg_command_pid;
extern string current_fg_command;
extern std::vector<Job> jobs;

void signal_handler(int sig_num) {

    if(current_fg_command_pid!=NO_FG_COMMAND && (waitpid(current_fg_command_pid,NULL,WNOHANG)==0)){

        if(sig_num==SIGTSTP){

            if(kill(current_fg_command_pid,sig_num) == 0){

                time_t now;
                time(&now);

                Job suspended(current_fg_command_pid, current_fg_command.c_str(), true, now);
                jobs.push_back(suspended);
                current_fg_command_pid=NO_FG_COMMAND;
            }
        }
        if(sig_num==SIGINT){
            if(kill(current_fg_command_pid,sig_num) == 0){
                current_fg_command_pid=NO_FG_COMMAND;
            }
        }
    }

}

bool send_signal(int sig_num, int job_pid) {

    if(kill(job_pid,sig_num) == 0) {

        cout<<"smash > signal "<<signal_dictionary(sig_num)<<" was sent to pid "<<job_pid<<endl;
        return true;
    }
    else {

        return false;
    }
}

string signal_dictionary(int sig_num) {

    switch (sig_num) {
        case 1: return "SIGHUP";
        case 2: return "SIGINT";
        case 3: return "SIGQUIT";
        case 4: return "SIGILL";
        case 5: return "SIGTRAP";
        case 6: return "SIGABRT";
        case 7: return "SIGBUS";
        case 8: return "SIGFPE";
        case 9: return "SIGKILL";
        case 10: return "SIGUSR1";
        case 11: return "SIGSEGV";
        case 12: return "SIGUSR2";
        case 13: return "SIGPIPE";
        case 14: return "SIGALRM";
        case 15: return "SIGTERM";
        case 16: return "SIGSTKFLT";
        case 17: return "SIGCHLD";
        case 18: return "SIGCONT";
        case 19: return "SIGSTOP";
        case 20: return "SIGTSTP";
        case 21: return "SIGTTIN";
        case 22: return "SIGTTOU";
        case 23: return "SIGURG";
        case 24: return "SIGXCPU";
        case 25: return "SIGXFSZ";
        case 26: return "SIGVTALRM";
        case 27: return "SIGPROF";
        case 28: return "SIGWINCH";
        case 29: return "SIGIO";
        case 30: return "SIGPWR";
        case 31: return "SIGSYS";
        case 34: return "SIGRTMIN";
        default:
            return "No Signal ID";
    }
}