#pragma once
#include <string>
#include <map>

#include "ipc.h"
using namespace std;

class Service {
public:
    Service(string name);
    ~Service();
    bool SendTo(string name, MessageData* msgData);
    bool EventHandler(const MessageData* msgData);
    bool Start();
    bool Stop();
    InterProcessCommunication* CreateCommunicate(string communicationName);
    string GetName() const;
private:
    string name;
    IPC *ipc;  // IPC pointer for this service
    map<string, IMsgDataService*> msgDataServices;  // module ID, IPC pointer
};