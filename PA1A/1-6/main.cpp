#include <iostream>
#include <cstdio>
#include <cstring>
#include <cassert>
using namespace std;

const int MAXN = 4e6 + 5;
char inputString[3200005];
char command[5], command1[5], command2[5];
int n, posLeft, posRight;
bool reversed = false;

class Stack {
private:
	char _data[MAXN];
	int _top;
public:
	Stack() : _top(0) {}
	void push(char ch) {
		_data[_top++] = ch;
	}
	char pop() {
		return _data[--_top];
	}
	char top() {
		return _data[_top];
	}
	bool empty() {
		return _top == 0;
	}
	void show()	{
		for (int i = 0; i < _top; i++)
			putchar(_data[i]);
	}
	void revshow() {
		for (int i = _top - 1; i >= 0; i--)
			putchar(_data[i]);
	}
} stackLeft, stackRight;

class Deque {
private:
	char _data[2 * MAXN + 1];
	int _head, _tail;
	bool reversed;
public:
	Deque() : _head(MAXN), _tail(MAXN), reversed(false) {}
	Deque(const char* inputString)
		: _head(MAXN - strlen(inputString) / 2)
		, _tail(MAXN - strlen(inputString) / 2)
		, reversed(false) {
		for (int i = 0; i < strlen(inputString); i++)
			_data[_tail++] = inputString[i];
	}
	void init(const char* inputString) {
		_head = _tail = MAXN - strlen(inputString) / 2;
		reversed = false;
		for (int i = 0; i < strlen(inputString); i++)
			_data[_tail++] = inputString[i];
	}
	bool empty() {
		return _head == _tail;
	}
	void push_back(char ch) {
		if (reversed)
			_data[--_head] = ch;
		else
			_data[_tail++] = ch;
	}
	void push_front(char ch) {
		if (reversed)
			_data[_tail++] = ch;
		else
			_data[--_head] = ch;
	}
	char pop_back() {
		if (reversed)
			return _data[_head++];
		else
			return _data[--_tail];
	}
	char pop_front() {
		if (reversed)
			return _data[--_tail];
		else
			return _data[_head++];
	}
	char get_back() {
		return reversed ? _data[_head] : _data[_tail - 1];
	}
	char get_front() {
		return reversed ? _data[_tail - 1] : _data[_head];
	}
	void reverse() {
		reversed = !reversed;
	}
	void show()	{
		if (reversed)
				for (int i = _tail - 1; i >= _head; i--)
					putchar(_data[i]);
			else
				for (int i = _head; i < _tail; i++)
					putchar(_data[i]);
	}
} deque;


void handleMove(char direction, char cursor) { 
	if (cursor == 'L') {
		if (direction == '<') {
			if (posLeft <= posRight) {
				if (!stackLeft.empty()) {
					deque.push_front(stackLeft.pop());
				} else {
					puts("F");
					return;
				}
			} else stackRight.push(deque.pop_back()); //posLeft > posRight时一定可以左移
		} else { //direction == '>'
			if (posLeft < posRight)
				stackLeft.push(deque.pop_front()); //posLeft < posRight时一定可以右移
			else if (posLeft >= posRight) {
				if (!stackRight.empty()) {
					deque.push_back(stackRight.pop());
				} else {
					puts("F");
					return;
				}
			}
		}
		posLeft += direction == '<' ? -1 : 1;
		puts("T");
	} else { //cursor == 'R'
		if (direction == '<') {
			if (posLeft < posRight)
				stackRight.push(deque.pop_back());
			else if (posLeft >= posRight) {
				if (!stackLeft.empty()) {
					deque.push_front(stackLeft.pop());
				} else {
					puts("F");
					return;
				}
			}
		} else { //direction == '>
			if (posLeft <= posRight) {
				if (!stackRight.empty()) {
					deque.push_back(stackRight.pop());
				} else {
					puts("F");
					return;
				}
			} else stackLeft.push(deque.pop_front());
		}
		posRight += direction == '<' ? -1 : 1;
		puts("T");
	}
}

void handleInsertBefore(char cursor, char ch) {
	if (cursor == 'L') {
		if (posLeft <= posRight) {
			stackLeft.push(ch);
			posRight += 1;
		} else deque.push_back(ch);
		posLeft += 1;
	} else { // cursor == 'R'
		if (posLeft < posRight) {
			deque.push_back(ch);
		} else { // posLeft >= posRight
			stackLeft.push(ch);
			posLeft += 1;
		} posRight += 1;
	} puts("T"); //Insert一定输出T
}

void handleDeleteAfter(char cursor) {
	if (cursor == 'L') {
		if (posLeft < posRight) {
			deque.pop_front();
			posRight -= 1;
		} else if (!stackRight.empty()) { //posLeft >= posRight
			stackRight.pop();
		} else {
			puts("F");
			return;
		}
	} else { //cursor == 'R'
		if (posLeft <= posRight) {
			if (!stackRight.empty()) {
				stackRight.pop();
			} else {
				puts("F");
				return;
			}
		} else {
			deque.pop_front(); // posLeft > posRight, 删除一定不会出问题
			posLeft -= 1;
		}
	} puts("T");
}

void handleRevert() {
	if (posLeft >= posRight) {
		puts("F");
		return;
	} 
	deque.reverse();
	puts("T");
}

void handleShow() {
	stackLeft.show();
	deque.show();
	stackRight.revshow();
	putchar('\n');
}

int main() {
	scanf("%s", inputString);
	deque.init(inputString);
	posRight = strlen(inputString);
	scanf("%d", &n);
    getchar();
    while (n--) {
        scanf("%s", command);
        switch (command[0]) {
            case '>':
                scanf("%s", command1);
                handleMove('>', command1[0]);
                break;
            case '<':
                scanf("%s", command1);
                handleMove('<', command1[0]);
                break;
            case 'I':
                scanf("%s", command1);
                scanf("%s", command2);
                handleInsertBefore(command1[0], command2[0]);
                break;
            case 'D':
                scanf("%s", command1);
                handleDeleteAfter(command1[0]);
                break;
            case 'R':
                handleRevert();
                break;
            case 'S':
				handleShow();
                break;
            default:
                assert(0);
        }
    }
	return 0;
}