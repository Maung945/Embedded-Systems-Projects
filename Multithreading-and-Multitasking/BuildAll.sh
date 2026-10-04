#Build Baseline
gcc baseline.c -o Baseline

#Build Multithreading
gcc multithreading.c -lm -o Multithreading

#Build Multitasking1
gcc multitasking1.c -lm -o Multitasking1

#Build Multitasking2
gcc -c functions.c
gcc -c calculateChunk.c
gcc -c multitasking2.c
gcc functions.o calculateChunk.o -o CalculateChunk
gcc functions.o multitasking2.o -o Multitasking2
