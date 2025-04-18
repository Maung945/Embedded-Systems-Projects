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

struct accessPriority {
    int process;
    int virtPage;
    int access;
};

int checkMain(bool arr[]);
int replacement(vector<MemUnit*> q, vector<accessPriority> p);
void display(vector<Process> l, int pf, int args);
bool compare(accessPriority &a, accessPriority &b);

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
    vector<accessPriority> priorityList;

    //Initialize the main memory to empty
    for (int i = 0; i < memSize; i++){
        mainMem[i]=false;
    }

    //Generate priority list
    if(file.is_open()){
        while (getline(file, line)) {
            istringstream iss(line);
            string process, address, type;
            int procNum, addr;
            getline(iss, process, '\t');
            getline(iss, address, '\t');
            procNum = stoi(process);
            addr = stoi(address);
            addr = addr >> 9;
            bool find = false;
            for(accessPriority &ea : priorityList){
                if(ea.process == procNum && ea.virtPage == addr){
                    ea.access++;
                    find = true;
                    break;
                }
            }
            if(!find){
                accessPriority temp;
                temp.access = 1;
                temp.process = procNum;
                temp.virtPage = addr;
                priorityList.push_back(temp);
            } 
        }
        file.close();
        sort(priorityList.begin(), priorityList.end(), compare);
    } else {
        cout << "Unable to open file\n";
    }
    file.open(filename);

    //Read file
    if(file.is_open()){
        while (getline(file, line)) {
            index = -1;
            current = nullptr;
            istringstream iss(line);
            string process, address, type;
            int procNum, addr;
            accessCount++;
            if (accessCount > 200){
                for(MemUnit *ea : memQueue){
                    ea->setRef(false);
                }
                accessCount = 0;
            }
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
                    int victim = replacement(memQueue, priorityList); //This function implements the different algorithms
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

int replacement(vector<MemUnit*> q, vector<accessPriority> p){
    //Different replacement algorithms go here
    int priority = -1;
    MemUnit *replace;
    for (MemUnit *ea : q){
        for(int i = 0; i < p.size(); i++){
            if(p.at(i).process == ea->getProcess() && p.at(i).virtPage == ea->getVirtual()){
                if(priority > -1){
                    if (i < priority){
                        priority = i;
                        replace = ea;
                    }
                } else {
                    priority = i;
                    replace = ea;
                }
            }
        }
    }
    return replace->getPhyAddr();
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
    
    cout << "Custom," << pf << "," << dw << "," << disk << endl;
}

bool compare(accessPriority &a, accessPriority &b){
    if(a.access < b.access){
        return true;
    } else if( a.access == b.access){
        if(a.virtPage > b.virtPage){
            return true;
        } 
    } 
    return false;
}