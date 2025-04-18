#include "Mem.h"

MemUnit::MemUnit(){
    dirty = false;
    ref = false;
    phyAddr = -1;
    cleanDiskWrites = 0;
    dirtyDiskWrites = 0;
    accesses = 0;
    timestamp = time(NULL);
    virt = -1;
}

void MemUnit::setProcess(int p){
    pro = p;
}

int MemUnit::getProcess(){
    return pro;
}

void MemUnit::setDirty(){
    dirty = true;
}

bool MemUnit::getDirty(){
    return dirty;
}

void MemUnit::setRef(bool x){
    ref = x;
}

bool MemUnit::getRef(){
    return ref;
}

int MemUnit::getPhyAddr(){
    return phyAddr;
}

int MemUnit::getCleanDiskWrites(){
    return cleanDiskWrites;
}

int MemUnit::getDirtyDiskWrites(){
    return dirtyDiskWrites;
}

void MemUnit::replace(){
    if(dirty){
        dirtyDiskWrites++;
    }
    dirty = false;
    phyAddr = -1;
    ref = false;
}
void MemUnit::assignAddr(int x){
    phyAddr = x;
    cleanDiskWrites++;
}

int MemUnit::getAccessCount(){
    return accesses;
}

void MemUnit::access(){
    timestamp = clock();
    ref = true;
    accesses++;
}

void MemUnit::setTime(){
    timestamp = clock();
}

clock_t MemUnit::getTime(){
    return timestamp;
}

void MemUnit::setVirtual(int x){
    virt = x;
}
int MemUnit::getVirtual(){
    return virt;
}

Process::Process(int x){
    procName = x;
    for(int i = 0; i < 128; i++){
        pageTable[i].setVirtual(i);
        pageTable[i].setProcess(procName);
    }
}

int Process::getName(){
    return procName;
}

MemUnit *Process::getPage(int x){
    return &pageTable[x];
}