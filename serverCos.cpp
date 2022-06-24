#include <iostream>
#include <vector>
#include "wines.h"
#include "serverAccept.h"
#include "cosmetics.h"
#include "serializerForItems.h"
#include "clientConnect.h"
#include "Thread.h"
using namespace std;


int main(){
    //测试
    /* serverAccept *serverC = new serverAccept(4000, 13 , "127.0.0.1");
    serverC->Accept();
    while (1)
    {
        serverC->ServerRead();
        serverC->ServerWrite("in serverCos");
    } */
    char *opt;
    //初始化
    serverAccept *serverC = new serverAccept(4000, 13 , "127.0.0.1");
    //realCos为化妆品实例。存储着商品与数量。
    cosmetics *realCos = new cosmetics(serverC);
    //tempWin用于供服务器B异常时使用
    wines *tempWin = new wines(serverC);
    //记录化妆品类与客户端的连接
    int ConnectedSocket1;
    //备用酒类与客户端的连接
    int ConnectedSocket2;
    //当异常发生时，flag=1；
    int flag=0;
    //初始化
    realCos->addItem("lipstick",10);
    realCos->addItem("mask",10);
    serverC->Listen();
    //化妆品类打开连接
    ConnectedSocket1 = serverC->Accept();

    try
    {
        //异常未发生
        while (flag == 0)
        {
            //opt用于接受指令
            char *opt = serverC->ServerRead(ConnectedSocket1);
            //抛出异常测试用
            throw 1;
            //收到查看商品的指令
            if (opt[0]=='1')
            {
                serverC->ServerWrite(SerializerForItems::Serialize(realCos->getVector()),ConnectedSocket1);
            }
            //收到购物指令
            else if (opt[0]=='2')
            {
                char *buf;
                vector <tradeThing> purchase;
                buf = serverC->ServerRead(ConnectedSocket1);
                SerializerForItems::Deserialize(buf, purchase);
                realCos->expense(purchase);
            }
            //收到客户端的异常提示
            else if(opt[0]=='0')
            {
                //接受另一服务器的连接并接受它的商品信息
                ConnectedSocket2 = serverC->Accept();
                char *buf = serverC->ServerRead(ConnectedSocket2);
                //反序列化并存储在备用的tempWin中
                SerializerForItems::Deserialize(buf, tempWin->things);
                flag = 1;
            }
        }
    }
    catch(const int e)
    {
        //异常发生时，尝试先把信息发送给另一服务器
        serverC->closeServer();
        clientConnect *wrongCient = new clientConnect(5000,"127.0.0.1");
        wrongCient->Connect();
        wrongCient->ClientWrite(SerializerForItems::Serialize(realCos->getVector()));
        exit(0);
    }
    //将信息存储于结构体中供多线程使用
    data *dataS = new data();
    dataS->ConnectedSocket1 = ConnectedSocket1;
    dataS->ConnectedSocket2 = ConnectedSocket2;
    dataS->serverC = serverC;
    dataS->realCos = realCos;
    dataS->tempWin = tempWin;
    data *dataW = new data();
    dataW->ConnectedSocket1 = ConnectedSocket1;
    dataW->ConnectedSocket2 = ConnectedSocket2;
    dataW->serverC = serverC;
    dataW->realCos = realCos;
    dataW->tempWin = tempWin;
    Thread thread1;
    Thread thread2;

    //因为每次只有一个连接工作，所以无需互斥
    thread1.no3(dataS);
    thread2.no4(dataW);
    thread1.WaitForDeath();
    thread2.WaitForDeath();
}