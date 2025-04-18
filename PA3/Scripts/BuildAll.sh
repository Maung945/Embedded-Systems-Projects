#Build Random
g++ ../Random/RAND.cpp ../COMMON/Mem.cpp -o ../Executibles/RAND.o

#Build Least Recently Used
g++ ../LeastRecent/LRU.cpp ../COMMON/Mem.cpp -o ../Executibles/LRU.o

#Build Periodic Reference Reset
g++ ../Periodic/PER.cpp ../COMMON/Mem.cpp -o ../Executibles/PER.o

#Build First In First Out
g++ ../FirstInFirstOut/FIFO.cpp ../COMMON/Mem.cpp -o ../Executibles/FIFO.o
