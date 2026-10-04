#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <math.h>

unsigned long long int numChunk = 0;
unsigned long long int N = 0;

void CalculateChunk(unsigned long long int start, int total[]);
void bigAdd(int running[], int current[]);
void display(double time, unsigned long long int total1, int total2[], unsigned long long int num);
void split(int arr[], unsigned long long int in);
void init(int arr[]);
void sendData(int data[], int fd[]);

int main(int argc, char const *argv[]){

    int base = 10;
    unsigned long long int numTasks = 2;

    if (argc > 1){
        N = strtoull(argv[1], NULL, base);
    }

    if(argc > 2){
        numTasks = atoi(argv[2]);
    }
    
    unsigned long long int total6B = 0;
    int fd[numTasks - 1][2];
    int total[20];
    int taskTrigger = (log((double)numTasks)/log(2.0));
    int task = 0;
    int pids[taskTrigger];
    
    clock_t start, end;

    unsigned long long int startValue[numTasks];

    if(!(N%numTasks)){
        numChunk=N/numTasks;
    }

    start = clock();

    for (int i = 0; i < numTasks - 1; i++) {
        if (pipe(fd[i]) == -1) {
            perror("pipe");
            exit(EXIT_FAILURE);
        }
    }
    
    init(total);

    for(int i = 0; i < numTasks; i++){
        startValue[i]= (numChunk * i);
    }

    for(int i = 0; i < taskTrigger; i++){
        pids[i] = fork();
    }
    int multiplier = taskTrigger - 1;
    for(int i = 0; i < taskTrigger; i++){
        if(pids[i]>0){
            task += pow(2, multiplier - i);
        }
    }
    int rec[20];
    int tempVal = 0;
    for(int i = 0; i < numTasks; i++){
        if(i == task && task != (numTasks - 1)){
            CalculateChunk(startValue[i], total);
            close(fd[i][0]);
            sendData(total, fd[i]);
            close(fd[i][1]);
            exit(EXIT_SUCCESS);
        } else if(i == task){
            CalculateChunk(startValue[i], total);
            for(int j = 0; j < numTasks - 1; j++){
                init(rec);
                for(int k = 0; k < 20; k++){
                    if (read(fd[j][0], &tempVal, sizeof(int)) != sizeof(int)) {
                        perror("read failed");
                        exit(EXIT_FAILURE);
                    }
                    rec[k] = tempVal;
                }
                bigAdd(total, rec);
            }
        }
    }
    
    end = clock();

    double time_taken = ((double)end - start)/CLOCKS_PER_SEC;
    display(time_taken, total6B, total,N);

    return 0;
}

void CalculateChunk(unsigned long long int start, int total[]){
    unsigned long long int end = start + numChunk;
    unsigned long long int tempTotal = start + 1;
    int flag = 1;

    if(end == N){
        end--;
    }

    for(unsigned long long int j = tempTotal+ 1; j <= end; j++){
        if(j > 6000000000){
             if(j == 6000000001 || (j > 6000000001 && flag)){
                split(total, tempTotal);
                flag--;
             } 
            int tempArr[20];
            split(tempArr, j);
            bigAdd(total, tempArr);

        } else{
            tempTotal += j;
            if ( j == end){
                split(total, tempTotal);
            }
        }
    }
}

void bigAdd(int running[], int current[]){
    int carry = 0;
    for(int i = 19; i>=0; i--){
        int temp = running[i] + current[i];
        if (carry){
            temp = temp + 1;
        }
        if(temp >= 10){
            carry = 1;
            running[i] = temp - 10;
        } else {
            running[i] = temp;
            carry = 0;
        }
    }
}

void display(double time, unsigned long long int total1, int total2[], unsigned long long int num ){
    int leading = 0;
    if(total1){
        printf("The sum from 0 to %llu is %llu\n", num, total1);
    } else {
        printf("The sum from 0 to %llu is ", num - 1);
        for(int i = 0; i < 20; i++){
            if(total2[i] > 0 && (!leading)){
                leading = 1;
            }
            if(leading){
                printf("%d", total2[i]);
            }
        }
        printf("\n");
    }
    printf("Time taken to calculate is %.5fs\n", time);
}

void split(int arr[], unsigned long long int in){
    unsigned long long int temp = in;
                for(int j = 19; j >= 0; j-- ){
                    arr[j] = in % 10;
                    in /= 10;
                }
}

void init(int arr[]){
    for (int i = 0; i < 20; i++){
        arr[i] = 0;
    }
}
void sendData(int data[], int fd[]){
    int temp = 0;
    for(int i = 0; i < 20; i++){
        temp = data[i];
        if (write(fd[1], &temp, sizeof(int)) != sizeof(int)) {
            perror("write failed");
            exit(EXIT_FAILURE);
        }

    }
}
