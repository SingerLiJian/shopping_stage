#ifndef _SERVERACCEPT_
#define _SERVERACCEPT_
#include <sys/socket.h>
#include <netinet/in.h>
#include <memory.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
using namespace std;

class serverAccept
{
private:
	//服务器的socket号
	int nListenSocket;
    int m_nServerPort;
    char* m_strBoundIP;
	//接听的数量上限
    int m_nLengthOfQueueOfListen;
	//第一个连接号
    int nConnectedSocket;
public:
	//通过给的端口号与ip实现实例的生成
    serverAccept(int nServerPort, int nLengthOfQueueOfListen = 100, const char *strBoundIP = NULL){
        m_nServerPort = nServerPort;
	    m_nLengthOfQueueOfListen = nLengthOfQueueOfListen;
        if(NULL == strBoundIP)
	    {
	        m_strBoundIP = NULL;
	    }
	    else
	    {
	        int length = strlen(strBoundIP);
	        m_strBoundIP = new char[length + 1];
	        memcpy(m_strBoundIP, strBoundIP, length + 1);
	    }
    }

    ~serverAccept(){
        if(m_strBoundIP != NULL)
	    {
	        delete [] m_strBoundIP;
	    }
    }
	//使用listen使得端口为被动接听状态
    int Listen(){
        nListenSocket = ::socket(AF_INET, SOCK_STREAM, 0);
	    if(-1 == nListenSocket)
	    {
	        std::cout << "socket error" << std::endl;
	        return -1;
	    }

	    sockaddr_in ServerAddress;
	    memset(&ServerAddress, 0, sizeof(sockaddr_in));
	    ServerAddress.sin_family = AF_INET;

	    if(NULL == m_strBoundIP)
	    {
	        ServerAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	    }
	    else
	    {
	        if(::inet_pton(AF_INET, m_strBoundIP, &ServerAddress.sin_addr) != 1)
	        {
		    std::cout << "inet_pton error" << std::endl;
		    ::close(nListenSocket);
		    return -1;
	        }
	    }

	    ServerAddress.sin_port = htons(m_nServerPort);
		//绑定socket
	    if(::bind(nListenSocket, (sockaddr *)&ServerAddress, sizeof(sockaddr_in)) == -1)
	    {
	        std::cout << "bind error" << std::endl;
	        ::close(nListenSocket);
	        return -1;
	    }
		//listen状态
	    if(::listen(nListenSocket, m_nLengthOfQueueOfListen) == -1)
	    {
	        std::cout << "listen error" << std::endl;
	        ::close(nListenSocket);
	        return -1;
	    }
    }
	//取出一个连接
	int Accept()
	{
		sockaddr_in ClientAddress;
	    socklen_t LengthOfClientAddress = sizeof(sockaddr_in);
	    nConnectedSocket = ::accept(nListenSocket, (sockaddr *)&ClientAddress, &LengthOfClientAddress);
	    if(-1 == nConnectedSocket)
	    {
	        std::cout << "accept error" << std::endl;
	        ::close(nListenSocket);
	        return -1;
	    }
		return nConnectedSocket;
	}
	//服务器读
    char* ServerRead(int cs){
        char *buf = new char [100];
	    ::read(cs, buf, 100);
		return buf;
    }
	//服务器写
    void ServerWrite(const char *buf,int cs){
        ::write(cs, buf, 100);
   }

	void closeServer(){
		close(nConnectedSocket);
	}
};

#endif
