#include <bits/stdc++.h>
using namespace std;

////////// Splay Tree
template <typename T>
struct SearchResult {
  bool found;
  T value;

  SearchResult() : found(false) {}
  SearchResult(T value) : found(true), value(value) {}
};

template <typename T>
class SplayTree {
  template <typename U>
  struct Node {
    U key;
    Node *left, *right;
    Node *parent;
    int count;

    Node(U key) :
      key(key), left(nullptr), right(nullptr), parent(nullptr), count(1) {}
    ~Node() {
      if (left != nullptr)
        delete left;

      if (right != nullptr)
        delete right;
    }

    void copy(const Node<U>& node) {
      this->key = node.key;
      this->count = node.count;
    }
  };

  Node<T> *root;

  bool isLeftChild(const Node<T> *const node) const {
    assert(node->parent != nullptr);
    return node->key < node->parent->key;
  }

  void rotateLeft(Node<T> *node) {
    auto q = node->right;
    assert(q != nullptr);
    node->right = q->left;
    if (q->left != nullptr)
      q->left->parent = node;
    q->left = node;
    if (node->parent != nullptr) {
      if (isLeftChild(node))
        node->parent->left = q;
      else
        node->parent->right = q;
    }
    q->parent = node->parent;
    node->parent = q;
  }

  void rotateRight(Node<T> *node) {
    auto q = node->left;
    assert(q != nullptr);
    node->left = q->right;
    if (q->right != nullptr)
      q->right->parent = node;
    q->right = node;
    if (node->parent != nullptr) {
      if (isLeftChild(node))
        node->parent->left = q;
      else
        node->parent->right = q;
    }
    q->parent = node->parent;
    node->parent = q;
  }

  void splay(Node<T> *node) {
    if (node == nullptr) return;

    while (node->parent != nullptr) {
      // parent of node
      auto p = node->parent;
      // grandparent of node
      auto g = p->parent;

      if (g == nullptr) {
        // zig operation
        if (isLeftChild(node)) {
          rotateRight(p);
        } else {
          rotateLeft(p);
        }
      } else if (isLeftChild(node) && isLeftChild(p)) {
        // zig-zig operation
        rotateRight(g);
        rotateRight(p);
      } else if (isLeftChild(node)) {
        // zig-zag operation
        rotateRight(p);
        rotateLeft(g);
      } else if (isLeftChild(p)) {
        // zig-zag operation
        rotateLeft(p);
        rotateRight(g);
      } else {
        // zig-zig operation
        rotateLeft(g);
        rotateLeft(p);
      }
    }

    // change root to node
    root = node;
  }

  Node<T> *minNode(Node<T> *node) const {
    assert(node != nullptr);
    Node<T> *parent = nullptr;
    while (node != nullptr) {
      parent = node;
      node = node->left;
    }
    return parent;
  }

  Node<T> *maxNode(Node<T> *node) const {
    assert(node != nullptr);
    Node<T> *parent = nullptr;
    while (node != nullptr) {
      parent = node;
      node = node->right;
    }
    return parent;
  }

  void printNode(Node<T> *node, int space = 0) const {
    std::cout << std::string(space, ' ');
    if (node == nullptr) std::cout << "X" << std::endl;
    else {
      std::cout << node->key << "(" << node->count << ")"<< std::endl;
      printNode(node->left, space+1);
      printNode(node->right, space+1);
    }
  }

  public:
  SplayTree() : root(nullptr) {
  }
  ~SplayTree() {
    if (root != nullptr)
      delete root;
  }

  void insert(T key) {
    if (root == nullptr) {
      root = new Node<T>(key);
      return;
    }

    Node<T> *node = root, *parent = nullptr;
    while (node != nullptr) {
      parent = node;

      if (key < node->key) {
        node = node->left;
      } else if (node->key < key) {
        node = node->right;
      } else {
        // Key repeated, increase count
        node->count++;
        break;
      }
    }

    if (node == nullptr) {
      node = new Node<T>(key);
      node->parent = parent;

      if (key < parent->key) {
        parent->left = node;
      } else {
        parent->right = node;
      }
    }

    splay(node);
  }

  void erase(T key) {
    Node<T> *node = root, *parent = nullptr;
    while (node != nullptr) {
      if (key < node->key) {
        parent = node;
        node = node->left;
      } else if (node->key < key) {
        parent = node;
        node = node->right;
      } else if (node->count > 1) {
        // More than one for this key, descrease count
        node->count--;
        node = nullptr;
      } else if (node->right == nullptr) {
        if (parent == nullptr) {
          root = node->left;
        } else if (node->key < parent->key) {
          parent->left = node->left;
        } else {
          parent->right = node->left;
        }

        if (node->left != nullptr)
          node->left->parent = parent;

        node->left = nullptr;
        delete node;
        node = nullptr;
      } else {
        Node<T> *tmp = minNode(node->right);
        node->copy(*tmp);
        tmp->count = 1;

        key = tmp->key; // change the key to delete
        parent = node;
        node = node->right; // continue search
      }
    }

    if (parent == nullptr) return;

    splay(parent);
  }

  T min() {
    auto node = minNode(root);
    return node->key;
  }

  T max() {
    auto node = maxNode(root);
    return node->key;
  }

  SearchResult<T> lowerBound(T key) const {
    Node<T> *node = root, *bound = nullptr;
    while (node != nullptr) {
      if (node->key < key) {
        node = node->right;
      } else {
        bound = node;
        node = node->left;
      }
    }

    if (bound == nullptr) {
      return SearchResult<T>();
    } else {
      return SearchResult<T>(bound->key);
    }
  }

  SearchResult<T> previous(T key) const {
    Node<T> *node = root;
    while (node != nullptr) {
      if (key < node->key)
        node = node->left;
      else if (node->key < key)
        node = node->right;
      else break;
    }

    if (node == nullptr)
      return SearchResult<T>();
    else if (node->left != nullptr)
      return SearchResult<T>(maxNode(node->left)->key);
    else {
      while (node->parent != nullptr && isLeftChild(node)) {
        node = node->parent;
      }

      if (node->parent == nullptr)
        return SearchResult<T>();
      else
        return SearchResult<T>(node->parent->key);
    }
  }

  bool empty() const {
    return root == nullptr;
  }

  void print() const {
    printNode(root);
  }
};
//////////

// template <class T>
// using CmpFun2 = bool (*)(T, T);

// template <class T>
// using CmpFun1 = bool (*)(T);

////////// Lista ligada
template <class T>
class List {
private:
  class Node {
  public:
    T data;
    Node *next;

  public:
    Node(T d): data(d), next(nullptr) {}
  } *head;

  // Construtor
public:
  List(void): head(nullptr) {}

  // Métodos
public:
  void insert(T data);              // Insere na frente da lista O(1)
//  bool search(T data, CmpFun2<T> cmp, T *res); // Busca data na lista utilizando cmp (T -> T -> bool) a:T = b:T
  bool search(T data, T *res); // Busca utilizando T1.cmp(T2) => T1 = T2
  void print(void);
};

// Imprimir lista (debug)
template <class T>
void List<T>::print(void) {
  auto n = List::head;

  while(n != nullptr) {
    cout << " " << n->data;
    n = n->next;
  }
}

// Inserção na lista. Inserção ocorre na cabeça da lista, portanto Theta(1)
template <class T>
void List<T>::insert(T data) {
  auto n = new List::Node(data);
  n->next = head;
  head = n;
}

//template <class T>
//bool List<T>::search(T data, CmpFun2<T> cmp, T *res) {
//  auto n = head;
//
//  bool found = false;
//  while (n != nullptr && !found) {
//    if(cmp(n->data, data)) {
//      found = true;
//      *res = n->data;
//    } else {
//      n = n->next;
//    }
//  }
//  return found;
//}

template <class T>
bool List<T>::search(T data, T *res) {
  auto *n = head;

  bool found = false;
  while (n != nullptr && !found) {
    if(data.cmp(n->data)) {
      found = true;
      *res = n->data;
    } else {
      n = n->next;
    }
  }

  return found;
}

////////// Tabela hash
class HashTbl {
private:
  class HashEntry {
  public:
    int key;
    int val;

    // Construtores
  public:
    HashEntry(int k, int v): key(k), val(v) {};

    // Métodos
  public:
    inline bool cmp(HashEntry a) { return key == a.key; }
  };

  int size;
  List<HashEntry> *table;
  inline int hash(int key) { return key % size; };

  // Construtores
public:
  HashTbl(int s): size(s) { table = new List<HashEntry>[size]; };

  // Métodos
  void insert(int key, int val) { table[hash(key)].insert(HashEntry(key,val)); }
  int lookup(int key);
};

// Retorna valor associado a chave. Caso não encontrar retorna -1.
int HashTbl::lookup(int key) {
  auto e = HashEntry(key,-1);
  table[hash(key)].search(e, &e);
  return e.val;
}

const int N = 100100;
int keys[N];

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  cout.tie(nullptr);

  SplayTree<int> order;
  HashTbl parent(N), level(N);

  int n;
  cin >> n;
  for (int i = 0, x; i < n; i++) {
    cin >> x;
    keys[i] = x;

    if (!order.empty()) {
      auto upper = order.lowerBound(x);

      if (!upper.found) {
        // all keys are less than x
        // sorted seq: ... W X
        int w = order.max();
        parent.insert(x, w);
        level.insert(x, level.lookup(w) + 1);
      } else if (upper.value == order.min()) {
        // all keys are greater than x
        // sorted seq: X W ...
        int w = order.min();
        parent.insert(x, w);
        level.insert(x, level.lookup(w) + 1);
      } else {
        int y = upper.value;
        auto lower = order.previous(y);
        int z = lower.value;

        // z and y are in the middle

        // sorted seq: ... Z X Y ...

        // upper (Y) is the parent? (Case 1)
        // Y is in the subtree of Z?
        if (level.lookup(y) > level.lookup(z)) {
          parent.insert(x, y);
          level.insert(x, level.lookup(y) + 1);
        } else {
          // lower (Z) is the parent?
          // Z is in the subtree of Y?
          parent.insert(x, z);
          level.insert(x, level.lookup(z) + 1);
        }
      }
    }

    order.insert(x);
  }

  int q;
  cin >> q;
  for (int i = 0, j; i < q; i++) {
    cin >> j;
    cout << parent.lookup(keys[j-1]);
    if (i < q-1)
      cout << " ";
  }
  cout << "\n";

  return 0;
}
