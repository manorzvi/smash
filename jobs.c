//
// Created by Manor on 4/6/2019.
//

#include "jobs.h"


void printJob(Job job) {

    if (job.stopped == 1) {
        printf("[%d] %s : %d %d secs (Stopped)", job.id, job.name, job.pid, job.creationTime); //TODO: fix time printing to be a substruction of current time and creation time
                                                                                               //      (right now it just print the creation time, and not the time since creation)
        return;
    }

    if (job.stopped == -1) {
        printf("[%d] %s : %d %d secs", job.id, job.name, job.pid, job.creationTime);
        return;
    }

    if (job.stopped == 0) {
        printf("[%d] %s : %d %d secs (NonValid)", job.id, job.name, job.pid, job.creationTime);
        return;
    }


}


int getJobsSize(Job jobs[MAXJOBS]) {

    int size = 0;

    for (int i=0;i<MAXJOBS;i++) {

        if (jobs[i].valid == VALID) {
            size++;
        }
    }
    return size;
}


void initJobsArray(Job jobs[MAXJOBS]) {
//    printf("[DEBUG] - in initJobsArray:\n");
    for (int i=0;i<MAXJOBS;i++) {
//        printf("[DEBUG] - init jobs[%d]:\n",i);
        jobs[i].valid = NONVALID;
        jobs[i].name = "undef";
        jobs[i].id = NONVALID;
        jobs[i].pid = NONVALID;
        jobs[i].creationTime = NONVALID;
        jobs[i].stopped = NONVALID;
    }
}


void printJobs(Job jobs[MAXJOBS]) {

    for(int i=0;i<MAXJOBS;i++) {
        if (jobs[i].valid == VALID) {
            printJob(jobs[i]);
        }
    }

    printf("[SMASH] - END OF JOBS LIST");
    return;
}


int insertProcess(Job jobs[MAXJOBS], char* name, int pid, int stopped) {

    // Evacuate processes which have finished by asserting their valid field to Non-Valid.
    // In that way, later on a new process could take their place in jobs.
    for (int i=0;i<MAXJOBS;i++) {

        if (jobs[i].done == DONE) {

            jobs[i].valid = NONVALID;
        }
    }

    // Insert the new process to the first empty slot.
    // An empty slot can originated from a slot which haven't populated yet,
    // or a process which have finished and evacuated from jobs array.
    // NOTE: The order of processes in jobs is different then the order of insertion.
    for (int i=0;i<MAXJOBS;i++) {

        if (jobs[i].valid == NONVALID) { // First non-valid process - first empty slot in array

            jobs[i].id = getJobsSize(jobs);
            jobs[i].pid = pid;
            strcpy(jobs[i].name, name);
            jobs[i].stopped = stopped;
            jobs[i].valid = VALID;
            jobs[i].creationTime = time(NULL);
            return SUCCESS;
        }
    }
    printf("[SMASH] - Too many active jobs at the moment. NO SOUP FOR YOU.");
    return FAILED;
}




