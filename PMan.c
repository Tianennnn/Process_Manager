#include "emalloc.h"
#include "linkedlist.h"
#include <readline/history.h>
#include <readline/readline.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>

Node *head = NULL;

/**
 * Function:  get_input
 * -------------------------------
 * @brief Prompt and get input from user, and store the input inan array.
 *
 * @param cmds[] The array that stores the input command and arguments seperated by " ".
 */
void get_input(char *cmds[]) {
    char *input;
    input = readline("PMan: >");
    int i = 0;
    cmds[i] = strtok(input, " ");
    while (cmds[i] != NULL) {
        i++;
        cmds[i] = strtok(NULL, " ");
    }
}

/**
 * Function:  bg
 * -------------------------------
 * @brief Start the given program in the background.
 *
 * @param commands[] The array that stores the input command and arguments seperated by " ".
 */
void bg(char **commands) {
    pid_t pid = fork();
    if (pid == 0) {
        // if is a child
        if (commands[1] == NULL) {
            printf("Plese enter an program to be started in the background.\n");
            exit(0);
        }
        if (execvp(commands[1], &commands[1]) == -1) {
            printf("Failed to execute the program.\n");
            exit(-1);
        }
    } else if (pid > 0) {
        // if is a parent
        Node *new = create(pid);
        int status;
        usleep(10000); // to wait for the child process to execute the program
        pid_t wpid = waitpid(pid, &status, WNOHANG);
        if (wpid > 0) {
            if (WIFEXITED(status)) {
                int exitCode = WEXITSTATUS(status);
                if (exitCode != 255) {
                    head = append_list(head, new);
                    printf("The program %d is started in the background.\n", pid);
                }
            }
        } else {
            head = append_list(head, new);
            printf("The program %d is started in the background.\n", pid);
        }
    } else {
        printf("Fork operation failed.\n");
        exit(0);
    }
}

/**
 * Function:  bglist
 * -------------------------------
 * @brief  Display a list of all the programs currently executing in the background as well
 *         as the count of the total background jobs.
 *
 * @param head the head of the list of all the background programs.
 */
void bglist(Node *head) {
    printList(head);
    int length = count(head);
    printf("Total background jobs: %d\n", length);
}

/**
 * Function:  bgkill
 * -------------------------------
 * @brief  Terminate the job with the given process id, pid.
 *
 * @param headl the head of the list of all the background programs.
 * @param pid the process id of the job to be terminated.
 */
void bgkill(Node *headl, pid_t pid) {
    if (if_exist(headl, pid) == 1) {
        int retVal = kill(pid, SIGTERM);
        if (retVal == 0) {
            head = list_remove(headl, pid);
            printf("Successfully terminated the job %d.\n", pid);
        } else {
            printf("Failed to terminate the job %d.\n", pid);
        }
    } else {
        printf("Error: Process %d does not exist.\n", pid);
    }
}

/**
 * Function:  bgstop
 * -------------------------------
 * @brief  Temporarily stop the job with the given process id, pid.
 *
 * @param headl the head of the list of all the background programs.
 * @param pid the process id of the job to be temporarily stopped.
 */
void bgstop(Node *headl, pid_t pid) {
    if (if_exist(headl, pid) == 1) {
        int retVal = kill(pid, SIGSTOP);
        if (retVal == 0) {
            printf("Successfully temporarily stopped the job %d.\n", pid);
        } else {
            printf("Failed to pause the job %d.\n", pid);
        }
    } else {
        printf("Error: Process %d does not exist.\n", pid);
    }
}

/**
 * Function:  bgstart
 * -------------------------------
 * @brief  Re-start the job (which has been previously stopped) with the given process id, pid.
 *
 * @param headl the head of the list of all the background programs.
 * @param pid the process id of the job to be re-started.
 */
void bgstart(Node *headl, pid_t pid) {
    if (if_exist(headl, pid) == 1) {
        int retVal = kill(pid, SIGCONT);
        if (retVal == 0) {
            printf("Successfully re-started the job %d.\n", pid);
        } else {
            printf("Failed to re-start the job %d.\n", pid);
        }
    } else {
        printf("Error: Process %d does not exist.\n", pid);
    }
}

/**
 * Function:  pstart
 * -------------------------------
 * @brief  list the information related to the job with the given process id,
 *
 * @param headl the head of the list of all the background programs.
 * @param pid the process id of the job to be re-started.
 */
void pstat(Node *headl, pid_t pid) {
    // extract the data from /proc/pid/stat
    if (if_exist(headl, pid) == 1) {
        char p1[] = "/proc/";
        char p2[20];
        sprintf(p2, "%d", pid);
        char p3[] = "/stat";
        char *stat = emalloc(strlen(p1) + strlen(p2) + strlen(p3) + 1);
        strcpy(stat, p1);
        strcat(stat, p2);
        strcat(stat, p3);

        FILE *buf;
        buf = fopen(stat, "r");

        int i = 0;
        char data[500];
        fgets(data, 500, buf);
        char *vals[500];
        vals[i] = strtok(data, " ");
        while (vals[i] != NULL) {
            i++;
            vals[i] = strtok(NULL, " ");
        }
        fclose(buf);

        printf("comm: %s\nstate: %s\nutime: %s\nstime: %s\nrss: %s\n",
               vals[1], vals[2], vals[13], vals[14], vals[23]);

        // extract the data from /proc/pid/status
        char p4[] = "/status";
        char *status = emalloc(strlen(p1) + strlen(p2) + strlen(p4) + 1);
        strcpy(status, p1);
        strcat(status, p2);
        strcat(status, p4);

        FILE *buff;
        buff = fopen(status, "r");
        char datas[500];
        char *statusVals[500];

        while (fgets(datas, 500, buff) != NULL) {
            int c = 0;
            statusVals[c] = strtok(datas, " \t");
            while (statusVals[c]) {
                c++;
                statusVals[c] = strtok(NULL, " \t");
            }
            if (strcmp(statusVals[0], "voluntary_ctxt_switches:") == 0) {
                printf("%s %s", statusVals[0], statusVals[1]);
            }
            if (strcmp(statusVals[0], "nonvoluntary_ctxt_switches:") == 0) {
                printf("%s %s", statusVals[0], statusVals[1]);
                break;
            }
        }
        fclose(buff);
        free (stat);
        free(status);
        printf("---------------------------------\n");
    } else {
        printf("Error: Process %d does not exist.\n", pid);
    }
}

int main() {
    char *cmds[100];
    for (;;) {
        get_input(cmds);
        if (cmds[0] == NULL) {
            continue;
        }
        else if (strcmp(cmds[0], "bg") == 0) {
            bg(cmds);
        } else if (strcmp(cmds[0], "bglist") == 0) {
            bglist(head);
        } else if (strcmp(cmds[0], "bgkill") == 0) {
            if (cmds[1] == NULL) {
                printf("Please include the process id of the job to be terminated.\n");
            } else {
                bgkill(head, atoi(cmds[1]));
            }
        } else if (strcmp(cmds[0], "bgstop") == 0) {
            if (cmds[1] == NULL) {
                printf("Please include the process id of the job to be stopped temporarily.\n");
            } else {
                bgstop(head, atoi(cmds[1]));
            }
        } else if (strcmp(cmds[0], "bgstart") == 0) {
            if (cmds[1] == NULL) {
                printf("Please include the process id of the job to be re-started.\n");
            } else {
                bgstart(head, atoi(cmds[1]));
            }
        } else if (strcmp(cmds[0], "pstat") == 0) {
            if (cmds[1] == NULL) {
                printf("Please include the process id of the job to search for.\n");
            } else {
                pstat(head, atoi(cmds[1]));
            }
        } else if (strcmp(cmds[0], "exit") == 0) {
            exit(1);
        } else {
            printf("Command not found.\n");
        }

        // Collect any zombile child process and remove it from the background programs list.
        int status;
        pid_t wpid = waitpid(-1, &status, WNOHANG);
        if(wpid>0){
            if (if_exist(head, wpid) == 1) {
                head = list_remove(head, wpid);
                printf("The job %d is terminated.\n", wpid);
            }
        }
    }
    free_list(head);
}

