#include <iostream>
using namespace std;

template <class T>
using CmpFun = bool (*)(T, T);

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
  void insert(T data);          // Insere na frente da lista O(1)
  T search(T data, CmpFun<T> cmp); // Busca data na lista utilizando cmp (T -> T -> bool) a:T = b:T
  void print(void);
};

// Imprimir lista (debug)
template <class T>
void List<T>::print(void) {
  List::Node *n = List::head;

  while(n != nullptr) {
    cout << " " << n->data;
    n = n->next;
  }
}

// Inserção na lista. Inserção ocorre na cabeça da lista, portanto Theta(1)
template <class T>
void List<T>::insert(T data) {
  List::Node* n = new List::Node(data);
  n->next = head;
  List::head = n;
}

template <class T>
T List<T>::search(T data, CmpFun<T> cmp) {
  List::Node *n = List::head;

  while (n != nullptr) {
    if(cmp(n->data, data)) return n->data;
  }
}
