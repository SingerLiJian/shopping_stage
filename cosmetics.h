#ifndef _COSMESTICS_
#define _COSMESTICS_
#include "items.h"
#include "clientConnect.h"
#include "serverAccept.h"
#include <vector>
#include "serializerForItems.h"
#include <string>
//物品类型

//化妆品类
class cosmetics : public items
{
public:
    //客户端可用
    clientConnect *ccon;
    //服务器端可用
    serverAccept *scon;
    //化妆品类商品数组
    vector<tradeThing> things;

public:
    cosmetics(clientConnect *temp){
        ccon = temp;
    }

    cosmetics(serverAccept *temp){
        scon = temp;
    }

    ~cosmetics(){
    }
    //添加具体的商品
    void addItem(const char name[10],int nums){
        tradeThing aitem;
        strcpy(aitem.name,name);
        aitem.nums = nums;
        things.push_back(aitem);
    }
    //用于查看一类所有的商品的信息
    void checkItems(){
        char* buf;
        //发出查看服务器中商品的指令—代号1
        ccon->ClientWrite("1");
        things.clear();
        buf = ccon->ClientRead();
        //当收不到信息时，认为服务器异常
        if(string(buf)==""){
            throw 4000;
        }
        SerializerForItems::Deserialize(buf, things);
        for (int i = 0; i < things.size(); i++)
        {
            cout<<things[i].name<<":"<<things[i].nums<<endl;
        }
        things.clear();
    }

    vector<tradeThing> getVector(){
        return things;
    }
    //客户但实现购买操作
    void shopItems(){
        char* buf;
        //发出购买商品的指令—代号2
        ccon->ClientWrite("2");
        //将购物车中这类商品的信息发送给对应服务器
        ccon->ClientWrite(SerializerForItems::Serialize(things));
        things.clear();
    }
    //服务器端减少售卖的商品
    void expense(vector<tradeThing> v){
        for (int i = 0; i < v.size(); i++)
        {
            //依次减少同名商品的数量
            for (int j = 0; j < things.size(); j++)
            {
                if (string(v[i].name)==string(things[j].name))
                {
                    things[j].nums = things[j].nums - v[i].nums;
                    break;
                }
            }
        }
    }
};

#endif