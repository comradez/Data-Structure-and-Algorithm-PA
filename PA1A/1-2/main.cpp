#include <iostream>
#include <cstdio>
using namespace std;

const int VAL = 200005;

int arr1[VAL], arr2[VAL], pool[VAL], poolindex;

struct Pair {
    int x, y;
    Pair(int _x, int _y) : x(_x), y(_y) {}
    Pair() : x(0), y(0) {}
} lines[VAL];

struct Point {
    int x, y;
    Point(int _x, int _y) : x(_x), y(_y) {}
};

class Cmp {
public:
    bool operator()(const Point& p1, const Pair& p2) const {
        return p2.x * 1ll * p2.y > p2.x * 1ll * p1.y + p2.y * 1ll * p1.x;
    }
};

void merge(int* arr, int lo, int mi, int hi) {
    int* arrAll = arr + lo;
    int* arrBack = arr + mi;
    int* arrFront = new int[mi - lo];
    for (int i = 0; i < mi - lo; i++)
        arrFront[i] = arrAll[i];
    int ptrFront = 0, ptrBack = 0, ptrAll = 0;
    while (ptrFront < mi - lo && ptrBack < hi - mi) {
        if (arrFront[ptrFront] <= arrBack[ptrBack]) {
            arrAll[ptrAll++] = arrFront[ptrFront++];
        } else {
            arrAll[ptrAll++] = arrBack[ptrBack++];
        }
    }
    while (ptrFront < mi - lo)
        arrAll[ptrAll++] = arrFront[ptrFront++];
    delete[] arrFront;
}

void mergeSort(int* arr, int lo, int hi) {
    if (hi - lo < 2)
        return;
    int mi = (lo + hi) >> 1;
    mergeSort(arr, lo, mi);
    mergeSort(arr, mi, hi);
    merge(arr, lo, mi, hi);
}

int biSearch(Pair* arr, int lo, int hi, const Point& point, const Cmp& cmp) {
    while (lo < hi) {
        int mi = (lo + hi) >> 1;
        if (cmp(point, arr[mi]))
            hi = mi;
        else
            lo = mi + 1;
    } return lo;
}

int main() {
    int n, m;
    scanf("%d", &n);
    for (int i = 0; i < n; i++)
        scanf("%d", &arr1[i]);
    for (int i = 0; i < n; i++)
        scanf("%d", &arr2[i]);
    mergeSort(arr1, 0, n);
    mergeSort(arr2, 0, n);
    for (int i = 0; i < n; i++)
        lines[i] = Pair(arr1[i], arr2[i]);
    scanf("%d", &m);
    int px, py;
    Cmp cmp;
    for (int i = 0; i < m; i++) {
        scanf("%d %d", &px, &py);
        printf("%d\n", biSearch(lines, 0, n, Point(px, py), cmp));
    }
}