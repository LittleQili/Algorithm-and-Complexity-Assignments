#include <vector>
#include <iostream>
#include <fstream>
#include <stack>
using namespace std;
//Please put this source code in the same directory with SCC.in
//And do NOT change the file name.
/*
This function computes the number of Strongly Connected Components in a graph
Args:
    n: The number of nodes in the graph. The nodes are indexed as 0~n-1
    edge: The edges in the graph. For each element (a,b) in edge, it means
          there is a directed edge from a to b
          Notice that there may exists multiple edge and self-loop
Return:
    The number of strongly connected components in the graph.
*/

stack<int> postnum;
bool isvisited[100005];
vector<pair<int,int>> _edge;

void DFS(int v){
    isvisited[v] = true;
    for(auto it = _edge.begin();it != _edge.end();++it){
        if(it->first == v){
            if(!isvisited[it->second]) DFS(it->second);
        }
    }
    postnum.push(v);
}
void DFS_Reverse(int v){
    isvisited[v] = true;
    for(auto it = _edge.begin();it != _edge.end();++it){
        if(it->second == v){
            if(!isvisited[it->first]) DFS_Reverse(it->first);
        }
    }
}

int SCC(int n, vector<pair<int,int>>& edge) {
    _edge = edge;

    for(int i = 0;i < n;++i) isvisited[i] = false;
    for(int i = 0;i < n;++i){
        if(!isvisited[i]) DFS(i);
    }

    for(int i = 0;i < n;++i) isvisited[i] = false;
    int SCCs = 0;
    while(!postnum.empty()){
        int tmpv = postnum.top();
        postnum.pop();
        if(!isvisited[tmpv]){
            DFS_Reverse(tmpv);
            ++SCCs;
        }
    }
    return SCCs;
}
//Please do NOT modify anything in main(). Thanks!
int main()
{
    int m,n;
    vector<pair<int,int>> edge;
    ifstream fin;
    ofstream fout;
    fin.open("SCC.in");
    fin>>n>>m;
    int tmp1,tmp2;
    for(int i=0;i<m;i++)
    {
        fin>>tmp1>>tmp2;
        edge.push_back(pair<int,int>(tmp1,tmp2));
    }
    fin.close();
    int ans=SCC(n,edge);
    fout.open("SCC.out");
    fout<<ans<<'\n';
    fout.close();
    return 0;
}
