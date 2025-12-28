#include <iostream>
#include <queue>
using namespace std;

class Node {
private:
  int val;
  Node *left;
  Node *right;

public:
  // Constructor
  Node(int v) : val(v), left(NULL), right(NULL) {}
  // setters
  void setVal(int v) { val = v; }
  void setLeft(Node *l) { left = l; }
  void setRight(Node *r) { right = r; }
  // Getters
  int getVal() { return val; }
  Node *getLeft() { return left; }
  Node *getRight() { return right; }
};

class Tree {
private:
  Node *root;
  Node *current;

  void displayInorder(Node *ptr) {
    if (ptr != NULL) {
      displayInorder(ptr->getLeft());
      cout << ptr->getVal() << " ";
      displayInorder(ptr->getRight());
    }
  }

public:
  // Constructor
  Tree() : root(NULL), current(NULL) {}
  // Helper Functions

  void addNode(int val) {
    Node *nNode = new Node(val);

    if (root == NULL) {
      root = nNode;
      return;
    }
    Node *temp = root;
    Node *parent = NULL;

    while (temp != NULL) {
      parent = temp;
      if (val < temp->getVal())
        temp = temp->getLeft();
      else
        temp = temp->getRight();
    }
    if (val < parent->getVal()) {
      parent->setLeft(nNode);
    } else {
      parent->setRight(nNode);
    }
  }

  void display() { displayInorder(root); }
  void displayLevelOrder() {
    if (root == NULL) {
      cout << "Tree is empty." << endl;
      return;
    }

    queue<Node *> q;
    q.push(root);

    cout << "Tree Level Display:" << endl;

    while (!q.empty()) {
      int nodesAtCurrentLevel = q.size(); // Number of nodes at this level

      // Process all nodes at the current level
      while (nodesAtCurrentLevel > 0) {
        Node *curr = q.front();
        q.pop();

        cout << curr->getVal() << " ";

        // Push children to the queue for the next level
        if (curr->getLeft() != NULL)
          q.push(curr->getLeft());
        if (curr->getRight() != NULL)
          q.push(curr->getRight());

        nodesAtCurrentLevel--;
      }
      // Finished one level, move to next line
      cout << endl;
    }
  }
};

int main() {
  Tree t;
  t.addNode(10);
  t.addNode(4);
  t.addNode(6);
  t.addNode(7);

  t.display();

  cout << endl << endl;

  t.displayLevelOrder();
}

// TODO : Making a Tree and Adding objects as children
