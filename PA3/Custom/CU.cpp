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
    int count;
    string type;
};

int checkMain(bool arr[]);
int replacement(vector<MemUnit*> q, vector<accessPriority> b);
void display(vector<Process> l, int pf, int args);
void splitString(string start, int &p, int &a, string &t);
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
    vector<accessPriority> buffer;

    //Initialize the main memory to empty
    for (int i = 0; i < memSize; i++){
        mainMem[i]=false;
    }

    //Read file
    if(file.is_open()){
        for (int i = 0; i < 500; i++){
            getline(file, line);
            int procNum, addr;
            string type;
            splitString(line, procNum, addr, type);
            accessPriority temp;
            temp.process = procNum;
            temp.virtPage = addr;
            temp.type = type;
            buffer.push_back(temp); 
        }
        while (buffer.size() > 0) { 
            index = -1;
            current = nullptr;
            string type;
            int procNum, addr;
            if(getline(file, line)){
                splitString(line, procNum, addr, type);
                accessPriority temp;
                temp.process = procNum;
                temp.virtPage = addr;
                temp.type = type;
                buffer.push_back(temp);
            }
            procNum = buffer.at(0).process;
            addr = buffer.at(0).virtPage;
            type = buffer.at(0).type;
            buffer.erase(buffer.begin());
            accessCount++;
            if (accessCount > 200){
                for(MemUnit *ea : memQueue){
                    ea->setRef(false);
                }
                accessCount = 0;
            }
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
            //Get the physical address if it exists
            current = processes.at(index).getPage(addr);
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
                    int victim = replacement(memQueue, buffer); //This function implements the different algorithms
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

int replacement(vector<MemUnit*> q, vector<accessPriority> b){
    //Different replacement algorithms go here
    vector<accessPriority> countList;
    bool found = false;
    for(accessPriority e : b){
        found = false;
        for(accessPriority &counted : countList){
            if(e.process == counted.process && e.virtPage == counted.virtPage){
                counted.count++;
                found = true;
            }
        }
        if(!found){
            accessPriority tmp;
            tmp.count = 1;
            tmp.process = e.process;
            tmp.type = e.type;
            tmp.virtPage = e.virtPage;
            countList.push_back(tmp);
        }
    }
    sort(countList.begin(), countList.end(), compare);
    int currentIndex = -1;
    MemUnit *selected;
    bool foundAhead[memSize];
    for(int j = 0; j < memSize; j++){
        for(int i = 0; i < countList.size(); i++){
            if(q.at(j)->getProcess() == countList.at(i).process && q.at(j)->getVirtual() == countList.at(i).virtPage){
                foundAhead[j] = true;
                if(currentIndex < 0){
                    selected = q.at(j);
                    currentIndex = i;
                } else {
                    if(i < currentIndex){
                        selected = q.at(j);
                        currentIndex = i;
                    }
                }
            }
        }
    }
    for(int i = 0; i < memSize; i++){
        if(!foundAhead[i]){
            selected = q.at(i);
            currentIndex = 1;
            break;
        }
    }
    if(currentIndex <0){
        selected = q.at(0);
    }
    return selected->getPhyAddr();
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
    
    cout << "CU," << pf << "," << dw << "," << disk << endl;
}

void splitString(string start, int &p, int &a, string &t){
    istringstream iss(start);
    string process, address;
    getline(iss, process, '\t');
    getline(iss, address, '\t');
    getline(iss, t, '\t');
    p = stoi(process);
    a = stoi(address);
    t = t[0];
    a = a >> 9;
}

bool compare(accessPriority &a, accessPriority &b){
    if(a.count < b.count){
        return true;
    } else if( a.count == b.count){
        if(a.virtPage > b.virtPage){
            return true;
        } 
    } 
    return false;
}