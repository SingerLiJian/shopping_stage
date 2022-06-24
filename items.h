#ifndef _ITEMS_
#define _ITEMS_
#include <vector>
using namespace std;

struct tradeThing
{
    //商品名称
    char name[10];
    //该类商品数量
    int nums;
};

class items
{
private:

public:
    items(){
    }
    virtual ~items(){
    }
public:
    virtual void checkItems()=0;
    virtual void addItem(const char name[10],int nums)=0;
    virtual void shopItems()=0;
    virtual vector<tradeThing> getVector()=0;
    virtual void expense(vector<tradeThing> v)=0;
};
#endif