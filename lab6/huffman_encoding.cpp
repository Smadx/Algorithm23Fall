#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <queue>
#include <stack>
#include <cmath>
using namespace std;
class h_tree//哈夫曼树
{
    public:
        char word;//字符(非叶子结点时为'\0')
        int freq;//频数
        h_tree * left;
        h_tree * right;
        h_tree * parent;
        h_tree();
        h_tree(char Word,int Freq);
};

h_tree::h_tree()
{
    word='\0';
    freq=0;
    left=nullptr;
    right=nullptr;
    parent=nullptr;
}

h_tree::h_tree(char Word,int Freq)
{
    word=Word;
    freq=Freq;
    left=nullptr;
    right=nullptr;
    parent=nullptr;
}
//优先队列比较函数
struct Compare
{
    bool operator()(const h_tree* a, const h_tree* b) const {
        if (a->freq != b->freq) {
            return a->freq > b->freq;
        } else {
            return a->word > b->word;
        }
    }
};

//在词汇表中查找字符,返回下标
int search_in_vocabulary(const char &ch,const string &vocabulary)
{
    for(int i=0;i<vocabulary.length();i++)
    {
        if(vocabulary[i]==ch)
            return i;
    }
    return -1;
}
//创建哈夫曼树
h_tree * CreateHuffman(const string &vocabulary,const vector<int> &frequence)
{
    priority_queue<h_tree*,vector<h_tree*>,Compare> Q;//最小优先队列
    for(int i=0;i<vocabulary.length();i++)//将每个字符作为一个结点加入优先队列
    {
        h_tree * t=new h_tree(vocabulary[i],frequence[i]);
        Q.push(t);
    }
    while(Q.size()!=1)//当队列中只剩一个结点时,结束循环
    {
        //取出频数最小的两个结点,合并为一个结点,并将其加入队列
        h_tree *z=new h_tree;
        z->left=Q.top();
        Q.pop();
        z->right=Q.top();
        Q.pop();
        z->freq=z->left->freq+z->right->freq;
        z->left->parent=z;
        z->right->parent=z;
        Q.push(z);
    }
    return Q.top();
}
//深度优先遍历,记录编码
void DFS(h_tree *T,string &path,const string &vocabulary,vector<string> &decode)
{
    if(T==nullptr)//如果是空结点,则返回
        return;
    if(T->parent!=nullptr)//如果不是根结点,则记录路径
    {
        if(T==T->parent->left)
            path+='0';
        else
            path+='1';
    }
    if(T->word!='\0')//如果是叶子结点,则记录编码
    {
        int index=search_in_vocabulary(T->word,vocabulary);
        decode[index]=path;
    }
    DFS(T->left,path,vocabulary,decode);
    DFS(T->right,path,vocabulary,decode);
    if(T->parent!=nullptr) path.erase(path.end()-1);//回溯
}
//编码
void encoding(h_tree *T,const string &vocabulary,vector<string> &decode)
{
    string path;
    DFS(T,path,vocabulary,decode);
}
//计算压缩率
void compressibility(const string &vocabulary,const vector<int> &frequence,const vector<string> &decode)
{
    int n=log2(vocabulary.length())+1;
    int length=0,d_len,v_len=0;
    for(int i=0;i<vocabulary.length();i++)//计算定长编码长度
    {
        length+=frequence[i];
    }
    d_len=length*n;
    for(int i=0;i<vocabulary.length();i++)//计算哈夫曼编码长度
    {
        v_len+=frequence[i]*decode[i].length();
    }
    cout<<(float)100*v_len/d_len<<"%"<<endl;
}
int main()
{
    ifstream fin;
    fin.open("orignal.txt");
    string vocabulary;//记录出现的所有字符
    vector<int> frequence;//记录字符出现的频数
    char ch;
    int index;
    while((ch=fin.get())!=EOF)
    {
        if((ch==' ')||(ch=='\n'))
            continue;
        if((index=search_in_vocabulary(ch,vocabulary))==-1)//如果不在词汇表中,则添加
        {
            vocabulary+=ch;
            frequence.push_back(1);
        }
        else//在词汇表中,增加对应频数
        {
            frequence[index]++;
        }
    }
    h_tree * T=CreateHuffman(vocabulary,frequence);//创建哈夫曼树
    vector<string> decode(vocabulary.length());//记录每个字符的编码
    encoding(T,vocabulary,decode);//编码
    ofstream fout;
    fout.open("table.txt");
    fout<<"char  freq  code"<<endl;
    for(int i=0;i<vocabulary.length();i++)//输出编码表
    {
        fout<<vocabulary[i]<<"     "<<frequence[i]<<"    "<<decode[i]<<endl;
    }
    compressibility(vocabulary,frequence,decode);//输出压缩率
    return 0;
}
