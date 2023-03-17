# UDP组播和广播功能实现

***设定本机地址为 192.168.10.110，子网掩码 255.255.255.0***

## UDP组播

1.短暂的有效（重新开机后，此功能失效）

*设定组播地址为225.1.1.100,不能是224.0.0.1*。
*发送端和接收端都需要添加多播地址路由*，命令如下
*sudo route add -net 225.1.1.100 netmask 255.255.255.255 eth1*

*sudo route add -net 192.168.184.128 netmask 255.255.255.255 eth1*

2.永久有效

在/etc/rc.local里添加

*sudo route add -net 225.1.1.100 netmask 255.255.255.255 eth1*

### 发送端

（1）不需要绑定端口和IP
（2）不需要设置socket选项
（3）往多播地址发送数据就行



```c++
#include <sys/socket.h>
#include <arpa/inet.h>
#include <iostream>
#include <unistd.h>
#include <cstring>
#include <thread>
#include <pthread.h>
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
    system("sudo route add -net 192.168.10.1 netmask 255.255.255.255 ens33");
    int l_sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (l_sock == -1)
    {
        return -1;
    }

    socklen_t l_socketLength = sizeof(sockaddr_in);


    //开启多播发送响应接收线程，发送端发送多播数据后，接收端会发送一个响应
    std::thread muthread(wj_multicastRecvThread, (void*)&l_sock);

    //多播地址
    struct sockaddr_in l_SendAddr;
    l_SendAddr.sin_family = AF_INET;
    l_SendAddr.sin_port = htons(8080);
    l_SendAddr.sin_addr.s_addr = inet_addr("192.168.10.1");
    // l_SendAddr.sin_addr.s_addr = inet_addr("225.1.1.100");
    char l_sendBuff[] = "hello, I'm multicast sender.";


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
            std::cout << "send OK" << std::endl;
        }
        usleep(100);
    }

    muthread.join();

    return 0;
}


```


### 接收端

（1）绑定组播端口和本机IP
（2）设置socket选项IP_ADD_MEMBERSHIP，将自己添加到多播组中



```c++
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

    int reuse = 1;
    setsockopt(l_sock, SOL_SOCKET, SO_REUSEADDR, (void*)&reuse,sizeof(reuse));  //设置端口号复用，一台设备可连接多个接收端

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
    //mreq.imr_multiaddr.s_addr = inet_addr("225.1.1.100");//多播组IP
    mreq.imr_multiaddr.s_addr = inet_addr("192.168.10.1");//多播组IP
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



```


## UDP广播

参考：[百度百科](https://baike.baidu.com/item/%E5%B9%BF%E6%92%AD%E5%9C%B0%E5%9D%80/8614095?fr=aladdin)

广播地址应用于网络内的所有[主机](https://baike.baidu.com/item/主机)

（1）**受限**广播

它不被[路由](https://baike.baidu.com/item/路由)发送，但会被送到相同[物理网络](https://baike.baidu.com/item/物理网络)段上的所有主机

[IP地址](https://baike.baidu.com/item/IP地址)的网络字段和主机字**段**全为1就是地址**255.255.255.255**

（2）**直接**广播

网络广播会被路由，并会发送到专门网络上的每台主机

IP地址的网络字段定义这个网络，主机字**段**通常全为1，如 192.168.10.**255**

### 直接广播地址 192.168.10.255

#### 发送端

（1）*发送端不需要绑定IP和端口;*
（2）*发送地址设置直接广播端口;*
（3）*发送地址设置IP为直接广播地址*



```c++
int main()
{
    int l_sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (l_sock == -1)
    {
        return -1;
    }
socklen_t l_socketLength = sizeof(sockaddr_in);

//广播设置，发送端必须设置该项
const int ON = 1; // -1或1都可以，0是关闭
setsockopt(l_sock, SOL_SOCKET, SO_BROADCAST, &ON, sizeof(ON));

//受限广播地址
struct sockaddr_in l_SendAddr;
l_SendAddr.sin_family = AF_INET;
l_SendAddr.sin_port = htons(6060);
l_SendAddr.sin_addr.s_addr = inet_addr("192.168.10.255");//只能是直接广播地址，不能是受限广播地址

char l_sendBuff[] = "hello, I'm broadcast sender.";

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
```


#### 接收端

（1）*接收端必须要绑定直接广播端口;*
（2）*IP不能设置为受限广播地址255.255.*255.255,可以设置为本机IP或0.0.0.0



```c++
int l_sock = socket(AF_INET, SOCK_DGRAM, 0);

if(l_sock < 0)

{

	perror("socket");

	return -1;

}
socklen_t l_sockLength = sizeof(sockaddr_in);

struct sockaddr_in l_localAddr;
l_localAddr.sin_family = AF_INET;
l_localAddr.sin_port = htons(6060); //绑定广播端口
l_localAddr.sin_addr.s_addr = INADDR_ANY; //直接广播通信只能绑定本机地址
if (bind(l_sock, (struct sockaddr *)&l_localAddr, l_sockLength) < 0)
{
    close(l_sock);
    perror("bind");
    return nullptr;
}

char l_recvBuff[1600] = {0};
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
}
```

### 受限广播地址 255.255.255.255

#### 发送端

（1）发送端必须绑定IP和任意端口（不能是广播端口）;
（2）发送地址设置受限广播端口;
（3）发送地址设置IP为受限广播地址*

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <cstring>

int main()
{
    int l_sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (l_sock == -1)
    {
        return -1;
    }

    socklen_t l_socketLength = sizeof(sockaddr_in);
    
    struct sockaddr_in l_addrTo;
    l_addrTo.sin_family = AF_INET;
    l_addrTo.sin_addr.s_addr= inet_addr("192.168.10.110"); //本机IP，不能是0.0.0.0
    l_addrTo.sin_port=htons(8000);
    
    //受限广播地址255.255.255.255通信，需要明确绑定端口和本机IP;直接广播地址就不需要
    if(bind(l_sock, (struct sockaddr*)&l_addrTo, l_socketLength) == -1)
    {
        std::cout<< "bind error..." << std::endl;
        return -1;
    }
    
    //广播设置，发送端必须设置该项
    const int ON = 1;
    setsockopt(l_sock, SOL_SOCKET, SO_BROADCAST, &ON, sizeof(ON));
    
    //受限广播地址
    struct sockaddr_in l_SendAddr;
    l_SendAddr.sin_family = AF_INET;
    l_SendAddr.sin_port = htons(6060);
    l_SendAddr.sin_addr.s_addr = INADDR_BROADCAST; //255.255.255.255
    
    char l_sendBuff[] = "hello, I'm broadcast sender.";
    
    while(1)
    {
        int t_count = sendto(l_sock, l_sendBuff, sizeof(l_sendBuff), 0, (struct sockaddr*)&l_SendAddr, l_socketLength);
        if (t_count < 0)
        {
            printf("send err\n");
        }
        usleep(1000*200);
    }
    
    return 0;
}

#### 接收端

（1）接收端必须要绑定受限广播端口;
（2）IP可以设置为受限广播地址255.255.255.255,也可以设置为本机IP或0.0.0.0

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
    }
    
    return nullptr;

# mems 处理广播demo

```c
static void ip_check_udp_data_handle(struct sys_info  *sys_handle, uint8_t *cmd_data, uint32_t data_size)
{
    int l_sock = socket(AF_INET, SOCK_DGRAM, 0);
    if(l_sock < 0)
    {
        LogPrint(LOG_LEVEL_ERR, LOG_MODULE_NAME, "broadcast socket create fail.");
        return;
    }    
    //bind失败的原因 
    struct sockaddr_in localaddr;
    localaddr.sin_family = AF_INET;
    localaddr.sin_port = htons(6060);
    localaddr.sin_addr.s_addr = INADDR_BROADCAST;
    socklen_t l_socketLength = sizeof(struct sockaddr_in);
    unsigned int value = 1;
    setsockopt(l_sock,SOL_SOCKET,SO_REUSEADDR,(void *)&value,sizeof(value));

    if (bind(l_sock, (struct sockaddr*)&localaddr, sizeof(localaddr)) < 0)
    {
        close(l_sock);
        perror("bind error:");
        LogPrint(LOG_LEVEL_ERR, LOG_MODULE_NAME, "broadcast socket bind fail.");
        exit -1;
    }
    uint8_t l_recvBuff[1600] = {0};
    uint8_t l_txBuff[1600]   = {0};
    int l_len = sizeof(localaddr);
    while(1)
    {
        memset(l_recvBuff, 0, sizeof(l_recvBuff));
        if(recvfrom(l_sock, cmd_data, sizeof(l_recvBuff), 0, (struct sockaddr*)&localaddr, &l_len) <= 0)
        {
            LogPrint(LOG_LEVEL_ERR, LOG_MODULE_NAME, "broadcast receive error -> receive length <= 0.");
        }else{       
    		// 逻辑处理
            }
        }
    }



}
```

