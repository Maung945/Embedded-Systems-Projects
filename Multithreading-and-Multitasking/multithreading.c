#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

unsigned long long int numChunk = 0;
unsigned long long int N = 0;

void* CalculateChunk(void* i);
void bigAdd(int running[], int current[]);
void display(double time, unsigned long long int total1, int total2[], unsigned long long int num);
void split(int arr[], unsigned long long int in);

int main(int argc, char const *argv[]){

    N = 100000000;
    unsigned long long int numThreads = 2;
    unsigned long long int total6B = 0;
    int* receive = NULL;
    int total[20];
    int base = 10;
    
    clock_t start, end;

    if (argc > 1){
        N = strtoull(argv[1], NULL, base);
    }

    if(argc > 2){
        numThreads = atoi(argv[2]);
    }

    pthread_t ids[numThreads];
    unsigned long long int startValue[numThreads];

    if(!(N%numThreads)){
        numChunk=N/numThreads;
    }

    start = clock();

    for(int i = 0; i < 20; i++){
        total[i] = 0;
    }

    for(int i = 0; i < numThreads; i++){
        startValue[i]= numChunk * i;
        pthread_create(&ids[i], NULL, CalculateChunk, (void*)&startValue[i]);
    }

    for(int i = 0; i < numThreads; i++){
        pthread_join(ids[i], (void**)&receive);
        bigAdd(total, receive);
        free(receive);
        receive = NULL;
    }

    
    end = clock();

    double time_taken = ((double)end - start)/CLOCKS_PER_SEC;
    display(time_taken, total6B, total,N);

    return 0;
}

void* CalculateChunk(void* i){
    unsigned long long int start = *((unsigned long long int*)i);
    unsigned long long int end = start + numChunk;
    unsigned long long int tempTotal = start + 1;
    int flag = 1;
    int *total = malloc(20 * sizeof(int));

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
    pthread_exit((void*)total);
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
        printf("The sum from 0 to %llu is %llu\n", num - 1, total1);
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

