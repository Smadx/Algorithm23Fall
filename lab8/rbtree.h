/*
 * rbtree.h
 */
#ifndef RBTREE_H
#define RBTREE_H

#include <iostream>
#include <fstream>
#include <vector>
#include <queue>

enum state{RED,BLACK};//枚举结点的颜色

template<typename t>
class rbnode//红黑树结点类
{
    public:
        state color;//颜色
        t key;//值
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
        friend std::ostream &operator<<(std::ostream &os,const rbnode<T> &node);
        template<typename T>
        friend std::ofstream &operator<<(std::ofstream &of,const rbnode<T> &node);
};

template<typename t>
class rbtree//红黑树类
{
    public:
        int num;//结点数
        rbnode<t> *root;//指向根节点
        rbnode<t> *nil;//指向哨兵
        rbtree();
        rbtree(rbnode<t> node);
        rbnode<t> *search(t target);//查找
        rbnode<t> *NLR_successor(rbnode<t> *x);//先序后继
        void left_rotate(rbnode<t> *x);//左旋
        void right_rotate(rbnode<t> *x);//右旋
        void rb_insert(rbnode<t> *z);//插入
        void rb_insert_fixup(rbnode<t> *z);//保持
};

#endif //rbtree.h