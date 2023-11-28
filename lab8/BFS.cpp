#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
using namespace std;
enum Color{WHITE,GRAY,BLACK};
/*图,邻接矩阵*/
class Graph
{
    public:
        int V; // 顶点数
        vector<vector<int> > M;//邻接矩阵
        vector<char> alphabet;//顶点集
        vector<Color> color;//顶点颜色
        Graph();
        Graph(int V,vector<char> alphabet);//根据顶点数和顶点集初始化
        int locateVex(char v);//返回顶点v在顶点集中的位置
        void addEdge(char v,char w);//添加边
        void BFS(char s);//广度优先搜索
};
/*
 * @brief 无向图构造函数
 */
Graph::Graph()
{
    V=0;
    M=vector<vector<int> >();
    alphabet=vector<char>();
    color=vector<Color>();
}
/*
 * @brief 无向图构造函数
 * @param num 顶点数
 * @param nodes 顶点集
 */
Graph::Graph(int num,vector<char> nodes)
{
    V=num;
    M=vector<vector<int> >(V,vector<int>(V,0));
    alphabet=nodes;
    color=vector<Color>(V,WHITE);
}
/*
 * @brief 返回顶点v在顶点集中的位置
 * @param v 顶点
 * @return 顶点v在顶点集中的位置
 */
int Graph::locateVex(char v)
{
    int i=0;
    for(;i<V;i++)
    {
        if(alphabet[i]==v)
            break;
    }
    return i;
}
/*
 * @brief 添加边
 * @param v 顶点
 * @param w 顶点
 */
void Graph::addEdge(char v,char w)
{
    int i=locateVex(v);
    int j=locateVex(w);
    M[i][j]=1;
    M[j][i]=1;
}
/*
 * @brief 广度优先搜索
 * @param s 搜索起点
 */
void Graph::BFS(char s)
{
    int i=locateVex(s);
    color[i]=GRAY;
    queue<int> Q;
    Q.push(i);
    while(!Q.empty())
    {
        int u=Q.front();
        Q.pop();
        cout<<alphabet[u]<<" ";
        for(int v=0;v<V;v++)
        {
            if(M[u][v]==1&&color[v]==WHITE)
            {
                color[v]=GRAY;
                Q.push(v);
            }
        }
        color[u]=BLACK;
    }
}
int main()
{
    ifstream fin("data.txt");
    int V;
    char ch;
    vector<char> alphabet;
    while((ch=fin.get())!='\n')
    {
        if(ch!=',')
        {
            V++;
            alphabet.push_back(ch);
        }
    }
    Graph G(V,alphabet);
    char v,w,temp;
    while(!fin.eof())
    {
        fin>>v>>temp>>w;
        G.addEdge(v,w);
    }
    G.BFS('A');
    return 0;
}
