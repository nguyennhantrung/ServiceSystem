#pragma once
#include <string>
#include <queue>
#include <functional>
#include <thread>
using namespace std;

enum class SystemId {
    Service1 = 0,
    Service2 = 1,
};
enum class MessageType {
    PrintString = 0,
};

class MessageData {
public:
    MessageData(MessageType msgType, void* msgData, int msgLength, SystemId msgSrc, SystemId msgDest);
    MessageType GetMsgType() const ;
    void* GetMsgData() const ;
    int GetMsgLength() const ;
    SystemId GetMsgSrc() const ;
    SystemId GetMsgDest() const ;
private:
    MessageType msgType;
    void* msgData;
    int msgLength;
    SystemId msgSrc;
    SystemId msgDest;
};

class IPC
{
public:
    IPC(std::function<bool(const MessageData*)> handler);
    bool EnQueue(MessageData* msgData);
    MessageData* DeQueue();
    bool Start();
    bool Stop();
private:
    std::function<bool(const MessageData*)> handler = nullptr;
    queue<MessageData*> queueIn;
    thread ipcThread; // Thread for IPC processing
    bool running = false; // Flag to control the IPC thread
};

enum class IMsgDataServiceType {
    InterProcessCommunication,
    Invalid,
};

class IMsgDataService {
public:
    IMsgDataService(string name, IPC* ipc);
    virtual ~IMsgDataService();
    virtual bool Send(MessageData* msgData);
    virtual bool Receive(MessageData* msgData);
protected:
    string name;
    IMsgDataServiceType type = IMsgDataServiceType::Invalid;
    IPC* ipc = nullptr; // IPC pointer for this service
};

class InterProcessCommunication : public IMsgDataService {
public:
    InterProcessCommunication(string name, IPC* ipc);
    bool Send(MessageData* msgData) override;
    bool Receive(MessageData* msgData) override;
    void SetTarget(InterProcessCommunication* targetService);
private:
    InterProcessCommunication *target;
};