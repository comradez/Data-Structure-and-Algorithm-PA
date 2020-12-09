#include <iostream>
#include <cstdio>
#include <cstring>
using namespace std;

const int INF = 105;

int n, m, k;
char str1[501005], str2[501005];
char store[501005][205];

char& get(int i, int j) {
    return store[i][101 + j];
}

int main() {
    scanf("%d%d%d", &n, &m, &k);
    scanf("%s", str1);
    scanf("%s", str2);
    if (n - m > k || n - m < -k) {
        puts("-1");
        return 0;
    } //直接暴毙
    for (int i = 0; i <= n; i++)
        for (int j = 0; j <= 204; j++)
            store[i][j] = INF;

    for (int j = -k; j <= k; j++)
        if (n + j >= 0 && n + j <= m)
            get(n, j) = m - j - n;

    for (int i = n - 1; i >= 0; i--) { //从后往前扫描
        for (int j = k; j >= -k; j--) {
            int ptr1 = i, ptr2 = i + j;
            if (ptr2 < 0 || ptr2 > m) {
                get(i, j) = INF;
                continue;
            }
            get(i, j) = min(min(get(i + 1, j - 1), get(i, j + 1)) + 1, INF);
            if (str1[ptr1] == str2[ptr2]) //这一位对应相等，直接后移一位
                get(i, j) = min(get(i + 1, j), get(i, j));
        }
    }
    cout << (get(0, 0) > k ? -1 : int(get(0, 0))) << endl;
    return 0;
}