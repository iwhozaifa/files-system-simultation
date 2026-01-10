#include <iostream>
#include <queue>
#include <string>
#include <vector>
using namespace std;

enum NodeType { FILE_NODE, FOLDER_NODE };

class Node {
private:
  int val;
  vector<Node *> children;
  NodeType type;

public:
  // Constructor
  Node(int v, NodeType t = FOLDER_NODE) : val(v), type(t) {}
  
  // setters
  void setVal(int v) { val = v; }
  
  bool addChild(Node *child) {
    if (type == FILE_NODE) {
      return false; // Files cannot have children
    }
    children.push_back(child);
    return true;
  } // Adds Child to the Vector
  // Getters
  int getVal() { return val; }
  
  NodeType getType() { return type; }
  
  vector<Node *> getChildren() { return children; }
  
  int getChildCount() { return children.size(); }
  
  Node *getChild(int index) {
    if (index >= 0 && index < children.size())
      return children[index];
    return NULL;
  }
  bool canHaveChildren() { return type == FOLDER_NODE; }
};
// The main tree Class
class Tree {
private:
  Node *root;
  Node *current;

  Node *findNode(Node *ptr, int val) {
    if (ptr == NULL)
      return NULL;
    if (ptr->getVal() == val)
      return ptr;

    vector<Node *> children = ptr->getChildren();
    for (int i = 0; i < children.size(); i++) {
      Node *found = findNode(children[i], val);
      if (found != NULL)
        return found;
    }
    return NULL;
  }

public:
  // Constructor
  Tree() : root(NULL), current(NULL) {}
  // Helper Functions

  void addNode(string command, int val) {
    NodeType nodeType;
    
    if (command == "touch") {
      nodeType = FILE_NODE;
    } else if (command == "mkdir") {
      nodeType = FOLDER_NODE;
    } else {
      cout << "Invalid command. Use 'touch' or 'mkdir'." << endl;
      return;
    }

    Node *nNode = new Node(val, nodeType);

    if (root == NULL) {
      root = nNode;
      return;
    }

    Node *targetNode = (current != NULL) ? current : root;
    
    if (!targetNode->canHaveChildren()) {
      cout << "Cannot add child to a file node." << endl;
      delete nNode;
      return;
    }

    if (!targetNode->addChild(nNode)) {
      cout << "Failed to add child node." << endl;
      delete nNode;
    }
  }

  void addNodeToParent(string command, int val, int parentVal) {
    NodeType nodeType;
    
    if (command == "touch") {
      nodeType = FILE_NODE;
    } else if (command == "mkdir") {
      nodeType = FOLDER_NODE;
    } else {
      cout << "Invalid command. Use 'touch' or 'mkdir'." << endl;
      return;
    }

    Node *nNode = new Node(val, nodeType);

    if (root == NULL) {
      root = nNode;
      return;
    }

    Node *parent = findNode(root, parentVal);
    if (parent != NULL) {
      if (!parent->canHaveChildren()) {
        cout << "Cannot add child to a file node." << endl;
        delete nNode;
        return;
      }
      if (!parent->addChild(nNode)) {
        cout << "Failed to add child node." << endl;
        delete nNode;
      }
    } else {
      // If parent not found, add to root
      if (!root->canHaveChildren()) {
        cout << "Cannot add child to root (it's a file node)." << endl;
        delete nNode;
        return;
      }
      if (!root->addChild(nNode)) {
        cout << "Failed to add child node." << endl;
        delete nNode;
      }
    }
  }

  void displayLevelOrder() {
    Node *targetNode = (current != NULL) ? current : root;

    if (targetNode == NULL) {
      cout << "Tree is empty." << endl;
      return;
    }

    cout << "Children of node " << targetNode->getVal() << ":" << endl;

    vector<Node *> children = targetNode->getChildren();
    if (children.size() == 0) {
      cout << "No children." << endl;
      return;
    }

    for (int i = 0; i < children.size(); i++) {
      cout << children[i]->getVal() << " ";
    }
    cout << endl;
  }
};

int main() {
  Tree t;
  t.addNode("mkdir", 10); // Root
  t.addNodeToParent("mkdir", 4, 10);
  t.addNodeToParent("mkdir", 6, 10);
  t.addNodeToParent("touch", 7, 10);
  t.addNodeToParent("touch", 2, 10);
  t.addNodeToParent("mkdir", 5, 10);

  t.addNodeToParent("touch", 69, 4);

  cout << endl << endl;

  t.displayLevelOrder();

}

// TODO:  Making a Tree and Adding objects as children
// TODO:  make a new file -> add a children to the parent with no sub children
// TODO:  Enum for diffrenetiation btw file and folder
// TODO:  Deleting a Node -> if a folder is deleted then all of it's children
// are deleted
// TODO:  Parent specification when adding children in a directory
