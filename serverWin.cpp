#include <iostream>
#include <vector>
#include "serverAccept.h"
#include "clientConnect.h"
#include "wines.h"
#include "cosmetics.h"
#include "serializerForItems.h"
#include "Thread.h"
using namespace std;
//无需过多注释，于serverCos.cpp大致相同

int main(){
    //测试
    /* serverAccept *serverW = new serverAccept(5000, 13 , "127.0.0.1");
    serverW->Accept();
    while (1)
    {
        serverW->ServerRead();
        serverW->ServerWrite("in serverWin");
    } */
    serverAccept *serverC = new serverAccept(5000, 13 , "127.0.0.1");
    wines *realWin = new wines(serverC);
    //准备好备用
    cosmetics *tempCos = new cosmetics(serverC);
    int ConnectedSocket1;
    //存储额外的因服务器A异常产生的套接字
    int ConnectedSocket2;
    int flag = 0;
    realWin->addItem("redwine",10);
    realWin->addItem("beer",10);
    serverC->Listen();
    ConnectedSocket1 = serverC->Accept();

    try
    {
        while (flag ==0)
        {
            char *opt = serverC->ServerRead(ConnectedSocket1);
            //收到查看商品的指令
            if (opt[0]=='1')
            {
                serverC->ServerWrite(SerializerForItems::Serialize(realWin->getVector()),ConnectedSocket1);
            }
            else if (opt[0]=='2')
            {
                char *buf;
                vector <tradeThing> purchase;
                buf = serverC->ServerRead(ConnectedSocket1);
                SerializerForItems::Deserialize(buf, purchase);
                realWin->expense(purchase);
            }
            else if(opt[0]=='0')
            {
                ConnectedSocket2 = serverC->Accept();
                char *buf = serverC->ServerRead(ConnectedSocket2);
                SerializerForItems::Deserialize(buf,tempCos->things);
                ConnectedSocket2 = serverC->Accept();
                flag = 1;
            }
        }
    }
    catch(const int e)
    {
        serverC->closeServer();
        clientConnect *wrongCient = new clientConnect(4000,"127.0.0.1");
        wrongCient->Connect();
        wrongCient->ClientWrite(SerializerForItems::Serialize(realWin->getVector()));
        exit(0);
    }

    data *dataS = new data();
    dataS->ConnectedSocket1 = ConnectedSocket1;
    dataS->ConnectedSocket2 = ConnectedSocket2;
    dataS->serverC = serverC;
    dataS->realWin = realWin;
    dataS->tempCos = tempCos;
    data *dataW = new data();
    dataW->ConnectedSocket1 = ConnectedSocket1;
    dataW->ConnectedSocket2 = ConnectedSocket2;
    dataW->serverC = serverC;
    dataW->realWin = realWin;
    dataW->tempCos = tempCos;

    Thread thread1;
    Thread thread2;

    //因为每次只有一个连接工作，所以无需互斥
    thread1.no1(dataW);
    thread2.no2(dataS);
    thread1.WaitForDeath();
    thread2.WaitForDeath();
}
