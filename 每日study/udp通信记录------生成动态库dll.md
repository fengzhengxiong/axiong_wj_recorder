udp通信------生成动态库dll



udp优点：

通讯快但不可靠，没有握手通话，所以没有办法解决丢包重发错序问题



------------------------------------------------------------------------------------------------------------------------------------------------------

发送端（客户端）代码：

#include <stdio.h>
#include <Ws2tcpip.h>
#include <winsock2.h>
#pragma comment(lib,"ws2_32.lib")
using namespace std;
int main()
{
	WSADATA wsaData;//初始化
	//初始化Socket
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	sockaddr_in RecvAddr;         //服务器地址
	sockaddr_in SenderAddr;       //本地地址
	int localPort = 4001;         //本地监听端口
	int Port = 4000;              //服务器监听端口
	
	char SendBuf[1024];           //发送数据的缓冲区
	int BufLen = 1024;            //缓冲区大小
	//创建发送数据包的Socket对象
	SOCKET SendSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	//设置服务器地址
	RecvAddr.sin_family = AF_INET;
	RecvAddr.sin_port = htons(Port);
	inet_pton(AF_INET, "127.0.0.1", &RecvAddr.sin_addr);
	
	//绑定本地监听地址
	SenderAddr.sin_family = AF_INET;
	SenderAddr.sin_port = htons(localPort);
	SenderAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	bind(SendSocket, (SOCKADDR*)&SenderAddr, sizeof(SenderAddr));
	
	int l_naddLen1 = sizeof(SenderAddr);
	printf("客户端发送：\n");
	while (1)
	{
		strcpy_s(SendBuf, "1234");
		int l_nLen = sendto(SendSocket, SendBuf, strlen(SendBuf), 0, (SOCKADDR*)&RecvAddr, sizeof(RecvAddr));
		if (l_nLen < 0)
		{
			perror("发送失败");
			exit(1);
		}
	
		printf("\nSend：");
		for (int i = 0; i < strlen(SendBuf); i++)
		{
			printf("%02x ", (UCHAR)SendBuf[i]);
		}
		int l_nReadLen = recvfrom(SendSocket, SendBuf, BufLen, 0, (struct sockaddr*)&SenderAddr, &l_naddLen1);
		printf("\nread：");
		for (int i = 0; i < l_nReadLen; i++)
		{
			printf("%02x ", SendBuf[i]);
		}
		Sleep(1000);
	
	}
	//发送完成，关闭Socket
	closesocket(SendSocket);
	WSACleanup();
	return 0;
}



-----------------------------------------------------------------------------------------------------------------------------------------------------

接收端（服务端）代码：

#include<WINSOCK2.H>
#include<iostream>
#pragma comment(lib,"WS2_32.lib")
using namespace std;
int main()
{
	WSADATA wsaData;//初始化
	//初始化Socket
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	sockaddr_in RecvAddr;        //本地地址
	sockaddr_in SenderAddr;      //服务器地址
	int Port = 4000;             //本地监听地址
	
	char RecvBuf[1024];//发送数据的缓冲区
	int BufLen = 1024;//缓冲区大小
	//创建接收数据报的socket对象
	SOCKET RecvSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	//绑定本地监听地址
	RecvAddr.sin_family = AF_INET;
	RecvAddr.sin_port = htons(Port);
	RecvAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	bind(RecvSocket, (SOCKADDR*)&RecvAddr, sizeof(RecvAddr));
	
	int SenderAddrSize = sizeof(SenderAddr);
	printf("服务器端接收：\n");
	while (1)
	{
		strcpy_s(RecvBuf, "4321");
		int l_nLen = recvfrom(RecvSocket, RecvBuf, BufLen, 0, (SOCKADDR*)&SenderAddr, &SenderAddrSize);
	
		if (l_nLen < 0)
		{
			perror("接收失败");
			exit(1);
		}
	
		printf("\nSend：");
		for (int i = 0; i < strlen(RecvBuf); i++)
		{
			printf("%02x ", (UCHAR)RecvBuf[i]);
		}
		int l_nReadLen = sendto(RecvSocket, RecvBuf, strlen(RecvBuf), 0, (SOCKADDR*)&SenderAddr, SenderAddrSize);
		printf("\nread：");
		for (int i = 0; i < l_nReadLen; i++)
		{
			printf("%02x ", RecvBuf[i]);
		}
		Sleep(1000);
	
	}
	//调用Recvfrom函数在绑定的socket上接收数据
	//关闭socket，结束接收数据
	closesocket(RecvSocket);
	//释放资源，退出
	WSACleanup();
	return 0;
}



------------------------------------------------------------------------------------------------------------------------------------------------------

生成dll动态库：

client_dll:参考下边的博客  :

[(59条消息) VS2019环境下C++动态链接库（DLL）的创建与调用_Aishen1024的博客-CSDN博客](https://blog.csdn.net/qq_30139555/article/details/103621955)
https://blog.csdn.net/qq_30139555/article/details/103621955




