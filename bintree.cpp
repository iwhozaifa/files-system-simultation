#include <iostream>
#include <queue>
#include <vector>
using namespace std;

class Node {
private:
  int val;
  vector<Node*> children;

public:
  // Constructor
  Node(int v) : val(v) {}
  // setters
  void setVal(int v) { val = v; }
  void addChild(Node *child) { children.push_back(child); }  //Adds Child to the Vector
  // Getters
  int getVal() { return val; }
  vector<Node*> getChildren() { return children; } 
  int getChildCount() { return children.size(); }
  Node* getChild(int index) { 
    if (index >= 0 && index < children.size())
      return children[index];
    return NULL;
  }
};

class Tree {
private:
  Node *root;
  Node *current;

  void displayPreorder(Node *ptr) {
    if (ptr != NULL) {
      cout << ptr->getVal() << " ";
      vector<Node*> children = ptr->getChildren();
      for (int i = 0; i < children.size(); i++) {
        displayPreorder(children[i]);
      }
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

    
    queue<Node*> q;
    q.push(root);

    while (!q.empty()) {
      Node *curr = q.front();
      q.pop();

      // Add as child of current node and return
      curr->addChild(nNode);
      return;
    }
  }

  void addNodeToParent(int val, int parentVal) {
    Node *nNode = new Node(val);

    if (root == NULL) {
      root = nNode;
      return;
    }

    Node *parent = findNode(root, parentVal);
    if (parent != NULL) {
      parent->addChild(nNode);
    } else {
      // If parent not found, add to root
      root->addChild(nNode);
    }
  }

private:
  Node* findNode(Node *ptr, int val) {
    if (ptr == NULL) return NULL;
    if (ptr->getVal() == val) return ptr;

    vector<Node*> children = ptr->getChildren();
    for (int i = 0; i < children.size(); i++) {
      Node *found = findNode(children[i], val);
      if (found != NULL) return found;
    }
    return NULL;
  }

public:

  void display() { displayPreorder(root); }
  void displayLevelOrder() {
    if (root == NULL) {
      cout << "Tree is empty." << endl;
      return;
    }

    queue<Node *> q;
    q.push(root);

    cout << "Tree Level Display:" << endl;

    while (!q.empty()) {
      int nodesAtCurrentLevel = q.size();

      while (nodesAtCurrentLevel > 0) {
        Node *curr = q.front();
        q.pop();

        cout << curr->getVal() << " ";

        vector<Node*> children = curr->getChildren();
        for (int i = 0; i < children.size(); i++) {
          q.push(children[i]);
        }

        nodesAtCurrentLevel--;
      }
      cout << endl;
    }
  }
};

int main() {
  Tree t;
  t.addNode(10);  // Root
  t.addNodeToParent(4, 10);   
  t.addNodeToParent(6, 10);   
  t.addNodeToParent(7, 10);     
  t.addNodeToParent(2, 10);    
  t.addNodeToParent(5, 10);   

  t.display();

  cout << endl << endl;

  t.displayLevelOrder();
}

// TODO : Making a Tree and Adding objects as children