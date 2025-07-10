Event-Drive:
    System:
        bool EventHandler(msgData);
        IPC ipc(EventHandler)
        Map<name, MsgDataService>MsgDataServices 
        bool SendTo(name, data)
            search MsgDataServices => msgDataService->send()

    IPC(EventHandler):
        queue_in
        bool Start()
            thread_in(EventHandler)

    MsgDataServiceInterface:
        Encode(msgData)
        Send(msgData)
        Decode(msgData)
        Receive(msgData)
    
    IPC :: MsgDataServiceInterface

