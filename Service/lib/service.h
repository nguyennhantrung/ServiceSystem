#pragma once
#include <string>
#include <map>

#include "../IPC/lib/ipc.h"
using namespace std;

class Service {
public:
    Service(string name){
        this->name = name;
    }

    bool CreateCommunicate(Service* other) {
        auto it = ipcList.find(other->name);
        if(it == ipcList.end()) {
            // add to list ipc
        }
    }
private:
    string name;
    map<string, IPC*> ipcList;  // module ID, IPC pointer
};