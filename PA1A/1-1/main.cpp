#include <iostream>
#include <iomanip>
#include <cstdio>
#include <string>
#include <cstring>
using namespace std;

const int VAL = 10000000;

class BigInteger {
private:
    unsigned long long number[1500];
    unsigned short index;
    bool lazy;
public:
    BigInteger(string val);
    BigInteger();
    BigInteger operator *(const BigInteger& b);
    void setIndex(int _index) {
        index = _index;
    }
    void addTo(unsigned short digit, unsigned long long value);
    BigInteger& operator=(const BigInteger& b) {
        memset(number, 0, sizeof(number));
        index = b.index;
        for (int i = 0; i < index; i++)
            number[i] = b.number[i];
        return *this;
    }
    void show();
    void debugshow();
    void carry();
};

BigInteger::BigInteger(string val): index(0), lazy(true) {
    memset(number, 0, sizeof(number));
    while (val.length() >= 8) {
        string tempstr = val.substr(val.length() - 7, 7);
        val = val.substr(0, val.length() - 7);
        number[index++] = stoi(tempstr);
    } 
    number[index++] = (unsigned int)(stoi(val));
}

BigInteger::BigInteger() : index(1), lazy(true) {
    memset(number, 0, sizeof(number));
}

BigInteger BigInteger::operator*(const BigInteger& b) {
    BigInteger ret;
    for (int i = 0; i < index; i++) {
        for (int j = 0; j < b.index; j++) {
            unsigned long long tempVal = number[i] * 1ll * b.number[j];
            ret.addTo(i + j, tempVal);
        }
    } ret.setIndex(index + b.index - 1);
    return ret;
}

void BigInteger::addTo(unsigned short digit, unsigned long long value) {
    lazy = false;
    number[digit] += value;
}

void BigInteger::show() {
    carry();
    for (int i = index - 1; i >= 0; i--)
        if (i == index - 1)
            cout << number[i];
        else
            cout << setfill('0') << setw(7) << number[i];
    cout << endl;
}

void BigInteger::carry() {
    if (!lazy) {
        for (int i = 0; i < index; i++) {
            number[i + 1] += number[i] / VAL;
            number[i] = number[i] % VAL;
        }
        if (number[index] != 0)
            index++;
        lazy = true;
    } 
}

int main() {
    ios::sync_with_stdio(false);
    int n;
    string s1, s2;
    cin >> n;
    while (n--) {
        cin >> s1 >> s2;
        BigInteger a(s1);
        BigInteger b(s2);
        BigInteger c = a * b;
        c.show();
    }
    return 0;
}
