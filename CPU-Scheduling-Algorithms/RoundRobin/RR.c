#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <sys/time.h>
/***********************************************************************************
*************
These DEFINE statements represent the workload size of each task and
the time quantum values for Round Robin scheduling for each task.
***********************************************************************************
**************/
#define WORKLOAD1 100000
#define WORKLOAD2 50000
#define WORKLOAD3 25000
#define WORKLOAD4 10000
#define QUANTUM1 1000
#define QUANTUM2 1000
#define QUANTUM3 1000
#define QUANTUM4 1000
/***********************************************************************************
*************
DO NOT CHANGE THE FUNCTION IMPLEMENTATION
***********************************************************************************
**************/
void myfunction(int param){
    int i = 2;
    int j, k;
    while(i < param){
        k = i;
        for (j = 2; j <= k; j++){
            if (k % j == 0){
                k = k/j;
                j--;
                if (k == 1){
                    break;
                }
            }
        }
        i++;
    }
}
/***********************************************************************************
*************/
int main(int argc, char const *argv[]){
    pid_t pids[4];
    int running[4];
    pids[0] = fork();
    if (pids[0] == 0){
        myfunction(WORKLOAD1);
        exit(0);
    }
    kill(pids[0], SIGSTOP);
    pids[1] = fork();
    if (pids[1] == 0){
        myfunction(WORKLOAD2);
        exit(0);
    }
    kill(pids[1], SIGSTOP);
    pids[2] = fork();
    if (pids[2] == 0){
        myfunction(WORKLOAD3);
        exit(0);
    }
    kill(pids[2], SIGSTOP);
    pids[3] = fork();
    if (pids[3] == 0){
        myfunction(WORKLOAD4);
        exit(0);
    }
    kill(pids[3], SIGSTOP);
/**********************************************************************************
**************
At this point, all newly-created child processes are stopped, and
ready for scheduling.
***********************************************************************************
**************/
/**********************************************************************************
**************
- Scheduling code starts here
- Below is a sample schedule. (which scheduling algorithm is this?)
- For the assignment purposes, you have to replace this part with the
other scheduling methods
to be implemented.
***********************************************************************************
*************/
    clock_t start, pEnd[4], cStart, cEnd;
    cStart = 0;
    int cCount = 0;
    double cSwitch = 0;
    for(int i = 0; i <4; i++){
        running[i] = 1;
    }
    start = clock();
    while (running[0] > 0 || running[1] > 0 || running[2] > 0 || running[3] > 0){

        for(int i = 0; i <4; i++){
            if(running[i] > 0){
                kill(pids[i], SIGCONT);
                if(cStart != 0){
                    cEnd = clock();
                    cCount++;
                    cSwitch += ((double)cEnd - cStart)/CLOCKS_PER_SEC;
                    cStart = 0;
                }
                usleep(QUANTUM1);
                cStart = clock();
                kill(pids[i], SIGSTOP);
                waitpid(pids[i], &running[i], WNOHANG);
                if(running[i] <= 0){
                    pEnd[i] = clock();
                }
            }
        }
    }
    double p1Response = ((double)pEnd[0] - start)/CLOCKS_PER_SEC;
    double p2Response = ((double)pEnd[1] - start)/CLOCKS_PER_SEC;
    double p3Response = ((double)pEnd[2] - start)/CLOCKS_PER_SEC;
    double p4Response = ((double)pEnd[3] - start)/CLOCKS_PER_SEC;

    if (argc > 1){
        printf("P1 Response,P2 Response,P3 Response,P4 Response,Context Total,Context Count\n");
    }
    printf("%.5f,%.5f,%.5f,%.5f,%.5f,%.i\n", p1Response, p2Response, p3Response, p4Response, cSwitch, cCount);
    /**********************************************************************************
    **************
    - Scheduling code ends here
    ***********************************************************************************
    *************/
    return 0;
}