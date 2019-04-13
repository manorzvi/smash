//
// Created by Manor on 4/12/2019.
//

#include "Job.h"

Job::Job(int pid, string command, bool suspended, time_t creation_time) :
    pid_(pid), command_(command), suspended_(suspended){
    time(&creation_time_);
}

int Job::get_pid() const {return pid_;}

string Job::get_command_name() const {return command_;}

time_t Job::get_creation_time() const {return creation_time_;}

bool Job::is_suspended() const {return suspended_;}

void Job::print_job(int id) const {

    time_t current_time, on_background_time;
    time(&current_time);

    on_background_time = current_time - creation_time_;

    std::cout<<"["<<id<<"] "<<command_<<": "<<pid_<<" "<<on_background_time<<" secs";

    if(suspended_) std::cout<<" (Stopped)"<<std::endl;
    else std::cout<<std::endl;
}

void Job::unsuspend() {suspended_ = false;}
void Job::suspend() {suspended_ = true;}


