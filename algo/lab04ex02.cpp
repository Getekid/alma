#include <iostream>
#include <vector>
#include <queue>

using namespace std;

class ring {
public:
    int K;
    int maxPathInternational = 0;
    vector<vector<int>> *countries;
    int *maxPathOut;
    ring();
    int getMaxSide(int k, vector<int> root);
    int getMaxPath();
};

ring::ring() {
    cin >> K;
    countries = new vector<vector<int>>[K];
    for (int k = 0; k < K; k++) {
        int countryLength;
        cin >> countryLength;
        vector<int> singleton;
        countries[k].push_back(singleton);
        if (countryLength == 1) {
            int dummyCity;
            cin >> dummyCity;
        }
        for (int country = 1; country < countryLength; country++) {
            countries[k].push_back(singleton);
            int parent;
            cin >> parent;
            countries[k][parent-1].push_back(country);
        }
    }
}

int ring::getMaxSide(int k, vector<int> root) {
    // Helper code to print stuff. - Still to make Debug work...
//    cout << "Children(" << root.size() << "): " << (root.empty() ? 0 : root[0]) << ", " << ((root.size() == 2) ? root[1] : 0) << endl;

    // At each step, also check for the combined path (aka the inner planet path.
    int degree, degreeLeft, degreeRight;
    switch (root.size()) {
        case 0:
            return 0;
        case 1:
            degree = getMaxSide(k, countries[k][root[0]]) + 1;
            if (maxPathInternational < degree)
                maxPathInternational = degree;
            return degree;
        case 2:
            degreeLeft = getMaxSide(k, countries[k][root[0]]) + 1;
            degreeRight = getMaxSide(k, countries[k][root[1]]) + 1;
            if (maxPathInternational < degreeLeft + degreeRight)
                maxPathInternational = degreeLeft + degreeRight;
            return max(degreeLeft, degreeRight);
    }

    return -1;
}

int ring::getMaxPath() {
    maxPathOut = new int[K];
    for (int k = 0; k < K; k++) {
        maxPathOut[k] = getMaxSide(k, countries[k][0]);
        // Helper code to print stuff. - Still to make Debug work...
//        cout << "Planet: " << k << " - Max Side Path: " << maxPathOut[k] << endl;
    }
    int maxPathInterSolar = 0;
    // First get the max path in the range [0, K-1].
    int maxPathRange = 0;
    priority_queue<int> maxPathHeap;
    for (int k = 1; k < K; k++) {
        int dist = (k <= K / 2) ? K - k : k;
        int maxPath = maxPathOut[0] + dist + maxPathOut[k];
        maxPathHeap.push(maxPath);
    }
    if (maxPathRange < maxPathHeap.top())
        maxPathRange = maxPathHeap.top();
    if (maxPathInterSolar < maxPathRange)
        maxPathInterSolar = maxPathRange;

    // Now use that range to cycle through the max of the rest of the ranges.
    for (int k = 1; k < K; k++) {
        // Step 1: For range [a, b] get the range [a+1, b].
        // In the case where the next node is holding the current max value, take the next one.
        if (maxPathRange == maxPathOut[k-1] + (K-1) + maxPathOut[k]) {
            maxPathHeap.pop();
            maxPathRange = maxPathHeap.top();
        } else
            maxPathRange = maxPathRange - 1 + (maxPathOut[k] - maxPathOut[k-1]);
        // The first K/2 elements might have increased more than the current max so we check them.
        int kAcross = (k + K/2 - 1) % (K-1);
        for (int l = 1; l <= kAcross; l++) {
            if (maxPathRange < maxPathOut[k] + (K-l) + maxPathOut[(k+l)%K]) {
                maxPathRange = maxPathOut[k] + (K-l) + maxPathOut[(k+l)%K];
            }
        }

        // Step 2: For range [a, b] get the range [a, b+1].
        int maxPathEdge = maxPathOut[k-1] + (K-1) + maxPathOut[k];
        if (maxPathRange < maxPathEdge)
            maxPathRange = maxPathEdge;
        if (maxPathInterSolar < maxPathRange)
            maxPathInterSolar = maxPathRange;
    }
    return max(maxPathInternational, maxPathInterSolar);
}

int main() {
    ring r;
    cout << r.getMaxPath() << endl;

     // Helper code to print stuff. - Still to make Debug work...
//    for (int k = 0; k < r.K; k++) {
//        cout << "Planet " << k+1 << " has " << r.countries[k].size() << " countries\n";
//        for (int c = 0; c < r.countries[k].size(); c++) {
//            cout << "Country " << c+1 << " has children: ";
//            for (auto child : r.countries[k][c]) {
//                cout << child+1 << ", ";
//            }
//            cout << endl;
//        }
//    }


    return 0;
}
