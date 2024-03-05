#include <algorithm>
#include <iterator>
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
using namespace std;

const int N = 100005;
int rt, tot, fa[N], ch[N][2], val[N], cnt[N], sz[N];

struct Splay {
  void maintain(int x) { 
    // 节点个数 
    sz[x] = sz[ch[x][0]] + sz[ch[x][1]] + cnt[x];
  }
  bool get(int x) { 
    // 节点是否为父亲节点的右儿子
    return x == ch[fa[x]][1]; 
  }
  void clear(int x) {
    ch[x][0] = ch[x][1] = fa[x] = val[x] = sz[x] = cnt[x] = 0;
  }
  void rotate(int x) {
    // y为x的父节点，z为x的祖父节点，检查x为左节点还是右节点
    int y = fa[x], z = fa[y], chk = get(x);
    /* 
      chk = 0 - 左儿子, 则右旋
      chk = 1 - 右儿子，则左旋
      形如
         y           x 
        / \         / \
       x   C       A   y 
      / \             / \ 
     A   B           B   C
    */ 
    ch[y][chk] = ch[x][chk ^ 1]; // 将x的右儿子（右旋）或左儿子（左旋）给父节点的左儿子或右儿子
    if (ch[x][chk ^  1]) fa[ch[x][chk ^ 1]] = y; // 修改父亲属性

    ch[x][chk ^ 1] = y; // 将父节点赋给x的右儿子(右旋)或左儿子（左旋）

    fa[y] = x;    // 修改父亲属性
    fa[x] = z;    // 修改父亲属性
    if (z) ch[z][y == ch[z][1]] = x; // 将x和y的位置对调

    maintain(y); // y 已经属于x的儿子了，先操作y
    maintain(x);
  }
  // splay操作
  /*
    zig:                            zig-zag:
      y          x                   z             x
     /    ===>    \                 /             / \ 
    x              y               y      ===>   y   z 
    y              x                \
     \    ===>    /                  x 
      x          y 
                                     z 
    zig-zig                           \            x
        z        x                     y  ===>    / \ 
       /          \                   /          z   y
      y   ===>     y                 x
     /              \
    x                z
    z                x
     \              /
      y            y
       \          /
        x        z
  */
  //根据上面这几种状态进行
  // void splay(int x) {
  //   for (int f = fa[x]; f = fa[x], f; rotate(x))
  //     if (fa[f]) rotate(get(x) == get(f) ? f : x);
  //   rt = x;
  // }

  // 如果想找到序列区间[L, R]代表的子树，只需要将代表aL-1的节点Splay到根，
  // 再将代表aR+1的节点Splay到根的右儿子即可
  // 以[1, 3]举例，首先调用splay(1), 会将1 splay到根，然后splay(3, 1)
  // 下面代码的结束条件就是(fa[x] != goal)，所以一定会3的父节点必须是1(右儿子)
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

  void ins(int k) {
    if (!rt) {
      val[++tot] = k;
      cnt[tot]++;
      rt = tot;
      maintain(rt);
      return;
    }
    int cur = rt, f = 0;
    while (1) {
      if (val[cur] == k) {
        cnt[cur]++;
        maintain(cur);
        maintain(f);
        splay(cur);
        break;
      }
      f = cur;
      cur = ch[cur][val[cur] < k];
      if (!cur) {
        val[++tot] = k;
        cnt[tot]++;
        fa[tot] = f;
        ch[f][val[f] < k] = tot;
        maintain(tot);
        maintain(f);
        splay(tot);
        break;
      }
    }
  }

  int kth(int k) {
    int cur = rt;
    while (1) {
      if (ch[cur][0] && k <= sz[ch[cur][0]]) {
        cur = ch[cur][0];
      } else {
        k -= cnt[cur] + sz[ch[cur][0]];
        if (k <= 0) {
          splay(cur);
          return val[cur];
        }
        cur = ch[cur][1];
      }
    }
  }

  int rk(int k) {
    int res = 0, cur = rt;
    while (1) {
      if (k < val[cur]) {
        cur = ch[cur][0];
      } else {
        res += sz[ch[cur][0]];
        if (k == val[cur]) {
          splay(cur);
          return res + 1;
        }
        res += cnt[cur];
        cur = ch[cur][1];
      }
    }
  }
  
  int pre() {
    int cur = ch[rt][0];
    if (!cur) return cur;
    while (ch[cur][1]) cur = ch[cur][1];
    splay(cur);
    return cur;
  }

  int nxt() {
    int cur =  ch[rt][1];
    if (!cur) return cur;
    while (ch[cur][0]) cur = ch[cur][0];
    splay(cur);
    return cur;
  }

  void del(int k) {
    rk(k);
    if (cnt[rt] > 1) {
      cnt[rt]--;
      maintain(rt);
      return ;
    }
    if (!ch[rt][0] && !ch[rt][1]) {
      clear(rt);
      rt = 0;
      return;
    }
    if (!ch[rt][0]) {
      int cur = rt;
      rt = ch[rt][1];
      fa[rt] = 0;
      clear(cur);
      return ;
    }
    if (!ch[rt][1]) {
      int cur = rt;
      rt = ch[rt][0];
      fa[rt] = 0;
      clear(cur);
      return;
    }
    int cur = rt, x = pre();
    fa[ch[cur][1]] = x;
    ch[x][1] = ch[cur][1];
    clear(cur);
    maintain(rt);
  }

  int depth(int root) {
    return !root ? 0 : (1 + std::max(depth(ch[root][0]), depth(ch[root][1])));
  }

  void write(int root, int row, int column, vector<vector<string>> &res, int depth) {
    if (!root) return;

    res[row][column] = to_string(val[root]);

    int cur = (row + 1) / 2;
    if (cur == depth) return;

    int gap = depth - cur - 1;

    if (ch[root][0]) {
      res[row + 1][column - gap] = "/";
      write(ch[root][0], row + 2, column - gap * 2, res, depth);
    }

    if (ch[root][1]) {
      res[row + 1][column + gap] = "\\";
      write(ch[root][1], row + 2, column + gap * 2, res, depth);
    }
  }

  void show() {
    if (!rt) return;
    int d = depth(rt);

    int h = d * 2 - 1;
    int w =  (2 << (d - 2)) * 3 + 1;
    auto res = vector<vector<string>>(h, vector<string>(w));
    for (int i = 0; i < h; ++i) {
      for (int j = 0; j < w; ++j) {
        res[i][j] = " ";
      }
    }

    write(rt, 0, w / 2, res, d);
    for (auto line : res) {
      string c;
      for (int i = 0; i < line.size(); ++i) {
        c.append(line[i]);
        if (line[i].size() > 1 && i < line.size() - 1) {
          i += line[i].size() > 4 ? 2 : line[i].size() - 1;
        }
      }
      // std::copy(line.begin(), line.end(), std::ostream_iterator<string>(std::cout, ""));
      cout << c << endl;
    }
  }
};

int main() {
  Splay s;
  s.ins(3);
  s.ins(2);
  s.ins(4);
  s.ins(5);
  s.ins(1);
  s.ins(2);
  s.show();
  cout << s.kth(3) << endl;
  cout << s.rk(3) << endl;
}


