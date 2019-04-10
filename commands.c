//		commands.c
//********************************************

#include "commands.h"


void ExeExternal(char *args[MAX_ARG], char* cmdString);  // Handle external commands.
int ExeCmd(void* jobs, char* lineSize, char* cmdString); // Execute non-complicated commands.


//********************************************
// function name: ExeCmd
// Description: interperts and executes built-in commands
// Parameters: pointer to jobs, command string
// Returns: 0 - success,1 - failure
//**************************************************************************************
int ExeCmd(void* jobs, char* lineSize, char* cmdString, int* history_idx)  {

	printf("[DEBUG] - %d\n",(*history_idx));
	(*history_idx)++;
	char* cmd; 
	char* args[MAX_ARG];
	char pwd[MAX_LINE_SIZE];
	int status;

	char* delimiters = " \t\n";  
	int i            = 0;
	int num_arg      = 0;
	int illegal_cmd  = FALSE; // illegal command

	cmd = strtok(lineSize, delimiters);
	if (cmd == NULL)
		return 0; 

	args[0] = cmd;

	for (i=1; i<MAX_ARG; i++) {

		args[i] = strtok(NULL, delimiters);
		if (args[i] != NULL) 
			num_arg++;
	}

	/*************************************************/
	if (!strcmp(cmd, "cd") ) {
		printf("[DEBUG] - %s",history_idx % HISTORY_SIZE);
//		strcpy(history[history_idx % HISTORY_SIZE], cmd);
//		history_idx ++;

		if (num_arg == 1) {

			if (!strcmp(args[1], "-")) {

				if (chdir(lastCWD) != 0) {

					printf("smash error: > \"%s\" - path not found.\n", lastCWD);
					illegal_cmd = TRUE;
				}
			} else {

				if (chdir(args[1]) != 0) {

					printf("smash error: > \"%s\" - path not found.\n", args[1]);
					illegal_cmd = TRUE;
				}
			}
		}
		else if (num_arg == 0) {

			printf("cd expects one argument. If U don't like the road U R walking, start paving another one. D.Parton\n");
			illegal_cmd = TRUE;
		}
		else {

			printf("cd expects exactly one argument. Two roads diverged in a yellow wood, and sorry I could not travel both... R.Frost\n");
			illegal_cmd = TRUE;
		}
	} 

	else if (!strcmp(cmd, "history")) {


	}

	/*************************************************/
	else if (!strcmp(cmd, "pwd")) {

		if (num_arg != 0) {

			printf("smash error: > pwd expects no arguments\n");
			illegal_cmd = TRUE;
		}
		else {

			if (getcwd(pwd, sizeof(pwd)) == NULL) {

				printf("smash error: > Failed on getcwd.\n");
				exit(1);
			}
			else {

				printf("%s\n", pwd);
			}
		}
	}

	/*************************************************/
	
	else if (!strcmp(cmd, "jobs")) {

		if (num_arg != 0) {

			printf("smash error: > jobs expects no arguments\n");
			illegal_cmd = TRUE;
		}
		else {

			printJobs(jobs);
		}
	}

	/*************************************************/
	else if (!strcmp(cmd, "showpid")) 
	{
		
	}
	/*************************************************/
	else if (!strcmp(cmd, "fg")) 
	{
		
	} 
	/*************************************************/
	else if (!strcmp(cmd, "bg")) 
	{
  		
	}
	/*************************************************/
	else if (!strcmp(cmd, "quit"))
	{
   		
	} 
	/*************************************************/
	else // external command
	{
 		ExeExternal(args, cmdString);
	 	return 0;
	}

	if (illegal_cmd == TRUE)
	{
		printf("smash error: > \"%s\"\n", cmdString);
		return 1;
	}
    return 0;
}


//**************************************************************************************
// function name: ExeExternal
// Description: executes external command
// Parameters: external command arguments, external command string
// Returns: void
//**************************************************************************************
void ExeExternal(char *args[MAX_ARG], char* cmdString) {

	int pID;

	switch(pID = fork()) {

	case -1:

		perror("Failed forking a child. Exit.\n");
		exit(1);

    case 0 : // Child Process

    	setpgrp();
    	//execve(args[0], args); //NOTE: Other variations exists online - exec, execvp. Ask teaching assistant.
    	execvp(args[0], args);   //      Maybe second arguments to execve (args) need to be chomped (remove first element).

    	printf("External Command: %s failed.\n", cmdString);
    	perror("Process execution failed.\n");
    	exit(1);

	default:

		wait(NULL); //NOTE: Not sure why the NULL is necessary. Origin: https://www.geeksforgeeks.org/wait-system-call-c/
		            //      Other variations: waitpid(pid, &status, WUNTRACED). Like: https://brennan.io/2015/01/16/write-a-shell-in-c/.
				    //      Not sure whats the difference. Ask teaching assistant.

		printf("Parent pid = %d\n", getpid()); //NOTE: Prints in order to gain more information.
		printf("Child pid = %d\n", pID);
		return;
	}
}


//**************************************************************************************
// function name: ExeComp
// Description: executes complicated command
// Parameters: command string
// Returns: 0- if complicated -1- if not
//**************************************************************************************
int ExeComp(char* lineSize)  {

	char ExtCmd[MAX_LINE_SIZE+2];
	char *args[MAX_ARG];

	if ((strstr(lineSize, "|")) || (strstr(lineSize, "<")) || (strstr(lineSize, ">")) || (strstr(lineSize, "*")) || (strstr(lineSize, "?")) || (strstr(lineSize, ">>")) || (strstr(lineSize, "|&")))  {

		args[0] = "csh";
		args[1] = "-f";
		args[2] = "-c";
		args[3] = "\""; //NOTE: Maybe not needed
		args[4] = lineSize;
		args[5] = "\""; //NOTE: Maybe not needed

		int pID;

		switch(pID = fork()) {

		case -1:

			perror("Failed forking a child. Exit.\n");
			exit(1);

		case 0 : // Child Process

			setpgrp();

			//execve(args[0], args); //NOTE: Other variations exists online - exec, execvp. Ask teaching assistant.
			execvp(args[0], args);   //      Maybe second arguments to execve (args) need to be chomped (remove first element).

			printf("Complicated Command: %s failed.\n", lineSize);
			perror("Process execution failed.\n");
			exit(1);

		default:

			wait(NULL); //NOTE: Not sure why the NULL is necessary. Origin: https://www.geeksforgeeks.org/wait-system-call-c/
			            //      Other variations: waitpid(pid, &status, WUNTRACED). Like: https://brennan.io/2015/01/16/write-a-shell-in-c/.
			            //      Not sure whats the difference. Ask teaching assistant.
			printf("Parent pid = %d\n", getpid()); //NOTE: Prints in order to gain more information.
			printf("Child pid = %d\n", pID);
			return 0;
		}
	}
	return -1;
}


//**************************************************************************************
// function name: BgCmd
// Description: if command is in background, insert the command to jobs
// Parameters: command string, pointer to jobs
// Returns: 0- BG command -1- if not
//**************************************************************************************
int BgCmd(char* lineSize, void* jobs) {

	char* Command;
	char* delimiters = " \t\n";
	char *args[MAX_ARG];
	int pID;
	Job newJob;

	if (lineSize[strlen(lineSize)-2] == '&') { //NOTE: What about Complicated Commands which ends with & (meant to run at background)?

		lineSize[strlen(lineSize)-2] = '\0';
		char cmdString[MAX_LINE_SIZE];
		strcpy(cmdString, lineSize);
		Command = strtok(lineSize, delimiters);
		if (Command == NULL) {

			printf("Empty Command. Give me something to work with.\n");
			return 0;
		}

		args[0] = cmdString;
		for (int i=1; i<MAX_ARG; i++) {
			args[i] = strtok(NULL, delimiters);
		}

		switch(pID = fork()) {

		case -1:

			perror("Failed forking a child. Exit.\n");
			exit(1);

		case 0 : // Child Process

			setpgrp();
			//execve(args[0], args); //NOTE: Other variations exists online - exec, execvp. Ask teaching assistant.
			execvp(args[0], args);   //      Maybe second arguments to execve (args) need to be chomped (remove first element).

			printf("Command: %s failed.\n", cmdString);
			perror("Process execution failed.\n");
			exit(1);

		default:

			insertProcess(jobs, cmdString, pID, NOTSTOPPED); //NOTE: I'm not sure why in case of command to background
															 //      we still perform exec on it (case 0).
															 //      When that code is being fetched to processor?

		}
	}
	return -1;
}


void initHistory(char *history[HISTORY_SIZE]) {

	for(int i=0;i<HISTORY_SIZE;i++) {
		history[i] = "nonvalid";
	}
}

