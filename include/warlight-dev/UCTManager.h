#ifndef __UCTMANAGER_H_INCLUDED__
#define __UCTMANAGER_H_INCLUDED__ 

//#include <stdio.h>
#include <string>
#include <vector>

#include "Node.h"

class UCTManager
{
 public:
  UCTManager();
  ~UCTManager();

  //createTree();
  //executeUCT();
  //selection();
  //expansion();
  //simulation();
  //backPropagate();
  //findBestMove();

 protected:
  Node* root;

};

#endif  // __UCTMANAGER_H_INCLUDED__
