//
// Created by Manor on 4/6/2019.
//
#include <time.h>

#define MAXJOBS 100

#define VALID 1
#define NONVALID -1

#define STOPPED 1
#define NOTSTOPPED -1

#define SUCCESS 1
#define FAILED -1

#define DONE 1
#define UNDONE -1

typedef struct job {

    char* name;            // Process name
    int id;                // Process id - an integer between 0 to MAXJOBS-1
    int pid;               // Process pid - as given by fork
    int stopped;           // Indicator if the process is suspended
    time_t creationTime;   // Time in seconds when the process first entered jobs array.
    int valid;             // Indicator if the process is valid or just garbage.
    int done;              // Indicator if a job finished.

} Job;


int insertProcess(Job jobs[MAXJOBS], char* name, int pid, int stopped); // Insert a job to jobs array

int getJobsSize(Job jobs[MAXJOBS]);                                     // Get number of valid

void initJobsArray(Job jobs[MAXJOBS]);                                  // Set all processes in jobs array to UNVALID

void printJobs(Job jobs[]);                                             // Print jobs array line by line

void printJob(Job job);                                                 // Print one job (as one line).