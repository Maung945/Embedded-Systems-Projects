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
    pid_t pid1, pid2, pid3, pid4;
    int running1, running2, running3, running4;
    pid1 = fork();
    if (pid1 == 0){
        myfunction(WORKLOAD1);
        exit(0);
    }
    kill(pid1, SIGSTOP);
    pid2 = fork();
    if (pid2 == 0){
        myfunction(WORKLOAD2);
        exit(0);
    }
    kill(pid2, SIGSTOP);
    pid3 = fork();
    if (pid3 == 0){
        myfunction(WORKLOAD3);
        exit(0);
    }
    kill(pid3, SIGSTOP);
    pid4 = fork();
    if (pid4 == 0){
        myfunction(WORKLOAD4);
        exit(0);
    }
    kill(pid4, SIGSTOP);
/**********************************************************************************
**************
At this point, all newly-created child processes are stopped, and
ready for scheduling.
***********************************************************************************
**************/
/**********************************************************************************
**************
- First come first serve scheduling
***********************************************************************************
*************/

    clock_t start, p1End, p2End, p3End, p4End, c1Start, c1End, c2Start, c2End, c3Start, c3End;

    start = clock();
    kill(pid1, SIGCONT);
    waitpid(pid1, NULL, 0);
    p1End = clock();
    c1Start = clock();

    kill(pid2, SIGCONT);
    c1End = clock();
    waitpid(pid2, NULL, 0);
    p2End = clock();
    c2Start = clock();

    kill(pid3, SIGCONT);
    c2End = clock();
    waitpid(pid3, NULL, 0);
    p3End = clock();
    c3Start = clock();

    kill(pid4, SIGCONT);
    c3End = clock();
    waitpid(pid4, NULL, 0);
    p4End = clock();

    double cSwitch1 = ((double)c1End - c1Start)/CLOCKS_PER_SEC;
    double cSwitch2 = ((double)c2End - c2Start)/CLOCKS_PER_SEC;
    double cSwitch3 = ((double)c3End - c3Start)/CLOCKS_PER_SEC;
    double p1Response = ((double)p1End - start)/CLOCKS_PER_SEC;
    double p2Response = ((double)p2End - start)/CLOCKS_PER_SEC;
    double p3Response = ((double)p3End - start)/CLOCKS_PER_SEC;
    double p4Response = ((double)p4End - start)/CLOCKS_PER_SEC;
    
    if (argc > 1){
        printf("P1 Response,P2 Response,P3 Response,P4 Response,Context 1,Context 2,Context 3\n");
    }
    printf("%.5f,%.5f,%.5f,%.5f,%.5f,%.5f,%.5f\n", p1Response, p2Response, p3Response, p4Response, cSwitch1, cSwitch2, cSwitch3);


    /**********************************************************************************
    **************
    - Scheduling code ends here
    ***********************************************************************************
    *************/
    return 0;
}