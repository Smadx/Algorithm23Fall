#include <iostream>
#include <fstream>
#include <vector>
#include <time.h>
#include <algorithm>
#include <random>
using namespace std;
ifstream fin;
ofstream fout;
//��������
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
//�Ż���������
//��������
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
//����ȡ��
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
int advanced_partition(vector<int> &A,int p,int r)
{
    int m=mid(A,p,r);
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
void advanced_qsort(vector<int> &A,int p, int r)
{
    if((p<r)&&((r-p+1)>10))
    {
        int q=advanced_partition(A,p,r);
        advanced_qsort(A,p,q-1);
        advanced_qsort(A,q+1,r);
    }
    else if((r-p+1)<=10)
        insertion_sort(A,p,r);
}
//�����׼
int random_partition(vector<int> &A,int p,int r)
{
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(p,r);//����[p,r]�Ͼ��ȷֲ�������
    int m=dis(gen);//����[p,r]�ϵ����������Ϊ��׼
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
int main()
{
    clock_t start_1,start_2,start_3,start_4,end_1,end_2,end_3,end_4;
    //��ȡ����
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
    vector<int> temp_4=data;
    //�������sort�����������������ʱ��
    fout<<"����sort����"<<endl;
    start_1=clock();
    sort(temp_1.begin(),temp_1.end());
    end_1=clock();
    for(int i=0;i<count;i++)
    {
        fout<<temp_1[i]<<' ';
    }
    fout<<endl;
    cout<<"ʹ��sort����time="<<(double)(end_1-start_1)*1000/CLOCKS_PER_SEC<<"ms"<<endl;
    //�������quick_sort�����������������ʱ��
    fout<<"����quick_sort����"<<endl;
    start_2=clock();
    quick_sort(temp_2,0,temp_2.size()-1);
    end_2=clock();
    for(int i=0;i<count;i++)
    {
        fout<<temp_2[i]<<' ';
    }
    fout<<endl;
    cout<<"ʹ��quick_sort����time="<<(double)(end_2-start_2)*1000/CLOCKS_PER_SEC<<"ms"<<endl;
    //�������advanced_qsort�����������������ʱ��
    fout<<"����advanced_qsort����"<<endl;
    start_3=clock();
    advanced_qsort(temp_3,0,temp_3.size()-1);
    end_3=clock();
    for(int i=0;i<count;i++)
    {
        fout<<temp_3[i]<<' ';
    }
    fout<<endl;
    cout<<"ʹ��advanced_qsort����time="<<(double)(end_3-start_3)*1000/CLOCKS_PER_SEC<<"ms"<<endl;
    //�������random_qsort�����������������ʱ��
    fout<<"����random_qsort����"<<endl;
    start_4=clock();
    random_qsort(temp_4,0,temp_4.size()-1);
    end_4=clock();
    for(int i=0;i<count;i++)
    {
        fout<<temp_4[i]<<' ';
    }
    fout<<endl;
    cout<<"ʹ��random_qsort����time="<<(double)(end_4-start_4)*1000/CLOCKS_PER_SEC<<"ms"<<endl;
    return 0;
}