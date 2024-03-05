#include <iostream>
using namespace std;
/*
一颗m阶的B树性质：
  1. 每个节点最多有m个子节点
  2. 每个非叶子节点（除根节点）最少有m/2(向上取整)个子节点
  3. 如果根节点不是叶子节点，那么它至少有两个子节点
  4. 有k个子节点的非叶子节点拥有k-1个键，且升序排列，满足k[i] < k[i+1]
  5. 每个节点至多包含2k-1个键
  6. 所有的叶子节点都在同一层
*/
class BTreeNode {
  int *keys;          // 元素数组
  int t;              // 最小阶(元素数量范围)
  BTreeNode **C;      // 子节点数组
  int n;              // 当前元素个数
  bool leaf;

public:
  BTreeNode(int _t, bool _leaf);

  void traverse();
  BTreeNode *search(int k);
  // 前提条件：节点没满
  void insertNonFull(int k);
  void splitChild(int i, BTreeNode *y);

  friend class BTree;
};

class BTree {
  BTreeNode *root;
  int t;

public:
  BTree(int _t) {
    root = nullptr, t = _t;
  }

  void traverse() {
    if (root != nullptr) root->traverse();
  }

  BTreeNode* search(int k) {
    return (nullptr == root) ? nullptr : root->search(k);
  }

  void insert(int);
};

BTreeNode::BTreeNode(int _t, bool _leaf) {
  t = _t;
  leaf = _leaf;
  
  keys = new int[2 * t - 1];
  C = new BTreeNode*[2 * t];

  n = 0;
}

BTreeNode* BTreeNode::search(int k) {
  int i = 0;
  while (i < n && k > keys[i]) i++;

  if (keys[i] == k) return this;

  if (leaf) return nullptr;

  return C[i]->search(k);
}

void BTreeNode::traverse() {
  int i;
  for (i = 0; i < n; i++) {
    // 如果当前节点不是叶子节点，先遍历以C[i]为根的子树
    if (!leaf) C[i]->traverse();
    cout << " " << keys[i];
  }
  // 打印以最后一个孩子为根的子树
  if (!leaf)
    C[i]->traverse();
}

// 如果节点满了，先分裂节点，然后再插入新元素
void BTree::insert(int k) {
  if (root == nullptr) {
    root = new BTreeNode(t, true);
    root->keys[0] = k; // 插入节点k
    root->n = 1; // 更新节点的元素格式为1
  } else {
    // 当根节点已满，则对B-树进行生长操作
    if (root->n == 2 * t - 1) {
      BTreeNode *s = new BTreeNode(t, false);

      s->C[0] = root;

      // 将旧的根节点分裂为两个，并将元素移到新的根节点
      s->splitChild(0, root);

      int i = 0;
      // 确定哪个孩子将拥有新的节点
      if (s->keys[0] < k) ++i;
      s->C[i]->insertNonFull(k);

      root = s;
    } else {
      root->insertNonFull(k);
    }
  }
}

void BTreeNode::insertNonFull(int k) {
  int i = n - 1;
  if (leaf) {
    // 找到合适位置
    while (i >= 0 && keys[i] > k) {
      keys[i + 1] = keys[i];
      i--;
    }
    // 插入新的元素
    keys[i+1] = k;
    n = n + 1;
  } else {
    // 找到第一个小于元素k的孩子节点, 
    while (i >= 0 && keys[i] > k) --i;

    // 检查孩子节点是否已满
    if (C[i + 1] -> n == 2 * t - 1) {
      splitChild(i + 1, C[i+1]);

      // 分裂后，C[i]中间的元素移到父节点
      // C[i]分裂成为两个孩子节点
      // 找到合适位置插入中间元素
      if (keys[i+1] < k) ++i;
    }

    C[i+1]->insertNonFull(k);
  }
}

void BTreeNode::splitChild(int i, BTreeNode *y) {
  BTreeNode *z = new BTreeNode(y->t, y->leaf);
  z->n = t - 1;

  // 将节点y的后t-1个元素拷贝到z中
  for (int j = 0; j < t - 1; j++) z->keys[j] = y->keys[j + t];

  // 如果y不是叶子节点，拷贝y的后t个孩子到z中
  if (!y->leaf) {
    for (int j = 0; j < t; j++) z->C[j] = y->C[j+t];
  }

  // 将y所包含的元素个数设置为t-1
  y->n = t - 1;

  // 给当前节点的指针分配新的空间
  // 因为有新的元素加入，父节点将多一个孩子
  for (int j = n; j >= i + 1; j--) C[j + 1] = C[j];

  // 当前节点的下一个孩子设置为z
  C[i + 1] = z;

  // 在父节点中找到合适位置
  for (int j = n - 1; j >= i; j--) {
    keys[j + 1] = keys[j];
  }

  // 拷贝中间元素到父节点中
  keys[i] = y->keys[t - 1];

  n = n + 1;
}

int main() {
  BTree t(3); // A B-Tree with minium degree 3

  t.insert(10);
  t.insert(20);
  t.insert(5);
  t.insert(6);
  t.insert(3);
  t.insert(2);

  cout << "Traveral of the constructed tree is";
  t.traverse();
}