#include <sys/socket.h>
#include <arpa/inet.h>
#include <iostream>
#include <unistd.h>
#include <cstring>
#include <thread>

void wj_multicastRecvThread(void* socket)
{
    int* l_sock = (int*)socket;

    socklen_t l_socketLength = sizeof(sockaddr_in);
    struct sockaddr_in l_addrFrom;
    char l_recvBuff[1600] = {0};
    int  l_recvCount = 1;

    while(1)
    {
        std::memset(l_recvBuff, 0, sizeof(l_recvBuff));
        int r_count = recvfrom(*l_sock, l_recvBuff, sizeof(l_recvBuff), 0, (struct sockaddr *)&l_addrFrom, &l_socketLength);
        if (r_count < 0)
        {
            std::cout << "recv err." << std::endl;
        }

        std::cout << "sender receive: " << l_recvCount << "  " << l_recvBuff << std::endl;
        l_recvCount++;
    }
}


int main()
{
    int l_sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (l_sock == -1)
    {
        return -1;
    }

    socklen_t l_socketLength = sizeof(sockaddr_in);

    // 发送端可以绑定，也可以不绑定
    // struct sockaddr_in l_localAddr;
    // l_localAddr.sin_family = AF_INET;
    // l_localAddr.sin_port = htons(8000);
    // l_localAddr.sin_addr.s_addr = INADDR_ANY;

    // if (bind(l_sock, (struct sockaddr *)&l_localAddr, l_socketLength) < 0)
    // {
    //     close(l_sock);
    //     std::cout << "bind error" << std::endl;
    //     return -1;
    // }

    //开启多播发送响应接收线程，发送端发送多播数据后，接收端会发送一个响应
    std::thread muthread(wj_multicastRecvThread, (void*)&l_sock);

    //多播地址
    struct sockaddr_in l_SendAddr;
    l_SendAddr.sin_family = AF_INET;
    l_SendAddr.sin_port = htons(6060);
    l_SendAddr.sin_addr.s_addr = inet_addr("225.1.1.100");

    char l_sendBuff[] = "hello, I'm multicast sender.";

#if 0
    //单播地址
    struct sockaddr_in l_SendAddr2;
    l_SendAddr2.sin_family = AF_INET;
    l_SendAddr2.sin_port = htons(6060);
    l_SendAddr2.sin_addr.s_addr = inet_addr("192.168.10.100");
#endif

    char l_sendBuff2[] = "hello, I'm unicast sender.";

    while(1)
    {
        //往广播地址发送数据
        int t_count = sendto(l_sock, l_sendBuff, sizeof(l_sendBuff), 0, (struct sockaddr*)&l_SendAddr, l_socketLength);
        if (t_count < 0)
        {
            std::cout << "multicast send err" << std::endl;
        }
        else
        {
            // std::cout << "send OK" << std::endl;
        }
        usleep(1000*200);

        // 往单播地址发送数据
        // int t_count2 = sendto(l_sock, l_sendBuff2, sizeof(l_sendBuff2), 0, (struct sockaddr*)&l_SendAddr2, l_socketLength);
        // if (t_count2 < 0)
        // {
        //     std::cout << "unicast send err" << std::endl;
        // }
        // else
        // {
        //     // std::cout << "send OK" << std::endl;
        // }
        // usleep(1000*200);
    }

    muthread.join();

    return 0;
}