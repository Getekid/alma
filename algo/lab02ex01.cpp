#include <iostream>
#include <vector>
using namespace std;

class kcredits {
    int *maxLengthDataIncr;
    int **maxLengthDataDecr;
    static int binSearch(vector<int>, int, bool);
public:
    kcredits();
    int N, K;
    vector<int> personCredit;
    int getMaxWithOneException(int);
    int getMax();
};

kcredits::kcredits() {
    // Get the number of the intersections and the minimum number of streets to light.
    cin >> N >> K;

    // Initialise the lengthData vectors.
    maxLengthDataIncr = new int [N]{1};
    maxLengthDataDecr = new int* [N];
    for (int i = 0; i < N; i++) {
        maxLengthDataDecr[i] = new int[N];
        maxLengthDataDecr[i][i] = 1;
    }

    for (int i = 1; i <= N; i++) {
        int personCr;
        cin >> personCr;
        personCredit.push_back(personCr);
    }
}

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

int kcredits::getMaxWithOneException(int end) {
    // Do a LIS for the sequence.
    vector<int> tailI;
    int length = 1;

    // Quick fix: Only run the LIS when we're looking for the whole size.
    if (end == N-1) {
        // Initialise for the first element.
        tailI.push_back(personCredit[0]);
        for (int i = 1; i <= end; i++) {
            if (personCredit[i] < tailI[0]){
                tailI[0] = personCredit[i];
                maxLengthDataIncr[i] = maxLengthDataIncr[i-1];
            }
            else if (personCredit[i] > tailI[length - 1]) {
                tailI.push_back(personCredit[i]);
                length++;
                maxLengthDataIncr[i] = length;
            }
            else {
                tailI[binSearch(tailI, personCredit[i])] = personCredit[i];
                maxLengthDataIncr[i] = maxLengthDataIncr[i-1];
            }
        }
    }

    // We also need to get the LIS when starting from an arbitrary point in the vector.
    // We look for the LDS instead from the inverted (in the respect of order of transversing) vector.
    vector<int> tailD;
    length = 1;

    // Initialise for the first element.
    tailD.push_back(personCredit[end]);
    for (int i = end-1; i >= 0; i--) {
        if (personCredit[i] > tailD[0]){
            tailD[0] = personCredit[i];
            maxLengthDataDecr[end][i] = maxLengthDataDecr[end][i+1];
        }
        else if (personCredit[i] < tailD[length - 1]) {
            tailD.push_back(personCredit[i]);
            length++;
            maxLengthDataDecr[end][i] = length;
        }
        else {
            tailD[binSearch(tailD, personCredit[i], false)] = personCredit[i];
            maxLengthDataDecr[end][i] = maxLengthDataDecr[end][i+1];
        }
    }

    // Find the max sum between the two max value vectors.
    int max = maxLengthDataIncr[end]; // Which is the same as maxLengthDataDecr[end][0].
    for (int i = 1; i <= end; i++) {
        if (max < maxLengthDataIncr[i-1] + maxLengthDataDecr[end][i])
            max = maxLengthDataIncr[i-1] + maxLengthDataDecr[end][i];
    }

    return max;
}

int kcredits::getMax() {
    int maxCreditData[N][K];
    // Set the initial values.
    for (int k = 0; k < K; k++)
        maxCreditData[0][k] = 1;
    for (int n = N-1; n >= 0; n--) {
        maxCreditData[n][0] = getMaxWithOneException(n);
    }
    // Now for the recursion.
    for (int n = 1; n < N; n++) {
        for (int k = 1; k < K; k++) {
            if (n <= k)
                maxCreditData[n][k] = n+1;
            else {
                int subMax = k;
                for (int i = k; i <= n; i++) {
                    int subMaxK = maxCreditData[i-1][k-1] + maxLengthDataDecr[n][i];
                    if (subMax < subMaxK)
                        subMax = subMaxK;
                }
                maxCreditData[n][k] = subMax;
            }
        }
    }

    return maxCreditData[N-1][K-1];
}

int main() {
    kcredits kc;
    if (kc.K == 1)
        cout << kc.getMaxWithOneException(kc.N-1) << "\n";
    else
        cout << kc.getMax();

    return 0;
};
