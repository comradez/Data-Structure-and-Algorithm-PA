#include <iostream>
#include <cstdio>
#include <cassert>
#include <cstring>
using namespace std;

int n, k;
char str[500005][80];

struct Node {
    Node *lc, *rc;
    int cnt, val;
    Node() : lc(nullptr), rc(nullptr), cnt(0), val(-1) {}
    ~Node() {
        delete lc; lc = nullptr;
        delete rc; rc = nullptr;
    }
    Node* getLc() { return lc != nullptr ? lc : lc = new Node(); }
    Node* getRc() { return rc != nullptr ? rc : rc = new Node(); }
};

class BinTree {
private:
    Node* _root;
public:
    BinTree() : _root(new Node()) {}
    Node* root() const { return _root; }
    Node* addWord(int wordIndex) {
        Node* currNode = _root;
        for (int i = 0; i < 64; i++) {
            currNode->cnt++;
            currNode = str[wordIndex][i] == '0' ? currNode->getLc() : currNode->getRc();
            //如果这位是0，左转；否则右转。注意getLc和getRc会check这个节点是否存在，如果不存在则创建。
        } currNode->cnt++;
        currNode->val = wordIndex;
        return currNode;
    }
    void delWord(int wordIndex) {
        Node* currNode = _root;
        Node* nextNode = nullptr;
        for (int i = 0; i < 64; i++) {
            currNode->cnt--;
            currNode = str[wordIndex][i] == '0' ? currNode->lc : currNode->rc;
            //这里一定存在，直接访问lc/rc即可，就不用getLc/getRc了
        } currNode->cnt--;
        
        Node** neoNode = &_root;
        for (int i = 0; i < 64; i++) {
            neoNode = str[wordIndex][i] == '0' ? &((*neoNode)->lc) : &((*neoNode)->rc);
            if ((*neoNode)->cnt <= 0) {
                delete (*neoNode);
                *neoNode = nullptr;
                return;
            }
        }
    }
    int query(int wordIndex) {
        Node* currNode = _root;
        for (int i = 0; i < 64; i++) {
            if (str[wordIndex][i] == '0') //如果有不同的就往不同的方向走，否则往相同方向走
                currNode = currNode->rc != nullptr && currNode->rc->cnt > 0 ? currNode->rc : currNode->lc;
            else
                currNode = currNode->lc != nullptr && currNode->lc->cnt > 0 ? currNode->lc : currNode->rc;
        }
        return currNode->val;
    }
};

BinTree tree;
int tail = 0, head = 0, rangel = 0, ranger = 0;
int store[500005];

int main() {
#ifndef _OJ_
    freopen("test.in", "r", stdin);
#endif
    scanf("%d%d", &n, &k);
    for (int i = 0; i < n; i++)
        scanf("%s", str[i]);
    head = tail = n - 1;
    for (int i = n - 1; i >= 0; i--) {
        rangel = max(i - k - 1, 0);
        ranger = min(i + k + 1, n - 1);
        while (head >= rangel)
            tree.addWord(head--);
        while (tail > ranger)
            tree.delWord(tail--);
        store[i] = tree.query(i);
    } 
    if (store[0] == 0) 
        store[0] = 1;
    //如果0查询到的结果为0，说明0能看到的整个范围内都和自己相等，这种情况特判，改为1
    for (int i = 0; i < n; i++)
        printf("%d\n", store[i]);
    return 0;
}