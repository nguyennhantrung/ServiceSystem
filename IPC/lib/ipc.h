#pragma once
#include <string>
#include <queue>
#include <functional>
#include <thread>
using namespace std;

class MessageData {
public:
    MessageData(int msgType, void* msgData, int msgLength, int msgSrc, int msgDest) {
        this->msgType = msgType;
        this->msgData = msgData;
        this->msgLength = msgLength;
        this->msgSrc = msgSrc;
        this->msgDest = msgDest;
    }
private:
    int msgType;
    void* msgData;
    int msgLength;
    int msgSrc;
    int msgDest;
};

class IPC
{
public:
    IPC(std::function<bool(const MessageData*)> handler) {
        this->handler = handler;
    }
    bool EnQueue(MessageData* msgData) {
        if(msgData == nullptr) {
            return false;
        }
        queueIn.push(msgData);
        return true;
    }
    MessageData* DeQueue() {
        if(queueIn.empty()) return nullptr;
        MessageData* msgData = queueIn.front();
        queueIn.pop();
        return msgData;
    }
    bool Start() {
        running = true;
        // Start the IPC, initialize threads, etc.
        // This is a placeholder for actual implementation
        ipcThread = std::thread([this]() {
            while (running) {
                MessageData* msgData = DeQueue();
                if (msgData != nullptr) {
                    handler(msgData);
                    delete msgData; // Clean up after processing
                }
            }
        });
        if (ipcThread.joinable()) {
            ipcThread.join(); // Wait for the thread to finish
        }
        return true;
    }
    bool Stop() {
        running = false;
        if (ipcThread.joinable()) {
            ipcThread.join(); // Wait for the thread to finish
        }
        return true;
    }
private:
    std::function<bool(const MessageData*)> handler = nullptr;
    queue<MessageData*> queueIn;
    thread ipcThread; // Thread for IPC processing
    bool running = false; // Flag to control the IPC thread
};

enum class IMsgDataServiceType {
    InterProcessCommunication,
};

class IMsgDataService {
public:
    IMsgDataService(string name, IPC* ipc) : name(name), ipc(ipc) {}
    virtual bool Send(MessageData* msgData) {
        return false; // Default implementation, can be overridden
    }
    virtual bool Receive(MessageData* msgData) {
        return false; // Default implementation, can be overridden
    }
protected:
    string name;
    IPC* ipc = nullptr; // IPC pointer for this service
};

class InterProcessCommunication : public IMsgDataService {
public:
    InterProcessCommunication(string name, IPC* ipc) : IMsgDataService(name, ipc) {}
    bool Send(MessageData* msgData) override {
        if(msgData == nullptr) {
            return false;           
        }
        if(target == nullptr) {
            // Handle the case where target is not set
            return false;
        }
        // Send the message to the target service
        return target->Receive(msgData);

    }
    bool Receive(MessageData* msgData) override {
        if(msgData == nullptr) {
            return false;
        }
        if(ipc != nullptr) {
            ipc->EnQueue(msgData);
        } else {
            // Handle the case where IPC is not initialized
            return false;
        }
        return true;
    }
    void SetTarget(InterProcessCommunication* targetService) {
        target = targetService;
    }
private:
    InterProcessCommunication *target;
};