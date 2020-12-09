#include "temperature.h"
#include <iostream>
#include <cstdio>

const int MAXN = 200005;
const int MAXM = 160005;
using namespace std;

struct Point {
	int num[2], maxval;
	unsigned int minval; 
	Point(int _x = 0, int _y = 0, int _maxval = -1, unsigned int _minval = 0x80000000) {
		num[0] = _x, num[1] = _y, maxval = _maxval, minval = _minval;
	}
} pool[MAXN], data[MAXN], cleanData[MAXN];

bool cmp(const Point& a, const Point& b, bool isSortedByX) {
	return isSortedByX ? a.num[0] <= b.num[0] : a.num[1] <= b.num[1];
}

bool Allcmp(const Point& a, const Point& b) {
	if (a.num[0] < b.num[0]) return true;
	else if (a.num[0] > b.num[0]) return false;
	else if (a.num[1] <= b.num[1]) return true;
	else return false;
}

void mergeSort(Point* array, int l, int r, bool isSortedByX, bool Allsort = false) {
	if (r - l <= 1) return;
	int mid = (r - l) / 2 + l;
	mergeSort(array, l, mid, isSortedByX, Allsort);
	mergeSort(array, mid, r, isSortedByX, Allsort);
	int ptrLeft = 0, ptrRight = 0, ptrAll = 0;
	int maxLeft = mid - l, maxRight = r - mid;
	Point* arrLeft = pool;
	for (int i = 0; i < mid - l; i++)
		arrLeft[i] = array[l + i];
	Point* arrRight = array + mid;
	Point* arrAll = array + l;
	while (ptrLeft < maxLeft && ptrRight < maxRight)
		if (!Allsort)
			arrAll[ptrAll++] = (cmp(arrLeft[ptrLeft], arrRight[ptrRight], isSortedByX)) ? arrLeft[ptrLeft++] : arrRight[ptrRight++];
		else arrAll[ptrAll++] = (Allcmp(arrLeft[ptrLeft], arrRight[ptrRight])) ? arrLeft[ptrLeft++] : arrRight[ptrRight++];
	while (ptrLeft < maxLeft)
		arrAll[ptrAll++] = arrLeft[ptrLeft++];
}

struct treeNode {
	int child[2], minn[2], maxn[2];
	int maxval;
	unsigned int minval = 0x80000000;
	Point point;
} tree[MAXM];

int n, global_cnt, root;

void build(int &u, int l, int r, int dep) {
	if (l > r) return;
	if (u == 0) {
		u = ++global_cnt;
		tree[u].child[0] = tree[u].child[1]	= 0;
	}
	int mid = (r - l) / 2 + l;
	bool isSortedByX = dep % 2;
	mergeSort(cleanData, l, r + 1, isSortedByX);
	tree[u].point = cleanData[mid];
	build(tree[u].child[0], l, mid - 1, dep + 1);
	build(tree[u].child[1], mid + 1, r, dep + 1);
	for (int i = 0; i < 2; i++) {
		tree[u].minn[i] = tree[u].maxn[i] = tree[u].point.num[i];
		if (tree[u].child[0]) {
			tree[u].minn[i] = min(tree[u].minn[i], tree[tree[u].child[0]].minn[i]);
			tree[u].maxn[i] = max(tree[u].maxn[i], tree[tree[u].child[0]].maxn[i]);
		}
		if (tree[u].child[1]) {
			tree[u].minn[i] = min(tree[u].minn[i], tree[tree[u].child[1]].minn[i]);
			tree[u].maxn[i] = max(tree[u].maxn[i], tree[tree[u].child[1]].maxn[i]);
		}
	}
	tree[u].maxval = max(max(tree[tree[u].child[0]].maxval, tree[tree[u].child[1]].maxval), tree[u].point.maxval);
	tree[u].minval = min(min(tree[tree[u].child[0]].minval, tree[tree[u].child[1]].minval), tree[u].point.minval);
}

bool inside(int x1, int y1, int x2, int y2, int X1, int Y1, int X2, int Y2) {
	return x1 <= X1 && x2 >= X2 && y1 <= Y1 && y2 >= Y2;
}

bool outside(int x1, int y1, int x2, int y2, int X1, int Y1, int X2, int Y2) {
	return x1 > X2 || x2 < X1 || y1 > Y2 || y2 < Y1;
}

void find(int u, int x1, int y1, int x2, int y2, int* ansX, unsigned int* ansY) {
	if (u == 0) return; 
	if (tree[u].maxval <= *ansX && tree[u].minval >= *ansY) return;
	if (inside(x1, y1, x2, y2, tree[u].minn[0], tree[u].minn[1], tree[u].maxn[0], tree[u].maxn[1])) {
		*ansX = max(*ansX, tree[u].maxval);
		*ansY = min(*ansY, tree[u].minval);
		return;
	}
	if (outside(x1, y1, x2, y2, tree[u].minn[0], tree[u].minn[1], tree[u].maxn[0], tree[u].maxn[1]))
		return;

	if (inside(x1, y1, x2, y2, tree[u].point.num[0], tree[u].point.num[1], tree[u].point.num[0], tree[u].point.num[1])) {
		*ansX = max(*ansX, tree[u].point.maxval);
		*ansY = min(*ansY, tree[u].point.minval);
	}
	find(tree[u].child[0], x1, y1, x2, y2, ansX, ansY);
	find(tree[u].child[1], x1, y1, x2, y2, ansX, ansY);
} //将程序由返回Pair的形式改为返回void、对两个指针进行读写的形式以避免TLE，这个想法在与高宇睿交流时提出。

void init(int n, const int *x, const int *y, const int *t) {
	for (int i = 0; i < n; i++)
		data[i] = Point(x[i], y[i], t[i], t[i]);
	mergeSort(data, 0, n, true, true);
	int cleanIndex = 0;
	cleanData[0] = data[0];
	for (int i = 1; i < n; i++) {
		if (cleanData[cleanIndex].num[0] == data[i].num[0] && cleanData[cleanIndex].num[1] == data[i].num[1]) {
			cleanData[cleanIndex].maxval = max(cleanData[cleanIndex].maxval, data[i].maxval);
			cleanData[cleanIndex].minval = min(cleanData[cleanIndex].minval, data[i].minval);
		} else cleanData[++cleanIndex] = data[i]; 
	}
	build(root, 0, cleanIndex, 0);
}

void query(int x1, int x2, int y1, int y2, int *tmin, int *tmax) {
	unsigned int* tmin_temp = new unsigned int;
	*tmin_temp = 0x80000000;
	*tmax = -1;
	find(root, x1, y1, x2, y2, tmax, tmin_temp);
	*tmin = (*tmin_temp == 0x80000000) ? -1 : *tmin_temp;
}
