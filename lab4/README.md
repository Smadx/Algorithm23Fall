# lab4:区间树上重叠区间的查找算法
## 实验目的
对红黑树进行修改，使其成为一颗区间数，并实现区间树上的重叠区间查找算法。
## 实验原理
额外维护子树区间中的最大端点和最小端点进行剪枝
## 实验代码
### 1.定义区间类
```cpp
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <queue>
#include <string>
using namespace std;

class interval//定义区间类型
{
    public:
        int low;//左端点
        int high;//右端点
        interval();
        interval(int a,int b);
        bool operator!=(const interval &A);
        bool operator==(const interval &A);
        friend ostream & operator<<(ostream &os,const interval &A);
        friend ofstream & operator<<(ofstream &of,const interval &A);
};
interval::interval()
{
    low=0;
    high=1;
}
interval::interval(int a,int b)
{
    if(a<b)
    {
        low=a;
        high=b;
    }
    else
    {
        cout<<"Error:b must be larger than a"<<endl;
        exit(1);
    }
}
bool interval::operator!=(const interval &A)
{
    if((low==A.low)&&(high==A.high))
        return false;
    return true;
}
bool interval::operator==(const interval &A)
{
    if((low==A.low)&&(high==A.high))
        return true;
    return false;
}
ostream & operator<<(ostream &os,const interval &A)
{
    os<<"["<<A.low<<","<<A.high<<"]";
    return os;
}
ofstream & operator<<(ofstream &of,const interval &A)
{
    of<<"["<<A.low<<","<<A.high<<"]";
    return of;
}
```
### 2.区间树节点类
```cpp
enum state{RED,BLACK};//枚举结点的颜色

class rbnode//红黑树结点类
{
    private:
        state color;//颜色
        interval key;//值
    public:
        int max;//记录子树最大区间端点
        int min;//记录子树最小区间端点
        rbnode *left;
        rbnode *right;
        rbnode *p;
        rbnode();//默认构造哨兵结点
        rbnode(interval Key,state Color=BLACK);
        state get_color(){return color;};
        int get_key(){return key.low;};
        interval _int(){return key;};
        void write_color(state Color){color=Color;};
        void write_key(interval Key){key=Key;max=Key.high;min=Key.low;};
        bool operator!=(rbnode node);
        bool operator==(rbnode node);
        friend ostream &operator<<(ostream &os,const rbnode &node);
        friend ofstream &operator<<(ofstream &of,const rbnode &node);
};
rbnode::rbnode()
{
    color=BLACK;
    interval temp(-1,0);
    key=temp;
    left=nullptr;
    right=nullptr;
    p=nullptr;
    max=0;
    min=0;
}
rbnode::rbnode(interval Key,state Color)
{
    color=Color;
    key=Key;
    left=nullptr;
    right=nullptr;
    p=nullptr;
    max=Key.high;
    min=Key.low;
}
bool rbnode::operator!=(rbnode node)
{
    if((node.key==key)&&(node.color==color))
        return false;
    return true;
}
bool rbnode::operator==(rbnode node)
{
    if((node.key==key)&&(node.color==color))
        return true;
    return false;
}
ostream &operator<<(ostream &os,const rbnode &node)
{
    if(node.color==BLACK)
        os<<node.key<<", BLACK";
    else
        os<<node.key<<", RED";
    return os;
}
ofstream &operator<<(ofstream &of,const rbnode &node)
{
    if(node.color==BLACK)
        of<<node.key<<", BLACK";
    else
        of<<node.key<<", RED";
    return of;
}
```
### 3.区间树类型
在插入和旋转操作时维护``int.max``和``int.min``
```cpp
class rbtree
{
    public:
        rbnode *root;//指向根节点
        rbnode *nil;//指向哨兵
        rbtree();
        rbtree(rbnode node);
        void left_rotate(rbnode *x);//左旋
        void right_rotate(rbnode *x);//右旋
        void rb_insert(rbnode *z);//插入
        void rb_insert_fixup(rbnode *z);//保持
};

rbtree::rbtree()
{
    nil=new rbnode();
    root=nil;
}

rbtree::rbtree(rbnode node)
{
    root=&node;
    nil=new rbnode();
    node.left=nil;
    node.right=nil;
    node.p=nil;
    nil->left=root;
    nil->right=root;
    nil->p=nullptr;
}

//左旋
void rbtree::left_rotate(rbnode *x)
{
    rbnode *y=x->right;
    x->right=y->left;
    if(y->left!=nil)
        y->left->p=x;
    y->p=x->p;
    if(x->p==nil)
    {
        root=y;
        nil->left=y;
        nil->right=y;
    }
    else if(x==x->p->left)
        x->p->left=y;
    else
        x->p->right=y;
    y->left=x;
    x->p=y;
    x->max=max(max(x->_int().high,x->left->max),x->right->max);
    y->max=max(max(y->_int().high,y->left->max),y->right->max);
    x->min=min(min(x->_int().low,x->left->min),x->right->min);
    y->min=min(min(y->_int().low,y->left->min),y->right->min);
}

/*右旋*/
void rbtree::right_rotate(rbnode *x)
{
    rbnode *y=x->left;
    x->left=y->right;
    if(y->right!=nil)
        y->right->p=x;
    y->p=x->p;
    if(x->p==nil)
    {
        root=y;
        nil->left=y;
        nil->right=y;
    }
    else if(x==x->p->left)
        x->p->left=y;
    else
        x->p->right=y;
    y->right=x;
    x->p=y;
    x->max=max(max(x->_int().high,x->left->max),x->right->max);
    y->max=max(max(y->_int().high,y->left->max),y->right->max);
    x->min=min(min(x->_int().low,x->left->min),x->right->min);
    y->min=min(min(y->_int().low,y->left->min),y->right->min);
}

//插入
void rbtree::rb_insert(rbnode *z)
{
    rbnode *y=nil;
    rbnode *x=root;
    while(x!=nil)
    {
        y=x;
        if(z->get_key()<x->get_key())
            x=x->left;
        else
            x=x->right;
        if(y->max<z->_int().high)
            y->max=z->_int().high;
        if(y->min>z->_int().low)
            y->min=z->_int().low;
    }
    z->p=y;
    if(y==nil)
    {
        root=z;
        nil->left=root;
        nil->right=root;
    }
    else if(z->get_key()<y->get_key())
        y->left=z;
    else
        y->right=z;
    z->left=nil;
    z->right=nil;
    z->write_color(RED);
    this->rb_insert_fixup(z);
}

/*保持*/
void rbtree::rb_insert_fixup(rbnode *z)
{
    while(z->p->get_color()==RED)
    {
        if(z->p==z->p->p->left)
        {
            rbnode *y=z->p->p->right;
            if(y->get_color()==RED)
            {
                z->p->write_color(BLACK);
                y->write_color(BLACK);
                z->p->p->write_color(RED);
                z=z->p->p;
                continue;
            }
            else if(z==z->p->right)
            {
                z=z->p;
                this->left_rotate(z);
            }
            if(z->p!=nil)
            {
                z->p->write_color(BLACK);
                if(z->p->p!=nil)
                {
                    z->p->p->write_color(RED);
                    this->right_rotate(z->p->p);
                }
            } 
        }
        else{
            rbnode *y=z->p->p->left;
            if(y->get_color()==RED)
            {
                z->p->write_color(BLACK);
                y->write_color(BLACK);
                z->p->p->write_color(RED);
                z=z->p->p;
                continue;
            }
            else if(z==z->p->left)
            {
                z=z->p;
                this->right_rotate(z);
            }
            if(z->p!=nil)
            {
                z->p->write_color(BLACK);
                if(z->p->p!=nil)
                {
                    z->p->p->write_color(RED);
                    this->left_rotate(z->p->p);
                }
            }
        }
    }
    root->write_color(BLACK);
}
```
### 4.判断是否重叠
```cpp
bool IsOverlap(rbnode *T,interval I)//判断结点T对应的区间是否与I重叠
{
    rbnode * x=T;
    if(x->get_key()==-1)
    {
        cout<<"Error:T is nil"<<endl;
        exit(1);
    }
    if((x->_int().high<I.low)||(x->_int().low>I.high))
        return false;
    return true;
}
```
### 5.输出所有的重叠区间
```cpp
vector<interval> findall_interval(rbnode *T,interval I)//找出以T为根节点的树中所有与I重叠的区间
{
    vector<interval> result;//存放找到的重叠区间
    queue<rbnode *> Q;//存放待处理结点
    Q.push(T);
    while(!Q.empty())
    {
        rbnode * node=Q.front();
        if(IsOverlap(node,I))
            result.push_back(node->_int());//找到一个压入result
        /*下一层入队*/
        if((node->left->get_key()!=-1)&&(node->left->max>=I.low)&&(node->left->min<=I.high))
            Q.push(node->left);
        if((node->right->get_key()!=-1)&&(node->right->min<=I.high)&&(node->right->max>=I.low))
            Q.push(node->right);
        Q.pop();//已处理的结点出队
    }
    return result;
}
```
### 6.主函数
```cpp
int main()
{
    //读入数据
    ifstream fin;
    fin.open("insert.txt");
    int count;
    fin>>count;
    int a,b;
    vector<interval> ints;
    for(int i=0;i<count;i++)
    {
        fin>>a;
        fin>>b;
        interval temp(a,b);
        ints.push_back(temp);
    }
    fin.close();
    //插入
    rbtree T;
    vector<rbnode> tn(count);
    for(int i=0;i<count;i++)
    {
        tn[i].write_key(ints[i]);
        T.rb_insert(&tn[i]);
    }
    string input;
    while (1)
    {
        cout<<"Find overlap interval:(input q to quit)"<<endl;
        getline(cin,input);
        if(input=="q")
        {
            cout<<"Quit"<<endl;
            break;
        }
        stringstream ss(input);
        if(ss>>a>>b)//输入区间端点
        {
            interval I(a,b);
            vector<interval> x;
            x=findall_interval(T.root,I);//找出所有的重叠区间
            if(x.size()==0)
            {
                cout<<"nil";
            }
            else
            {
                cout<<"The intervals that overlap with "<<I<<" are:"<<endl;
                for(int i=0;i<x.size();i++)
                {
                    cout<<x[i]<<" ";
                }
            }
            cout<<endl;
        }
        else
        {
            cout<<"Error:Invalid input"<<endl;
            exit(1);
        }
    }
    return 0;
}
```
## 实验结果
在查找所有的重叠区间时进行了剪枝操作,保证每次进入下一层查找时,下一层有重叠的区间,在最坏情况下,需要遍历所有的结点$O(n)$,但是有剪枝处理后,我们只搜索有重叠区间的子树,即$O(重叠区间数*树高)=k\log n$,因此复杂度为$O(min(n,k*\log n))$
![](结果.png)
## 总结
学习了数据结构的扩张,更熟悉了红黑树的操作,复习剪枝的思想(在机器学习课上决策树也用到了)
