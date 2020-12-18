#include <iostream>
#include <cstdio>
#include <cassert>
using namespace std;

constexpr int MAXN = 1e6 + 5;
constexpr int INF = 9e6 + 5;

struct SplayNode {
	int child[2], value, size, prt;
	SplayNode() : value(0), size(0), prt(0) {
		child[0] = child[1] = 0;
	}
	//左右儿子，节点内的值，子树规模，父亲
};

class SplayTree {
private:
	SplayNode nodes[MAXN];
	int root, top, cnt;
	
	void update(int v) { //更新子树规模
		nodes[v].size = nodes[nodes[v].child[0]].size + nodes[nodes[v].child[1]].size + 1;
	}
	void rotate(int v, int cDirection) { //zig和zag；cDirection == 0为zig, 1为zag
		int p = nodes[v].prt; //parent
		int g = nodes[p].prt; //grandparent
		int c = nodes[v].child[cDirection ^ 1]; //变了父亲的那个child
		if (g != 0) {
			int pDirection = nodes[g].child[0] == p ? 0 : 1; //p是g的哪个儿子
			nodes[g].child[pDirection] = v;
		}
		nodes[v].prt = g;
		nodes[p].prt = v;
		nodes[v].child[cDirection ^ 1] = p;
		nodes[c].prt = p;
		nodes[p].child[cDirection] = c;
		update(p);
		update(v);
	}
	void splay(int v, int dst) { //将节点v不断zig-zag直至其成为dst的子节点，dst == 0表示旋转到根
		int p = nodes[v].prt; //parent
		int g = nodes[p].prt; //grandparent
		while (p != dst) {
			int vDirection = nodes[p].child[0] == v ? 0 : 1;
			int pDirection = nodes[g].child[0] == p ? 0 : 1;
			if (g == dst) { //旋到顶了
				rotate(v, vDirection);
			} else if (vDirection ^ pDirection == 1) { //gpv不在一条线上
				rotate(v, vDirection);
				rotate(v, pDirection);
			} else { //gpv在一条线上
				rotate(p, pDirection);
				rotate(v, vDirection);
			}
			p = nodes[v].prt;
			g = nodes[p].prt;
		} update(v);
		if (dst == 0) root = v;
	}
	void find(int value) { //找到值为value的节点而且将它旋转到根
		int currPos = root;
		if (cnt == 0 || cnt == 1) return;
		while (nodes[currPos].value != value) {
			int cDirection = nodes[currPos].value > value ? 0 : 1;
			currPos = nodes[currPos].child[cDirection];
		} splay(currPos, 0);
	}
public:
	SplayTree() : root(0), cnt(0) {}
	int getPred(int value) {
		find(value);
		int currPos = nodes[root].child[0]; //左子树的根
		if (currPos == 0) return -1;
		while (nodes[currPos].child[1] != 0)
			currPos = nodes[currPos].child[1];
		return currPos;
	}
	int getNext(int value) {
		find(value);
		int currPos = nodes[root].child[1]; //右子树的根
		if (currPos == 0) return -1;
		while (nodes[currPos].child[0] != 0)
			currPos = nodes[currPos].child[0];
		return currPos;
	}
	void insertNode(int value) { //插入一个值为value的节点
		cnt += 1;
		if (cnt == 1) { //空树的第一个节点
			root = cnt;
			nodes[cnt].size = 1;
			nodes[cnt].value = value;
		} else {
			int currPos = root, prtPos, cDirection;
			while (nodes[currPos].size > 0) { //直到找到一个没初始化过的位置为止
				prtPos = currPos;
				assert(nodes[currPos].value != value);
				if (nodes[currPos].value > value) {
					currPos = nodes[currPos].child[0];
					cDirection = 0;
				} else {
					currPos = nodes[currPos].child[1];
					cDirection = 1;
				}
			}
			nodes[cnt].prt = prtPos;
			nodes[cnt].value = value;
			nodes[cnt].size = 1;
			nodes[prtPos].child[cDirection] = cnt;
			splay(cnt, 0);
		}
	}
	void deleteNode(int value) {
		int predPos = getPred(value);
		int nextPos = getNext(value);
		splay(predPos, 0);
		splay(nextPos, root);
		nodes[nextPos].child[0] = 0;
	}
	int query(int value) { //输出不超过value的树上的最大节点的值
		int currPos = root, prePos = root;
		while (nodes[currPos].size > 0) {
			if (nodes[currPos].value > value) {
				currPos = nodes[currPos].child[0];
			} else if (nodes[currPos].value < value) {
				prePos = currPos;
				currPos = nodes[currPos].child[1];
			} else { //nodes[currPos].value == value
				prePos = currPos;
				break;
			}
		} assert(prePos > 0);
		splay(prePos, 0);
		return nodes[root].value;
	}
} splayTree;

int n, temp;
char op[2];

int main() {
	scanf("%d", &n);
	splayTree.insertNode(-1);
	splayTree.insertNode(INF);
	for (int i = 0; i < n; i++) {
		scanf("%s", op);
		scanf("%d", &temp);
		switch (op[0]) {
			case 'A': 
				splayTree.insertNode(temp);
			break;
			case 'B':
				splayTree.deleteNode(temp);
			break;
			case 'C':
				printf("%d\n", splayTree.query(temp));
			break;
			default:
				assert(0);
			break;
		}
	}
	return 0;
}