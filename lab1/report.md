# lab1:快速排序与优化
## 实验目的
1. 实现快速排序算法
2. 对快速排序进行优化
## 实验原理
### 1.文件的读取
采用``fstream``库完成文件I/O操作
```cpp
ifstream fin;
ofstream fout;
```
### 2.运行时间的计算
采用``time.h``来计算时间
```cpp
clock_t start,end;
start=clock();
func();//需要计算运行时间的函数
end=clock();
cout<<"time="<<(double)(end-start)*1000/COLCKS_PER_SEC<<"ms"<<endl;//转换单位
```
### 3.优化思路
1. 采用三数取中的方式确定划分的基准
2. 当递归的子数组大小小于10时,采用插入排序
## 实验代码
导入需要的库
```cpp
#include <iostream>
#include <fstream>
#include <vector>
#include <time.h>
#include <algorithm>
#include <random>
```
### 1.读取数据与预处理
首先设置几个时刻变量用于稍后计算时间,接着利用``fin``读取数据,最后拷贝三份数据分别用于接下来的排序
```cpp
    clock_t start_1,start_2,start_3,end_1,end_2,end_3;
    //读取数据
    fin.open("data.txt");
    fout.open("sorted.txt");
    if(!fin)
    {
        cout<<"Error:Failed to read data.txt"<<endl;
        exit(1);
    }
    int count;//数组大小
    fin>>count;
    vector<int> data;
    int value;
    for(int i=0;i<count;i++)
    {
        fin>>value;
        data.push_back(value);
    }
    fin.close();
    vector<int> temp_1=data;//调用sort
    vector<int> temp_2=data;//调用quick_sort
    vector<int> temp_3=data;//调用advanced_qsort
```
### 2.调用sort函数进行排序
在``sort``函数前后插入时刻变量记录当前时刻,最后输出运行时间
```cpp
    //计算调用sort函数进行排序的运行时间
    fout<<"调用sort函数"<<endl;
    start_1=clock();
    sort(temp_1.begin(),temp_1.end());
    end_1=clock();
    for(int i=0;i<count;i++)
    {
        fout<<temp_1[i]<<' ';//输出到文件
    }
    fout<<endl;
    cout<<"使用sort函数time="<<(double)(end_1-start_1)*1000/CLOCKS_PER_SEC<<"ms"<<endl;
```
### 3.调用quick_sort进行排序
原版快速排序算法,(固定基准)
```cpp
    //快速排序
    int partition(vector<int> &A,int p,int r)
    {
        int x=A[r];
        int i=p-1;
        for(int j=p;j<r;j++)
        {
            if(A[j]<=x)
            {
                i++;
                swap(A[i],A[j]);
            }
        }
        swap(A[i+1],A[r]);
        return i+1;
    }
    void quick_sort(vector<int> &A,int p,int r)
    {
        if(p<r)
        {
            int q=partition(A,p,r);
            quick_sort(A,p,q-1);
            quick_sort(A,q+1,r);
        }
    }
```
调用函数,输出结果和运行时间
```cpp
    //计算调用quick_sort函数进行排序的运行时间
    fout<<"调用quick_sort函数"<<endl;
    start_2=clock();
    quick_sort(temp_2,0,temp_2.size()-1);
    end_2=clock();
    for(int i=0;i<count;i++)
    {
        fout<<temp_2[i]<<' ';
    }
    fout<<endl;
    cout<<"使用quick_sort函数time="<<(double)(end_2-start_2)*1000/CLOCKS_PER_SEC<<"ms"<<endl;
```
### 4.调用advanced_qsort进行排序
三数取中
```cpp
    int mid(vector<int> &A,int p,int r)
    {
        int mid=(p+r)/2;
        if(A[p]>A[mid])
            swap(A[p],A[mid]);
        if(A[p]>A[r])
            swap(A[p],A[r]);
        if(A[mid]>A[r])
            swap(A[mid],A[r]);
        return mid;
    }
```
插入排序
```cpp
    void insertion_sort(vector<int> &A,int p,int r)
    {

        int i,j,key;
        for(j=p+1;j<=r;j++)
        {
            key=A[j];
            i=j-1;
            while((i>=0)&&(A[i]>key))
            {
                A[i+1]=A[i];
                i--;
            }
            A[i+1]=key;
        }
    }
```
优化后的快速排序
```cpp
    int advanced_partition(vector<int> &A,int p,int r)
    {
        int m=mid(A,p,r);//三数取中选择基准
        int x=A[m];
        int i=p-1;
        for(int j=p;j<r;j++)
        {
            if(A[j]<=x)
            {
                i++;
                swap(A[i],A[j]);
            }
        }
        swap(A[i+1],A[m]);
        return i+1;
    }
    void advanced_qsort(vector<int> &A,int p, int r)
    {
        if((p<r)&&((r-p+1)>10))
        {
            int q=advanced_partition(A,p,r);
            advanced_qsort(A,p,q-1);
            advanced_qsort(A,q+1,r);
        }
        else if((r-p+1)<=10)
            insertion_sort(A,p,r);//当子数组较小时,使用插入排序
    }
```
输出结果和运行时间
```cpp
    //计算调用advanced_qsort函数进行排序的运行时间
    fout<<"调用advanced_qsort函数"<<endl;
    start_3=clock();
    advanced_qsort(temp_3,0,temp_3.size()-1);
    end_3=clock();
    for(int i=0;i<count;i++)
    {
        fout<<temp_3[i]<<' ';
    }
    fout<<endl;
    cout<<"使用advanced_qsort函数time="<<(double)(end_3-start_3)*1000/CLOCKS_PER_SEC<<"ms"<<endl;
```
### 5.随机基准
```cpp
int random_partition(vector<int> &A,int p,int r)
{
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(p,r);//产生[p,r]上均匀分布的整数
    int m=dis(gen);//产生[p,r]上的随机整数作为基准
    int x=A[m];
    int i=p-1;
    for(int j=p;j<=r;j++)
    {
        if(j==m)
            continue;
        if(A[j]<=x)
        {
            i++;
            swap(A[i],A[j]);
        }
    }
    swap(A[i+1],A[m]);
    return i+1;
}
void random_qsort(vector<int> &A,int p, int r)
{
    if((p<r)&&((r-p+1)>10))
    {
        int q=random_partition(A,p,r);
        random_qsort(A,p,q-1);
        random_qsort(A,q+1,r);
    }
    else if((r-p+1)<=10)
        insertion_sort(A,p,r);
}
```
### 6.主函数
```cpp
int main()
{
    clock_t start_1,start_2,start_3,end_1,end_2,end_3;
    //读取数据
    fin.open("data.txt");
    fout.open("sorted.txt");
    if(!fin)
    {
        cout<<"Error:Failed to read data.txt"<<endl;
        exit(1);
    }
    int count;
    fin>>count;
    vector<int> data;
    int value;
    for(int i=0;i<count;i++)
    {
        fin>>value;
        data.push_back(value);
    }
    fin.close();
    vector<int> temp_1=data;
    vector<int> temp_2=data;
    vector<int> temp_3=data;
    //计算调用sort函数进行排序的运行时间
    fout<<"调用sort函数"<<endl;
    start_1=clock();
    sort(temp_1.begin(),temp_1.end());
    end_1=clock();
    for(int i=0;i<count;i++)
    {
        fout<<temp_1[i]<<' ';
    }
    fout<<endl;
    cout<<"使用sort函数time="<<(double)(end_1-start_1)*1000/CLOCKS_PER_SEC<<"ms"<<endl;
    //计算调用quick_sort函数进行排序的运行时间
    fout<<"调用quick_sort函数"<<endl;
    start_2=clock();
    quick_sort(temp_2,0,temp_2.size()-1);
    end_2=clock();
    for(int i=0;i<count;i++)
    {
        fout<<temp_2[i]<<' ';
    }
    fout<<endl;
    cout<<"使用quick_sort函数time="<<(double)(end_2-start_2)*1000/CLOCKS_PER_SEC<<"ms"<<endl;
    //计算调用advanced_qsort函数进行排序的运行时间
    fout<<"调用advanced_qsort函数"<<endl;
    start_3=clock();
    advanced_qsort(temp_3,0,temp_3.size()-1);
    end_3=clock();
    for(int i=0;i<count;i++)
    {
        fout<<temp_3[i]<<' ';
    }
    fout<<endl;
    cout<<"使用advanced_qsort函数time="<<(double)(end_3-start_3)*1000/CLOCKS_PER_SEC<<"ms"<<endl;
    return 0;
}
```
### 实验结果与分析
![](run_time.png)
经过多次实验发现前三个快速排序算法的运行时间每次都不同,且差别也不大,明显快于随机基准,说明算法实现的效果与标准库较接近。 
另一方面,优化的效果不明显很可能与数据集规模有关,换用更大的数据集可能运行时间区别较明显。