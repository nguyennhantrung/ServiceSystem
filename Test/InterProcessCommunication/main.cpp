#include <iostream>
#include <string>
#include <thread>
#include <chrono>

// #include "ipc.h"
#include "service.h"

#define DEBUG_LOG cout << __FILE__ << ":" << __LINE__ << " - " << __FUNCTION__ << std::endl;

using namespace std;
int main() {
    std::cout << "Start !!!" << std::endl;

    Service service1("Service_1");
    Service service2("Service_2");

    IMsgDataService* interProcessCommunication1 = service1.CreateCommunicate(service2.GetName());
    IMsgDataService* interProcessCommunication2 = service2.CreateCommunicate(service1.GetName());
    static_cast<InterProcessCommunication*>(interProcessCommunication1)->SetTarget(static_cast<InterProcessCommunication*>(interProcessCommunication2));
    static_cast<InterProcessCommunication*>(interProcessCommunication2)->SetTarget(static_cast<InterProcessCommunication*>(interProcessCommunication1));

    service1.Start();
    service2.Start();
    std::this_thread::sleep_for(std::chrono::seconds(1)); 

    string str1 = "this is message 1";
    MessageData msg1(MessageType::PrintString, (void*)(str1.c_str()), static_cast<int>(str1.length()), SystemId::Service1, SystemId::Service2);
    string str2 = "this is message 2";
    MessageData msg2(MessageType::PrintString, (void*)(str2.c_str()), static_cast<int>(str2.length()), SystemId::Service2, SystemId::Service1);

    service1.SendTo(service2.GetName(), &msg1);
    service2.SendTo(service1.GetName(), &msg2);
    std::this_thread::sleep_for(std::chrono::seconds(1)); 
    // Stop the services
    service1.Stop();
    service2.Stop();
    std::cout << "Stop  !!!" << std::endl;

    return 0;
}