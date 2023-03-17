#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <cstring>

void create_net_thread(void);

int main()
{
    create_net_thread();
    while(1)
    {
        usleep(100*1000);
    }
    return 0;
}

//单播通信接收端
void* wj_unicastRecvThread(void*)
{
    int l_sock = socket(AF_INET, SOCK_DGRAM, 0);
    if(l_sock < 0)
    {
        perror("socket");
        return nullptr;
    }

    socklen_t l_sockLength = sizeof(sockaddr_in);

    struct sockaddr_in l_localAddr;
    l_localAddr.sin_family = AF_INET;
    l_localAddr.sin_port = htons(49998);
    l_localAddr.sin_addr.s_addr = INADDR_ANY; //绑定本机地址0.0.0.0,一般，服务端需要绑定IP和端口

    if (bind(l_sock, (struct sockaddr *)&l_localAddr, l_sockLength) < 0)
    {
        close(l_sock);
        perror("bind");
        return nullptr;
    }

    char l_recvBuff[1600] = {0};
    char l_unicastResp[] = "hello, I,m unicast receiver.";
    struct sockaddr_in l_addrFrom;

    int recv_count = 1;

    while(1)
    {
        memset(l_recvBuff, 0, sizeof(l_recvBuff));
        int l_recvCount = recvfrom(l_sock, l_recvBuff, sizeof(l_recvBuff), 0, (struct sockaddr*)&l_addrFrom, &l_sockLength);
        if (l_recvCount <= 0)
        {
            perror("recvfrom() error: \n");
        }
        else
        {
            printf("server 49998 receive:%d, %s\n", l_recvCount, l_recvBuff);
            recv_count++;

            int l_sendCount = sendto(l_sock, l_unicastResp, sizeof(l_unicastResp), 0, (struct sockaddr*)&l_addrFrom, l_sockLength);
            if(l_sendCount <= 0)
            {
                perror("sendto() error: \n");
            }
        }
    }

    return nullptr;
}

//多波设置，有限广播地址设置255.255.255.255，广播通信方式主要是设置发送端
void* wj_broadcastRecvThread(void*)
{
    int l_sock = socket(AF_INET, SOCK_DGRAM, 0);
    if(l_sock < 0)
    {
        perror("socket");
        return nullptr;
    }

    socklen_t l_sockLength = sizeof(sockaddr_in);

    struct sockaddr_in l_localAddr;
    l_localAddr.sin_family = AF_INET;
    l_localAddr.sin_port = htons(6060); //绑定广播端口
    l_localAddr.sin_addr.s_addr = INADDR_BROADCAST; //绑定广播地址或者本机地址都可以，但端口必须是广播端口

    if (bind(l_sock, (struct sockaddr *)&l_localAddr, l_sockLength) < 0)
    {
        close(l_sock);
        perror("bind");
        return nullptr;
    }
    // 广播接收端不设置也可以
    // const int opt = 1;
    // int res = setsockopt(l_sock, SOL_SOCKET, SO_BROADCAST, (char*)&opt, sizeof(opt));

    char l_recvBuff[1600] = {0};
    char l_broadcasstResp[] = "hello, I,m broadcast receiver.";
    struct sockaddr_in l_addrFrom;

    int recv_count = 1;

    while(1)
    {
        memset(l_recvBuff, 0, sizeof(l_recvBuff));
        int l_recvCount = recvfrom(l_sock, l_recvBuff, sizeof(l_recvBuff), 0, (struct sockaddr*)&l_addrFrom, &l_sockLength);
        if (l_recvCount <= 0)
        {
            perror("recvfrom() error: \n");
        }
        else
        {
            printf("receiver receive: %d, %s\n", l_recvCount, l_recvBuff);
            recv_count++;

            int l_sendCount = sendto(l_sock, l_broadcasstResp, sizeof(l_broadcasstResp), 0, (struct sockaddr*)&l_addrFrom, l_sockLength);
            if(l_sendCount <= 0)
            {
                perror("sendto() error: \n");
            }
        }
    }

    return nullptr;
}

void create_net_thread(void)
{

	pthread_t l_unicastRecvThread_id = 0;
    pthread_create(&l_unicastRecvThread_id, NULL, wj_unicastRecvThread, nullptr);

	pthread_t l_broadcastRecvThread_id = 0;
    pthread_create(&l_broadcastRecvThread_id, NULL, wj_broadcastRecvThread, nullptr);
}