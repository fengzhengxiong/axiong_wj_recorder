#include <sys/socket.h>
#include <arpa/inet.h>
#include <iostream>
#include <unistd.h>
#include <cstring>

int main()
{
    int l_sock = socket(AF_INET, SOCK_DGRAM, 0);
    if(l_sock < 0)
    {
        std::cout << "socket create error" << std::endl;
        return -1;
    }

    socklen_t l_socketLength = sizeof(sockaddr_in);

    struct sockaddr_in l_localAddr;
    l_localAddr.sin_family = AF_INET;
    l_localAddr.sin_port = htons(8080); //绑定多播端口
    l_localAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(l_sock, (struct sockaddr *)&l_localAddr, l_socketLength) < 0)
    {
        close(l_sock);
        std::cout << "bind error" << std::endl;
        return -1;
    }

    struct ip_mreq mreq;
    mreq.imr_multiaddr.s_addr = inet_addr("225.1.1.100");//多播组IP
    mreq.imr_interface.s_addr = INADDR_ANY;//本机IP
    //设置socket选项，添加到组播组
    if(setsockopt(l_sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) < 0)
    {
        std::cout << "IP_ADD_MEMBERSHIP error" << std::endl;
        return -1;
    }

    char l_recvBuff[1600] = {0};
    char l_broadcasstResp[] = "hello, I,m multicast receiver.";
    struct sockaddr_in l_addrFrom;

    int l_recvCount = 1;

    while(1)
    {
        memset(l_recvBuff, 0, sizeof(l_recvBuff));
        int l_recvNum = recvfrom(l_sock, l_recvBuff, sizeof(l_recvBuff), 0, (struct sockaddr*)&l_addrFrom, &l_socketLength);
        if (l_recvNum <= 0)
        {
            std::cout << "recvfrom() error." << std::endl;
        }
        else
        {
            char l_ip[20];
            inet_ntop(AF_INET, &l_addrFrom.sin_addr.s_addr, l_ip, 20);
            
            std::cout << "receiver receive: " << l_recvCount << " ip:" << l_ip  << " port:" << (int)l_addrFrom.sin_port << std::endl;
            // std::cout << "receiver receive: " << l_recvCount << "  " << l_recvBuff  << "   ip:" << l_ip  << "   port:" << (int)l_addrFrom.sin_port << std::endl;
            l_recvCount++;

            // int l_sendCount = sendto(l_sock, l_broadcasstResp, sizeof(l_broadcasstResp), 0, (struct sockaddr*)&l_addrFrom, l_socketLength);
            // if(l_sendCount <= 0)
            // {
            //     std::cout << "sendto() error." << std::endl;
            // }
        }
    }
    return 0;
}


