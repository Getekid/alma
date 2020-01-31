#include <iostream>
#include <vector>
#include <map>

using namespace std;

int find(int parent[], int i)
{
    int p;
    while (parent[i] != -1) {
        p = parent[i];
        // Check the parent and assign its parent for this node
        // to avoid future recursions.
        if (parent[p] != -1)
            parent[i] = parent[p];
        i = parent[i];
    }
    return i;
}

/*
 * Union algorithm by GeeksforGeeks.
 * @see https://www.geeksforgeeks.org/union-find/
 */
// A utility function to do union of two subsets
//void Union(int parent[], int x, int y)
//{
//    int xset = find(parent, x);
//    int yset = find(parent, y);
//    if(xset != yset)
//    {
//        parent[xset] = yset;
//    }
//}

class distances {
public:
    int N, M;
    distances();
    pair<int, int> *edges;
    map<int, int> *mst;
    int *spEdges;
    // Helper functions and variables.
    int *parent;
    int *ufSubsets;
    bool hasCycle(pair<int, int>*);
    void buildMST();
    vector<int> dfs_init(int);
    void dfs(bool*, int, int*);
    void printBinarySPs();
};

distances::distances() {
    cin >> N >> M;

    edges = new pair<int, int>[M];
    mst = new map<int, int>[N];
    spEdges = new int[M];
    parent = new int[N];
    ufSubsets = new int[N];
    for (int m = 0; m < M; m++) {
        int a, b, c;
        cin >> a >> b >> c;
        edges[c] = make_pair(a, b);
    }
    for (int n = 0; n < N; n++)
        ufSubsets[n] = -1;
}

bool distances::hasCycle(pair<int, int> *candEdge) {
    int subsetSource = find(ufSubsets, candEdge->first);
    int subsetDest = find(ufSubsets, candEdge->second);
    if (subsetSource == subsetDest)
        return true;
    // Union.
    ufSubsets[subsetSource] = subsetDest;

    return false;
}

void distances::buildMST() {
    for (int i = 0; i < M; i++) {
        pair<int, int> candEdge(edges[i].first - 1, edges[i].second - 1);
        if (hasCycle(&candEdge)) {
            continue;
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

void distances::printBinarySPs() {
    bool startPrint = false;
    for (int i = 0; i < M-1; i++) {
        int edge = spEdges[i];
        spEdges[i] = edge % 2;
        spEdges[i+1] += (edge / 2);
    }

    // Special handling for the last element.
    if (spEdges[M-1] != 0) {
        startPrint = true;
        string binEdge;
        while (spEdges[M-1] >= 2) {
            binEdge += to_string(spEdges[M-1] % 2);
            spEdges[M-1] = spEdges[M-1] / 2;
        }
        cout << spEdges[M-1] << binEdge;
    }
    for (int i = M-2; i >= 0; i--)
        if (!startPrint && spEdges[i] == 0)
            continue;
        else {
            startPrint = true;
            cout << spEdges[i];
        }
    // If the printing still hasn't started, print 0.
    if (!startPrint)
        cout << 0;
    cout << endl;
}

int main() {
    distances dist;
    dist.buildMST();
    dist.dfs_init(0);
    dist.printBinarySPs();

    return 0;
}
