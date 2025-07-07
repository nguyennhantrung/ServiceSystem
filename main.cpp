#include <iostream>
#include <string>

#include "IPC/lib/ipc.h"
#include "Service/lib/service.h"

using namespace std;
void main() {
    std::cout << "Start !!!" << std::endl;

    Service service1("Service_1");
    Service service2("Service_2");

    service1.CreateCommunicate(&service2);
    service2.CreateCommunicate(&service1);

    string str1 = "this is message 1";
    MessageData msg1(1, &str1, str1.length(), 1, 2);
    string str2 = "this is message 2";
    MessageData msg2(2, &str2, str2.length(), 2, 1);

    service1.SendTo(service2, &msg1);
    service2.SendTo(service1, &msg2);








    std::cout << "Stop  !!!" << std::endl;
}