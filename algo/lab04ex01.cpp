#include <iostream>
#include <limits>
#include <climits>
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

/**
 * Min Heap structure. Code taken from the GeeksforGeeks platform
 * and has been adjusted for the needs of the exercise.
 */

// Prototype of a utility function to swap two integers
void swap(long *x, long *y);

// A class for Min Heap
class MinHeap
{
    pair<long, long> *harr; // pointer to array of elements in heap
    long capacity; // maximum possible size of min heap
    long heap_size; // Current number of elements in min heap
public:
    // Constructor
    MinHeap(long capacity);

    // to heapify a subtree with the root at given index
    void MinHeapify(long );

    long parent(long i) { return (i-1)/2; }

    // to get index of left child of node at index i
    long left(long i) { return (2*i + 1); }

    // to get index of right child of node at index i
    long right(long i) { return (2*i + 2); }

    // to extract the root which is the minimum element
    pair<long, long> extractMin();

    // Decreases key value of key at index i to new_val
    void decreaseKey(long i, long new_val);

    // Returns the minimum key (key at root) from min heap
    long getMin() { return harr[0].second; }

    // Returns the index of a key
    long getIndex(long k);

    // Deletes a key stored at index i
    void deleteKey(long i);

    // Inserts a new key 'k'
    void insertKey(long k, long l);
};

// Constructor: Builds a heap from a given array a[] of given size
MinHeap::MinHeap(long cap)
{
    heap_size = 0;
    capacity = cap;
    harr = new pair<long, long>[cap];
}

// Inserts a new key 'k'
void MinHeap::insertKey(long k, long l)
{
    if (heap_size == capacity)
    {
        cout << "\nOverflow: Could not insertKey\n";
        return;
    }

    // First insert the new key at the end
    heap_size++;
    long i = heap_size - 1;
    harr[i].first = k;
    harr[i].second = l;

    // Fix the min heap property if it is violated
    while (i != 0 && harr[parent(i)].second > harr[i].second)
    {
        swap(&harr[i].first, &harr[parent(i)].first);
        swap(&harr[i].second, &harr[parent(i)].second);
        i = parent(i);
    }
}

// Decreases value of key at index 'i' to new_val. It is assumed that
// new_val is smaller than harr[i].
void MinHeap::decreaseKey(long i, long new_val)
{
    harr[i].second = new_val;
    while (i != 0 && harr[parent(i)].second > harr[i].second)
    {
        swap(&harr[i].first, &harr[parent(i)].first);
        swap(&harr[i].second, &harr[parent(i)].second);
        i = parent(i);
    }
}

// Method to remove minimum element (or root) from min heap
pair<long, long> MinHeap::extractMin()
{
    if (heap_size <= 0)
        return make_pair(-1, -1);
    if (heap_size == 1)
    {
        heap_size--;
        return harr[0];
    }

    // Store the minimum value, and remove it from heap
    pair<long, long> root = harr[0];
    harr[0] = harr[heap_size-1];
    heap_size--;
    MinHeapify(0);

    return root;
}

long MinHeap::getIndex(long k)
{
    long i = 0;
    while (harr[i].first != k)
        i++;
    return i;
}


// This function deletes key at index i. It first reduced value to minus
// infinite, then calls extractMin()
void MinHeap::deleteKey(long i)
{
    decreaseKey(i, INT_MIN);
    extractMin();
}

// A recursive method to heapify a subtree with the root at given index
// This method assumes that the subtrees are already heapified
void MinHeap::MinHeapify(long i)
{
    long l = left(i);
    long r = right(i);
    long smallest = i;
    if (l < heap_size && harr[l].second < harr[i].second)
        smallest = l;
    if (r < heap_size && harr[r].second < harr[smallest].second)
        smallest = r;
    if (smallest != i)
    {
        swap(&harr[i].first, &harr[smallest].first);
        swap(&harr[i].second, &harr[smallest].second);
        MinHeapify(smallest);
    }
}

// A utility function to swap two elements
void swap(long *x, long *y)
{
    long temp = *x;
    *x = *y;
    *y = temp;
}

/**
 * Implementation of the solution.
 */

class racing {
public:
    long N, M, K, L, B;
    long *raceRoute;
    map<long, long> *cityNetwork;
    long *shortestGasStation;
    MinHeap *dijkstraHeap;
    racing();
    void calculateShortestGasStation();
    unsigned long long int calculateMinRouteTime();
};

racing::racing() {
    N = readLong(), M = readLong(), K = readLong(), L = readLong(), B = readLong();

    // Build the graph array for the city network.
    cityNetwork = new map<long, long>[N];
    for (int m = 0; m < M; m++) {
        long cityA = readLong(), cityB = readLong(), dist = readLong();
        cityNetwork[cityA-1].insert(make_pair(cityB - 1, dist));
        cityNetwork[cityB-1].insert(make_pair(cityA - 1, dist));
    }

    // Get the race route.
    raceRoute = new long[K];
    for (int k = 0; k < K; k++) {
        long city = readLong();
        raceRoute[k] = city - 1;
    }

    // Initialise the shortest gas station array.
    shortestGasStation = new long[N];
    dijkstraHeap = new MinHeap(N);
    for (int n = 0; n < N; n++) {
        shortestGasStation[n] = numeric_limits<long>::max() - 60000;
        dijkstraHeap->insertKey(n, numeric_limits<long>::max() - 60000);
    }

    // Get the gas stations and add the respective nodes to the heap
    // for running the Dijkstra algorithm later.
    for (int b = 0; b < B; b++) {
        long city = readLong();
        shortestGasStation[city - 1] = 0;
        dijkstraHeap->decreaseKey(dijkstraHeap->getIndex(city - 1), 0);
    }
}

void racing::calculateShortestGasStation() {
    // Dijkstra
    bool *isSpt = new bool[N]{false};

    for (int i = 0; i < N; i++) {
        long u = dijkstraHeap->extractMin().first;
        isSpt[u] = true;
        if (u == -1)
            break;

        for (auto & streetTo : cityNetwork[u]) {
            long v = streetTo.first;
            long d = streetTo.second;
            if (isSpt[v])
                continue;

            if (shortestGasStation[v] > shortestGasStation[u] + d) {
                shortestGasStation[v] = shortestGasStation[u] + d;
                dijkstraHeap->decreaseKey(dijkstraHeap->getIndex(v), shortestGasStation[u] + d);
            }
        }
    }
}

unsigned long long int racing::calculateMinRouteTime() {
    long minTime = 0;
    bool *isUsed = new bool[N]{false};

    for (int k = 1; k < K; k++)
        minTime += cityNetwork[raceRoute[k-1]][raceRoute[k]];
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
//    for (long n = 0; n < r.N; n++) {
//        for (auto & streetTo : r.cityNetwork[n]) {
//            cout << "Street (" << n << ", " << streetTo.first << "): " << streetTo.second << endl;
//        }
//    }

    return 0;
}
