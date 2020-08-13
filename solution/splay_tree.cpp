#include <iostream>
#include <cassert>

template <typename T>
class SplayTree {
  template <typename U>
  struct Node {
    U key;
    Node *left, *right;
    Node *parent;

    Node(U key) : key(key), left(nullptr), right(nullptr), parent(nullptr) {}
    ~Node() {
      if (left != nullptr)
        delete left;

      if (right != nullptr)
        delete right;
    }
  };

  Node<T> *root;

  Node<T> *rotateLeft(Node<T> *node) {
    auto q = node->right;
    node->right = q->left;
    q->left = node;
    q->parent = node->parent;
    node->parent = q;
    return q;
  }

  Node<T> *rotateRight(Node<T> *node) {
    auto q = node->left;
    node->left = q->right;
    q->right = node;
    q->parent = node->parent;
    node->parent = q;
    return q;
  }

  Node<T> *splay(Node<T> *node) {
    if (node == nullptr)
      return nullptr;

    auto parent = node->parent;
    if (parent == nullptr)
      return node;

    auto grandparent = parent->parent;

    if (grandparent == nullptr) {
      // zig operation
      if (parent->left == node) {
      }
    } else {

    }
  }

  const Node<T> *minNode(const Node<T> *node) const {
    assert(node != nullptr);
    const Node<T> *parent = nullptr;
    while (node != nullptr) {
      parent = node;
      node = node->left;
    }
    return parent;
  }

  const Node<T> *maxNode(Node<T> *node) const {
    assert(node != nullptr);
    const Node<T> *parent = nullptr;
    while (node != nullptr) {
      parent = node;
      node = node->right;
    }
    return parent;
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
      } else {
        node = node->right;
      }
    }

    if (key < parent->key) {
      parent->left = new Node<T>(key);
    } else {
      parent->right = new Node<T>(key);
    }

    // TODO: splay parent
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
      } else if (node->right == nullptr) {
        if (parent == nullptr) {
          root = node->left;
        } else if (node->key < parent->key) {
          parent->left = node->left;
        } else {
          parent->right = node->left;
        }

        node->left = nullptr;
        delete node;
        node = nullptr;
      } else {
        const Node<T> *tmp = minNode(node->right);
        node->key = tmp->key;

        key = tmp->key; // change the key to delete
        parent = node;
        node = node->right; // continue search
      }
    }

    // not found the key
    if (parent == nullptr) return;

    // TODO: splay parent
  }

  T min() const {
    auto node = minNode(root);
    return node->key;
  }

  T max() const {
    auto node = maxNode(root);
    return node->key;
  }
};

int main() {
  auto splayTree = new SplayTree<int>();

  char op;
  while (std::cin >> op) {
    if (op == 'Q') break;
    int key;

    switch (op) {
      case 'I':
        std::cin >> key;
        splayTree->insert(key);
        break;
      case 'D':
        std::cin >> key;
        splayTree->erase(key);
        break;
      case '0':
        std::cout << splayTree->min() << std::endl;
        break;
      case '9':
        std::cout << splayTree->max() << std::endl;
        break;
    }
  }

  delete splayTree;
  return 0;
}
