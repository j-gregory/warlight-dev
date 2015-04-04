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

typedef tree<State> Tree;
typedef tree<State>::iterator TreeIterator;
typedef tree<State>::sibling_iterator TreeSiblingIterator;
typedef tree<State>::leaf_iterator TreeLeafIterator;

class UCTManager
{
 public:
  UCTManager();
  ~UCTManager();

  std::string execute(std::string name, std::vector<Region> regions, double timelimit);
  void printTree(Tree game_tree);

 protected:
  //tree<State> game_tree;
  std::string getRandomMove(State& state);
  void simulateTurn(State& state, std::string move, std::vector<Region> regions, State& result);
  void simulateBattle(int attack_armies, int defend_armies, int& survive_attack, int& survive_defend);
  std::string findBestMove(Tree game_tree);

};

#endif  // __UCTMANAGER_H_INCLUDED__
