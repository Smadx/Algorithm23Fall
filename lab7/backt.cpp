#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;
class task
{
    public:
        int id;//任务编号
        int cost;//任务时间
        int machine;//任务分配的机器
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
        cout<<base<<endl;
        vector<vector<task> > result;//最优解
        vector<vector<task> > solve(k);//solve[i]表示第i+1台机器的任务
        Backtrack(1,k,t,result,solve,base);
        Output(result,k,i+1);
    }
    return 0;
}
