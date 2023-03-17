#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <cstring>

void* wj_broadcastRecvThread(void* socket)
{
    int* l_sock = (int*)socket;

    socklen_t l_socketLength = sizeof(sockaddr_in);

    char l_recvBuff[1600] = {0};
    char l_sendBuff[] = "hello, I'm broadcast sender.";
    struct sockaddr_in l_addrFrom;
    int  l_recvCount = 1;

    while(1)
    {
        std::memset(l_recvBuff, 0, sizeof(l_recvBuff));
        int r_count = recvfrom(*l_sock, l_recvBuff, sizeof(l_recvBuff), 0, (struct sockaddr *)&l_addrFrom, &l_socketLength);
        if (r_count < 0)
        {
            printf("recv err\n");
        }

        printf("sender receive: %d, %s\n", l_recvCount, l_sendBuff);
        l_recvCount++;
    }

    return nullptr;
}


int main()
{
    int l_sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (l_sock == -1)
    {
        return -1;
    }

    socklen_t l_socketLength = sizeof(sockaddr_in);

   // 直接广播地址可以不绑定IP和端口，绑定了也可以
    // struct sockaddr_in l_addrTo;
    // l_addrTo.sin_family = AF_INET;
    // l_addrTo.sin_addr.s_addr= inet_addr("192.168.10.110"); //本机IP，不能是0.0.0.0
    // l_addrTo.sin_port=htons(8000);

    // if(bind(l_sock, (struct sockaddr*)&l_addrTo, l_socketLength) == -1)
    // {
    //     std::cout<< "bind error..." << std::endl;
    //     return -1;
    // }

    //广播设置，发送端必须设置该项
    const int ON = 1; // -1或1都可以，0是关闭
    setsockopt(l_sock, SOL_SOCKET, SO_BROADCAST, &ON, sizeof(ON));

    //开启广播发送响应接收线程，发送端发送广播数据后，接收端会发送一个接收响应
	pthread_t l_broadcastRecvThread_id = 0;
    pthread_create(&l_broadcastRecvThread_id, NULL, wj_broadcastRecvThread, (void*)&l_sock);

    //有限广播地址
    struct sockaddr_in l_SendAddr;
    l_SendAddr.sin_family = AF_INET;
    l_SendAddr.sin_port = htons(6060);
    // l_SendAddr.sin_addr.s_addr = INADDR_BROADCAST; 
    l_SendAddr.sin_addr.s_addr = inet_addr("192.168.10.255");//只能是直接广播地址，不能是有限广播地址

    char l_sendBuff[] = "hello, I'm broadcast sender.";

    char temp = 'i';

    // while (std::cin >> temp)
    while(1)
    {
        int t_count = sendto(l_sock, l_sendBuff, sizeof(l_sendBuff), 0, (struct sockaddr*)&l_SendAddr, l_socketLength);
        if (t_count < 0)
        {
            printf("send err\n");
        }
        else
        {
            printf("send ok\n");
        }
        usleep(1000*200);
    }

    return 0;
}