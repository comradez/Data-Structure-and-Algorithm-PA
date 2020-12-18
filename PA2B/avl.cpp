#include <iostream>
#include <cstdio>
#include <cassert>
using namespace std;

constexpr int INF = 9e6 + 5;

struct AVLNode {
	int data, height;
	int size;
	AVLNode* lc;
    AVLNode* rc;
	AVLNode(int _data = 0): data(_data), height(1), size(1), lc(nullptr), rc(nullptr){}
};
typedef AVLNode* AVLTree;
int getSize(AVLTree p) { return p ? p->size : 0; }
int getHeight(AVLTree p) { return p ? p->height : 0; }
int getBalanceFactor(AVLTree p) { return getHeight(p->lc) - getHeight(p->rc); }
int getFlag(AVLTree p, int value, bool useValue = false) {
    if (-1 <= getBalanceFactor(p) && getBalanceFactor(p) <= 1) return -1;
    if (useValue)
        return getBalanceFactor(p) > 1 ? (value < p->lc->data ? 0 : 1) : (value > p->rc->data ? 3 : 2);
    else 
        return getBalanceFactor(p) > 1 ? (getHeight(p->lc->lc) > getHeight(p->lc->rc) ? 0 : 1) : (getHeight(p->rc->rc) > getHeight(p->rc->lc) ? 3 : 2);
}
class AVL {
	AVLTree root;
	void update(AVLTree& p) {
		p->size = getSize(p->lc) + getSize(p->rc) + 1;
		p->height = max(getHeight(p->lc), getHeight(p->rc)) + 1;
	}
    void reBalance(AVLTree& p, int flag) {
        switch (flag) {
            case -1: return;
            case 0: p = connect34(p->lc->lc, p->lc, p, p->lc->lc->lc, p->lc->lc->rc, p->lc->rc, p->rc); break; //zig-zig
            case 1: p = connect34(p->lc, p->lc->rc, p, p->lc->lc, p->lc->rc->lc, p->lc->rc->rc, p->rc); break; //zag-zig
            case 2: p = connect34(p, p->rc->lc, p->rc, p->lc, p->rc->lc->lc, p->rc->lc->rc, p->rc->rc); break; //zig-zag
            case 3: p = connect34(p, p->rc, p->rc->rc, p->lc, p->rc->lc, p->rc->rc->lc, p->rc->rc->rc); break; //zag-zag
            default: return; //no need to rebalance
        }
    } 
    AVLTree connect34(AVLTree a, AVLTree b, AVLTree c, AVLTree t0, AVLTree t1, AVLTree t2, AVLTree t3) {
        a->lc = t0; a->rc = t1; update(a);
        c->lc = t2; c->rc = t3; update(c);
        b->lc = a; b->rc = c; update(b);
        return b;
    }
	void _insertNode_(AVLTree &p, int x) {
		if (p == nullptr) {
			p = new AVLNode(x);
			return;
		}
		if (p->data > x) {
			_insertNode_(p->lc, x);
            update(p);
            reBalance(p, getFlag(p, x, true));
		} else {
			_insertNode_(p->rc, x);
            update(p);
            reBalance(p, getFlag(p, x, true));
		}
		update(p);
	}
	void _deleteNode_(AVLTree& p, int x) {
		if (p == nullptr) return;
		if (p->data > x) {
			_deleteNode_(p->lc, x);
            update(p);
            reBalance(p, getFlag(p, -1));
		}
		else if(p->data < x) {
			_deleteNode_(p->rc, x);
            update(p);
            reBalance(p, getFlag(p, -1));
		}
		else {
			if (p->lc && p->rc) {
				AVLTree q = p->rc;
				while (q->lc) q = q->lc;
				p->data = q->data;
				_deleteNode_(p->rc, q->data);
				update(p);
                reBalance(p, getFlag(p, -1));
			} else {
				AVLTree q = p;
				if (p->lc) p = p->lc;
				else if (p->rc) p = p->rc;
				else p = nullptr;
				delete q;
				q = nullptr;
			}
		}
		if (p == nullptr) return;
		update(p);
	}
public:
    void deleteNode(int value) { _deleteNode_(root, value); }
    void insertNode(int value) { _insertNode_(root, value); }
    int query(int value) const {
        AVLTree node = root;
        AVLTree prevNode = root;
        while (node != nullptr) {
            if (node->data == value) {
                return value;
            } else if (node->data < value) {
                prevNode = node;
                node = node->rc;
            } else {
                node = node->lc;
            }
        } return prevNode->data;
    }
} avlTree;

int n, temp;
char op[2];

int main() {
	scanf("%d", &n);
	avlTree.insertNode(-1);
	avlTree.insertNode(INF);
	for (int i = 0; i < n; i++) {
		scanf("%s", op);
		scanf("%d", &temp);
		switch (op[0]) {
			case 'A': avlTree.insertNode(temp); break;
			case 'B': avlTree.deleteNode(temp); break;
			case 'C': printf("%d\n", avlTree.query(temp)); break;
			default: assert(0); break;
		}
	}
	return 0;
}