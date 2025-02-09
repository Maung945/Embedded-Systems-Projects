#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include "functions.h"

unsigned long long int numChunk = 0;
unsigned long long int N = 0;

void CalculateChunk(unsigned long long int start, int total[]);
void display(double time, int total2[], unsigned long long int num);
void convert(char arr[], int intArr[]);

int main(int argc, char const *argv[]){

    N = 100000000;
    unsigned long long int numTasks = 2;
    int total[20];
    int base = 10;
    FILE *fp[numTasks];
    char command[256];

    void displayTest(int total1[], int total2[]);
    
    clock_t start, end;

    if (argc > 1){
        N = strtoull(argv[1], NULL, base);
    }

    if(argc > 2){
        numTasks = atoi(argv[2]);
    }

    unsigned long long int startValue[numTasks];

    if(!(N%numTasks)){
        numChunk=N/numTasks;
    }

    start = clock();
    
    init(total);

    for(int i = 0; i < numTasks; i++){
        startValue[i]= (numChunk * i);
    }

    

    for(int i = 0; i < numTasks - 1; i++){
        sprintf(command, "./CalculateChunk %llu %llu", startValue[i], numChunk);
        fp[i] = popen(command, "r");
    }

    CalculateChunk(startValue[numTasks - 1], total);

    // unsigned long long int testEnd = startValue[numTasks - 1] + numChunk;

    // printf("Parent calculation from %llu to %llu: ", startValue[numTasks - 1], testEnd-1);

    // displaytest(total);

    // printf("\n");

    int rec[20];
    int tempVal = 0;

    char testStr[256];

    for(int i = 0; i < numTasks - 1; i++){
        init(rec);
        fgets(testStr, 256, fp[i]);
        convert(testStr, rec);
        // for(int j = 0; j < 20; j++){
        //     // fscanf(fp[i], "%d", &tempVal);
        //     fgets(testStr, 256, fp[i]);
        //     // printf("TempValue: %d\n", tempVal);
        //     printf("Teststr %d : ", j);
        //     displaytest(testStr);
        //     printf("\n");
        //     rec[j] = tempVal;
        // }
        bigAdd(total, rec);
        pclose(fp[i]);
    }
    
    end = clock();

    double time_taken = ((double)end - start)/CLOCKS_PER_SEC;
    display(time_taken, total,N);

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

void display(double time, int total2[], unsigned long long int num ){
    int leading = 0;

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
    printf("Time taken to calculate is %.5fs\n", time);
}

// void displaytest(int arr[]){
//     for(int i = 0; i < 20; i++){
//         printf("%d", arr[i]);
//     }
// }

void convert(char arr[], int intArr[]){
    for(int i = 0; i < 20; i++){
        intArr[i] = arr[i] - '0';
    }
}
