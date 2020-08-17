#include <iostream>
#include <cassert>

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

  Node<T> *minNode(Node<T> *node) {
    assert(node != nullptr);
    Node<T> *parent = nullptr;
    while (node != nullptr) {
      parent = node;
      node = node->left;
    }
    return parent;
  }

  Node<T> *maxNode(Node<T> *node) {
    assert(node != nullptr);
    Node<T> *parent = nullptr;
    while (node != nullptr) {
      parent = node;
      node = node->right;
    }
    return parent;
  }

  void inorderNode(Node<T> *node) const {
    if (node == nullptr) {
      return;
    }

    inorderNode(node->left);
    std::cout << " " << node->key;
    inorderNode(node->right);
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
    splay(node);
    return node->key;
  }

  T max() {
    auto node = maxNode(root);
    splay(node);
    return node->key;
  }

  void inorder() const {
    inorderNode(root);
    std::cout << std::endl;
  }

  void print() const {
    printNode(root);
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
};

int main() {
  auto splayTree = SplayTree<int>();

  char op;
  while (std::cin >> op) {
    if (op == 'Q') break;
    int key;

    switch (op) {
      case 'I':
        std::cin >> key;
        splayTree.insert(key);
        break;
      case 'D':
        std::cin >> key;
        splayTree.erase(key);
        break;
      case '0':
        std::cout << splayTree.min() << std::endl;
        break;
      case '9':
        std::cout << splayTree.max() << std::endl;
        break;
      case 'P':
        splayTree.inorder();
        break;
      case 'L':
        std::cin >> key;
        auto lower = splayTree.lowerBound(key);
        if (lower.found)
          std::cout << lower.value << std::endl;
        else
          std::cout << "NOT FOUND" << std::endl;
        break;
    }
  }

  return 0;
}
