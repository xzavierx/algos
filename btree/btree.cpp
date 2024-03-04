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
  int *keys; 
  int t;
  BTreeNode **C;
  int n;
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


