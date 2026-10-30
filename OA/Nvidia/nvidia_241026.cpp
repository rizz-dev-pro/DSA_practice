#include <iostream>
#include <vector>

using namespace std;

struct Edge
{
    int u, v, w;
    Edge(int u, int v, int w){
        this->u = u;
        this->v = v;
        this->w = w;
    }
    void to_string(){
        printf("%d ---(%d)---> %d\n", u, w, v);
    }
};

int main()
{
    vector<Edge> edges;

    int N; cin>>N;

    for (int i=0; i<N-1; ++i){
        int u, v, w;
        cin>>u; cin>>v; cin>>w;
        edges.push_back(Edge(u, v, w));
    }

    for (int i=0; i<N-1; ++i){
        edges.at(i).to_string();
    }

    
}

void dp()
