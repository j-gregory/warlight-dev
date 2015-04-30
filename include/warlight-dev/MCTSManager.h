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

#define LUCK_MODIFIER 0.16

class State;

typedef tree<State> Tree;
typedef tree<State>::iterator TreeIterator;
typedef tree<State>::sibling_iterator TreeSiblingIterator;
typedef tree<State>::leaf_iterator TreeLeafIterator;
typedef tree<State>::fixed_depth_iterator TreeFixedDepthIterator;

class MCTSManager
{
 public:
  MCTSManager();
  ~MCTSManager();

  std::string execute(std::string name, std::vector<Region> all_regions, std::vector<int> owned_regions, double timelimit);
  void printTree(Tree game_tree);

  void printTreeBracketed(const Tree& t);
  void printSubtreeBracketed(const Tree& t, Tree::iterator iRoot);

 protected:
  double UCT(TreeFixedDepthIterator& node_itr, int cutoff, std::vector<State> envelope, State& s_prime);
  std::string getRandomMove(State& state);
  void simulateOurTurn(State& state, State& result);
  void simulateOpponentsTurn(State& state, State& result);
  void simulateBattle(int attacking_armies, int defending_armies, int& attacking_destroyed, int& defending_destroyed);
  double calculateWinPercentage(std::vector<Region> regions, State& state);
  std::string findBestMove(Tree game_tree);

};

#endif  // __MCTSMANAGER_H_INCLUDED__
