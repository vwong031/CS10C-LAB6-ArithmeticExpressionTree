// Collaborated with: Valerie Wong, Kira Hammond,
// Dr. Reem Ali's Week 4 slides
// https://stackoverflow.com/a/62728283
// https://www.techiedelight.com/append-char-end-string-cpp/
// https://www.geeksforgeeks.org/deletion-binary-tree/
// https://www.techiedelight.com/delete-given-binary-tree-iterative-recursive/

#include "arithmeticExpression.h"

/* Initializes an empty tree and sets the infixExpression
to the value of parameter passed in. */
arithmeticExpression::arithmeticExpression(const string &expression) {
  infixExpression = expression;
  root = 0;
}
/*
Algorithm 
1. Starting at root, find the deepest and rightmost node in binary tree and node which we want to delete. 
2. Replace the deepest rightmost node’s data with node to be deleted. 
3. Then delete the deepest rightmost node.
*/
void arithmeticExpression::deleteSubtree(TreeNode *&node) {
  if (!node) {
    return;
  }

  // Delete children
  deleteSubtree(node->left);
  deleteSubtree(node->right);
  
  // Delete current node
  delete node;
  node = 0;
}

/* Implementation of destructor is optional.
   The destructor should deallocate all the nodes in the tree. */
arithmeticExpression::~arithmeticExpression() {
  deleteSubtree(root);
}

/* Converts the infixExpression to its equivalent postfix string
   and then generates the tree and assigns the root node to the
   root data field. The key for the first node is 'a', 'b' for the second node and so on. */
/* ALGORITHM (from Dr. Reem Ali's Chapter 4 slides)
  1) make a stack of node pointers
  2) operands - push a new tree onto the stack
  3) operators - pop 2 trees from the stack
    - use operator as root of new tree w/ popped trees
    - push new tree onto the stack
  */
void arithmeticExpression::buildTree() {
  // Note: Algorithm assumes each operand is a single character

  string postfixExpression;
  stack<TreeNode*> expressionTree; // LIFO Principle
  char c; 
  TreeNode *treeToPush, *nextToLastTree, *lastTree;

  // convert infix to postfix
  postfixExpression = infix_to_postfix();

  for (long unsigned i = 0; i < postfixExpression.size(); ++i) {
    c = postfixExpression.at(i);
    if (c == '+' || c == '-' || c == '*' || c == '/') {
      lastTree = expressionTree.top();
      expressionTree.pop();

      nextToLastTree = expressionTree.top();
      expressionTree.pop();

      treeToPush = new TreeNode(c, 'a' + i);
      treeToPush->left = nextToLastTree;
      treeToPush->right = lastTree;

      expressionTree.push(treeToPush);
    }
    else {
      treeToPush = new TreeNode(c, 'a' + i);
      expressionTree.push(treeToPush);
    }
  
  }

  root = expressionTree.top();
}

/* Calls the recursive infix function. */
void arithmeticExpression::infix() {
  infix(root);
}

/* Calls the recursive prefix function. */
void arithmeticExpression::prefix() {
  prefix(root);
}

/* Calls the recursive postfix function. */
void arithmeticExpression::postfix() {
  postfix(root);
}


// ========== ========== ==========
//         Helper Functions
// ========== ========== ==========

/* Helper function that outputs the infix notation of the arithmetic expression tree
   by performing the inorder traversal of the tree.
   An opening and closing parenthesis must be added at the
   beginning and ending of each expression. */
void arithmeticExpression::infix(TreeNode *node) {
  // taken from wk 4 Trees/BST slides
  if (node) {
    // if left & right node exists, output '('
    if (node->left && node->right) {
      cout << '(';
    }

    // traverse left subtree
    infix(node->left);
    // print
    cout << node->data;
    // traverse right subtree
    infix(node->right);

    // if left & right node exists, output ')'
    if (node->left && node->right) {
      cout << ')';
    }
  }
}

/* Helper function that outputs the prefix notation of the arithmetic expression tree
   by performing the preorder traversal of the tree. */
void arithmeticExpression::prefix(TreeNode *node) {
  if (node) {
    // print preorder
    cout << node->data;
    // traverse/print left subtree preorder
    prefix(node->left);
    // traverse/print right subtree preorder
    prefix(node->right);
  }
}

/* Helper function that outputs the postfix notation of the arithmetic expression tree
   by performing the postorder traversal of the tree. */
void arithmeticExpression::postfix(TreeNode *node) {
  if (node) {
    // traverse/print left subtree postorder
    postfix(node->left);
    // traverse/print right subtree postorder
    postfix(node->right);
    // print
    cout << node->data;
  }
}

/* Helper function that returns an integer according to
   the priority of the given operator. */
int arithmeticExpression::priority(char op) {
  int priority = 0;
  if (op == '(')
  {
    priority = 3;
  }
  else if (op == '*' || op == '/')
  {
    priority = 2;
  }
  else if (op == '+' || op == '-')
  {
    priority = 1;
  }
  return priority;
}

/* Helper function that returns the postfix notation equivalent
   to the given infix expression */
string arithmeticExpression::infix_to_postfix() {
  stack<char> s;
  ostringstream oss;
  char c;
  for (unsigned i = 0; i < infixExpression.size(); ++i)
  {
    c = infixExpression.at(i);
    if (c == ' ')
    {
      continue;
    }
    if (c == '+' || c == '-' || c == '*' || c == '/' || c == '(' || c == ')')
    { //c is an operator
      if (c == '(')
      {
        s.push(c);
      }
      else if (c == ')')
      {
        while (s.top() != '(')
        {
          oss << s.top();
          s.pop();
        }
        s.pop();
      }
      else
      {
        while (!s.empty() && priority(c) <= priority(s.top()))
        {
          if (s.top() == '(')
          {
            break;
          }
          oss << s.top();
          s.pop();
        }
        s.push(c);
      }
    }
    else
    { //c is an operand
      oss << c;
    }
  }
  while (!s.empty())
  {
    oss << s.top();
    s.pop();
  }
  return oss.str();
}


// ========== ========== ==========
//        Tree Visualization
// ========== ========== ==========

/* Helper function for generating the dotty file. This is a recursive function. */
void arithmeticExpression::visualizeTree(ofstream &outFS, TreeNode *node) {
  string edgeTo = "  ";
  edgeTo.push_back(node->key);
  edgeTo += " -> ";

  outFS << "  " << node->key << " [label=\"" << node->data << "\"];" << endl;
  if (node->left && node->right) {
    visualizeTree(outFS, node->left);
    visualizeTree(outFS, node->right);
    outFS << edgeTo << node->left->key << ";" << endl;
    outFS << edgeTo << node->right->key << ";" << endl;
  }
}

/* Calls the recursive visualizeTree function and generates the .png file using system call. */
void arithmeticExpression::visualizeTree(const string &outputFilename) {
  ofstream outFS(outputFilename.c_str());
  if (!outFS.is_open())
  {
    cout << "Error opening " << outputFilename << endl;
    return;
  }
  outFS << "digraph G {" << endl;
  visualizeTree(outFS, root);
  outFS << "}";
  outFS.close();
  string jpgFilename = outputFilename.substr(0, outputFilename.size() - 4) + ".jpg";
  string command = "dot -Tjpg " + outputFilename + " -o " + jpgFilename;
  system(command.c_str());
}
