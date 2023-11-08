# lab5:最长公共子序列(LCS)
## 实验目的
1. 编程实现最长公共子序列（LCS）算法，并理解其核心思想。
2. 时间复杂度 O(mn)，空间复杂度 O(mn)，求出 LCS 及其长度。
3. 时间复杂度 O(mn)，空间复杂度 O(2*min(m,n))，求出 LCS 的长度。
4. 时间复杂度 O(mn)，空间复杂度 O(min(m,n))，求出 LCS 的长度
## 实验原理
采用动态规划,用递推公式建立LCS,由于计算表单的下一行只需要当前行和上一行的数据,可以进行优化
## 实验代码
### 1.定义表单项
```cpp
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;
enum state{NIL,UP,UP_LEFT,LEFT};
class Form_item
{
    public:
        int length;//记录LCS的长度
        int arrow;//指向LCS的下一个元素
        Form_item();
        Form_item(int Length,int Arrow);
        void write_arrow(int Arrow);
        Form_item operator+(int a);
        bool operator>=(Form_item B);
};
Form_item::Form_item()
{
    length=0;
    arrow=NIL;
}
Form_item::Form_item(int Length,int Arrow)
{
    length=Length;
    arrow=Arrow;
}
void Form_item::write_arrow(int Arrow)
{
    arrow=Arrow;
}
Form_item Form_item::operator+(int a)
{
    Form_item result(length+a,arrow);
    return result;
}
bool Form_item::operator>=(Form_item B)
{
    if(length>=B.length)
        return true;
    return false;
}
```
### 2.计算LCS的长度
```cpp
void LCS_Length(string X,string Y,vector<vector<Form_item> > &C)
{
    int m=X.length(),n=Y.length();
    C.resize(m+1);
    for(int i=0;i<C.size();i++)
    {
        C[i].resize(n+1);
    }
    Form_item item;
    for(int i=1;i<=m;i++)
    {
        C[i][0]=item;
    }
    for(int i=0;i<=n;i++)
    {
        C[0][i]=item;
    }
    for(int i=1;i<=m;i++)
    {
        for(int j=1;j<=n;j++)
        {
            if(X[i-1]==Y[j-1])
            {
                C[i][j]=C[i-1][j-1]+1;
                C[i][j].write_arrow(UP_LEFT);
            }
            else 
            {
                if(C[i-1][j]>=C[i][j-1])
                {
                    C[i][j]=C[i-1][j];
                    C[i][j].write_arrow(UP);
                }
                else
                {
                    C[i][j]=C[i][j-1];
                    C[i][j].write_arrow(LEFT);
                }
            }
        }
    }
}
```
### 3.构造LCS
```cpp
void Create_LCS(const vector<vector<Form_item> > &C,string X,int i,int j,string &LCS)
{
    if((i==0)||(j==0))
        return;
    if(C[i][j].arrow==UP_LEFT)
    {
        Create_LCS(C,X,i-1,j-1,LCS);
        LCS=LCS+X[i-1];
    }
    else
    {
        if(C[i][j].arrow==UP)
            Create_LCS(C,X,i-1,j,LCS);
        else
            Create_LCS(C,X,i,j-1,LCS);
    }
}
```
### 4.存储优化
```cpp
int advanced_LCS_length(string X,string Y)
{
    int m,n,l;
    m=X.length();
    n=Y.length();
    l=min(m,n);
    vector<int> A(l+1),B(l+1);
    if(l==m)
    {
        for(int i=1;i<=n;i++)
        {
            for(int j=1;j<=l;j++)
            {
                if(Y[i-1]==X[j-1])
                    B[j]=A[j-1]+1;
                else
                    B[j]=max(A[j],B[j-1]);
            }
            A=B;
        }
    }
    else
    {
        for(int i=1;i<=m;i++)
        {
            for(int j=1;j<=l;j++)
            {
                if(X[i-1]==Y[j-1])
                    B[j]=A[j-1]+1;
                else
                    B[j]=max(A[j],B[j-1]);
            }
            A=B;
        }
    }
    return B[l];
}
int advancedd_LCS_length(string X,string Y)//利用B[0]保存上一行的内容
{
    int m,n,l,temp;
    m=X.length();
    n=Y.length();
    l=min(m,n);
    vector<int> B(l+1);
    if(l==m)
    {
        for(int i=1;i<=n;i++)
        {
            for(int j=1;j<=l;j++)
            {
                temp=B[0];
                B[0]=B[j];
                if(Y[i-1]==X[j-1])
                {
                    B[j]=temp+1;
                }
                else
                    B[j]=max(B[j],B[j-1]);
            }
        }
    }
    else
    {
        for(int i=1;i<=m;i++)
        {
            for(int j=1;j<=l;j++)
            {
                temp=B[0];
                B[0]=B[j];
                if(X[i-1]==Y[j-1])
                    B[j]=temp+1;
                else
                    B[j]=max(B[j],B[j-1]);
            }
        }
    }
    return B[l];
}
```
### 5.主函数
```cpp
int main()
{
    ifstream fin;
    fin.open("data.txt");
    string str1,str2;
    getline(fin,str1);
    getline(fin,str2);
    vector<vector<Form_item> > C;
    LCS_Length(str1,str2,C);
    string LCS;
    Create_LCS(C,str1,str1.length(),str2.length(),LCS);
    cout<<"LCS:"<<LCS<<",  length:"<<LCS.length()<<endl;
    cout<<"length:"<<advanced_LCS_length(str1,str2)<<endl;
    cout<<"length:"<<advancedd_LCS_length(str1,str2)<<endl;
    return 0;
}
```
## 实验结果
![](结果.png)
## 总结
本次实验让我熟悉了动态规划方法,锻炼了存储优化能力