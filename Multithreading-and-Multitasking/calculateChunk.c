#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "functions.h"

int main(int argc, char const *argv[]){
    unsigned long long int start = 0;
    unsigned long long int numChunk = 0;
    unsigned long long int end = 0;
    unsigned long long int tempTotal = 0;
    int total[20];
    int flag = 1;

    init(total);

    if (argc > 1){
        start = strtoull(argv[1], NULL, 10);
        numChunk = strtoull(argv[2], NULL, 10);
    }

    end = start + numChunk;
    tempTotal = start + 1;

    for(unsigned long long int j = tempTotal+1; j <= end; j++){
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

    for (int i = 0; i < 20; i++){
        printf("%d", total[i]);
    }
    return 0;
}
