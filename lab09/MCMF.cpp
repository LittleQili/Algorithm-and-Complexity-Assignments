#include <algorithm>
#include <queue>
#include <vector>
#include <stack>
#include <iostream>
#include <fstream>
using namespace std;

struct Myedge{
    int src,dst;
    int c,a;
    int flow;
    Myedge(int xsrc,int xdst,int xc,int xa,int xflow)
        :src(xsrc),dst(xdst),c(xc),a(xa),flow(xflow){};
};

struct MyMCMF{
    vector<Myedge> edge;
    bool isvisited[100005];
    int n,m,s,t;
    int flow,cost;
    stack<int> shortestpath;
    vector<int> Graph[100005];
    inline void inputedge(int xsrc,int xdst,int xc,int xa){
        edge.push_back(Myedge(xsrc,xdst,xc,xa,0));
        edge.push_back(Myedge(xdst,xsrc,0,0 - xa,0));
        int tmpm = edge.size();
        Graph[xsrc].push_back(tmpm - 2);
        Graph[xdst].push_back(tmpm - 1);
    }
    bool findpath(){
        int M[n+1],prev[n+1];
        bool updated[n+1];
        for(int i = 1;i <=n;++i){
            M[i] = 0x0FFFFFFF;
            prev[i] = n+1;
            updated[i] = false;
        }
        M[s] = 0,updated[s] = true;
        bool isloopupdated;
        for(int i = 1;i < n;++i){
            isloopupdated = false;
            for(int u = 1;u <= n;++u){
                if(updated[u]){
                    for(auto j = edge.begin();j != edge.end();++j){
                        if(j->src == u){
                            if(M[j->dst]> M[u] + j->a){
                                M[j->dst]= M[u] + j->a;
                                prev[j->dst] = u;
                                updated[j->dst] = true;
                                isloopupdated = true;
                            }
                        }
                    }
                }
            }
            if(!isloopupdated) break;
        }
        while(!shortestpath.empty())shortestpath.pop();
        shortestpath.push(t);
        while(prev[shortestpath.top()] != n+1)
            shortestpath.push(prev[shortestpath.top()]);
        if(shortestpath.top() != s) return false;
        return true;
    }
    bool Changeflow(){
        int M[n+2],_flow[n+2],_path[n+2];
        for(int i = 0;i <= n;++i )isvisited[i] = false;
        for (int i = 0; i <= n; i++)M[i] = 0x0FFFFFFF;
        isvisited[s] = 1; M[s] = 0; _flow[s] = 0x0FFFFFFF; _path[s] = 0;
        queue<int> _quev;
        _quev.push(s);

        //findpath();
        while (!_quev.empty()) {
            int u = _quev.front();
            _quev.pop();
            isvisited[u] = false;
            for (int i = 0; i < Graph[u].size(); i++) {
                Myedge &tmpe = edge[Graph[u][i]];
                if (tmpe.c > tmpe.flow && M[tmpe.dst] > M[u] + tmpe.a) {
                    M[tmpe.dst] = M[u] + tmpe.a;
                    _path[tmpe.dst] = Graph[u][i];
                    _flow[tmpe.dst] = min(_flow[u], tmpe.c - tmpe.flow);
                    if (!isvisited[tmpe.dst]) {
                        isvisited[tmpe.dst] = true;
                        _quev.push(tmpe.dst);
                    }
                }
            }
        }

        if (M[t] == 0x0FFFFFFF) return false;
        cost += M[t] * _flow[t];
        flow += _flow[t];
        int u = t;
        while (u != s) {
            edge[_path[u]].flow += _flow[t];
            edge[_path[u] ^ 1].flow -= _flow[t];
            u = edge[_path[u]].src;
        }
        return true;
    }
};
MyMCMF mcmf;
int main() {
    ifstream fin;
    ofstream fout;
    fin.open("MCMF.in");
    fin>>mcmf.n>>mcmf.m>>mcmf.s>>mcmf.t;
    int xsrc,xdst, xc, xa;
    for(int i=0;i<mcmf.m;i++)
    {
        fin>>xsrc>>xdst>>xc>>xa;
        mcmf.inputedge(xsrc,xdst,xc,xa);
    }
    fin.close();
    mcmf.flow = 0,mcmf.cost = 0;
    while(mcmf.Changeflow());
    cout << mcmf.flow << ' ' << mcmf.cost;
    fout.open("MCMF.out");
    fout<<mcmf.flow << ' ' << mcmf.cost <<'\n';
    fout.close();
    return 0;
}
