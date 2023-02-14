A Process Manager (PMan)

PMan allows a program to be started in the background—that is, the program is
running, but PMan continues to accept input from the user. This is a
simplified version of background process manager that supports executing processes in the
background. PMan will contantly prompts 
PMan: >
for user input until "exit" is received and the process manager will exit.

Supported commands：
    - bg <program>: PMan will start the program in the background. The program will
                    execute and PMan will also continue to execute and give prompt to accept other
                    commands. 
                    e.g., PMan: >bg foo
    
    -bglist:        PMan will display a list of all the programs currently executing in the 
                    background.
                    e.g., PMan: >bglist
                          123: /home/user/CSC/foo
                          456: /home/user/CSC/foo
                          Total background jobs: 2
                    In this case, there are 2 background jobs, both running the program "foo",
                    the first one with process ID 123 and the second one with 456,
    
    -bgkill <pid>:  PMan will send the "terminate" signal to the job with the given process id, pid,
                    to terminate that job.   
                    e.g., PMan: >bgkill 345

    -bgstop <pid>:  PMan will send the "stop" signal to the job with the given process id, pid,
                    to stop (temporarily) that job.   
                    e.g., PMan: >bgstop 345

    -bgstart <pid>: PMan will send the "continue" signal to the job with the given process id, pid,
                    to re-start that job.   
                    e.g., PMan: >bgstart 345

    -pstat <pid>:   PMan will list the following information related to given process id:
    
                    1. comm : The filename of the executable, in parentheses. 
                    2. state : One of the following characters, indicating process state: R (Running), 
                               S (Sleeping in an interruptible wait), D (Waiting in uninterruptible disk sleep), 
                               Z (Zombie), T (Stopped (on a signal) or (before Linux 2.6.33) trace stopped ), 
                               t (Tracing stop (Linux 2.6.33 onward)), W (Paging (only before Linux 2.6.0)), 
                               X (Dead (from Linux 2.6.0 onward)), x (Dead (Linux 2.6.33 to 3.13 only)), 
                               K (Wakekill (Linux 2.6.33 to 3.13 only)), W (Waking (Linux 2.6.33 to 3.13 only)), 
                               P (Parked (Linux 3.9 to 3.13 only)).
                    3. utime: Amount of time that this process has been scheduled in user mode.
                    4. stime: Amount of time that this process has been scheduled in kernel mode.
                    5. rss: Resident Set Size: number of pages the process has in real memory.
                    6. voluntary ctxt switches: Number of voluntary context switches (since Linux 2.6.23).
                    7. nonvoluntary ctxt switches: Number of involuntary context switches (since Linux 2.6.23).
    
    In summary, PMan supports these commands: bg, bglist, bgkill, bgstop, bgstart, and pstat. If the user types an 
    unrecognized command, an error message is given by PMan,
    e.g., PMan:> ttest
          PMan:> ttest: command not found


How to compile and run PMan:

There is a make file provided, so simply type "make" into the terminal to compile.
Then type ./PMan into the terminal to execute PMan.