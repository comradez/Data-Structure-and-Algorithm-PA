#include <iostream>
#include <cstdio>
#include <cstring>
#include <utility>
#include <cassert>
using namespace std;

//基本常量和变量
const int MAXN = 1e5 + 5;
const int MAXM = 4e5 + 5;
const int MOD = 998244353;
int n, m, q, cnt;
int leftBound[MAXM], rightBound[MAXM];
long long value[MAXM];

//邻接表
struct Node {
	int succ, next;
	long long val;
    Node(int _succ = 0, int _next = 0, long long _val = 0)
        : succ(_succ), next(_next), val(_val) {}
} graph[2 * MAXM];
int head[MAXN];
long long srcDist[MAXN], dstDist[MAXN];
long long srcCnt[MAXN], dstCnt[MAXN];
bool vis[MAXN];

void addEdge(int a, int b, long long c) {
	graph[++cnt] = Node(b, head[a], c);
	head[a] = cnt;
    graph[++cnt] = Node(a, head[b], c);
    head[b] = cnt;
}

//堆
class Heap {
private:
    int size;
    pair<long long,int> heap[1000005];
    int prt(int x) { return (x - 1) / 2; }
    int lkid(int x) { return 2 * x + 1; }
    int rkid(int x) { return 2 * x + 2; }
	void Shiftup(int x) { //位置在x的节点上移
		if (x == 0) return;
		if (heap[x] < heap[prt(x)]) {
			swap(heap[x], heap[prt(x)]);
			Shiftup(prt(x));
		}
	}
	void Shiftdown(int x) { //位置在x的节点下移
		if (heap[x] > heap[lkid(x)] && lkid(x) < size) {
			swap(heap[x], heap[lkid(x)]);
			Shiftdown(lkid(x));
		}
		if (heap[x] > heap[rkid(x)] && rkid(x) < size) {
			swap(heap[x], heap[rkid(x)]);
			Shiftdown(rkid(x));
		}
	}

public:
	Heap() : size(0) {
        memset(heap, 0, sizeof(heap));
    }
    
    void Push(pair<long long,int> x) { //插入值x
		heap[size++] = x;
		Shiftup(size - 1);
	}
    pair<long long,int> Pop() {
		assert(size >= 0);
		if (empty()) return make_pair(-1, -1);
		auto ans = heap[0];
		heap[0] = heap[size - 1];
		heap[size - 1] = make_pair(__LONG_LONG_MAX__, INT32_MAX);
		Shiftdown(0);
		size--;
		return ans;
	}
	pair<long long,int> Top() const { return heap[0]; }
	bool empty() const { return size == 0; }
	void refresh() { size = 0; }
} priorityQueue;

//并查集
int parent[MAXN];
int Find(int x) { return parent[x] == x ? x : parent[x] = Find(parent[x]); }
void Union(int x, int prt) { parent[Find(x)] = prt; }

//缩点后的节点
int cleanPoints[MAXN];
int cleanIndex = 0;

void Dijkstra(int src, long long* dist, long long* routeCnt, int length) {
	memset(dist, 0x3f, sizeof(long long) * length); //注意传指针进来的时候是没有长度信息的
	memset(routeCnt, 0, sizeof(long long) * length);
	memset(vis, 0, sizeof(vis)); //vis偷懒用全局即可。
	dist[src] = 0, routeCnt[src] = 1;
	priorityQueue.refresh();
	priorityQueue.Push(make_pair(0, src));
	while (priorityQueue.empty() == false) {
		auto thisPair = priorityQueue.Pop();
		if (thisPair.second == -1) break;
		while (vis[thisPair.second]) {
			thisPair = priorityQueue.Pop();
		} if (thisPair.second == -1) break;
		int chosen = thisPair.second;
		vis[chosen] = true;
		for (int i = head[chosen]; i; i = graph[i].next) {
			if (dist[graph[i].succ] > dist[chosen] + graph[i].val) {
				routeCnt[graph[i].succ] = routeCnt[chosen] % MOD;
				routeCnt[graph[i].succ] %= MOD;
				dist[graph[i].succ] = dist[chosen] + graph[i].val;
				priorityQueue.Push(make_pair(dist[graph[i].succ], graph[i].succ));
			} else if (dist[graph[i].succ] == dist[chosen] + graph[i].val) {
				routeCnt[graph[i].succ] += routeCnt[chosen] % MOD;
				routeCnt[graph[i].succ] %= MOD;
				priorityQueue.Push(make_pair(dist[graph[i].succ], graph[i].succ));
			}
		}
	}
}

int main() {
	scanf("%d%d", &n, &m);
	for (int i = 1; i <= n; i++)
		parent[i] = i;
	for (int i = 0; i < m; i++) {
		scanf("%d%d%lld", &leftBound[i], &rightBound[i], &value[i]);
		if (value[i] == 0)
			Union(leftBound[i], rightBound[i]); //先缩一遍点，后面再扫描加边
	}
	for (int i = 0; i < m; i++)
		if (value[i] != 0)
			addEdge(Find(leftBound[i]), Find(rightBound[i]), value[i]);
	for (int i = 1; i <= n; i++)
		if (Find(i) == i) 
			cleanPoints[cleanIndex++] = i;
	//预处理结束后，所有剩余的节点存储在cleanPoints中
	Dijkstra(Find(1), srcDist, srcCnt, MAXN - 1);
	Dijkstra(Find(n), dstDist, dstCnt, MAXN - 1);
    scanf("%d", &q);
    for (int i = 0; i < q; i++) {
        int u, v, w;
        scanf("%d%d%d", &u, &v, &w);
		long long lengthUV = srcDist[Find(u)] + w + dstDist[Find(v)];
		long long lengthVU = srcDist[Find(v)] + w + dstDist[Find(u)];
		long long minVal = min(min(lengthUV, lengthVU), srcDist[Find(n)]);
		long long ans = 0;
		if (minVal == lengthUV)
			ans += (srcCnt[Find(u)] % MOD) * (dstCnt[Find(v)] % MOD) % MOD;
		if (minVal == lengthVU)
			ans += (srcCnt[Find(v)] % MOD) * (dstCnt[Find(u)] % MOD) % MOD;
		ans %= MOD;
		if (minVal == srcDist[Find(n)])
			ans += srcCnt[Find(n)] % MOD;
		ans %= MOD;
		printf("%lld\n", ans);
    } 
	return 0;
}