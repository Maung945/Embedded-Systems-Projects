#ifndef MEM_H
#define MEM_H

#include <ctime>
#include <iostream>

class MemUnit {
    public:
        MemUnit();
        void setProcess(int p);
        int getProcess();
        void setDirty();
        bool getDirty();
        void setRef(bool x);
        bool getRef();
        int getPhyAddr();
        void replace();
        int getCleanDiskWrites();
        int getDirtyDiskWrites();
        void assignAddr(int x);
        int getAccessCount();
        void access();
        void setTime();
        clock_t getTime();
        void setVirtual(int x);
        int getVirtual();
    private:
        int pro;
        int virt;
        bool dirty;
        bool ref;
        int phyAddr;
        int cleanDiskWrites;
        int dirtyDiskWrites;
        int accesses;
        time_t timestamp;


};

class Process {
    public:
        Process(int x);
        int getName();
        MemUnit *getPage(int x);
    private:
        int procName;
        MemUnit pageTable[128];
        
};

#endif // MEM_H