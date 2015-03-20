#ifndef __NODE_H_INCLUDED__
#define __NODE_H_INCLUDED__

#include <stdlib.h>
#include <vector>

#include "State.h"

class Node
{
 public:
  Node();
  Node(State s);
  Node(State s, Node p);
  ~Node();

  State* getCurrentState()         const { return current_state; }
  Node* getParent()                const { return parent; }
  std::vector<Node*> getChildren() const { return children; }
  int getNumChildren()             const { return (int)children.size(); }
  float getWinPercentage()         const { return win_percentage; }

  void setCurrentState(State& s);
  void setParent(Node& p);
  void setChildren(std::vector<Node*> c);
  void setWinPercentage(float p);

 protected:
  State* current_state;
  Node *parent;
  std::vector<Node*> children;
  float win_percentage;
  

};

#endif  // __NODE_H_INCLUDED__
