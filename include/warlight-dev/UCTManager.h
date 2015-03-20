#ifndef __UCTMANAGER_H_INCLUDED__
#define __UCTMANAGER_H_INCLUDED__ 

#include <stdio.h>
#include <string>
#include <vector>

#include "Node.h"
#include "Region.h"
#include "State.h"

class UCTManager
{
 public:
  UCTManager();
  ~UCTManager();

  void createTree(std::string name, std::vector<Region> regions);
  bool execute(double timelimit, std::string result);
  //findBestMove();

 protected:
  Node* root;

};

#endif  // __UCTMANAGER_H_INCLUDED__
