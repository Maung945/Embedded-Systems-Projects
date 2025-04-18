#Build Random
g++ ../Random/RAND.cpp ../COMMON/Mem.cpp -o ../Executibles/RAND.o

#Build Periodic Reference Reset
g++ ../Periodic/PER.cpp ../COMMON/Mem.cpp -o ../Executibles/PER.o

#Build Least Recently Used
g++ ../LeastRecent/LRU.cpp ../COMMON/Mem.cpp -o ../Executibles/LRU.o

#Build First In First Out
g++ ../FirstInFirstOut/FIFO.cpp ../COMMON/Mem.cpp -o ../Executibles/FIFO.o

#Build Custom
g++ ../Custom/CU.cpp ../COMMON/Mem.cpp -o ../Executibles/CU.o
