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