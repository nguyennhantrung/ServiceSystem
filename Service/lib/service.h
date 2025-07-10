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

    bool SendTo(string name, MessageData* msgData) {
        if(msgData == nullptr) {
            return false;
        }
        auto it = msgDataServices.find(name);
        if(it != msgDataServices.end()) {
            return it->second->Send(msgData);
        }
        return false;
    }

    bool EventHandler(const MessageData* msgData) {
        return true;
    }

    bool Start() {
        // Start the service, initialize IPC, etc.
        ipc = new IPC([this](const MessageData* msgData) {
            return this->EventHandler(const_cast<MessageData*>(msgData));
        });
        return ipc->Start();
    }

    bool Stop() {
        // Stop the service, clean up IPC, etc.
        if(ipc != nullptr) {
            ipc->Stop();
            delete ipc;
            ipc = nullptr;
        }
        return true;
    }

    InterProcessCommunication* CreateCommunicate(string name) {
        InterProcessCommunication *ipcService = new InterProcessCommunication(name, ipc);
        msgDataServices[name] = ipcService;
        return ipcService;
    }

    string GetName() const {
        return name;
    }
private:
    string name;
    IPC *ipc;  // IPC pointer for this service
    map<string, IMsgDataService*> msgDataServices;  // module ID, IPC pointer
};