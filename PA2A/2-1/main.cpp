#include <iostream>
#include <cstdio>
#include <cassert>
using namespace std;

int n, m, numSibling, isLastChild[1000005];

struct treeNode {
	treeNode* parent;
	treeNode* prevSibling;
	treeNode* nextSibling;
	treeNode* firstChild;
	treeNode* lastChild;
	int index, suffMax, volume, childNum;
	treeNode(treeNode* _prt=nullptr, int _index=-1)
		: parent(_prt)
		, prevSibling(nullptr), nextSibling(nullptr)
		, firstChild(nullptr), lastChild(nullptr)
		, index(_index), suffMax(0), volume(-1), childNum(0) {}
	treeNode* getChild(int childIndex, bool& flag, int& lastValid) {
		//要有能处理非法输入的能力
		//如果自己没孩子或者超出范围，返回自己
		if (flag == false || childNum == 0 || childNum <= childIndex) {
			flag = false;
			return this;
		}
		treeNode* retNode = firstChild;
		while (childIndex--)
			retNode = retNode->nextSibling;
		lastValid = childIndex; //lastValid即为整条路径上最后一个合法点的rank
		return retNode;
	}
	int getHeight() const {
		if (childNum == 0) return 0;
		return firstChild->suffMax + 1;
	}
	void rmChild(int childIndex, treeNode* childNode = nullptr) {
		if (childNode == nullptr) {
			childNode = firstChild;
			while (childIndex--) childNode = childNode->nextSibling;
		} //实际上用的时候child已经给好了
		assert(childNum != 0);
		if (childNum == 1) { //既是first也是last
			firstChild = nullptr;
			lastChild = nullptr;
			calcHeight();
		} else if (childNode == firstChild) { //只是first
			childNode->nextSibling->prevSibling = nullptr;
			firstChild = childNode->nextSibling;
			childNode->nextSibling = nullptr;
			calcHeight();
		} else if (childNode == lastChild) { //只是last
			childNode->prevSibling->nextSibling = nullptr;
			lastChild = childNode->prevSibling;
			childNode->prevSibling = nullptr;
			lastChild->calcHeight();
		} else { //一般情况
			childNode->prevSibling->nextSibling = childNode->nextSibling;
			childNode->nextSibling->prevSibling = childNode->prevSibling;
			childNode->nextSibling = nullptr;
			childNode->prevSibling->calcHeight();
			childNode->prevSibling = nullptr;
		}
		childNode->parent = nullptr;
		childNode->prevSibling = nullptr;
		childNode->nextSibling = nullptr;
		childNum -= 1;
		updateVolume(-1 * childNode->volume);
	}
	void addChild(int childIndex, treeNode* childNode) {
		childNode->parent = this;
		if (childNum == 0) { //没有儿子的节点插入
			firstChild = childNode;
			lastChild = childNode;
			firstChild->calcHeight();
		} else if (childIndex == 0) { //插入为firstChild
			childNode->nextSibling = firstChild;
			firstChild->prevSibling = childNode;
			firstChild = childNode;
			firstChild->calcHeight();
		} else if (childIndex == childNum) { //插入为lastChild
			lastChild->nextSibling = childNode;
			childNode->prevSibling = lastChild;
			lastChild = childNode;
			lastChild->calcHeight();
		} else { //最一般的插入
			treeNode* istNode = firstChild;
			while (childIndex-->1)
				istNode = istNode->nextSibling;
			childNode->nextSibling = istNode->nextSibling;
			istNode->nextSibling->prevSibling = childNode;
			istNode->nextSibling = childNode;
			childNode->prevSibling = istNode;
			childNode->calcHeight();
		}
		childNum += 1;
		updateVolume(childNode->volume);
	}
	void calcHeight() {
		suffMax = 0;
		if (firstChild != nullptr)
			suffMax = max(firstChild->suffMax + 1, suffMax);
		if (nextSibling != nullptr)
			suffMax = max(nextSibling->suffMax, suffMax);
		if (prevSibling)
			prevSibling->calcHeight();
		else if (parent)
			parent->calcHeight();
		//既没有前一个兄弟又没有父节点的节点只能是根
	}
	int calcVolume() {
		if (firstChild == nullptr) return 1;
		for (treeNode* child = firstChild; child != lastChild->nextSibling; child = child->nextSibling)
			volume += child->calcVolume();
		return volume;
	}
	void updateVolume(int offset) {
		volume += offset;
		if (parent != nullptr) 
			parent->updateVolume(offset);
	}
};

class Tree {
private:
	int size;
	treeNode* points;
public:
	Tree(int _size)
		: size(_size), points(new treeNode[_size + 1]) { 
		for (int i = 0; i < _size + 1; i++)
			points[i].index = i;
	}
	~Tree() { delete[] points; }
	treeNode* root() const { return &points[1]; }
	void insertAsChild(int prt, int child) {
		treeNode* prtNode = &points[prt];
		treeNode* childNode = &points[child];
		childNode->parent = prtNode;
		if (prtNode->childNum > 0) { //有孩子
			prtNode->lastChild->nextSibling = childNode;
			childNode->prevSibling = prtNode->lastChild;
			prtNode->lastChild = childNode;
		} else {
			prtNode->firstChild = childNode;
			prtNode->lastChild = childNode;
		} prtNode->childNum += 1;
	}
	void calcHeightAndVolume() {
		treeNode* currNode = &points[1];
		do {
			if (currNode->childNum > 0) { //有孩子
				if (currNode->firstChild->volume == -1) { //-1表示没访问过
					currNode = currNode->firstChild;
					while (currNode->nextSibling != nullptr)
						currNode = currNode->nextSibling;
					continue;
				}
				currNode->volume = 1;
				for (auto loopNode = currNode->firstChild; loopNode != currNode->lastChild->nextSibling; loopNode = loopNode->nextSibling) {
					currNode->volume += loopNode->volume;
				} currNode->suffMax = max(currNode->suffMax, currNode->firstChild->suffMax + 1);
			} else currNode->volume = 1; //是叶子
			if (currNode->nextSibling != nullptr)
				currNode->suffMax = max(currNode->suffMax, currNode->nextSibling->suffMax);
			currNode = currNode->prevSibling != nullptr ? currNode->prevSibling : currNode->parent;
		} while (currNode != nullptr); //在currNode == root的时候会currNode = nullptr进而退出
	}
};

int main() {
	scanf("%d%d", &n, &m);
	Tree tree(n);

	for (int i = 1; i <= n; i++) {
		scanf("%d", &numSibling);
		for (int j = 0; j < numSibling; j++) {	
			int indexSibling;
			scanf("%d", &indexSibling);
			tree.insertAsChild(i, indexSibling);
			if (j == numSibling - 1)
				isLastChild[indexSibling] = i;
		}
	} tree.calcHeightAndVolume();
	for (int i = 0; i < m; i++) {
		int flag, length, pos;
		scanf("%d", &flag);

		bool _flag = true;
		int _lastValid = -1;
		scanf("%d", &length);
		treeNode* srcNode = tree.root();
		for (int i = 0; i < length; i++) {
			scanf("%d", &pos);
			if (_flag) srcNode = srcNode->getChild(pos, _flag, _lastValid);
		} //srcNode就是我们感兴趣（要查询/要被移动）的节点
		//而且srcNode是它的parent的第_lastValid个子节点
		switch (flag) {
			case 0: {//移动
				treeNode* prtNode = srcNode->parent;
				prtNode->rmChild(_lastValid, srcNode);
				_flag = true;
				_lastValid = -1;
				scanf("%d", &length);
				treeNode* dstNode = tree.root();
				for (int i = 0; i < length; i++) {
					scanf("%d", &pos);
					dstNode = dstNode->getChild(pos, _flag, _lastValid);
				} scanf("%d", &pos);
				dstNode->addChild(pos, srcNode);
			} break;
			case 1: { //查询高度
				printf("%d\n", srcNode->getHeight());
			} break;
			case 2: {//查询容量
				printf("%d\n", srcNode->volume);
			} break;
			default: assert(0);
		}
	}
	return 0;
}