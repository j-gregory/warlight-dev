#ifndef __MCTSMANAGER_H_INCLUDED__
#define __MCTSMANAGER_H_INCLUDED__ 

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

class MCTSManager
{
 public:
  MCTSManager();
  ~MCTSManager();

  std::string execute(std::string name, std::vector<Region> regions, double timelimit);
  void printTree(Tree game_tree);

  void printTreeBracketed(const Tree& t);
  void printSubtreeBracketed(const Tree& t, Tree::iterator iRoot);

 protected:
  std::string getRandomMove(State& state);
  void simulateOurTurn(State& state, std::vector<Region> regions, State& result);
  void simulateOpponentsTurn(State& state, std::vector<Region> regions, State& result);
  void simulateBattle(int attack_armies, int defend_armies, int& survive_attack, int& survive_defend);
  double calculateWinPercentage(std::vector<Region> regions, State& state);
  std::string findBestMove(Tree game_tree);

};

#endif  // __MCTSMANAGER_H_INCLUDED__
