#include <iostream>
#include <string>
#include <vector>
using namespace std;

enum NodeType { FILE_NODE, FOLDER_NODE };

class Node {
private:
  string val;
  vector<Node *> children;
  NodeType type;

public:
  // Constructor
  Node(string v, NodeType t = FOLDER_NODE) : val(v), type(t) {}

  // setters
  void setVal(string v) { val = v; }

  // Getters
  string getVal() { return val; }

  NodeType getType() { return type; }

  vector<Node *> getChildren() { return children; }

  int getChildCount() { return children.size(); }

  Node *getChild(int index) {
    if (index >= 0 && index < children.size())
      return children[index];
    return NULL;
  }

  // Member Function
  bool addChild(Node *child) {
    if (type == FILE_NODE) {
      return false; // Files cannot have children
    }
    children.push_back(child);
    return true;
  } // Adds Child to the Vector

  bool removeChild(Node *child) {
    for (int i = 0; i < children.size(); i++) {
      if (children[i] == child) {
        children.erase(children.begin() + i);
        return true;
      }
    }
    return false;
  }

  bool canHaveChildren() { return type == FOLDER_NODE; }
};

// The main tree Class
class Tree {
private:
  Node *root;
  Node *current;

  // Helper Function For finding the Node based on value
  Node *findNode(Node *ptr, string val) {
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

  // Helper function to find parent of a node
  Node *findParent(Node *ptr, string val, Node *parent = NULL) {
    if (ptr == NULL)
      return NULL;
    if (ptr->getVal() == val)
      return parent;

    vector<Node *> children = ptr->getChildren();
    for (int i = 0; i < children.size(); i++) {
      Node *found = findParent(children[i], val, ptr);
      if (found != NULL)
        return found;
    }
    return NULL;
  }

  // Helper function to recursively delete all children of a node
  void deleteChildren(Node *ptr) {
    if (ptr == NULL)
      return;

    vector<Node *> children = ptr->getChildren();
    for (int i = 0; i < children.size(); i++) {
      // Recursively delete children first
      deleteChildren(children[i]);
      delete children[i];
    }
  }

public:
  // Constructor
  Tree() : root(NULL), current(NULL) {}

  // Main Functions

  void addNode(string command, string val) {
    NodeType nodeType;

    if (command == "touch") {
      nodeType = FILE_NODE;
    } else if (command == "mkdir") {
      nodeType = FOLDER_NODE;
    } else {
      cout << "Invalid command. Use 'touch' or 'mkdir' to add new files."
           << endl;
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

  void addNodeToParent(string command, string val, string parentVal) {
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

  void setCurrent(string command, string val) {
    if (root == NULL) {
      cout << "Tree is empty." << endl;
      return;
    }

    if (command != "cd") {
      cout << "Invalid command format. Use 'cd'." << endl;
      return;
    }

    Node *found = findNode(root, val);
    if (found == NULL) {
      cout << "No such directory exists." << endl;
      return;
    }

    // Check if the node is a folder
    if (found->getType() != FOLDER_NODE) {
      cout << "No such directory exists." << endl;
      return;
    }

    // Set current to the found folder node
    current = found;
    cout << "Current root set to node " << val << "." << endl;
  }

  void removeNode(string command, string val) {
    if (command != "rm") {
      cout << "Invalid command. Use 'rm' to remove nodes." << endl;
      return;
    }

    if (root == NULL) {
      cout << "Tree is empty." << endl;
      return;
    }

    Node *nodeToDelete = findNode(root, val);
    if (nodeToDelete == NULL) {
      cout << "Node with value " << val << " not found." << endl;
      return;
    }

    // If it's a folder, delete all its children recursively
    if (nodeToDelete->getType() == FOLDER_NODE) {
      deleteChildren(nodeToDelete);
    }

    // Handle root deletion
    if (nodeToDelete == root) {
      delete root;
      root = NULL;
      current = NULL;
      cout << "Root node " << val << " deleted." << endl;
      return;
    }

    // Find parent and remove node from parent's children
    Node *parent = findParent(root, val);
    if (parent != NULL) {
      parent->removeChild(nodeToDelete);
    }

    // If current points to the deleted node or any of its descendants, reset it
    if (current == nodeToDelete ||
        (current != NULL &&
         findNode(nodeToDelete, current->getVal()) != NULL)) {
      current = NULL;
    }

    delete nodeToDelete;
    cout << "Node " << val << " deleted successfully." << endl;
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

  string getCurrentValue() {
    Node *targetNode = (current != NULL) ? current : root;
    if (targetNode == NULL)
      return "";
    return targetNode->getVal();
  }
};

int main() {
  Tree t;
  int choice;
  string command;
  string val, parentVal;
  int addChoice;

  do {
    cout << "\n-----File System Simulation-----" << endl;
    string currentDir = t.getCurrentValue();
    cout << "Current Directory: " << (currentDir != "" ? currentDir : "Root")
         << endl;
    cout << "1. Add a File / Folder" << endl;
    cout << "2. Show members of current folder" << endl;
    cout << "3. Delete a file or a folder" << endl;
    cout << "4. Change directory" << endl;
    cout << "5. Exit" << endl;
    cout << "Enter your choice: ";
    cin >> choice;

    switch (choice) {
    case 1: {
      cout << "\n---Add a File / Folder---" << endl;
      cout << "1. Add to current directory" << endl;
      cout << "2. Add to specific parent" << endl;
      cout << "Enter choice: ";
      cin >> addChoice;

      cout << "Enter command (touch for file, mkdir for folder): ";
      cin >> command;
      cout << "Enter name: ";
      cin >> val;

      if (addChoice == 1) {
        t.addNode(command, val);
      } else if (addChoice == 2) {
        cout << "Enter parent name: ";
        cin >> parentVal;
        t.addNodeToParent(command, val, parentVal);
      } else {
        cout << "Invalid choice." << endl;
      }
      break;
    }
    case 2: {
      cout << "\n---Members of Current Folder---" << endl;
      t.displayLevelOrder();
      break;
    }
    case 3: {
      cout << "\n---Delete a File or Folder---" << endl;
      cout << "Enter name of node to delete: ";
      cin >> val;
      t.removeNode("rm", val);
      break;
    }
    case 4: {
      cout << "\n---Change Directory---" << endl;
      cout << "Enter name of directory to switch to: ";
      cin >> val;
      t.setCurrent("cd", val);
      break;
    }
    case 5: {
      cout << "Exiting program..." << endl;
      break;
    }
    default: {
      cout << "Invalid choice. Please try again." << endl;
      break;
    }
    }
  } while (choice != 5);

  return 0;
}

// TODO:  Making a Tree and Adding objects as children
// TODO:  make a new file -> add a children to the parent with no sub children
// TODO:  Enum for diffrenetiation btw file and folder
// TODO:  Deleting a Node -> if a folder is deleted then all of it's children
// are deleted
// TODO:  Parent specification when adding children in a directory
