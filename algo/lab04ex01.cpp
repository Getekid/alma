#include <iostream>
#include <limits>
#include <map>
#define BSIZE 1<<15

using namespace std;

char buffer[BSIZE];
long bpos = 0L, bsize = 0L;

long readLong()
{
    long d = 0L, x = 0L;
    char c;

    while (1)  {
        if (bpos >= bsize) {
            bpos = 0;
            if (feof(stdin)) return x;
            bsize = fread(buffer, 1, BSIZE, stdin);
        }
        c = buffer[bpos++];
        if (c >= '0' && c <= '9') { x = x*10 + (c-'0'); d = 1; }
        else if (d == 1) return x;
    }
    return -1;
}

class racing {
public:
    long N, M, K, L, B;
    long *raceRoute;
    map<pair<long, long>, long> *cityNetwork;
    struct Graph* graph;
    long *shortestGasStation;
    racing();
    void calculateShortestGasStation();
    unsigned long long int calculateMinRouteTime();
};

racing::racing() {
    N = readLong(), M = readLong(), K = readLong(), L = readLong(), B = readLong();

    // Build the graph array for the city network.
    cityNetwork = new map<pair<long, long>, long>;
    for (int m = 0; m < M; m++) {
        long cityA = readLong(), cityB = readLong(), dist = readLong();
        pair<long, long> street = make_pair(cityA - 1, cityB - 1);
        pair<pair<long, long>, long> pp = make_pair(street, dist);
        cityNetwork->insert(pp);
    }

    // Get the race route.
    raceRoute = new long[K];
    for (int k = 0; k < K; k++) {
        long city = readLong();
        raceRoute[k] = city - 1;
    }

    // Initialise the shortest gas station array.
    shortestGasStation = new long[N];
    for (int n = 0; n < N; n++)
        shortestGasStation[n] = numeric_limits<long>::max() - 60000;

    // Get the gas stations.
    for (int b = 0; b < B; b++) {
        long city = readLong();
        shortestGasStation[city - 1] = 0;
    }
}

void racing::calculateShortestGasStation() {
    // Bellman-Ford
    for (int i = 0; i < N; i++) {
        for (auto & edge : *cityNetwork) {
            long v = edge.first.first;
            long u = edge.first.second;
            long dist = edge.second;
            if (shortestGasStation[v] > shortestGasStation[u] + dist)
                shortestGasStation[v] = shortestGasStation[u] + dist;
            if (shortestGasStation[u] > shortestGasStation[v] + dist)
                shortestGasStation[u] = shortestGasStation[v] + dist;
        }
    }
}

unsigned long long int racing::calculateMinRouteTime() {
    long minTime = 0;
    bool isUsed[N]{false};

    for (int k = 1; k < K; k++) {
        pair<long, long> street = make_pair(raceRoute[k-1], raceRoute[k]);
        if (!cityNetwork->count(street))
            street = make_pair(raceRoute[k], raceRoute[k-1]);
        minTime += cityNetwork->at(street);
    }
    // TODO: Could improve by using a heap.
    for (int l = 0; l < L; l++) {
        long min = numeric_limits<long>::max();
        long minCity = 0;
        for (int k = 1; k < K-1; k++) {
            if (isUsed[raceRoute[k]])
                continue;
            if (shortestGasStation[raceRoute[k]] < min) {
                min = shortestGasStation[raceRoute[k]];
                minCity = raceRoute[k];
            }
        }
        isUsed[minCity] = true;
        minTime += min;
    }

    return minTime;
}

int main() {
    racing r;
    r.calculateShortestGasStation();
    cout << r.calculateMinRouteTime() << endl;

//    // Helper code to print stuff.
//    cout << r.N << " " << r.M << " " << r.K << " " << r.L << " " << r.B << endl;
//    cout << "Printing race route:\n";
//    for (int k = 0; k < r.K; k++) {
//        cout << "City " << k << ": " << r.raceRoute[k] << endl;
//    }
//    cout << "Printing shortest gas station:\n";
//    for (int n = 0; n < r.N; n++) {
//        cout << "City " << n << ": " << r.shortestGasStation[n] << endl;
//    }
//    cout << "Printing the city network\n";
//    for (pair<pair<long, long>, long> p : r.cityNetwork)
//        cout << "Street (" << p.first.first << ", " << p.first.second << "): " << p.second << endl;

    return 0;
}
