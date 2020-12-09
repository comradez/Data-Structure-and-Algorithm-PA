#include <iostream>
#include <cstdio>
#include <cstring>
#include <cstdlib>
using namespace std;

const int MAXN = 1e6 + 5;
const int MAXM = 1e5 + 5;

int queapPHead, queapPTail, queapP[MAXN]; //维护最大值
int queapQHead, queapQTail, queapQ[MAXN]; //原始队列
int intervalLeft[MAXN], infectedMax[MAXN];
int x[MAXN], n, t;

void push(int value) {
	queapQ[queapQTail++] = value;
	while (queapPHead < queapPTail && queapP[queapPTail - 1] < value)
		queapPTail--;
	queapP[queapPTail++] = value;
}

void pop() {
	if (queapQHead == queapQTail)
		return;
	if (queapQ[queapQHead] == queapP[queapPHead])
		queapPHead++;
	queapQHead++;
}

int getMax() {
	return queapP[queapPHead];
}

template <typename T>
T read() {
	T ret = 0;
	int sig = 1;
	char ch = getchar();
	while (ch < '0' || ch > '9') {
		if (ch == '-') {
			sig = -1;
		} ch = getchar();
	}
	while ('0' <= ch && ch <= '9') {
		ret = (ret << 3) + (ret << 1) + (ch ^ 48);
		ch = getchar();
	} return ret * sig;
}


int cmp(const void* a, const void* b) {
	return *((int *)a) - *((int *)b);
}

int biSearch(int* array, long long value, int lo, int hi) {
	while (lo < hi) {
		int mid = (lo + hi) >> 1;
		if (value <= array[mid])
			hi = mid;
		else lo = mid + 1;
	}
	return lo - 1; //小于value的最大者
}

int main() {
	n = read<int>();
	for (int i = 1; i <= n; i++)
		x[i] = read<int>();
	//样例中隐含了x[0] = 0，因此从1开始计
	for (long long i = 1; i <= n; i++) {
		long long length;
		length = read<long long>();
		intervalLeft[i] = (int)max(0ll, i - length);
	} //每天查询区间的左端点，取max防止找到负下标
	int currentLeft = 0; //currentRight == i - 1
	for (int i = 1; i <= n; i++) {
		for (; currentLeft < intervalLeft[i]; currentLeft++)
			pop();
		push(x[i - 1]);
		infectedMax[i] = getMax();
	}
	qsort(infectedMax, n + 1, sizeof(int), cmp);
	t = read<int>();
	long long p, q;
	int lowRisk = 0, midRisk = 0;
	for (int i = 0; i < t; i++) {
		p = read<long long>();
		q = read<long long>();
		lowRisk = p != 0 ? biSearch(infectedMax, p, 0, n + 1) : 0;
		midRisk = biSearch(infectedMax, q, 0, n + 1);
		printf("%d %d\n", lowRisk, midRisk - lowRisk);
	}
	return 0;
}