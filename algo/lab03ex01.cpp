#include <iostream>
#include <vector>
#include <map>

using namespace std;

/*
 * Union/Find algorithm by GeeksforGeeks.
 * @see https://www.geeksforgeeks.org/union-find/
 */
// A utility function to find the subset of an element i
int find(int parent[], int i)
{
    if (parent[i] == -1)
        return i;
    return find(parent, parent[i]);
}

// A utility function to do union of two subsets
void Union(int parent[], int x, int y)
{
    int xset = find(parent, x);
    int yset = find(parent, y);
    if(xset != yset)
    {
        parent[xset] = yset;
    }
}

class distances {
public:
    int N, M;
    distances();
    pair<int, int> *edges;
    map<int, int> *mst;
    int *spEdges;
    // Helper functions and variables.
    int *parent;
    bool isCycle(vector<pair<int, int>>);
    void buildMST();
    vector<int> dfs_init(int);
    void dfs(bool*, int, int*);
    void calculateSPs();
    int getBinarySPs();
};

distances::distances() {
    cin >> N >> M;

    edges = new pair<int, int>[M];
    mst = new map<int, int>[N];
    parent = new int[N];
    spEdges = new int[M];
    for (int i = 0; i < M; i++) {
        int a, b, c;
        cin >> a >> b >> c;
        edges[c] = make_pair(a, b);
    }
}

bool distances::isCycle(vector<pair<int, int>> mstEdges) {
    int subsets[N];
    for (int i = 0; i < N; i++) {
        subsets[i] = -1;
    }

    for (int i = 0; i < mstEdges.size(); i++) {
        int subsetSource = find(subsets, mstEdges[i].first - 1);
        int subsetDest = find(subsets, mstEdges[i].second - 1);
        if (subsetSource == subsetDest)
            return true;
        Union(subsets, subsetSource, subsetDest);
    }
    return false;
}

void distances::buildMST() {
    vector<pair<int, int>> mstEdges;
    for (int i = 0; i < M; i++) {
        mstEdges.emplace_back(edges[i].first, edges[i].second);
        if (isCycle(mstEdges)) {
            mstEdges.pop_back();
        } else {
            // Also add the Edge to the mst array to use in the later DFS.
            mst[edges[i].first - 1].insert(make_pair(edges[i].second - 1, i));
            mst[edges[i].second - 1].insert(make_pair(edges[i].first - 1, i));
        }
    }
}

void distances::dfs(bool visited[], int s, int degree[]) {
    visited[s] = true;
    for (pair<int,int> p : mst[s]) {
        if (visited[p.first])
            continue;
        parent[p.first] = s;
        dfs(visited, p.first, degree);
        degree[s] += degree[p.first];
    }
    degree[s]++;
    if (s != 0) {
        spEdges[mst[s][parent[s]]] = (N - degree[s]) * degree[s];
    }
}

vector<int> distances::dfs_init(int s) {
    vector<int> dfsOrder;
    bool visited[N];
    int degree[N];
    for (int i = 0; i < N; i++)
        visited[i] = false, degree[i] = 0;
    dfs(visited, 0, degree);
    return dfsOrder;
}

//void distances::calculateSPs() {
//    vector<int> dfsOrder = dfs_init(0);
//    cout << dfsOrder.size();
////    for (int i = 0; i < N; i++) {
////        cout << dfsOrder[i] << endl;
////    }
//}

int distances::getBinarySPs() {
    int binSum = 0;
    for (int i = 0; i < M-1; i++) {
        int edge = spEdges[i];
        spEdges[i] = edge % 2;
        spEdges[i+1] += (edge / 2);
    }

    // Special handling for the last element.
    string binEdge;
    while (spEdges[M-1] >= 2) {
        binEdge += to_string(spEdges[M-1] % 2);
        spEdges[M-1] = spEdges[M-1] / 2;
    }
    cout << spEdges[M-1] << binEdge;
    for (int i = M-2; i >= 0; i--)
        cout << spEdges[i];
    cout << endl;
    return binSum;
}

int main() {
    distances dist;
    dist.buildMST();
    dist.dfs_init(0);
    dist.getBinarySPs();
//    cout << dist.getBinarySPs() << endl;

    return 0;
}
