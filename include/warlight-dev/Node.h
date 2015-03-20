#ifndef __NODE_H_INCLUDED__
#define __NODE_H_INCLUDED__

#include <stdlib.h>
#include <vector>

class Node
{
 public:
  Node();
  ~Node();

 protected:
  Node *parent;
  std::vector<Node*> children;
  float win_percentage;

};

#endif  // __NODE_H_INCLUDED__
