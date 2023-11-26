# lab7:最佳调度问题的回溯算法
## 实验目的
设计一个回溯算法找出完成$n$个任务的最佳调度,使完成全部任务的时间最早.
## 实验原理
### 1.回溯算法
每一层为一个任务分配一个机器,有$k$种分方案,选择其中一种然后扩展到下一层,直到所有任务都分配完毕,计算完成全部任务的时间,然后回溯到上一层,选择另一种方案,直到所有方案都尝试完毕.
### 2.剪枝策略
我设置了四个剪枝策略: 
1. 把所有的任务按完成时间从大到小排序然后再进入回溯算法,这样可以使得回溯算法的效率更高.
2. 我们先按贪心算法的思想,计算出贪心调度的时间作为基准时间,如果回溯算法的时间超过了基准时间,就不再继续回溯.
3. 在每次回溯时,如果当前完成时间已经超过了当前最优解的时间,就不再继续回溯.
4. 考虑到每个机器的效率是一样的,在分配前$k$个任务时,找到最优解只需要$\min \{k,n\}$个机器,据此可以在初期限制问题增长的规模.
## 实验代码
### 1.定义mtime类用于计时
```cpp
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <time.h>
#include <algorithm>
using namespace std;
//定义mtime类用于计时
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
### 2.定义任务类
```cpp
/*
 * @brief 任务
 * @param id 任务编号
 * @param cost 任务时间
 * @param machine 任务分配的机器
 */
class task
{
    public:
        int id;
        int cost;
        int machine;
        task():id(0),cost(0),machine(0){};
        task(int i,int t):id(i),cost(t),machine(0){};
        task(int i,int t,int m):id(i),cost(t),machine(m){};
        friend ostream &operator<<(ostream &os,const task &t);
        bool operator<(const task &t)const;
        bool operator>(const task &t)const;
};
ostream &operator<<(ostream &os,const task &t)
{
    os<<"Task"<<t.id<<":"<<t.cost<<" ";
    return os;
}
bool task::operator<(const task &t)const
{
    return this->cost<t.cost;
}
bool task::operator>(const task &t)const
{
    return this->cost>t.cost;
}
```
### 3.计算某个调度完成的时间
```cpp
/*
 * @brief 计算某个调度完成的总时间
 * @param arragement 调度策略
 * @param k 机器数
 */
int total_time(const vector<vector<task> > &arrangement,int k)
{
    int max=0;
    for(int i=1;i<=k;i++)
    {
        int temp=0;
        for(int j=0;j<arrangement[i-1].size();j++)
        {
            temp+=arrangement[i-1][j].cost;
        }
        if(temp>max)
            max=temp;
    }
    return max;
}
```
### 4.输出最优解
```cpp
/*
 * @brief 输出最优解
 * @param result 存放最优解
 * @param k 机器数
 * @param test 测试用例编号
 */
void Output(const vector<vector<task> > &result,int k,int test)
{
    //输出最优解
    string str=to_string(test);
    ofstream fout("result"+str+".txt");
    fout<<"Time:"<<total_time(result,k)<<endl;
    for(int i=0;i<result.size();i++)
    {
        fout<<"Machine "<<i+1<<":";
        for(int j=0;j<result[i].size();j++)
        {
            fout<<"Task"<<result[i][j].id<<",";
        }
        fout<<endl;
    }
}
```
### 5.贪心调度
```cpp
/*
 * @brief 按照先排序后贪心的策略进行调度
 * @param t 任务集合
 * @param arrangement 调度策略
 * @param k 机器数
 */
void Greedy(const vector<task> &t,vector<vector<task> > &arrangement,int k)
{
    //首先,对任务按照时间从小到大排序
    vector<task> temp(t);
    for(int i=0;i<temp.size();i++)
    {
        for(int j=i+1;j<temp.size();j++)
        {
            if(temp[j-1].cost>temp[j].cost)
            {
                swap(temp[j-1],temp[j]);
            }
        }
    }
    //然后,按照贪心策略进行调度
    int j=0;
    for(int i=1;i<=temp.size();i++)
    {
        if(i%k==1)
            j=0;
        temp[i-1].machine=j+1;
        arrangement[j].push_back(temp[i-1]);
        j++;
    }
}
```
### 6.回溯算法
```cpp
/*
 * @brief 限界函数:如果当前可行解的完成时间小于等于基准时间,则返回true,否则返回false
 * @param base 基准时间
 * @param solve 当前可行解
 * @param k 机器数
 */
bool Bound(int base,const vector<vector<task> > &solve,int k)
{
    return total_time(solve,k)<=base;
}
/*
 * @brief 回溯法求解
 * @param n 当前处理的任务序号
 * @param k 机器数
 * @param t 任务集合
 * @param result 存放所有可行解
 * @param solve 存放当前可行解
 * @param base 基准时间
 */
void Backtrack(int n,int k,vector<task> &t,vector<vector<task> > &result,vector<vector<task> > &solve,int base)
{
    if (!result.empty()&&total_time(result,k)<=total_time(solve,k)) return;
    if(n>t.size())
    {
        result=solve;
        return;
    }
    else
    {
        for(int i=1;i<=min(k,n);i++)
        {
            t[n-1].machine=i;
            solve[i-1].push_back(t[n-1]);//将第n个任务分配给第i台机器
            if(Bound(base,solve,k))
                Backtrack(n+1,k,t,result,solve,base);//递归求解
            solve[i-1].pop_back();//回溯
        }
    }
}
```
### 7.主函数
```cpp
int main()
{
    for(int i=0;i<3;i++)
    {
        string str=to_string(i+1);
        string path="test"+str+".txt";//文件路径
        int n;//任务数
        int k;//机器数
        ifstream fin(path);
        fin>>n>>k;
        vector<task> t(n);//存放任务
        int temp;
        for(int j=0;j<n;j++)
        {
            fin>>temp;
            t[j].id=j+1;
            t[j].cost=temp;
        }
        fin.close();
        sort(t.begin(),t.end(),greater<task>());//按照时间从大到小排序
        vector<vector<task> > greedy_arrange(k);//存放贪心策略
        Greedy(t,greedy_arrange,k);
        int base=total_time(greedy_arrange,k);//基准时间
        vector<vector<task> > result;//最优解
        vector<vector<task> > solve(k);//solve[i]表示第i+1台机器的任务
        mtime t_1;
        Backtrack(1,k,t,result,solve,base);
        mtime t_2;
        Output(result,k,i+1);
        cout<<"Test"<<str<<", Time: "<<t_2-t_1<<endl;
    }
    return 0;
}
```
## 实验结果
### 1.测试用例1
```
Time:112
Machine 1:Task1,Task6,Task2,
Machine 2:Task4,Task8,Task3,
Machine 3:Task10,Task7,Task9,Task5,
```
### 2.测试用例2
```
Time:182
Machine 1:Task6,Task11,
Machine 2:Task1,Task2,
Machine 3:Task8,Task9,Task5,
Machine 4:Task13,Task10,Task4,Task7,
Machine 5:Task14,Task3,Task12,Task15,
```
### 3.测试用例3
```
Time:126
Machine 1:Task5,Task3,
Machine 2:Task16,Task7,
Machine 3:Task12,Task9,
Machine 4:Task8,Task4,
Machine 5:Task6,Task10,
Machine 6:Task15,Task19,Task11,
Machine 7:Task14,Task2,Task13,
Machine 8:Task18,Task1,Task17,
```
## 实验总结
本次实验我学习了回溯算法的基本思想,我还学到了如何使用不同的剪枝,起初我只想到了用贪心法作为基准,但是在实验中我发现有很多解可以逼近贪心解的时间,最终我发现在初期限制问题增长的规模是一个很好的剪枝策略,这样可以使得回溯算法的效率更高.