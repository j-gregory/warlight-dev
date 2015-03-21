#ifndef __UCTMANAGER_H_INCLUDED__
#define __UCTMANAGER_H_INCLUDED__ 

//#include <algorithm>
#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>

#include "tree.hh"
#include "tree_util.hh"

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
  //tree<State> game_tree;
};

#endif  // __UCTMANAGER_H_INCLUDED__
