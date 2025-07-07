#pragma once
#include <string>
#include <queue>
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
    IPC(string name) {
        this->name = name;
    }
    bool EnQueue(MessageData* msgData) {
        if(msgData == nullptr) {
            return false;
        }
        queueOut.push(msgData);
        return true;
    }

    MessageData* DeQueue() {
        if(queueIn.empty()) return nullptr;
        MessageData* msgData = queueIn.front();
        queueIn.pop();
        return msgData;
    }

    virtual bool EventHandler(MessageData* msgData) {
        return true;
    }
private:
    string name;
    queue<MessageData*> queueIn;
    queue<MessageData*> queueOut;
};




enum class IpcType {
    DEFAULT
};

IPC* CreateIPC(string name = "", IpcType type = IpcType::DEFAULT) {
    switch (type) {
        case IpcType::DEFAULT:
        default:
            return new IPC(name);
    }
}
