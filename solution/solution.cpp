#include <iostream>
using namespace std;

// Lista ligada
class List {
private:
  class Node {
  public:
    int data;
    Node *next;

  public:
    Node(int d): data(d), next(nullptr) {}
  } *head;

  // Construtor
public:
  List(void): head(nullptr) {}

  // Métodos
public:
  void insert(int data);          // Insere na frente da lista O(1)
  void print(void);
};

// Imprimir lista (debug)
void List::print(void) {
  List::Node *n = List::head;

  while(n != nullptr) {
    cout << " " << n->data;
    n = n->next;
  }
}

// Inserção na lista. Inserção ocorre na cabeça da lista, portanto Theta(1)
void List::insert(int data) {
  List::Node* n = new List::Node(data);
  n->next = head;
  List::head = n;
}
