#include <iostream>
#include <vector>
#include <map>

using namespace std;

class submarine {
public:
    int N, M, K, X;
    unsigned long long int **BinC;
    map<pair<int, int>, pair<int, int>> pCrossings;
    map<pair<int, int>, vector<pair<int, int>>> G;
    map<pair<int, int>, unsigned long long int*> paths;
    submarine();
    void dfs(pair<int, int>, bool);
    void dfs_init();
    unsigned long long int calculateMaxPaths();
};

submarine::submarine() {
    cin >> N >> M >> K >> X;

    // Build the start and end boxes to use later when building the Graph.
    pair<int, int> startBox(N-1, M-1);
    pair<int, int> endBox(0, 0);

    for (int k = 0; k < K; k++) {
        int S, E;
        cin >> S >> E;
        pair<int, int> s(S / M, S % M);
        pair<int, int> e(E / M, E % M);
        // Build the Graph.
        G[startBox].push_back(s);
        G[s].push_back(endBox);
        for (auto & pCrossing : pCrossings) {
            if (pCrossing.first.first <= s.first && pCrossing.first.second <= s.second)
                G[s].push_back(pCrossing.first);
            else if (pCrossing.first.first >= s.first && pCrossing.first.second >= s.second)
                G[pCrossing.first].push_back(s);
        }
        pCrossings[s] = e;
    }

    BinC = new unsigned long long int*[N];
    for (int n = 0; n < N; n++)
        BinC[n] = new unsigned long long int[M];

    // Build the network's Binomial Coefficient array.
    for (int n = 0; n < N; n++) {
        for (int m = 0; m < M; m++) {
            // Base Cases
            if (n == 0 || m == 0)
                BinC[n][m] = 1;
            else
                BinC[n][m] = BinC[n][m - 1] + BinC[n - 1][m];
        }
    }
}

void submarine::dfs(pair<int, int> s, bool forProfit) {
    for (pair<int, int> child : G[s]) {
        if (!paths[child])
            paths[child] = new unsigned long long int[X+1];
        unsigned long long int pathUncrossed = BinC[s.first - child.first][s.second - child.second];
        if (s.first == N-1 && s.second == M-1) {
            for (int x = 0; x <= X; x++)
                paths[child][x] = pathUncrossed;
            dfs(child, false);
            continue;
        }
        if (X == 0 && child.first == 0 && child.second == 0) {
            if (!forProfit)
                paths[child][0] += paths[s][0] * pathUncrossed;
            else
                paths[child][0] -= paths[s][0] * pathUncrossed;
            continue;
        }
        paths[child][0] = paths[s][0] * pathUncrossed;
        if (X == 0) {
            dfs(child, !forProfit);
            continue;
        }

        // Check for cases where X > 0;
        pair<int, int> e = pCrossings[s];
        if (e.first >= child.first && e.second >= child.second) {
            unsigned long long int pathCrossed = pathUncrossed - BinC[e.first - child.first][e.second - child.second];
            for (int x = 1; x <= X; x++) {
                unsigned long long int uncrossed = paths[s][x] * pathUncrossed;
                unsigned long long int crossed = paths[s][x-1] * pathCrossed;
                unsigned long long int maxPath = min(uncrossed, crossed);
                if (x == X && child.first == 0 && child.second == 0)
                    if (!forProfit)
                        paths[child][x] += maxPath;
                    else
                        paths[child][x] -= maxPath;
                else
                    paths[child][x] = maxPath;
            }
        } else {
            for (int x = 1; x <= X; x++) {
                paths[child][x] = paths[s][x] * pathUncrossed;
            }
        }
        if (child.first != 0 || child.second != 0)
            dfs(child, !forProfit);
    }
}

void submarine::dfs_init() {
    pair<int, int> startBox(N-1, M-1);
    paths[startBox] = new unsigned long long int[X+1];
    for (int x = 0; x <= X; x++)
        paths[startBox][x] = 1;
    dfs(startBox, false);
}

unsigned long long int submarine::calculateMaxPaths() {
    dfs_init();
    if (!paths[make_pair(0, 0)])
        return BinC[N-1][M-1];
    return ((BinC[N-1][M-1] - paths[make_pair(0, 0)][X]) % 1000000103);
}

int main() {
    submarine sm;
    cout << sm.calculateMaxPaths() << endl;

//    // Helper code to print stuff.
//    int k = 0;
//    for (auto & pCrossing : sm.pCrossings) {
//        cout << "Pilot crossing " << k << ": ";
//        cout << "(" << pCrossing.first.first << ", " << pCrossing.first.second << ") - ";
//        cout << "(" << pCrossing.second.first << ", " << pCrossing.second.second << ")";
//        cout << endl;
//        k++;
//    }
//    for (int n = 0; n < sm.N; n++) {
//        for (int m = 0; m < sm.M; m++) {
//            pair<int, int> node(n, m);
//            if (sm.pCrossings[node].first)
//                cout << "X";
//            else
//                cout << ".";
//        }
//        cout << endl;
//        cout << "BinC(" << n << ", " << m << ") = " << sm.BinC[n][m] << endl;
//    }
//
//    cout << "Checking Graph" << endl;
//    for (auto & nodes : sm.G) {
//        cout << "Box (" << nodes.first.first << ", " << nodes.first.second << ") goes to" << endl;
//        for (auto & boxes : nodes.second)
//            cout << "(" << boxes.first << ", " << boxes.second << "), ";
//        cout << endl;
//    }
//    cout << "Checking paths." << endl;
//    for (auto & nodes : sm.paths) {
//        cout << "Box (" << nodes.first.first << ", " << nodes.first.second << ") has with exceptions" << endl;
//        for (int x = 0; x <= sm.X; x++) {
//            cout << nodes.second[x] << " - ";
//        }
//        cout << endl;
//    }

    return 0;
}
