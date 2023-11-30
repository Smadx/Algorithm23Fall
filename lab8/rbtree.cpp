#include "rbtree.h"
using namespace std;

/*
 * @brief 默认构造函数
 */
template<typename T>
rbnode<T>::rbnode():key()
{
    color=BLACK;
    left=nullptr;
    right=nullptr;
    p=nullptr;
}

/*
 * @brief 构造函数
 * @param Key 键值
 * @param Color 颜色
 */
template<typename T>
rbnode<T>::rbnode(T Key,state Color)
{
    color=Color;
    key=Key;
    left=nullptr;
    right=nullptr;
    p=nullptr;
}

/*
 * @brief 重载!=运算符
 * @param node 另一个结点
 * @return 结点不相等返回true,否则返回false
 */
template<typename T>
bool rbnode<T>::operator!=(rbnode<T> node)
{
    if((node.key==key)&&(node.color==color))
        return false;
    return true;
}

/*
 * @brief 重载==运算符
 * @param node 另一个结点
 * @return 结点相等返回true,否则返回false
 */
template<typename T>
bool rbnode<T>::operator==(rbnode<T> node)
{
    if((node.key==key)&&(node.color==color))
        return true;
    return false;
}

/*
 * @brief 重载<<运算符
 * @param os 输出流
 * @param node 结点
 * @return 输出流
 */
template<typename T>
ostream &operator<<(ostream &os,const rbnode<T> &node)
{
    if(node.color==BLACK)
        os<<node.key<<", BLACK";
    else
        os<<node.key<<", RED";
    return os;
}

/*
 * @brief 重载<<运算符
 * @param of 输出流
 * @param node 结点
 * @return 输出流
 */
template<typename T>
ofstream &operator<<(ofstream &of,const rbnode<T> &node)
{
    if(node.color==BLACK)
        of<<node.key<<", BLACK";
    else
        of<<node.key<<", RED";
    return of;
}

/*
 * @brief 默认构造函数
 */
template<typename T>
rbtree<T>::rbtree()
{
    num=0;
    nil=new rbnode<T>();
    root=nil;
}

/*
 * @brief 构造函数
 * @param node 根结点
 */
template<typename T>
rbtree<T>::rbtree(rbnode<T> node)
{
    num=1;
    root=&node;
    nil=new rbnode<T>();
    node.left=nil;
    node.right=nil;
    node.p=nil;
    nil->left=root;
    nil->right=root;
    nil->p=nullptr;
}

/*
 * @brief 查找
 * @param target 目标值
 * @return 查找到的结点
 */
template<typename T>
rbnode<T> * rbtree<T>::search(T target)
{
    rbnode<T> *x=root;
    while(x!=nil&&x->get_key()!=target)
    {
        if(target<x->get_key())
            x=x->left;
        else
            x=x->right;
    }
    if(x==nil)
        return nullptr;
    return x;
}

/*
 * @brief 返回x按先序遍历的后继
 * @param x 结点
 * @return 先序后继
 */
template<typename T>
rbnode<T> * rbtree<T>::NLR_successor(rbnode<T> *x)
{
    rbnode<T> *y=x;
    if(y->left!=nil)
        return y->left;
    if(y->right!=nil)
        return y->right;
    while(y->p!=nil)
    {
        if(y==y->p->left)
        {
            if(y->p->right!=nil)
                return y->p->right;
            y=y->p;
        }
        else
            y=y->p;
    }
    return nullptr;
}

/*
 * @brief 左旋
 * @param x 要旋转的结点
 */
template<typename T>
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

/*
 * @brief 右旋
 * @param x 要旋转的结点
 */
template<typename T>
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

/*
 * @brief 插入
 * @param z 要插入的结点
 */
template<typename T>
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
    num++;
}

/*
 * @brief 保持红黑树性质
 * @param z 要保持的结点
 */
template<typename T>
void rbtree<T>::rb_insert_fixup(rbnode<T> *z)
{
    while(z->p->get_color()==RED)
    {
        if(z->p==z->p->p->left)
        {
            rbnode<T> *y=z->p->p->right;
            if(y->get_color()==RED)
            {
                //case 1
                z->p->write_color(BLACK);
                y->write_color(BLACK);
                z->p->p->write_color(RED);
                z=z->p->p;
                continue;
            }
            else if(z==z->p->right)
            {
                //case 2
                z=z->p;
                this->left_rotate(z);
            }
            //case 3
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
                //case 4
                z->p->write_color(BLACK);
                y->write_color(BLACK);
                z->p->p->write_color(RED);
                z=z->p->p;
                continue;
            }
            else if(z==z->p->left)
            {
                //case 5
                z=z->p;
                this->right_rotate(z);
            }
            //case 6
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