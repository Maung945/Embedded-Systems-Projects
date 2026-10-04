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
    pid_t level1[4];
    pid_t level2[4];
    int status[4];
    int timing[4];
    level1[0] = fork();
    if (level1[0] == 0){
        myfunction(WORKLOAD1);
        exit(0);
    }
    kill(level1[0], SIGSTOP);
    level1[1] = fork();
    if (level1[1] == 0){
        myfunction(WORKLOAD2);
        exit(0);
    }
    kill(level1[1], SIGSTOP);
    level1[2] = fork();
    if (level1[2] == 0){
        myfunction(WORKLOAD3);
        exit(0);
    }
    kill(level1[2], SIGSTOP);
    level1[3] = fork();
    if (level1[3] == 0){
        myfunction(WORKLOAD4);
        exit(0);
    }
    kill(level1[3], SIGSTOP);
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
    clock_t start, pEnds[4], cStart, cEnd;
    int cCount = 0;
    int count = 0;
    double cSwitch = 0;
    for(int i = 0; i <4; i++){
        status[i] = 1;
    }

    start = clock();

    for(int i = 0; i < 4; i++){
        kill(level1[i], SIGCONT);
        if(i > 0){
            cEnd = clock();
            cCount++;
            cSwitch += ((double)cEnd - cStart)/CLOCKS_PER_SEC;
        }
        usleep(QUANTUM1);
        cStart = clock();
        kill(level1[i], SIGSTOP);
        waitpid(level1[i], &status[i], WNOHANG);
        if (status[i] <= 0){
            pEnds[i] = clock();
            level1[i] = 0;
        } else{
            level2[count] = level1[i];
            timing[count] = i;
            count++;
        }
    }

    // kill(level1[0], SIGCONT);
    // usleep(QUANTUM1);
    // kill(level1[0], SIGSTOP);
    // waitpid(level1[0], &status[0], WNOHANG);
    // if (status[0] <= 0){
    //     pEnds[0] = clock();
    // }
    
    // kill(level1[1], SIGCONT);
    // usleep(QUANTUM2);
    // kill(level1[1], SIGSTOP);
    // waitpid(level1[1], &status[1], WNOHANG);
    // if (status[1] <= 0){
    //     pEnds[1] = clock();
    // }
    
    // kill(level1[2], SIGCONT);
    // usleep(QUANTUM3);
    // kill(level1[2], SIGSTOP);
    // waitpid(level1[2], &status[2], WNOHANG);
    // if (status[2] <= 0){
    //     pEnds[2] = clock();
    // }
    
    // kill(level1[3], SIGCONT);
    // usleep(QUANTUM4);
    // kill(level1[3], SIGSTOP);
    // waitpid(level1[3], &status[3], WNOHANG);
    // if (status[3] <= 0){
    //     pEnds[3] = clock();
    // }

    for (int i = 0; i < count; i++){
        kill(level2[i], SIGCONT);
        cEnd = clock();
        cCount++;
        cSwitch += ((double)cEnd - cStart)/CLOCKS_PER_SEC;
        waitpid(level2[i], NULL, 0);
        pEnds[timing[i]] = clock();
        if(i != count - 1){
            cStart = clock();
        }
    }

    double p1Response = ((double)pEnds[0] - start)/CLOCKS_PER_SEC;
    double p2Response = ((double)pEnds[1] - start)/CLOCKS_PER_SEC;
    double p3Response = ((double)pEnds[2] - start)/CLOCKS_PER_SEC;
    double p4Response = ((double)pEnds[3] - start)/CLOCKS_PER_SEC;
    
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