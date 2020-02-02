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
    unsigned long long int * pCrossAvoid;
    submarine();
//    void usePilotCrossing(pair<pair<int, int>, pair<int, int>>);
    unsigned long long int dfs(pair<int, int>, bool);
    unsigned long long int dfs_init();
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

    // Build the number of paths excluded for each pilot crossing.
    pCrossAvoid = new unsigned long long int[N];
    int c = 0;
    for (auto & pCrossing : pCrossings) {
        int x = pCrossing.first.first;
        int y = pCrossing.first.second;
        pCrossAvoid[c] = BinC[x][y] * BinC[N-1 - x][M-1 - y];
        c++;
    }
}

//void usePilotCrossing(pair<pair<int, int>, pair<int, int>>* pCrossing);

unsigned long long int submarine::dfs(pair<int, int> s, bool sign) {
    unsigned long long int paths = 0;
    for (pair<int, int> child : G[s]) {
        unsigned long long int edge = BinC[s.first - child.first][s.second - child.second];
        // We switch the bool value for the sign to change in each level.
        if (child.first != 0 && child.second != 0)
            edge = (-1) * edge * dfs(child, !sign);
        // Add or subtract the paths depending on the sign.
        if (sign)
            paths += edge;
        else
            paths = paths - edge;
    }
    return paths;
}

unsigned long long int submarine::dfs_init() {
    return dfs(make_pair(N-1, M-1), true);
}

unsigned long long int submarine::calculateMaxPaths() {
    for (int k = 0; k < K; k++){
//        pair<pair<int, int>, pair<int, int>> minPCB = getMaxPilotCrossingBase();
//        usePilotCrossing(minPCB);
    }
    unsigned long long int maxPaths = BinC[N-1][M-1] - dfs_init();
    return (maxPaths % 1000000103);
}

int main() {
    submarine sm;
    cout << sm.calculateMaxPaths() << endl;

//    // Print stuff.
//    int k = 0;
//    for (auto & pCrossing : sm.pCrossings) {
//        cout << "Pilot crossing " << k << ": ";
//        cout << "(" << pCrossing.first.first << ", " << pCrossing.first.second << ") - ";
//        cout << "(" << pCrossing.second.first << ", " << pCrossing.second.second << ")";
//        cout << endl;
//        k++;
//    }
//    for (int n = 0; n < sm.N; n++)
//        for (int m = 0; m < sm.M; m++)
//            cout << "BinC(" << n << ", " << m << ") = " << sm.BinC[n][m] << endl;
//
//    for (int k = 0; k < sm.K; k++)
//        cout << "Pilot crossing " << k << " avoids " << sm.pCrossAvoid[k] << endl;
//    cout << "Checking Graph" << endl;
//    for (auto & nodes : sm.G) {
//        cout << "Box (" << nodes.first.first << ", " << nodes.first.second << ") goes to" << endl;
//        for (auto & boxes : nodes.second)
//            cout << "(" << boxes.first << ", " << boxes.second << "), ";
//        cout << endl;
//    }

    return 0;
}
