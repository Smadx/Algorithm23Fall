# lab8:图搜索BFS算法及存储优化
## 实验目的
针对图，根据给定的数据选择合适的存储方式（邻接矩阵和邻接表中的一种）进行存储，并进行图的广度优先遍历的过程。
## 实验原理
### 图的存储
对于小数据集,边比较密集,采用邻接矩阵存储,对于大数据集,边比较稀疏,采用邻接表存储.
邻接表的结构如下:(定义图结点类型为Gnode)
|Adj(Gtrnode)<br>基于红黑树组织||Adjnode<br>单链表||
|:-:|:-:|:-:|:-:|
|Gnode\|head|$\rightarrow$|Gnode*\|next|$\cdots$|
|Gnode\|head|$\rightarrow$|Gnode*\|next|$\cdots$|
|$\vdots$|$\vdots$|$\vdots$|$\cdots$|
|Gnode\|head|$\rightarrow$|Gnode*\|next|$\cdots$|
### 图的BFS遍历
对于邻接矩阵,用一个数组记录结点是否被访问过,每次访问一个结点,将其标记为已访问,并将其邻接结点入队列,直到队列为空. 
对于邻接表,用颜色标记结点是否被访问过,每次访问一个结点,将其标记为已访问,并将其邻接结点入队列,直到队列为空,由于使用红黑树存储图结点,每次遍历完一个连通块后,在红黑树中按照先序顺序找到下一个未访问的结点,并从该结点开始进行下一次遍历.初始时遍历根节点.
## 实验代码
### 1.遍历data.txt
在``BFS.cpp``中,处理小数据集,采用邻接矩阵存储.
```cpp
#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
using namespace std;
enum Color{WHITE,GRAY,BLACK};
/*图,邻接矩阵*/
class Graph
{
    public:
        int V; // 顶点数
        vector<vector<int> > M;//邻接矩阵
        vector<char> alphabet;//顶点集
        vector<Color> color;//顶点颜色
        Graph();
        Graph(int V,vector<char> alphabet);//根据顶点数和顶点集初始化
        int locateVex(char v);//返回顶点v在顶点集中的位置
        void addEdge(char v,char w);//添加边
        void BFS(char s);//广度优先搜索
};
/*
 * @brief 无向图构造函数
 */
Graph::Graph()
{
    V=0;
    M=vector<vector<int> >();
    alphabet=vector<char>();
    color=vector<Color>();
}
/*
 * @brief 无向图构造函数
 * @param num 顶点数
 * @param nodes 顶点集
 */
Graph::Graph(int num,vector<char> nodes)
{
    V=num;
    M=vector<vector<int> >(V,vector<int>(V,0));
    alphabet=nodes;
    color=vector<Color>(V,WHITE);
}
/*
 * @brief 返回顶点v在顶点集中的位置
 * @param v 顶点
 * @return 顶点v在顶点集中的位置
 */
int Graph::locateVex(char v)
{
    int i=0;
    for(;i<V;i++)
    {
        if(alphabet[i]==v)
            break;
    }
    return i;
}
/*
 * @brief 添加边
 * @param v 顶点
 * @param w 顶点
 */
void Graph::addEdge(char v,char w)
{
    int i=locateVex(v);
    int j=locateVex(w);
    M[i][j]=1;
    M[j][i]=1;
}
/*
 * @brief 广度优先搜索
 * @param s 搜索起点
 */
void Graph::BFS(char s)
{
    int i=locateVex(s);
    color[i]=GRAY;
    queue<int> Q;
    Q.push(i);
    while(!Q.empty())
    {
        int u=Q.front();
        Q.pop();
        cout<<alphabet[u]<<" ";
        for(int v=0;v<V;v++)
        {
            if(M[u][v]==1&&color[v]==WHITE)
            {
                color[v]=GRAY;
                Q.push(v);
            }
        }
        color[u]=BLACK;
    }
}
int main()
{
    ifstream fin("data.txt");
    int V;
    char ch;
    vector<char> alphabet;
    while((ch=fin.get())!='\n')
    {
        if(ch!=',')
        {
            V++;
            alphabet.push_back(ch);
        }
    }
    Graph G(V,alphabet);
    char v,w,temp;
    while(!fin.eof())
    {
        fin>>v>>temp>>w;
        G.addEdge(v,w);
    }
    G.BFS('A');
    return 0;
}
```
### 2.遍历twitter_small.txt
在``BFS-big.cpp``中,处理大数据集,采用邻接表存储.在``rbtree.h``中,根据实验三的代码定义了红黑树类型及其操作,并在``BFS-big.cpp``中引用.
```cpp
#include "rbtree.h"
#include <time.h>
using namespace std;

/*
 * @brief 计时类,用于计算程序运行时间,以毫秒为单位输出
 * @param t 时钟
 */
class mtime
{
    public:
        clock_t t;
        mtime();
        mtime(clock_t tt);
        mtime operator-(mtime t_2);
        friend ostream &operator<<(ostream &os,const mtime &t);
};

mtime::mtime()
{
    t=clock();
}
mtime::mtime(clock_t tt)
{
    t=tt;
}
mtime mtime::operator-(mtime t_2)
{
    mtime result(t-t_2.t);
    return result;
}
ostream & operator<<(ostream &os,const mtime &t)
{
    os<<(double)(t.t*1000/CLOCKS_PER_SEC)<<"ms";
    return os;
}
```
### 3.图结点类型
```cpp
/*
 * @brief 图结点颜色
 * @param WHITE 白色(未遍历)
 * @param GRAY 灰色(已发现)
 * @param GBLACK 黑色(已遍历)
 */
enum Color{WHITE,GRAY,GBLACK};

/*
 * @brief 图结点类
 * @param data 数据
 * @param color 颜色(遍历状态)
 */
class Gnode
{
    public:
        int data;
        Color color;
        Gnode();
        Gnode(int x);
        bool operator==(Gnode x);
        bool operator!=(Gnode x);
        bool operator>=(Gnode x);
        bool operator<=(Gnode x);
        bool operator>(Gnode x);
        bool operator<(Gnode x);
};
/*
 * @brief 结点构造函数
 */
Gnode::Gnode()
{
    data=0;
    color=WHITE;
}
/*
 * @brief 结点构造函数
 * @param x 数据
 */
Gnode::Gnode(int x)
{
    data=x;
    color=WHITE;
}
/*
 * @brief 判断结点是否相等
 * @param x 结点
 * @return 结点是否相等
 */
bool Gnode::operator==(Gnode x)
{
    return data==x.data;
}
/*
 * @brief 判断结点是否不相等
 * @param x 结点
 * @return 结点是否不相等
 */
bool Gnode::operator!=(Gnode x)
{
    return data!=x.data;
}
/*
 * @brief 判断结点是否大于等于
 * @param x 结点
 * @return 结点是否大于等于
 */
bool Gnode::operator>=(Gnode x)
{
    return data>=x.data;
}
/*
 * @brief 判断结点是否小于等于
 * @param x 结点
 * @return 结点是否小于等于
 */
bool Gnode::operator<=(Gnode x)
{
    return data<=x.data;
}
/*
 * @brief 判断结点是否大于
 * @param x 结点
 * @return 结点是否大于
 */
bool Gnode::operator>(Gnode x)
{
    return data>x.data;
}
/*
 * @brief 判断结点是否小于
 * @param x 结点
 * @return 结点是否小于
 */
bool Gnode::operator<(Gnode x)
{
    return data<x.data;
}
```
### 4.邻接表结点类型(单链表)
```cpp
/*
 * @brief 邻接表结点类
 * @param node 指向红黑树中的图结点
 * @param next 指向下一个结点
 */
class Adjnode
{
    public:
        Gnode * node;
        Adjnode * next;
        Adjnode();
        Adjnode(Gnode * x);
};
/*
 * @brief 结点构造函数
 */
Adjnode::Adjnode()
{
    node=nullptr;
    next=nullptr;
}
/*
 * @brief 结点构造函数
 * @param x 结点
 */
Adjnode::Adjnode(Gnode * x)
{
    node=x;
    next=nullptr;
}
```
### 5.邻接表结点类型(红黑树)
```cpp
/*
 * @brief 邻接表树结点类
 * @param node 结点
 * @param head 邻接表头指针
 */
class Gtrnode
{
    public:
        Gnode node;
        Adjnode * head;
        Gtrnode();
        Gtrnode(Gnode x);
        bool operator==(Gtrnode x);
        bool operator!=(Gtrnode x);
        bool operator>=(Gtrnode x);
        bool operator<=(Gtrnode x);
        bool operator>(Gtrnode x);
        bool operator<(Gtrnode x);
        void insert(Gnode *x);
};

/*
 * @brief 邻接表树结点构造函数
 */
Gtrnode::Gtrnode()
{
    node=Gnode();
    head=nullptr;
}
/*
 * @brief 邻接表树结点构造函数
 * @param x 结点
 */
Gtrnode::Gtrnode(Gnode x)
{
    node=x;
    head=nullptr;
}
/*
 * @brief 判断结点是否相等
 * @param x 结点
 * @return 结点是否相等
 */
bool Gtrnode::operator==(Gtrnode x)
{
    return node==x.node;
}
/*
 * @brief 判断结点是否不相等
 * @param x 结点
 * @return 结点是否不相等
 */
bool Gtrnode::operator!=(Gtrnode x)
{
    return node!=x.node;
}
/*
 * @brief 判断结点是否大于等于
 * @param x 结点
 * @return 结点是否大于等于
 */
bool Gtrnode::operator>=(Gtrnode x)
{
    return node>=x.node;
}
/*
 * @brief 判断结点是否小于等于
 * @param x 结点
 * @return 结点是否小于等于
 */
bool Gtrnode::operator<=(Gtrnode x)
{
    return node<=x.node;
}
/*
 * @brief 判断结点是否大于
 * @param x 结点
 * @return 结点是否大于
 */
bool Gtrnode::operator>(Gtrnode x)
{
    return node>x.node;
}
/*
 * @brief 判断结点是否小于
 * @param x 结点
 * @return 结点是否小于
 */
bool Gtrnode::operator<(Gtrnode x)
{
    return node<x.node;
}
/*
 * @brief 把x插入邻接表
 * @param x 结点
 */
void Gtrnode::insert(Gnode *x)
{
    Adjnode * p=head;
    if(p==nullptr)
    {
        head=new Adjnode(x);
        return;
    }
    while(p->next!=nullptr)
    {
        p=p->next;
    }
    p->next=new Adjnode(x);
}
```
### 6.图类型及其操作
```cpp
/*
 * @brief 有向图类
 * @param V 结点数
 * @param Adj 邻接表
 */
class Graph
{
    public:
        int V;//结点数
        rbtree<Gtrnode> Adj;//邻接表树
        Graph();
        Graph(int V);//根据结点数初始化
        Graph(Gnode x);//根据结点初始化
        bool is_in_Graph(Gnode x);//判断结点是否在图中
        void insert(Gnode x,Gnode y);//插入边
        int BFS();//广度优先搜索
};
/*
 * @brief 图构造函数
 */
Graph::Graph()
{
    V=0;
    Adj=rbtree<Gtrnode>();
}
/*
 * @brief 构造有V个结点的图
 * @param num 结点数
 */
Graph::Graph(int num)
{
    V=num;
    Adj=rbtree<Gtrnode>();
}
/*
 * @brief 构造只有一个结点x的图
 * @param x 结点
 */
Graph::Graph(Gnode x)
{
    V=1;
    Adj=rbtree<Gtrnode>();
}
/*
 * @brief 判断结点x是否在图中
 * @param x 结点
 * @return 结点是否在图中
 */
bool Graph::is_in_Graph(Gnode x)
{
    Gtrnode temp(x);
    return Adj.search(temp)!=nullptr;
}
/*
 * @brief 插入边
 * @param x 结点
 * @param y 结点
 */
void Graph::insert(Gnode x,Gnode y)
{
    if(!is_in_Graph(x))
    {
        rbnode<Gtrnode> * rbnode_x=new rbnode<Gtrnode>(x);
        Adj.rb_insert(rbnode_x);//把x加入邻接表
        V++;
    }
    if(!is_in_Graph(y))
    {
        rbnode<Gtrnode> * rbnode_y=new rbnode<Gtrnode>(y);
        Adj.rb_insert(rbnode_y);//把y加入邻接表
        V++;
    }
    Gtrnode xtemp(x);
    Gtrnode ytemp(y);
    rbnode<Gtrnode> * p=Adj.search(xtemp);//找到x
    Gnode * q=&(Adj.search(ytemp)->key.node);//找到y
    p->key.insert(q);//把y插入x的邻接表
}
/*
 * @brief 广度优先搜索
 * @param x 起始结点
 */
int Graph::BFS()
{
    Gnode *x=&(Adj.root->key.node);
    int num=0;
    x->color=GRAY;
    queue<Gnode *> Q;
    Q.push(x);
    while(x!=nullptr)
    {
        while(!Q.empty())
        {
            Gnode *y=Q.front();
            Q.pop();
            num++;
            y->color=GBLACK;
            Adjnode *p=Adj.search(*y)->key.head;//找到y的邻接表
            while(p!=nullptr)
            {
                if(p->node->color==WHITE)
                {
                    p->node->color=GRAY;
                    Q.push(p->node);
                }
                p=p->next;
            }
        }
        //找到x的先序后继
        rbnode<Gtrnode> *rbnode_x=Adj.search(*x);
        while((rbnode_x!=nullptr)&&(rbnode_x->key.node.color==GBLACK))
        {
            rbnode_x=Adj.NLR_successor(rbnode_x);
        }
        if(rbnode_x==nullptr) break;
        x=&(rbnode_x->key.node);
        x->color=GRAY;
        Q.push(x);
    }
    return num;
}
```
### 7.主函数
```cpp
int main()
{
    ifstream fin("dataset/twitter_small.txt");
    int a,b,num;
    Graph G;
    while(fin>>a>>b)
    {
        Gnode A(a);
        Gnode B(b);
        G.insert(A,B);
    }
    mtime t_1;
    num=G.BFS();
    mtime t_2;
    cout<<"Number: "<<num<<", Time: "<<t_2-t_1<<endl;
}
```
## 实验结果
![](结果.png)
## 实验总结
本次实验我复习了图的BFS遍历算法,并尝试优化的图的储存方式,我采用了红黑树来加快结点的查找速度,但是产生了很多的空间开销,导致无法处理``twitter_large.txt``数据集,后面我认为应该尽量减少邻接表中指针的数量来优化存储空间.