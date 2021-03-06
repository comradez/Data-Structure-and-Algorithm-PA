# PA1-2解题报告

## 使用的数据结构和算法

由于保证任两条线段不相交，首先将两坐标轴上的点排序，然后依次对应连接得到n条线段。

由于第k条线段和坐标轴构成的三角形包含在第k+1条线段和坐标轴构成的三角形中，可知如果OP与线段的相交与否具有单调性，如果和第k条线段相交就一定会
和第k-1条线段相交；如果第k条线段不相交就一定不和第k+1条线段相交，可以用二分查找来处理每个查询。

这里我采取了建立Point类型（表示P点）和Pair类型（表示每条线段）的方式来实现。

## 遇到的问题和解决方案

最初的实现中没有区分P点的横纵坐标和线段的横纵截距，导致逻辑混乱；修改后就解决了这一问题。

因为横纵坐标最大可以到2^31，因此ToLeft测试时要开long long。

## 时空复杂度估算

存储n条线段的复杂度为O(n)，m组询问即用即丢，只需要O(1)的额外空间，因此空间复杂度为O(n)。

对O(n)的数组排序需要O(nlogn)时间，m组查询，每组查询需要O(logn)时间，总时间复杂度为O((n+m)logn)