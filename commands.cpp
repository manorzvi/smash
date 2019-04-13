//
// Created by Manor on 4/12/2019.
//

//		commands.c

#include "commands.h"
#include "signals.h"
#include "Job.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <errno.h>
#include <sys/wait.h>

std::vector<Job>            jobs;
int    current_fg_command_pid = NO_FG_COMMAND;
string current_fg_command     = "";
std::vector<string>         history;

char last_pwd[MAX_LINE_SIZE];



/* function name: ExeCmd
Description:      interperts and executes built-in commands
Parameters:       pointer to jobs, command string
Returns:          0 - success,1 - failure
 */
int ExeCmd(char* lineSize, char* cmdString)
{
    char* cmd;
    char* args[MAX_ARG];
    char* delimiters = " \t\n";
    char  pwd[MAX_LINE_SIZE];

    int i = 0, num_arg = 0;
    bool illegal_cmd = false; // illegal command

    cmd = strtok(lineSize, delimiters);

    if (cmd == NULL)
        return 0;

    args[0] = cmd;

    for (i=1; i<MAX_ARG; i++) {
        args[i] = strtok(NULL, delimiters);
        if (args[i] != NULL)
            num_arg++;
    }

    getcwd(pwd,sizeof(pwd));
    update_history(string(cmdString));
    clean_finished_jobs();

    if (!strcmp(cmd, "cd") ) {

        if (num_arg == 1) {

            int res;

            if (!strcmp(args[1], "-")) {
                if (strcmp(last_pwd, "") != 0) {
                    int result = chdir(last_pwd);

                    if (result != 0) {

                        std::cerr<<"smash error: >"<<"\""<<last_pwd<<"\""<<" - path not found"<<std::endl;
                        illegal_cmd=true;
                    } else {
                        std::cout<<last_pwd<<std::endl;
                        strcpy(last_pwd,pwd);
                    }
                }
                else {

                    std::cerr << "smash error: > Last pwd is not exist. "
                                 "A journey of a thousand miles begins with a single step. "
                                 "Tao Te Ching" << std::endl;
                    illegal_cmd = true;
                }
            } else {
                int result = chdir(args[1]);
                if (result != 0) {

                    std::cerr<<"smash error: >"<<" \""<<args[1]<<" - path not found\""<<std::endl;
                    illegal_cmd = true;
                } else {
                    std::cout<<args[1]<<std::endl;
                    strcpy(last_pwd, pwd);
                }
            }
        } else if (num_arg == 0) {

            std::cerr << "smash error: > cd expects one argument. "
                         "If U don't like the road U R walking, start paving another one. D.Parton" << std::endl;
            illegal_cmd = true;
        } else {

            std::cerr << "smash error: > cd expects exactly one argument. "
                         "Two roads diverged in a yellow wood, and sorry I could not travel both... R.Frost" << std::endl;
            illegal_cmd = true;
        }
    }


    else if (!strcmp(cmd, "pwd"))
    {
        if (num_arg != 0) {

            std::cout<<"smash error: > \"pwd expects no arguments.\""<<std::endl;
            illegal_cmd = true;
        } else {

            std::cout<<pwd<<std::endl;
        }
    }


    else if (!strcmp(cmd, "history")) {

        if (num_arg!=0) {

            std::cout<<"smash error: > \"history expects no arguments.\""<<std::endl;
            illegal_cmd=true;
        } else {

            for (int i=0; i<history.size(); i++) {
                std::cout << history[i] << std::endl;
            }
        }
    }


    else if (!strcmp(cmd, "jobs")) {

        if (num_arg!=0) {

            std::cout<<"smash error: > \"jobs expects no arguments.\""<<std::endl;
            illegal_cmd=true;
        } else {

            for (int i=0;i<jobs.size();i++) {
                jobs[i].print_job(i);
            }
        }
    }


    else if (!strcmp(cmd, "showpid")) {

        if (num_arg!=0) {

            std::cout<<"smash error: > \"showpid expects no arguments.\""<<std::endl;
            illegal_cmd=true;
        } else {
            std::cout<<"smash pid is "<<getpid()<<std::endl;
        }
    }

    else if (!strcmp(cmd, "kill")) {

        if (num_arg!=2) {

            std::cout<<"smash error: > \"kill expects exactly 2 arguments.\""<<std::endl;
            illegal_cmd=true;
        } else {

            int sig_num = atoi(args[1]);
            if (sig_num >= 0) {
                std::cout<<"smash error: > \"specify signal number with a '-' before.\""<<std::endl;
                illegal_cmd=true;
            }

            int job_num = atoi(args[2]);
            sig_num = abs(sig_num);
            int job_pid;
            bool job_exist = true;

            if (job_num < 0 || job_num > jobs.size()-1) {

                job_exist = false;
            } else {

                job_pid = jobs[i].get_pid();
            }
            if (!job_exist) {
                std::cerr<<"smash error: > kill "<<args[2]<<" - job does not exist"<<std::endl;
                illegal_cmd=true;
            }

            if (!send_signal(sig_num, job_pid)) {

                std::cerr<<"smash error: > kill "<<args[2]<<" - cannot send signal"<<std::endl;

            } else {

                if (sig_num == SIGTSTP) {
                    jobs[job_num].suspend();
                }
                if (sig_num == SIGCONT) {
                    jobs[job_num].unsuspend();
                }
            }
        }
    }


    else if (!strcmp(cmd, "fg")) {

        if (num_arg>1) {

            std::cout<<"smash error: > \"fg accepts 1 argument or none.\""<<std::endl;
            illegal_cmd=true;
        } else {
            if (num_arg == 1) {

                int job_num = atoi(args[1]);
                if (job_num < 0) {

                    std::cout<<"smash error: > \"command index do not exist.\""<<std::endl;
                    illegal_cmd=true;
                } else if (job_num > jobs.size()-1) {

                    std::cout<<"smash error: > \"command index do not exist.\""<<std::endl;
                    illegal_cmd=true;
                } else {

                    current_fg_command_pid = jobs[job_num].get_pid();
                    current_fg_command     = jobs[job_num].get_command_name();
                    std::cout << current_fg_command << std::endl;
                    jobs.erase(jobs.begin() + job_num);
                    kill(current_fg_command_pid,SIGCONT);
                    waitpid(current_fg_command_pid,NULL,WUNTRACED);
                    current_fg_command_pid = NO_FG_COMMAND;
                    current_fg_command = "";
                }
            } else {

                current_fg_command_pid = jobs[jobs.size()-1].get_pid();
                current_fg_command     = jobs[jobs.size()-1].get_command_name();
                std::cout << current_fg_command << std::endl;
                jobs.erase(jobs.end());
                kill(current_fg_command_pid,SIGCONT);
                waitpid(current_fg_command_pid,NULL,WUNTRACED);
                current_fg_command_pid = NO_FG_COMMAND;
                current_fg_command = "";
            }
        }
    }


    else if (!strcmp(cmd, "bg")) {

        if(num_arg>1) {

            std::cout<<"smash error: > \"bg accepts 1 argument or none.\""<<std::endl;
            illegal_cmd=true;
        } else {

            if (num_arg == 1) {

                int job_num = atoi(args[1]);
                if (job_num < 0) {

                    std::cout<<"smash error: > \"command index do not exist.\""<<std::endl;
                    illegal_cmd=true;
                } else if (job_num > jobs.size()-1) {

                    std::cout<<"smash error: > \"command index do not exist.\""<<std::endl;
                    illegal_cmd=true;
                } else {

                    if (jobs[job_num].is_suspended()) {

                        std::cout<<jobs[job_num].get_command_name()<<std::endl;
                        jobs[job_num].unsuspend();
                        kill(jobs[job_num].get_pid(), SIGCONT);
                    }
                    else {

                        std::cout<<jobs[job_num].get_command_name()<<"is already running in background"<<std::endl;
                    }
                }
            } else {

                int most_recent_suspended = -1;
                for (int i=0;i<jobs.size();i++) {

                    if (jobs[i].is_suspended() and i > most_recent_suspended) {

                        most_recent_suspended = i;
                    }
                }

                if (most_recent_suspended == -1) {

                    std::cout<<"smash error: > \"there are no suspended jobs in background.\""<<std::endl;
                    illegal_cmd=true;
                } else {

                    std::cout<<jobs[most_recent_suspended].get_command_name()<<std::endl;
                    jobs[most_recent_suspended].unsuspend();
                    kill(jobs[most_recent_suspended].get_pid(), SIGCONT);
                }
            }
        }
    }


    else if (!strcmp(cmd, "quit")) {

        if(num_arg>1) {

            std::cout<<"smash error: > \"bg accepts 1 argument or none.\""<<std::endl;
            illegal_cmd=true;
        } else {

            if (num_arg == 0) {
                std::cout<<"[DEBUG] - exit smash."<<std::endl;
                exit(0); //TODO: should i send SIGTERM/SIGKILL to myself? - kill(0,SIGTERM)
            }
            else {

                if (strcmp(args[1], "kill") != 0) {

                    std::cerr<<"smash error: > \"argument is not supported.\""<<std::endl;
                    illegal_cmd=true;
                } else {

                    for (int i=0;i<jobs.size();i++) {
                        time_t now;
                        time_t five_sec_later;
                        std::cout<<"["<<i<<"] "<<jobs[i].get_command_name()<<" - Sending SIGTERM... ";
                        time(&now);
                        five_sec_later = now+5;
                        kill(jobs[i].get_pid(), SIGTERM);
                        while(now < five_sec_later) {

                            int waitpid_return = waitpid(jobs[i].get_pid(),NULL,WNOHANG);
                            if (waitpid_return == -1) {

                                std::cout<<"[DEBUG] - waitpid returned with -1 (error indication)."<<std::endl;
                            } else if (waitpid_return != 0) {
                                break;
                            }
                            time(&now);
                        }
                        if (now > five_sec_later) {

                            std::cout<<"(5 sec passed) Sending SIGKILL... ";
                            if (waitpid(jobs[i].get_pid(),NULL,WNOHANG) == 0) {
                                kill(jobs[i].get_pid(),SIGKILL);
                            }
                        }
                        std::cout<<"Done."<<std::endl;
                    }
                    exit(0);
                }
            }
        }
    }


    else if (!strcmp(cmd, "cp")) {

        if(num_arg != 2) {

            std::cerr<<"smash error: > \"cp accepts exactly 2 argument.\""<<std::endl;
            illegal_cmd=true;
        } else {

            fstream fin(args[1], ios::in | ios::binary);
            fstream fout(args[2], ios::out | ios::binary);

            if (fin == NULL) {

                std::cerr << "smash error: > “can't open "<<args[1]<<" file.\"" <<std::endl;
                return 1;
            }

            if (fout == NULL) {

                std::cerr << "smash error: > “can't open "<<args[2]<<" file.\"" <<std::endl;
                return 1;
            }

            char c;
            while(!fin.eof()) {

                fin.get(c);
                fout.put(c);
            }

            std::cout<<args[1]<<" has been copied to "<<args[2]<<std::endl;
        }
    }


    else // external command
    {
        ExeExternal(args, cmdString);
        return 0;
    }


    if (illegal_cmd == true)
    {
        printf("smash error: > \"%s\"\n", cmdString);
        return 1;
    }
    return 0;
}


/*
function name: ExeExternal
Description:   executes external command
Parameters:    external command arguments, external command string
Returns:       None
*/
void ExeExternal(char *args[MAX_ARG], char* cmdString) {

    int pid;

    pid = fork();

    switch(pid) {

        case -1:
            cerr << "smash error: > “Failed forking a child command: \"" << string(cmdString) << endl;
            return;
        case 0 :
            setpgrp();
            execvp(args[0], args);
            /* If we got up here - something went wrong */
            cerr << "smash error: > “Child execution failed: \"" << string(cmdString) << endl;
            kill(getpid(), SIGKILL);
            return;
        default:
            std::cout << "[DEBUG] - Foreground & external command. Wait to finish." << endl;
            current_fg_command_pid = pid;
            current_fg_command     = string(cmdString);

            waitpid(pid, NULL, WUNTRACED);

            current_fg_command_pid = NO_FG_COMMAND;
            current_fg_command = "";
            return;
    }
}


/*
function name: ExeComp
Description:   executes complicated command
Parameters:    command string
Returns:       0- if complicated -1- if not
*/
int ExeComp(char* cmdString) {

    if ((strstr(cmdString, "|")) ||
    (strstr(cmdString, "<")) ||
    (strstr(cmdString, ">")) ||
    (strstr(cmdString, "*")) ||
    (strstr(cmdString, "?")) ||
    (strstr(cmdString, ">>")) ||
    (strstr(cmdString, "|&")))  {

        update_history(string(cmdString));
        clean_finished_jobs();

        int pid;

        pid = fork();

        switch (pid) {

            case -1:
                cerr << "smash error: > “Failed forking a child command: \"" << string(cmdString) << endl;
                return -1;

            case 0:
                setpgrp();
                execl("/bin/csh", "-cf", cmdString, NULL);
                /* If we got up here - something went wrong */
                cerr << "smash error: > “Child execution failed: \"" << string(cmdString) << endl;
                kill(getpid(),SIGKILL);
                return -1;

            default:
                /* 2 cases here:
                1) if the command should run on background:
                   create a job of that kind, then push it to jobs vector
                2) otherwise: wait for it to finish
                */
                if (cmdString[strlen(cmdString)-2] == '&') {
                    std::cout<<"[DEBUG] - Background & complicated command. Add to Jobs"<<endl;
                    time_t t;
                    time(&t);
                    jobs.push_back(Job(pid,string(cmdString), false, t));
                    return 0;
                } else {
                    std::cout<<"[DEBUG] - Foreground & complicated command. Wait to finish."<<endl;
                    current_fg_command_pid = pid;
                    current_fg_command = string(cmdString);

                    if (waitpid(pid, NULL, WUNTRACED) == -1);

                    current_fg_command_pid = NO_FG_COMMAND;
                    current_fg_command = "";
                    return 0;
                }
        }
    }
    return -1;
}


/*
function name: BgCmd
Description:   if command is in background, insert the command to jobs
Parameters:    command string, pointer to jobs
Returns:       0- BG command -1- if not
*/
int BgCmd(char* lineSize, char* cmdString) {

    char* cmd;
    char* args[MAX_ARG];
    char* delimiters = " \t\n";
    int i = 0, num_arg = 0;

    if (lineSize[strlen(lineSize)-2] == '&') {

        update_history(string(cmdString));
        clean_finished_jobs();

        lineSize[strlen(lineSize)-2] = '\0';
        cmd = strtok(lineSize, delimiters);

        if (cmd == NULL) {
            cerr << "smash error: > “Empty command. gimme something to work with.\"" <<endl;
            return 0;
        }

        args[0] = cmd;
        for (int i=1; i<MAX_ARG; i++) {
            args[i] = strtok(NULL, delimiters);
            if (args[i] != NULL)
                num_arg++;
        }

        int pid;

        pid = fork();

        switch (pid) {
            case -1:
                cerr << "smash error: > “Failed forking a child command: \"" << string(cmdString) << endl;
                return -1;
            case 0:
                setpgrp();
                execvp(args[0], args);
                /* If we got up here - something went wrong */
                cerr << "smash error: > “Child execution failed: \"" << string(cmdString) << endl;
                kill(getpid(),SIGKILL);
            default:
                std::cout<<"[DEBUG] - Background command. Add to Jobs"<<endl;
                time_t t;
                time(&t);
                jobs.push_back(Job(pid,string(cmdString), false, t));
                return 0;
        }
    }
    return -1;
}


/*
function name: update_history
Description:   add new command to history. replace oldest command in history size exceed MAX_HISTORY
Parameters:    command string
Returns:       None
 */
void update_history(string last_command) {

    if (history.size() < MAX_HISTORY) {
        history.push_back(last_command);
    }
    else if (history.size() == MAX_HISTORY) {
        history.erase(history.begin());
        history.push_back(last_command);
    }
}


/*
function name: clean_finished_jobs
Description:   iterate through background jobs and remove finished jobs from vector
Parameters:    None
Returns:       None
 */
void clean_finished_jobs() {

    for(int i=0;i<jobs.size();i++) {

        int waitpid_return = waitpid(jobs[i].get_pid(),NULL,WNOHANG);
        if (waitpid_return == -1) {

            std::cout<<"[DEBUG] - (in clean_finished_jobs) - waitpid returned with -1 (error indication)."<<std::endl;
        } else if (waitpid_return != 0) {

            jobs.erase(jobs.begin()+i);
            i--;
        }
    }
}


