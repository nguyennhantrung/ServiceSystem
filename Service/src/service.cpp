#include <iostream>
#include <string>
#include "service.h"
using namespace std;

Service::Service(string name) {
    cout << "Service created with name: " << name << endl;
    this->name = name;
    ipc = new IPC([this](const MessageData* msgData) {
        return this->EventHandler(const_cast<MessageData*>(msgData));
    });
}

Service::~Service() {
    // Clean up dynamically allocated InterProcessCommunication objects
    for (auto& pair : msgDataServices) {
        delete pair.second;
    }
    msgDataServices.clear();
}


bool Service::SendTo(string name, MessageData* msgData) {
    if(msgData == nullptr) {
        return false;
    }
    auto it = msgDataServices.find(name);
    if(it != msgDataServices.end()) {
        cout << "Sending message to service: " << name << endl;
        return it->second->Send(msgData);
    }
    return false;
}

bool Service::EventHandler(const MessageData* msgData) {
    if(msgData == nullptr) {
        cout << "Error: EventHandler null message data." << endl;
        return false;
    }
    // Handle the message data here
    switch(msgData->GetMsgSrc()) {
        case SystemId::Service1:
        case SystemId::Service2:
        {
            switch (msgData->GetMsgType())
            {
                case MessageType::PrintString:
                {
                    string msgStr((char*)msgData->GetMsgData(), msgData->GetMsgLength());
                    cout << "Received message from " << static_cast<int>(msgData->GetMsgSrc()) 
                         << "[" << msgData->GetMsgLength() << "]: " << msgStr << endl;
                    return true;
                }
                default:
                    return false;
            }
        }
        default:
            return false;
    }
}

bool Service::Start() {
    // Start the service, initialize IPC, etc.
    cout << "Starting service: " << name << endl;
    return ipc->Start();
}

bool Service::Stop() {
    // Stop the service, clean up IPC, etc.
    cout << "Stopping service: " << name << endl;
    if(ipc != nullptr) {
        ipc->Stop();
        delete ipc;
        ipc = nullptr;
    }
    cout << "Service stopped: " << name << endl;
    return true;
}

string Service::GetName() const {
    return name;
}

InterProcessCommunication* Service::CreateCommunicate(string communicationName) {
    InterProcessCommunication *ipcService = new InterProcessCommunication(communicationName, ipc);
    msgDataServices[communicationName] = ipcService;
    return ipcService;
}