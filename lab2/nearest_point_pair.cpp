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

//比较
bool compareX(const point &A,const point &B)
{
    return A.x<B.x;
}
bool compareY(const point &A,const point &B)
{
    return A.y<B.y;
}

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
//分治算法
tuple<point,point,double> divide_compare(vector<point> P,vector<point> X,vector<point> Y)
{
    if(P.size()<=3)
        return directly_compare(P);
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
    tuple<point,point,double> res_L=divide_compare(P_L,X_L,Y_L);
    tuple<point,point,double> res_R=divide_compare(P_R,X_R,Y_R);
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
    return divide_compare(P,X,Y);
}
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