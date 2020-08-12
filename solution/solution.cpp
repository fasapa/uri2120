#include <iostream>
using namespace std;

template <class T>
using CmpFun2 = bool (*)(T, T);

template <class T>
using CmpFun1 = bool (*)(T);

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
  bool search(T data, CmpFun2<T> cmp, T *res); // Busca data na lista utilizando cmp (T -> T -> bool) a:T = b:T
  bool search(T data, T *res); // Busca utilizando T1.cmp(T2) => T1 = T2
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
bool List<T>::search(T data, CmpFun2<T> cmp, T *res) {
  List::Node *n = List::head;

  bool found = false;
  while (n != nullptr && !found) {
    if(cmp(n->data, data)) {
      found = true;
      *res = n->data;
    } else {
      n = n->next;
    }
  }
  return found;
}

template <class T>
bool List<T>::search(T data, T *res) {
  List::Node *n = List::head;

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

