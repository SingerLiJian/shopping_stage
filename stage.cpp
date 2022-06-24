#include <iostream>
#include "items.h"
#include "cosmetics.h"
#include "wines.h"
#include "clientConnect.h"
using namespace std;
//进入购物界面
void shopcar(items *tempI){
    cout<<"商品如下:"<<endl;
    tempI->checkItems();
    cout<<"你想买几种商品:";
    int n;
    cin>>n;
    cout<<"请依次输入物品名称与数量"<<endl;
    for(int i=0;i < n;i++)
    {
        tradeThing temp;
        cin>>temp.name;
        cin>>temp.nums;
        //实例中存贮着购物信息
        tempI->addItem(temp.name,temp.nums);
    }
}
//展示实例中已选择的物品数量
void show(items *tempI){
    vector<tradeThing> temp = tempI->getVector();
    for (int i = 0; i < temp.size(); i++)
    {
        cout<<temp[i].name<<":"<<temp[i].nums<<endl;
    }
}
//发出出售的指令
void shop(items *tempI){
    tempI->shopItems();
}

int main(){
    //测试
   /*  clientConnect *cosm = new clientConnect (4000,"127.0.0.1");
    cosm->Connect();
    clientConnect *wine = new clientConnect (5000,"127.0.0.1");
    wine->Connect();
    cosm->ClientWrite("123");
    cosm->ClientRead();
    wine->ClientWrite("124");
    wine->ClientRead();
    return 0; */
    //初始化
    clientConnect *cosm = new clientConnect (4000,"127.0.0.1");
    cosm->Connect();
    clientConnect *wine = new clientConnect (5000,"127.0.0.1");
    wine->Connect();
    int opt;
    items *tempItem;
    //设立两个实例用于调用函数以及当各类购物车的作用
    cosmetics *a= new cosmetics(cosm);
    wines *b= new wines(wine);
    //此处可扩展辣条
    
    while (1)
    {
        try
        {
            cout<<"请问你需要购买什么？(1化妆品,2酒类,3购物车)"<<endl;
            cin>>opt;
            if(opt==1){
                tempItem = a;
                shopcar(tempItem);
            }
            else if(opt==2)
            {
                tempItem = b;
                shopcar(tempItem);
            }
            //可扩展辣条
            else if (opt==3)
            {
                int select;
                cout<<"购物车如下:"<<endl;
                show(a);
                show(b);
                //可扩展辣条
                cout<<"确定购买吗:(1买,其他不买)"<<endl;
                cin>>select;
                if (select == 1)
                {
                    shop(a);
                    shop(b);
                    //可扩展辣条
                }
            }
            else
            {
                cout<<"错误选择";
            }
        }
        //服务器出现了异常，进一步导致客户端异常
        catch(const int wrong)
        {
            cout<<"客户您好，服务器出了点问题"<<endl;
            if (wrong==4000)
            {
                //借wine向服务器B发送消息，告知服务器A异常
                wine->ClientWrite("0");
                //建立新的指向服务器B的连接
                cosm = new clientConnect (5000,"127.0.0.1");
                //让服务器先处理异常的商品信息交换
                sleep(1);
                cosm->Connect();
                a->ccon = cosm;
            }
            else
            {
                cosm->ClientWrite("0");
                wine = new clientConnect (4000,"127.0.0.1");
                sleep(1);
                wine->Connect();
                b->ccon = wine;
            }
        }
    } 
}