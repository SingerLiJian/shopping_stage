#ifndef _CLIENTCONNECT_
#define _CLIENTCONNECT_
#include <sys/socket.h>
#include <netinet/in.h>
#include <memory.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
using namespace std;

class clientConnect
{
private:
    //服务器端口  
    int m_nServerPort;
    //服务器ip
    char *m_strServerIP;
    //连接的file号
    int nConnectedSocket;
public:
    //通过给的端口号与ip实现实例的生成
    clientConnect(int nServerPort, const char *strServerIP)
    {
        m_nServerPort = nServerPort;
	    int nlength = strlen(strServerIP);
	    m_strServerIP = new char [nlength + 1];
	    strcpy(m_strServerIP, strServerIP);
        int nClientSocket = ::socket(AF_INET, SOCK_STREAM, 0);
        nConnectedSocket = nClientSocket;
	    if(-1 == nClientSocket)
	    {
	        std::cout << "socket error" << std::endl;
	    }
    }

    ~clientConnect()
    {
        close(nConnectedSocket);
    }
    //主动向服务器发起连接
    int Connect(){
        sockaddr_in ServerAddress;
	    memset(&ServerAddress, 0, sizeof(sockaddr_in));
	    ServerAddress.sin_family = AF_INET;
	    if(::inet_pton(AF_INET, m_strServerIP, &ServerAddress.sin_addr) != 1)
	    {
	        std::cout << "inet_pton error" << std::endl;
	        ::close(nConnectedSocket);
	        return -1;
	    }
        ServerAddress.sin_port = htons(m_nServerPort);
        if(::connect(nConnectedSocket, (sockaddr*)&ServerAddress, sizeof(ServerAddress)) == -1)
	    {
	        std::cout << "connect error" << std::endl;
	        ::close(nConnectedSocket);
	        return -1;
	    }
    }
    //客户端读
    char* ClientRead(){
        char *buf = new char [100];
	    ::read(nConnectedSocket, buf, 100);
	    return buf;
    }
    //客户端写
    void ClientWrite(const char *buf){
        ::write(nConnectedSocket, buf, 100);
    }
};
#endif