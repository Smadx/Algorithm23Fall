# lab2:平面最邻近点对问题
## 实验目的
编程实现求平面上n个顶点的最邻近点对问题
## 实验原理
### 1.预备
定义``mtime``类用于计算运行时间,定义``point``类来描述平面上的点.
### 2.暴力算法
令两个数组A,B,其中A[i]记录i后与P[i]距离最近的点的距离,B[i]记录i后与P[i]距离最近的点,对于每一个点,从它开始向后遍历,记录它与后面点之间的最小值(A[i]),和最小值出现的位置(B[i]),此时最邻近点对的距离必定包含在A中,遍历A,输出最小值和对应的B[i].
### 3.分治算法
首先分别对X,Y按横纵坐标排序,然后进入递归程序.在递归开始前,首先检查点集的大小是否$\le 3$,若是,则调用暴力算法求解,否则,以横坐标的中间值为标准做划分,分别为P_L,P_R,X_L,X_R,Y_L,Y_R,做递归,递归结束后的输出记为$detla(p_1,p_2,distance)$,此时对分割线附近的点做处理,考虑与线的距离小于$delta.distance$的点集Y',对于Y'中每个点,计算其与后7个点之间的最短距离,并输出这些点中的最邻近点对,记为$delta'(p_3,p_4,distance')$,最后输出二者间的最小值.
## 实验代码
### 1.预备
预备操作,导入需要的库,定义``mtime``类和``point``类,并定义按坐标的比较函数
```cpp
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cmath>
#include <vector>
#include <time.h>
#include <algorithm>
using namespace std;

//定义mtime类用于计时
class mtime
{
    public:
        clock_t t;//时刻
        mtime();
        mtime(clock_t tt);
        mtime operator-(mtime t_2);
        friend ostream &operator<<(ostream &os,const mtime &t);//以毫秒为单位输出
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

//定义平面上的点类
class point
{
    public:
        int num;//编号
        double x;//横坐标
        double y;//纵坐标
        point();
        point(int n,int xx,int yy);
        double distance(const point &B);//求与B点的距离
        friend istream &operator>>(istream &is,point &A);
        friend ostream &operator<<(ostream &os,const point &A);
        friend ifstream &operator>>(ifstream &ifs,point &A);
        friend ofstream &operator<<(ofstream &ofs,const point &A);
};

point::point()
{
    num=-1;
    x=0;
    y=0;
}
point::point(int n,int xx,int yy)
{
    num=n;
    x=xx;
    y=yy;
}
double point::distance(const point &B)
{
    return sqrt(((B.x-x)*(B.x-x)+(B.y-y)*(B.y-y)));
}
istream & operator>>(istream &is,point &A)
{
    is>>A.num>>A.x>>A.y;
    return is;
}
ostream & operator<<(ostream &os,const point &A)
{
    os<<A.num<<" "<<A.x<<" "<<A.y;
    return os;
}
ifstream & operator>>(ifstream &ifs,point &A)
{
    ifs>>A.num>>A.x>>A.y;
    return ifs;
}
ofstream & operator<<(ofstream &ofs,const point &A)
{
    ofs<<A.num<<" "<<A.x<<" "<<A.y;
    return ofs;
}

//按横坐标比较
bool compareX(const point &A,const point &B)
{
    return A.x<B.x;
}
//按纵坐标比较
bool compareY(const point &A,const point &B)
{
    return A.y<B.y;
}
```
### 2.暴力算法
采用直接比较的方式求最邻近点对,以``tuple``类型输出
```cpp
//暴力算法
tuple<point,point,double> directly_compare(vector<point> P)
{
    vector<double> A(P.size());//A[i]记录i后与P[i]距离最近的点的距离
    vector<int> B(P.size());//B[i]记录i后与P[i]距离最近的点
    for(int i=0;i<P.size()-1;i++)//计算A与B
    {
        double min=P[i].distance(P[i+1]);
        A[i]=min;
        B[i]=i+1;
        for(int j=i+2;j<P.size();j++)
        {
            if(P[i].distance(P[j])<min)
            {
                min=P[i].distance(P[j]);
                A[i]=min;
                B[i]=j;
            }
        }
    }
    A[A.size()-1]=A[A.size()-2];//设置最后一位
    B[B.size()-1]=P.size()-1;
    double min=A[0];
    int m=0;
    for(int i=1;i<A.size();i++)//从A[i]中找到最短距离
    {
        if(A[i]<min)
        {
            min=A[i];
            m=i;
        }
    }
    return make_tuple(P[m],P[B[m]],A[m]);
}
```
### 3.分治算法
分支算法对规模足够小的子问题暴力求解,对于规模比较大的问题进行递归划分,最后处理在分割线附近的点,并输出最小值
```cpp
//分治算法
tuple<point,point,double> divide_compare(vector<point> P,vector<point> X,vector<point> Y)
{
    //判断规模
    if(P.size()<=3)
        return directly_compare(P);
    //划分
    double line=X[X.size()/2].x;
    vector<point> P_L,P_R,X_L,X_R,Y_L,Y_R;
    for(int i=0;i<P.size();i++)
    {
        if(P[i].x<line)
            P_L.push_back(P[i]);
        else
            P_R.push_back(P[i]);
    }
    for(int i=0;i<X.size();i++)
    {
        if(X[i].x<line)
            X_L.push_back(X[i]);
        else
            X_R.push_back(X[i]);
    }
    for(int i=0;i<Y.size();i++)
    {
        if(Y[i].x<line)
            Y_L.push_back(Y[i]);
        else
            Y_R.push_back(Y[i]);
    }
    //递归解决
    tuple<point,point,double> res_L=divide_compare(P_L,X_L,Y_L);
    tuple<point,point,double> res_R=divide_compare(P_R,X_R,Y_R);
    //处理分割线附近的点
    tuple<point,point,double> delta,delta_prime;
    if(get<2>(res_L)<get<2>(res_R))
        delta=res_L;
    else
        delta=res_R;
    //建立Y'
    vector<point> Y_prime;
    for(int i=0;i<P.size();i++)
    {
        if(( Y[i].x >= (line-get<2>(delta)) ) && ( Y[i].x <= (line+get<2>(delta)) ))
            Y_prime.push_back(Y[i]);
    }
    if (Y_prime.size()<=1) return delta;
    //记录Y'中所有最近点对的距离和delta_prime
    vector<double> A(Y_prime.size());//A[i]记录i后7个点中最近点的距离
    vector<int> B(Y_prime.size());//B[i]记录Y'[i]后7个点中的最近点
    int m=0;
    for(int i=0;i+1<Y_prime.size();i++)
    {
        double min=Y_prime[i].distance(Y_prime[i+1]);
        m=i+1;
        for(int j=i+2;( j <= (i+7) ) && ( j < Y_prime.size() );j++)
        {
            if(Y_prime[i].distance(Y_prime[j])<min)
            {
                min=Y_prime[i].distance(Y_prime[j]);
                m=j;
            }
        }
        A[i]=min;
        B[i]=m;
    }
    double min=A[0];
    m=0;
    for(int i=1;i+1<A.size();i++)
    {
        if(A[i]<min)
        {
            min=A[i];
            m=i;
        }
    }
    //得到delta_prime
    delta_prime=make_tuple(Y_prime[m],Y_prime[B[m]],A[m]);
    //输出最邻近点对
    if(get<2>(delta_prime)<get<2>(delta))
        return delta_prime;
    else
        return delta;

}
tuple<point,point,double> divide_nearest_point_pair(vector<point> P)
{
    //预排序
    vector<point> X=P,Y=P;
    sort(X.begin(),X.end(),compareX);
    sort(Y.begin(),Y.end(),compareY);
    //进入递归程序
    return divide_compare(P,X,Y);
}
```
### 4.主函数
``main``函数导入数据后分别调用暴力算法和分治算法求解最邻近点对问题,并输出运行时间
```cpp
int main()
{
    //导入数据
    ifstream ifs;
    ifs.open("data.txt");
    vector<point> P;
    point A;
    if(!ifs)
    {
        cout<<"Error:failed to read data.txt"<<endl;
        exit(1);
    }
    while(ifs)
    {
        ifs>>A;
        P.push_back(A);
    }
    P.pop_back();//弹出重复项
    //暴力求解
    mtime t_1;
    tuple<point,point,double> result_1=directly_compare(P);
    cout<<fixed<<setprecision(3)<<get<0>(result_1)<<endl;
    cout<<fixed<<setprecision(3)<<get<1>(result_1)<<endl;
    cout<<"distance:"<<fixed<<setprecision(3)<<get<2>(result_1)<<endl;
    mtime t_2;
    cout<<"It took "<<t_2-t_1<<" to solve the problem using direct compare"<<endl;
    //分治算法
    mtime t_3;
    tuple<point,point,double> result_2=divide_nearest_point_pair(P);
    cout<<fixed<<setprecision(3)<<get<0>(result_2)<<endl;
    cout<<fixed<<setprecision(3)<<get<1>(result_2)<<endl;
    cout<<"distance:"<<fixed<<setprecision(3)<<get<2>(result_2)<<endl;
    mtime t_4;
    cout<<"It took "<<t_4-t_3<<" to solve the problem using divide compare"<<endl;
    return 0;
}
```
## 实验结果与分析
![](result.png)  
可以看到,使用直接比较算法耗时更长，而使用分治算法耗时更短，分治算法的运行时间仅为暴力算法的$16\%$,效率大大提升。这是因为分治算法在每一步都将问题划分为较小的子问题，从而减少了需要比较的点对数量。相比之下，直接比较算法需要检查所有可能的点对，因此时间复杂度较高。  
这表明分治算法在处理大规模数据集时具有较好的可扩展性和效率
由于在第一次递归之前做了预排序,分治算法的时间复杂度为$O(n\lg n)$
