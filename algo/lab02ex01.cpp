#include <iostream>
#include <vector>
using namespace std;

class kcredits {
    static int binSearch(vector<int>, int, bool);
public:
    int N, K;
    vector<int> persons;
    void getInput();
    static int getMaxWithOneException(vector<int>);
};

int kcredits::binSearch(vector<int> v, int key, bool isIncreasing = true) {
    int l = -1;
    int r = v.size() - 1;
    while (r - l > 1) {
        int m = (l + r)/2;
        // First take care of the obvious case.
        if (key == v[m])
            return m;
        // Now Logic FTW!
        // We can map the cases when we need "r = m" or "l = m"
        // to a truth table using the isIncreasing and (key < v[m]) booleans.
        // By doing so we see that it maps the identity function so we can
        // simply check if "(key < v[m]) == isIncreasing" .
        if ((key < v[m]) == isIncreasing)
            r = m;
        else
            l = m;
    }

    return r;
}

int kcredits::getMaxWithOneException(vector<int> seq) {
    int n = seq.size();

    // Do a LIS for the sequence.
    vector<int> tailI;
    int length = 1;
    // Keep the max length up until each element.
    vector<int> subMaxI(n, 1);

    // Initialise for the first element.
    tailI.push_back(seq[0]);
    for (int i = 1; i < n; i++) {
        if (seq[i] < tailI[0]){
            tailI[0] = seq[i];
            subMaxI[i] = subMaxI[i-1];
        }
        else if (seq[i] > tailI[length - 1]) {
            tailI.push_back(seq[i]);
            length++;
            subMaxI[i] = length;
        }
        else {
            tailI[binSearch(tailI, seq[i])] = seq[i];
            subMaxI[i] = subMaxI[i-1];
        }
    }

    // We also need to get the LIS when starting from an arbitrary point in the vector.
    // We look for the LDS instead from the inverted (in the respect of order of transversing) vector.
    vector<int> tailD;
    length = 1;
    // Keep the max length up until each element.
    vector<int> subMaxD(n, 1);

    // Initialise for the first element.
    tailD.push_back(seq[n-1]);
    for (int i = n-2; i >= 0; i--) {
        if (seq[i] > tailD[0]){
            tailD[0] = seq[i];
            subMaxD[i] = subMaxD[i+1];
        }
        else if (seq[i] < tailD[length - 1]) {
            tailD.push_back(seq[i]);
            length++;
            subMaxD[i] = length;
        }
        else {
            tailD[binSearch(tailD, seq[i], false)] = seq[i];
            subMaxD[i] = subMaxD[i+1];

        }
    }

    // Find the max sum between the two max value vectors.
    int max = subMaxD[0]; // Which is the same as subMaxI[n-1].
    for (int i = 0; i < n-2; i++) {
        if (max < subMaxI[i] + subMaxD[i+1])
            max = subMaxI[i] + subMaxD[i+1];
    }

    return max;
}

void kcredits::getInput() {
    // Get the number of the intersections and the minimum number of streets to light.
    cin >> N >> K;

    for (int i = 1; i <= N; i++) {
        int person;
        cin >> person;
        persons.push_back(person);
    }
}

int main() {
    kcredits kc;
    kc.getInput();
    if (kc.K == 1)
        cout << kc.getMaxWithOneException(kc.persons) << "\n";
    else
        cout << 0;

    return 0;
};
