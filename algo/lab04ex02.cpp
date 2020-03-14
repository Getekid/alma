#include <iostream>
#include <vector>

using namespace std;

class ring {
public:
    int K;
    int maxPathInternational = 0;
    vector<vector<int>> *countries;
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
    int *maxPathOut = new int[K];
    for (int k = 0; k < K; k++) {
        maxPathOut[k] = getMaxSide(k, countries[k][0]);
        // Helper code to print stuff. - Still to make Debug work...
//        cout << "Planet: " << k << " - Max Side Path: " << pathSide << endl;
    }
    int maxPathInterSolar = 0;
    for (int k = 0; k < K; k++)
        for (int p = 0; p < K; p++) {
            if (p == k)
                continue;
            int dist = (k-p < 0) ? K + (k-p) : k-p;
            int maxPath = maxPathOut[k] + dist + maxPathOut[p];
            if (maxPathInterSolar < maxPath)
                maxPathInterSolar = maxPath;
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
