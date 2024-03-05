/*
题目描述
这是一道模板题。
您需要写一种数据结构（可参考题目标题），来维护一个序列，其中需要提供以下操作：
翻转一个区间，例如原有序序列是 5 4 3 2 1，翻转区间是 [2, 4] 的话，结果是 5 2 3 4 1。

输入格式
第一行为 n, m，n 表示初始序列有 n 个数，这个序列依次是{ 1, 2, ... n - 1, n }，m 表示翻转操作次数。
接下来 m 行每行两个数 [l, r]，数据保证 1 <= l <= r <= n。

输出格式
输出一行 n 个数字，表示原始序列经过 m 次变换后的结果。

输入
5 3
1 3
1 3
1 4

输出
4 3 2 1 5

数据范围与提示
1 <= n, m <= 10e5
*/

#include <algorithm>
#include <cstdio>
const int N = 100005;

int n, m, l, r, a[N];

int rt, tot, fa[N], ch[N][2], val[N], sz[N], lazy[N];

struct Splay {
  void maintain(int x) { sz[x] = sz[ch[x][0]] + sz[ch[x][1]] + 1; }

  bool get(int x) { return x == ch[fa[x]][1]; }

  void clear(int x) {
    ch[x][0] = ch[x][1] = fa[x] = val[x] = sz[x] = lazy[x] = 0;
  }

  void rotate(int x) {
    int y = fa[x], z = fa[y], chk = get(x);
    ch[y][chk] = ch[x][chk ^ 1];
    if (ch[x][chk ^ 1]) fa[ch[x][chk ^ 1]] = y;
    ch[x][chk ^ 1] = y;
    fa[y] = x;
    fa[x] = z;
    if (z) ch[z][y == ch[z][1]] = x;
    maintain(y);
    maintain(x);
  }

  void splay(int x, int goal = 0) {
    if (goal == 0) rt = x;
    while (fa[x] != goal) {
      int f = fa[x], g = fa[fa[x]];
      if (g != goal) {
        if (get(f) == get(x))
          rotate(x);
        else
          rotate(f);
      }
      rotate(x);
    }
  }

  void tagrev(int x) {
    std::swap(ch[x][0], ch[x][1]);
    lazy[x] ^= 1;
  }

  void pushdown(int x) {
    if (lazy[x]) {
      tagrev(ch[x][0]);
      tagrev(ch[x][1]);
      lazy[x] = 0;
    }
  }

  int build(int l, int r, int f) {
    if (l > r) return 0;
    int mid = (l + r) / 2, cur = ++tot;
    val[cur] = a[mid], fa[cur] = f;
    ch[cur][0] = build(l, mid - 1, cur);
    ch[cur][1] = build(mid + 1, r, cur);
    maintain(cur);
    return cur;
  }

  int kth(int k) {
    int cur = rt;
    while (1) {
      pushdown(cur);
      if (ch[cur][0] && k <= sz[ch[cur][0]]) {
        cur = ch[cur][0];
      } else {
        k -= 1 + sz[ch[cur][0]];
        if (k <= 0) {
          splay(cur);
          return cur;
        }
        cur = ch[cur][1];
      }
    }
  }

  void reverse(int l, int r) {
    int L = kth(l), R = kth(r + 2);
    splay(L), splay(R, L);
    int tmp = ch[ch[L][1]][0];
    tagrev(tmp);
  }

  void print(int x) {
    pushdown(x);
    if (ch[x][0]) print(ch[x][0]);
    if (val[x] >= 1 && val[x] <= n) printf("%d ", val[x]);
    if (ch[x][1]) print(ch[x][1]);
  }
} tree;

int main() {
  // scanf("%d%d", &n, &m);
  auto n = 5;
  auto m = 1;
  for (int i = 0; i <= n + 1; i++) a[i] = i;
  rt = tree.build(0, n + 1, 0);
  while (m--) {
    auto l = 1, r = 3;
    // scanf("%d%d", &l, &r);
    tree.reverse(l, r);
  }
  tree.print(rt);
  return 0;
}