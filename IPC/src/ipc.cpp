#include "ipc.h"
#include <iostream>
#include <string>
using namespace std;

/*
    MessageData class implementation
    This class encapsulates the message data structure used in IPC.
    It includes the message type, data, length, source, and destination system IDs.
    The constructor initializes these attributes, and getter methods provide access to them.
*/
MessageData::MessageData(MessageType msgType, void* msgData, int msgLength, SystemId msgSrc, SystemId msgDest) {
    this->msgType = msgType;
    this->msgData = msgData;
    this->msgLength = msgLength;
    this->msgSrc = msgSrc;
    this->msgDest = msgDest;
}
MessageType MessageData::GetMsgType() const { return msgType; }
void* MessageData::GetMsgData() const { return msgData; }
int MessageData::GetMsgLength() const { return msgLength; }
SystemId MessageData::GetMsgSrc() const { return msgSrc; }
SystemId MessageData::GetMsgDest() const { return msgDest; }

/*
    IPC class implementation
    This class manages inter-process communication (IPC) using a message queue.
    It provides methods to enqueue and dequeue messages, start and stop the IPC thread,
    and handle incoming messages using a user-defined handler function.
*/
IPC::IPC(std::function<bool(const MessageData*)> handler) {
    this->handler = handler;
}
bool IPC::EnQueue(MessageData* msgData) {
    if(msgData == nullptr) {
        cout << "Error: Attempted to enqueue a null message." << endl;
        return false;
    }
    cout << "Enqueuing message of type: " << static_cast<int>(msgData->GetMsgType()) << endl;
    queueIn.push(msgData);
    return true;
}
MessageData* IPC::DeQueue() {
    if(queueIn.empty()) return nullptr;
    cout << "Dequeuing message." << endl;
    MessageData* msgData = queueIn.front();
    queueIn.pop();
    return msgData;
}
bool IPC::Start() {
    running = true;
    // Start the IPC, initialize threads, etc.
    // This is a placeholder for actual implementation
    ipcThread = std::thread([this]() {
        cout << "IPC thread started." << endl;
        while (running) {
            MessageData* msgData = DeQueue();
            if (msgData != nullptr) {
                handler(msgData);
            }
        }
    });
    return true;
}
bool IPC::Stop() {
    running = false;
    if (ipcThread.joinable()) {
        ipcThread.join(); // Wait for the thread to finish
    }
    return true;
}

/*
    IMsgDataService class implementation
    This class serves as a base class for message data services.
    It provides default implementations for sending and receiving messages,
    which can be overridden by derived classes.
*/
IMsgDataService::IMsgDataService(string name, IPC* ipc) : name(name), ipc(ipc) {}
IMsgDataService::~IMsgDataService() {}
bool IMsgDataService::Send(MessageData* msgData) {
    cout << "Send method not implemented in base class." << endl;
    return false; // Default implementation, can be overridden
}
bool IMsgDataService::Receive(MessageData* msgData) {
    cout << "Receive method not implemented in base class." << endl;
    return false; // Default implementation, can be overridden
}


/*
    InterProcessCommunication class implementation
    This class extends IMsgDataService to provide specific functionality for inter-process communication.
    It allows sending and receiving messages, and setting a target service for message forwarding.
*/
InterProcessCommunication::InterProcessCommunication(string name, IPC* ipc) : IMsgDataService(name, ipc) {
    type = IMsgDataServiceType::InterProcessCommunication;
    target = nullptr; // Initialize target to nullptr
}
bool InterProcessCommunication::Send(MessageData* msgData) {
    if(msgData == nullptr) {
        cout << "Error: Attempted to send a null message." << endl;
        return false;           
    }
    if(target == nullptr) {
        // Handle the case where target is not set
        cout << "Error: Target service is not set." << endl;
        return false;
    }
    // Send the message to the target service
    cout << "Sending message of type: " << static_cast<int>(msgData->GetMsgType()) 
         << " from " << static_cast<int>(msgData->GetMsgSrc()) 
         << " to " << static_cast<int>(msgData->GetMsgDest()) << endl;
    return target->Receive(msgData);

}
bool InterProcessCommunication::Receive(MessageData* msgData) {
    if(msgData == nullptr) {
        cout << "Error: Attempted to receive a null message." << endl;
        return false;
    }
    if(ipc != nullptr) {
        cout << "Receiving message of type: " << static_cast<int>(msgData->GetMsgType()) 
             << " from " << static_cast<int>(msgData->GetMsgSrc()) 
             << " to " << static_cast<int>(msgData->GetMsgDest()) << endl;
        ipc->EnQueue(msgData);
    } else {
        // Handle the case where IPC is not initialized
        return false;
    }
    return true;
}
void InterProcessCommunication::SetTarget(InterProcessCommunication* targetService) {
    target = targetService;
}
