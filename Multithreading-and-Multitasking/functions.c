#include "functions.h"

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