#ifndef _WINES_
#define _WINES_
#include "items.h"
#include "clientConnect.h"
#include "serverAccept.h"
#include <vector>
#include "serializerForItems.h"
#include <string>
//不做过多注释，与cosmetics.h相似

//酒类
class wines : public items
{
public:
    //客户端可用
    clientConnect *ccon;
    //服务器端可用
    serverAccept *scon;
    //化妆品类商品数组
    vector<tradeThing> things;

public:
    wines(clientConnect *temp){
        ccon = temp;
    }

    wines(serverAccept *temp){
        scon = temp;
    }

    ~wines(){
    }

    void addItem(const char name[10],int nums){
        tradeThing aitem;
        strcpy(aitem.name,name);
        aitem.nums = nums;
        things.push_back(aitem);
    }

    void checkItems(){
        char* buf;
        //发出查看服务器中商品的指令—代号3
        ccon->ClientWrite("1");
        things.clear();
        buf = ccon->ClientRead();
        if(string(buf)==""){
            throw 5000;
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
    
    void shopItems(){
        char* buf;
        //发出购买商品的指令—代号4
        ccon->ClientWrite("2");
        ccon->ClientWrite(SerializerForItems::Serialize(things));
        things.clear();
    }

    void expense(vector<tradeThing> v){
        for (int i = 0; i < v.size(); i++)
        {
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
