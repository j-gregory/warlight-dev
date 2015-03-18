#ifndef __NODE_H_INCLUDED__
#define __NODE_H_INCLUDED__

#include <vector>

class Node
{
 public:
  Node();
  ~Node();

 protected:
  Node *parent;
  std::vector<Node*> children;

};

#endif  // __NODE_H_INCLUDED__
