#include <iostream>
#include <vector>

using namespace std;

class ring {
public:
    int K;
    int maxPathInner = 0;
    vector<vector<int>> *countries;
    ring();
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

int main() {
    ring r;

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
