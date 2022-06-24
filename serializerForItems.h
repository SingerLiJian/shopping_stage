#ifndef _Serializer_
#define _Serializer_
#include <iostream>
#include <vector>
#include <stdio.h>
#include <string>
#include <cstdlib>
using namespace std;



class SerializerForItems{

public:
    //序列化：向量->字符流
    static char* Serialize(vector<tradeThing> v){
        tradeThing temp;
        char a[4];
        char *buf = new char [100];
        int j=0;
        for(int i=0;i<v.size();i++){
            //数字转字符串
            sprintf(a,"%d",v[i].nums);
            //依次注入buf
            for (int k = 0; k < 10;k++,j++)
            {
                buf[j] = v[i].name[k];
            }
            for (int k= 0; k<4; k++,j++)
            {
                buf[j] = a[k];
            }
        }
        //%号作为终止标志
        buf[j] = '%';
        return buf;
    }
    //反序列化：字符流->向量
    static void Deserialize(char* buf,vector<tradeThing>& v){
        //碰到终结符再停止
        for (int i = 0;buf[i]!='%';)
        {
            tradeThing aitem;
            char a[4];
            for (int j = 0; j < 10;i++,j++)
            {
                aitem.name[j]=buf[i];
            }
            for (int j= 0; j<4; i++,j++)
            {
                a[j] = buf[i];
            }
            //获取到的数量由字符串转int
            aitem.nums = atoi(a);
            v.push_back(aitem);
        }
    }
};
#endif