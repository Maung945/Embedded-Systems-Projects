#include <cstdlib>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <ctime>
#include <algorithm>
#include "../COMMON/Mem.h"

using namespace std;

int checkMain(bool arr[]);
int replacement(vector<MemUnit*> q);
void display(vector<Process> l, int pf, int args);
bool compare(MemUnit *a, MemUnit *b);

const int memSize = 32;
string filename = "../DataFiles/dataTest01.txt";

int main(int argc, char* argv[]){
    if (argc > 1){
        filename = "../DataFiles/";
        filename = filename + argv[1];
    }
    ifstream file(filename);
    string line;
    vector<Process> processes;
    int index;
    MemUnit *current;
    int pageFaults = 0;
    bool mainMem[memSize];
    vector<MemUnit*> memQueue;
    int accessCount = 0;

    //Initialize the main memory to empty
    for (int i = 0; i < memSize; i++){
        mainMem[i]=false;
    }

    //Read file
    if(file.is_open()){
        while (getline(file, line)) {
            index = -1;
            current = nullptr;
            istringstream iss(line);
            string process, address, type;
            int procNum, addr;
            accessCount++;
            //Split line into individual parts
            getline(iss, process, '\t');
            getline(iss, address, '\t');
            getline(iss, type, '\t');
            procNum = stoi(process);
            addr = stoi(address);
            type = type[0];
            //check the process list for the process that was read 
            for(int i = 0; i < processes.size(); i++){
                if(procNum == processes.at(i).getName()){
                    index = i; 
                }
            }
            //If the process was not found, add it to the list
            if (index < 0){
                processes.push_back(Process(procNum));
                index = processes.size() - 1;
            }
            //Create the virtual page number and get the physical address if it exists
            int vpn = addr >> 9;
            current = processes.at(index).getPage(vpn);
            current->access();
            int phy = current->getPhyAddr();
            //Check the main memory for an empty page
            if(phy < 0){
                pageFaults++;
                phy = checkMain(mainMem);
                if(phy >= 0){
                    current->assignAddr(phy);
                    current->setTime();
                    mainMem[phy] = true;
                    memQueue.push_back(current);
                } else { //If there are no empty pages a page needs to be replaced
                    int victim = replacement(memQueue); //This function implements the different algorithms
                    for(int i = 0; i < memQueue.size(); i++){
                        if(victim == memQueue.at(i)->getPhyAddr()){
                            memQueue.at(i)->replace();
                            current->assignAddr(victim);
                            memQueue.erase(memQueue.begin() + i);
                            memQueue.push_back(current);
                            break;
                        }
                    }
                }
            }
            //If it is a write access, set the page to dirty
            if(type == "W"){
                current->setDirty();
            }
        }
        file.close();
    } else {
        cout << "Unable to open file\n";
    }

    display(processes, pageFaults, argc);
    return 0;
}

int checkMain(bool arr[]){
    for (int i = 0; i < memSize; i++){
        if(!arr[i]){
            return i;
        }
    }
    return -1;
}

int replacement(vector<MemUnit*> q){
    //Different replacement algorithms go here
    vector<MemUnit*> oldest;
    vector<MemUnit*> cleanList;
    vector<MemUnit*> dirtyList;
    bool clean = false;
    oldest.push_back(q.at(0));
    for(int i = 1; i < q.size(); i++){
        clock_t temp  = q.at(i)->getTime();
        if(temp < oldest.at(0)->getTime()){
            oldest.clear();
            oldest.push_back(q.at(i));
        } else if ( temp == oldest.at(0)->getTime()){
            oldest.push_back(q.at(i));
        }  
    }
    //In case of a tie, separate the tied pages into dirty and clean
    //then sort by physical address
    if (oldest.size() > 1){
        for( MemUnit *ea : oldest){
            if(ea->getDirty()){
                dirtyList.push_back(ea);
            } else {
                cleanList.push_back(ea);
            }
        }
        sort(cleanList.begin(), cleanList.end(), compare);
        sort(dirtyList.begin(), dirtyList.end(), compare);
        oldest.clear();
        if (cleanList.size() > 0){
            oldest.push_back(cleanList.at(0));
        } else {
            oldest.push_back(dirtyList.at(0));
        }
    }
    return oldest.at(0)->getPhyAddr();
}

void display(vector<Process> l, int pf, int args){
    MemUnit *temp;
    int disk = 0;
    int dw = 0;
    for(Process proc : l){
        for(int i =0; i < 128; i++){
            temp = proc.getPage(i);
            dw += temp->getDirtyDiskWrites();
            disk += temp->getDirtyDiskWrites();
            disk += temp->getCleanDiskWrites();
        }
    }

    if (args > 2){
        cout << "Type,Page faults,Dirty writes,Total disk references" << endl;
    }
    
    cout << "LRU," << pf << "," << dw << "," << disk << endl;
}

bool compare(MemUnit *a, MemUnit *b){
    int aAddr = a->getPhyAddr();
    int bAddr = b->getPhyAddr();
    return aAddr < bAddr;
}