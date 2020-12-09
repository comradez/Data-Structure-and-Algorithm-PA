#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cassert>
using namespace std;

const int MAXM = 2e5 + 5;

char op[MAXM], opTemp[2];
int opLeft[MAXM], opRight[MAXM]; //原始上下界
int disLeft[MAXM], disRight[MAXM]; //离散化后的上下界
int bucket[2 * MAXM]; //存放去重后的标签->实际值的映射
int n, m; 

struct Pair {
	int index, value, flag;
	Pair(int _index = 0, int _value = 0, int _flag = 0)
		: index(_index), value(_value), flag(_flag) {}
} sortArray[2 * MAXM];

int cmp(const void* A, const void* B) {
	return (*(Pair*)(A)).value - (*(Pair*)(B)).value;
}

struct treeNode {
	long long val;
	int lb, rb, lazy;
} segTree[8 * MAXM];

void pushdown(int p) {
    if (segTree[p].rb - segTree[p].lb == 1) {
        segTree[p].lazy = 0;
        return;
    }
	segTree[2 * p].lazy += segTree[p].lazy;
	segTree[2 * p + 1].lazy += segTree[p].lazy;
    segTree[2 * p].val += segTree[p].lazy * 1ll * (bucket[segTree[2 * p].rb] - bucket[segTree[2 * p].lb]);
	segTree[2 * p + 1].val += segTree[p].lazy * 1ll * (bucket[segTree[2 * p + 1].rb] - bucket[segTree[2 * p + 1].lb]);
	segTree[p].lazy = 0;
}

void init(int p, int l, int r) {
	segTree[p].lb = l; segTree[p].rb = r;
	if (r - l == 1) return; 
	int mid = (r - l) / 2 + l;
	init(2 * p, l, mid);
	init(2 * p + 1, mid, r);
}

void modify(int p, int x, int y) {
	int l = segTree[p].lb, r = segTree[p].rb;
	if (segTree[p].lazy) pushdown(p);
	if (x <= l && r <= y) {
		segTree[p].val += (bucket[r] - bucket[l]) * 1ll;
		segTree[p].lazy += 1;
		return;
	} 
	int mid = (r - l) / 2 + l;
	if (x < mid)
		modify(2 * p, x, y);
	if (y > mid)
		modify(2 * p + 1, x, y);
	segTree[p].val = segTree[2 * p].val + segTree[2 * p + 1].val;
}

long long query(int p, int x, int y) {
	int l = segTree[p].lb, r = segTree[p].rb;
	if (segTree[p].lazy) pushdown(p);
	if (x <= l && r <= y) return segTree[p].val;
    long long ans = 0;
	int mid = (r - l) / 2 + l;
	if (x < mid)
		ans += query(2 * p , x, y);
	if (y > mid)
		ans += query(2 * p + 1, x, y);
	return ans;
}

int main() {
	scanf("%d%d", &n, &m);
	for (int i = 0; i < m; i++) {
		scanf("%s", opTemp);
		op[i] = opTemp[0];
		scanf("%d%d", &opLeft[i], &opRight[i]);
		opLeft[i] -= 1;
        assert(opLeft[i] < opRight[i]);
		sortArray[2 * i] = Pair(i, opLeft[i], -1);
		sortArray[2 * i + 1] = Pair(i, opRight[i], 1);
	} qsort(sortArray, 2 * m, sizeof(Pair), cmp);

	int realIndex = 0;
	bucket[0] = sortArray[0].value;
	for (int i = 0; i < 2 * m; i++) {
		if (i > 0 && sortArray[i].value > sortArray[i - 1].value)
			bucket[++realIndex] = sortArray[i].value;
		if (sortArray[i].flag == -1)
			disLeft[sortArray[i].index] = realIndex;
		else 
			disRight[sortArray[i].index] = realIndex;
	} realIndex++; //离散化、去重
	init(1, 0, realIndex - 1);
	for (int i = 0; i < m; i++) {
		if (op[i] == 'H')
			modify(1, disLeft[i], disRight[i]);
		else
            printf("%lld\n", query(1, disLeft[i], disRight[i])); //op[i] == 'Q'
	}
	return 0;
}