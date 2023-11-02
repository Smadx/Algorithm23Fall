#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
using namespace std;

enum state{RED,BLACK};//枚举结点的颜色

ifstream fin;
ofstream fout1;
ofstream fout2;
ofstream fout3;

template<typename t>
class rbnode//红黑树结点类
{
    private:
        state color;//颜色
        t key;//值
    public:
        rbnode<t> *left;
        rbnode<t> *right;
        rbnode<t> *p;
        rbnode();//默认构造哨兵结点
        rbnode(t Key,state Color=BLACK);
        state get_color(){return color;};
        t get_key(){return key;};
        void write_color(state Color){color=Color;};
        void write_key(t Key){key=Key;};
        bool operator!=(rbnode<t> node);
        bool operator==(rbnode<t> node);
        template<typename T>
        friend ostream &operator<<(ostream &os,const rbnode<T> &node);
        template<typename T>
        friend ofstream &operator<<(ofstream &of,const rbnode<T> &node);
};
template<typename T>
rbnode<T>::rbnode()
{
    color=BLACK;
    key=-1;
    left=nullptr;
    right=nullptr;
    p=nullptr;
}
template<typename T>
rbnode<T>::rbnode(T Key,state Color)
{
    color=Color;
    key=Key;
    left=nullptr;
    right=nullptr;
    p=nullptr;
}
template<typename T>
bool rbnode<T>::operator!=(rbnode<T> node)
{
    if((node.key==key)&&(node.color==color))
        return false;
    return true;
}
template<typename T>
bool rbnode<T>::operator==(rbnode<T> node)
{
    if((node.key==key)&&(node.color==color))
        return true;
    return false;
}
template<typename T>
ostream &operator<<(ostream &os,const rbnode<T> &node)
{
    if(node.color==BLACK)
        os<<node.key<<", BLACK";
    else
        os<<node.key<<", RED";
    return os;
}
template<typename T>
ofstream &operator<<(ofstream &of,const rbnode<T> &node)
{
    if(node.color==BLACK)
        of<<node.key<<", BLACK";
    else
        of<<node.key<<", RED";
    return of;
}

template<typename t>
class rbtree
{
    public:
        rbnode<t> *root;//指向根节点
        rbnode<t> *nil;//指向哨兵
        rbtree();
        rbtree(rbnode<t> node);
        void left_rotate(rbnode<t> *x);//左旋
        void right_rotate(rbnode<t> *x);//右旋
        void rb_insert(rbnode<t> *z);//插入
        void rb_insert_fixup(rbnode<t> *z);//保持
};

template<typename T>
rbtree<T>::rbtree()
{
    nil=new rbnode<T>();
    root=nil;
}

template<typename T>
rbtree<T>::rbtree(rbnode<T> node)
{
    root=&node;
    nil=new rbnode<T>();
    node.left=nil;
    node.right=nil;
    node.p=nil;
    nil->left=root;
    nil->right=root;
    nil->p=nullptr;
}

template<typename T>
//左旋
void rbtree<T>::left_rotate(rbnode<T> *x)
{
    rbnode<T> *y=x->right;
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
}

template<typename T>
/*右旋*/
void rbtree<T>::right_rotate(rbnode<T> *x)
{
    rbnode<T> *y=x->left;
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
}

template<typename T>
//插入
void rbtree<T>::rb_insert(rbnode<T> *z)
{
    rbnode<T> *y=nil;
    rbnode<T> *x=root;
    while(x!=nil)
    {
        y=x;
        if(z->get_key()<x->get_key())
            x=x->left;
        else
            x=x->right;
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

template<typename T>
/*保持*/
void rbtree<T>::rb_insert_fixup(rbnode<T> *z)
{
    while(z->p->get_color()==RED)
    {
        if(z->p==z->p->p->left)
        {
            rbnode<T> *y=z->p->p->right;
            if(y->get_color()==RED)
            {
                cout<<"case 1"<<endl;
                z->p->write_color(BLACK);
                y->write_color(BLACK);
                z->p->p->write_color(RED);
                z=z->p->p;
                continue;
            }
            else if(z==z->p->right)
            {
                cout<<"case 2"<<endl;
                z=z->p;
                this->left_rotate(z);
            }
            cout<<"case 3"<<endl;
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
            rbnode<T> *y=z->p->p->left;
            if(y->get_color()==RED)
            {
                cout<<"case 4"<<endl;
                z->p->write_color(BLACK);
                y->write_color(BLACK);
                z->p->p->write_color(RED);
                z=z->p->p;
                continue;
            }
            else if(z==z->p->left)
            {
                cout<<"case 5"<<endl;
                z=z->p;
                this->right_rotate(z);
            }
            cout<<"case 6"<<endl;
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

template<typename T>
void NLR(rbnode<T> *Tree)
{
    if(Tree->get_key()!=-1)
    {
        fout1<<*Tree<<endl;;
        NLR(Tree->left);
        NLR(Tree->right);
    }
}

template<typename T>
void LNR(rbnode<T> *Tree)
{
    if(Tree->get_key()!=-1)
    {
        LNR(Tree->left);
        fout2<<*Tree<<endl;
        LNR(Tree->right);
    }
}


template<typename T>
void LOT(rbnode<T> *Tree)
{
    queue<rbnode<T> *> Q;
    Q.push(Tree);
    while(!Q.empty())
    {
        rbnode<T> *node=Q.front();
        if(node->get_key()!=-1)
        {
            fout3<<*node<<endl;
            Q.push(node->left);
            Q.push(node->right);
        }
        Q.pop();
    }
}

int main()
{
    fin.open("insert.txt");
    int count,temp;
    fin>>count;
    vector<int> keys;
    for(int i=0;i<count;i++)
    {
        fin>>temp;
        keys.push_back(temp);
    }
    fin.close();
    //插入
    rbtree<int> T;
    vector<rbnode<int> > tn(count);
    for(int i=0;i<count;i++)
    {
        cout<<i<<endl;
        tn[i].write_key(keys[i]);
        T.rb_insert(&tn[i]);
    }
    //遍历
    fout1.open("NLR.txt");
    NLR(T.root);
    fout1.close();
    fout2.open("LNR.txt");
    LNR(T.root);
    fout2.close();
    fout3.open("LOT.txt");
    LOT(T.root);
    fout3.close();
    return 0;
}