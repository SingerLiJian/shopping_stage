#ifndef THREAD_H
#define THREAD_H
#include <pthread.h>
#include <iostream>
#include <vector>
#include "serverAccept.h"
#include "clientConnect.h"
#include "wines.h"
#include "cosmetics.h"
#include "serializerForItems.h"
using namespace std;

struct data
{
    int ConnectedSocket1;
    int ConnectedSocket2;
	serverAccept *serverC;
	wines *realWin;
	cosmetics *tempCos;
	wines *tempWin;
	cosmetics *realCos;
};

class Thread
{
public:
	Thread();
	~Thread();
	int no1(void *ft);
    int no2(void *ft);
	int no3(void *ft);
	int no4(void *ft);
	int WaitForDeath();
	int ForDeath();

private:
    static void* StartFunctionOfno1(void *ft);
    static void* StartFunctionOfno2(void *ft);
	static void* StartFunctionOfno3(void *ft);
	static void* StartFunctionOfno4(void *ft);

private:
	//线程号
	pthread_t m_ThreadID; 
};

Thread::Thread()
{
}

Thread::~Thread()
{
}
//服务器B的酒销售线程入口
int Thread::no1(void *ft)
{	
	int r = pthread_create(&m_ThreadID, 0, StartFunctionOfno1, ft);
	if(r != 0)
	{
		return -1;
	}
	return 0;
}
//服务器B的化妆品销售线程入口
int Thread::no2(void *ft)
{	
	int r = pthread_create(&m_ThreadID, 0, StartFunctionOfno2, ft);
	if(r != 0)
	{
		return -1;
	}
	return 0;
}
//服务器A的化妆品销售线程入口
int Thread::no3(void *ft)
{	
	int r = pthread_create(&m_ThreadID, 0, StartFunctionOfno1, ft);
	if(r != 0)
	{
		return -1;
	}
	return 0;
}
//服务器A的酒销售线程入口
int Thread::no4(void *ft)
{	
	int r = pthread_create(&m_ThreadID, 0, StartFunctionOfno2, ft);
	if(r != 0)
	{
		return -1;
	}
	return 0;
}
//服务器B的酒销售线程
void* Thread::StartFunctionOfno1(void *ft){
    data *dataS = (data*)ft;
	while (1)
	{
		char *opt = dataS->serverC->ServerRead(dataS->ConnectedSocket1);
		//收到查看商品的指令
		if (opt[0]=='1')
		{
			dataS->serverC->ServerWrite(SerializerForItems::Serialize(dataS->realWin->getVector()),dataS->ConnectedSocket1);
		}
		else if (opt[0]=='2')
		{
			char *buf;
			vector <tradeThing> purchase;
			buf = dataS->serverC->ServerRead(dataS->ConnectedSocket1);
			SerializerForItems::Deserialize(buf, purchase);
			dataS->realWin->expense(purchase);
		}
		sleep(1);
	}
}
//服务器B的化妆品销售线程
void* Thread::StartFunctionOfno2(void *ft)
{
	data *dataS = (data*)ft;
	while (1)
	{
		char *opt = dataS->serverC->ServerRead(dataS->ConnectedSocket2);
		if (opt[0]=='1')
		{
			dataS->serverC->ServerWrite(SerializerForItems::Serialize(dataS->tempCos->getVector()),dataS->ConnectedSocket2);
		}
		else if (opt[0]=='2')
		{
			char *buf;
			vector <tradeThing> purchase;
			buf = dataS->serverC->ServerRead(dataS->ConnectedSocket2);
			SerializerForItems::Deserialize(buf, purchase);
			dataS->tempCos->expense(purchase);
		}
		sleep(1);
	}
}
//服务器A的化妆品销售线程
void* Thread::StartFunctionOfno3(void *ft){
    data *dataS = (data*)ft;
	while (1)
	{
		char *opt = dataS->serverC->ServerRead(dataS->ConnectedSocket1);
		//收到查看商品的指令
		if (opt[0]=='1')
		{
			dataS->serverC->ServerWrite(SerializerForItems::Serialize(dataS->realCos->getVector()),dataS->ConnectedSocket1);
		}
		else if (opt[0]=='2')
		{
			char *buf;
			vector <tradeThing> purchase;
			buf = dataS->serverC->ServerRead(dataS->ConnectedSocket1);
			SerializerForItems::Deserialize(buf, purchase);
			dataS->realCos->expense(purchase);
		}
		sleep(1);
	}
}
//服务器A的酒销售线程
void* Thread::StartFunctionOfno4(void *ft)
{
	data *dataS = (data*)ft;
	while (1)
	{
		char *opt = dataS->serverC->ServerRead(dataS->ConnectedSocket2);
		if (opt[0]=='1')
		{
			dataS->serverC->ServerWrite(SerializerForItems::Serialize(dataS->tempWin->getVector()),dataS->ConnectedSocket2);
		}
		else if (opt[0]=='2')
		{
			char *buf;
			vector <tradeThing> purchase;
			buf = dataS->serverC->ServerRead(dataS->ConnectedSocket2);
			SerializerForItems::Deserialize(buf, purchase);
			dataS->tempWin->expense(purchase);
		}
		sleep(1);
	}
}

int Thread::WaitForDeath()
{
	int r = pthread_join(m_ThreadID, 0);
	if(r != 0)
	{
		return -1;
	}
	return 0;
}

int Thread::ForDeath()
{
	int r= pthread_detach(m_ThreadID);
	if(r != 0)
	{
		return -1;
	}
	return 0;
}
#endif
