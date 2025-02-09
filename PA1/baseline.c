#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

void bigAdd(int running[], int current[]);
void display(double time, unsigned long long int total1, int total2[], unsigned long long int num);
unsigned long long int calculate(unsigned long long int num, int t[]);
void split(int arr[], unsigned long long int in);

int main(int argc, char const *argv[]){

    unsigned long long int N = 100000000;
    unsigned long long int total6B = 0;
    int total[20];
    int base = 10;

    clock_t start, end;

    if (argc > 1){
        N = strtoull(argv[1], NULL, base);
    }

    start = clock();
    total6B = calculate(N, total);
    end = clock();

    double time_taken = ((double)end - start)/CLOCKS_PER_SEC;
    display(time_taken, total6B, total,N);

    return 0;
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
    if(total1){
        printf("The sum from 0 to %llu is %llu\n", num - 1, total1);
    } else {
        printf("The sum from 0 to %llu is ", num - 1);
        for(int i = 0; i < 20; i++){
            printf("%d", total2[i]);
        }
        printf("\n");
    }
    printf("Time taken to calculate is %.5fs\n", time);
}

unsigned long long int calculate(unsigned long long int num, int t[]){
    unsigned long long int total = 0;
    for(unsigned long long int i = 0; i < num; i++){
        if(i > 6000000000){
             if(i == 6000000001){
                split(t,total);
             } 
            int tempArr[20];

            split(tempArr,i);
            bigAdd(t, tempArr);

        } else{
            total += i;
        }
    }
    return total;
}

void split(int arr[], unsigned long long int in){
    unsigned long long int temp = in;
                int count = 0;
                while (temp != 0){
                    temp /= 10;
                    count++;
                }
                for(int j = count - 1; j >= 0; j-- ){
                    arr[j] = in % 10;
                    in /= 10;
                }
}