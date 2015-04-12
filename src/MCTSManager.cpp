#include "MCTSManager.h"

#include "string_util.hpp"

MCTSManager::MCTSManager()
{

}

MCTSManager::~MCTSManager()
{

}

std::string MCTSManager::execute(std::string name, std::vector<Region> regions, double timelimit)
{
  std::cout << "Executing MCTS\n";
  std::cout << "Initializing tree\n";

  // Create root node with current state of game
  Tree game_tree;
  game_tree.insert(game_tree.begin(), State(name, regions));
  std::cout << "Tree initialization complete\n";

  std::cout << "Printing tree\n";
  //kptree::print_tree_bracketed(test, std::cout);
  printTree(game_tree);
  //printTreeBracketed(game_tree);

  /* =============================================================================== */
  // This will be based on time, but for debuggin purposes just an arbitrary number of iterations
  int iterations = 0;
  //while(time is almost up)
  //while(iterations < 10)

  /*! ------------------------------------------------------------------------------
   *  SELECTION
   *  UCT (and win percentage) WILL BE USED IN THIS STEP TO BALANCE EXPLORATION AND EXPLOITATION
   *  For now, traverse "random" branch until random location or child node
   *  ------------------------------------------------------------------------------
   */
  std::cout << "Starting SELECTION process\n";
  TreeSiblingIterator node_itr = game_tree.begin();
  //int num_nodes = game_tree.size();
  int max_depth = game_tree.max_depth();              // Opportunity for optimization!
  if(max_depth == 0) max_depth = 1;                   // Avoid floating point exception!
  int rand_depth = rand() % max_depth;
  int curr_depth = 0;                                 // Effectively creates new random branches  
  std::cout << "Random depth of search is " << rand_depth << " \n";
  // If we have reached random depth OR a child node, stop and simulate from this location
  while((curr_depth < rand_depth) && (node_itr.number_of_children() > 0))
  {
    // Traverse random children
    int index = 0;
    int random = rand() % node_itr.number_of_children();
    std::cout << "Current node has " << node_itr.number_of_children() << " children" << std::endl;
    std::cout << "Randomly chose " << random << " to traverse " << std::endl;
    while(index != random)
    {
      node_itr++;
      index++;
    }
    // Need this ... no?
    //TreeIterator loc = std::find(game_tree.begin(), game_tree.end(), (*leaf));
    //leaf = game_tree.begin(loc);
    curr_depth++;
  }
  std::cout << "Randomly selected leaf: " << (*node_itr).getName() << std::endl;


  /*! ------------------------------------------------------------------------------
   *  EXPANSION
   *  Expand current path by one node representing a random move
   *  ------------------------------------------------------------------------------
   */
  std::cout << "Starting EXPANSION process\n";
  State result(name, regions, 0.5);
  simulateOurTurn((*node_itr), regions, result);

  // Add new node to tree, iterate there
  node_itr = game_tree.append_child(node_itr, result);
  std::cout << "Now at new leaf: " << (*node_itr).getName() << std::endl;


  /*! ------------------------------------------------------------------------------
   *  SIMULATION
   *  Simulate the rest of the game to determine the win_percentage of added node
   *  ------------------------------------------------------------------------------
   */
  std::cout << "Starting SIMULATION process\n";
  State curr = result;
  State next;
  int steps = 0;
  // while(some amount of time)
  while(steps < 5)
  {
    // Their turn? Guess one of their moves
    if(steps%2 == 0) simulateOpponentsTurn(curr, regions, next);
    // Our turn? Simulate a random move
    else simulateOurTurn(curr, regions, next);

    curr = next;
    steps++;
  }


  /*! ------------------------------------------------------------------------------
   *  BACK PROPAGATION
   *  Update all parent nodes using win_percentage information we gained
   *  ------------------------------------------------------------------------------
   */
  std::cout << "Starting BACK PROPAGATION process\n";
  double updated_win_percentage = next.getWinPercentage();
  while(node_itr != game_tree.begin())
  {
    (*node_itr).setWinPercentage(updated_win_percentage);
    node_itr = game_tree.parent(node_itr);
  }

  // DONE SINGLE ITERATION OF MCTS

  //}
  /* =============================================================================== */

  // Find our best move - Find child node with largest win_percentage, return that command
  return (name + findBestMove(game_tree));
}

std::string MCTSManager::getRandomMove(State& state)
{
  std::string rand_move = "No moves\n";
  
  // Look at state.owned_regions, pick a random valid move

  /*  ------------------------------------------------ */
  /* Implementation for choosing region with largest number of armies */
  int from_id = 0;
  int from_index = 0;
  int max_armies = 0;
  int temp_region = 0;
  int temp_armies = 0;
  std::vector<Region> owned_regions = state.getOwnedRegions();
  for(int i = 0; i < (int)owned_regions.size(); i++)
  {
    temp_region = owned_regions[i].getID();
    temp_armies = owned_regions[i].getNumArmies();
    if (temp_armies > max_armies)
    {
      from_id = temp_region;
      from_index = i;
      max_armies = temp_armies;
    }
  }
  /*  ------------------------------------------------ */
  
  // Find (random) adjacent region
  int owned_region = owned_regions[from_index].getID();
  int rand_index = rand() % owned_regions[owned_region].getNumNeighbors();
  int to = owned_regions[owned_region].getNeighbors()[rand_index];
  
  return (owned_regions[from_index].getOwner() + 
	  " attack/transfer " + 
	  std::to_string(from_id) + " " + 
	  std::to_string(to) + " " + 
	  std::to_string(max_armies-1) + "\n");
}

void MCTSManager::simulateOurTurn(State& state, std::vector<Region> regions, State& result)
{
  std::cout << "-- Simulating our turn --" << std::endl;;
  // Get random, valid move
  std::string move = getRandomMove(state);
  
  // Produce resulting state for random valid move by simulating turn
  std::cout << "Executing random move: " << move;

  std::vector<std::string> tokens = split(move, ' ');
  if(tokens[1] == "attack/transfer")
  {
    int from = std::stoi(tokens[2]);
    int to = std::stoi(tokens[3]);
    int attack_armies = std::stoi(tokens[4]);
    int defend_armies = regions[to].getNumArmies();
    
    std::cout << "Simulating attack from region " << from 
	      << " with " << attack_armies << " armies" 
	      << " to region " << to 
	      << " that has " << defend_armies << " armies " << std::endl;
    
    int survive_attack = 0;
    int survive_defend = 0;
    simulateBattle(attack_armies, defend_armies, survive_attack, survive_defend);
    
    std::cout << "After battle: " 
	      << survive_attack << " armies survived attack   "
	      << survive_defend << " armies survived defend " << std::endl;

    // The outcome of an attack is ONE of the following:
    //   1) Win:  Move the surviving armies onto new region
    //   2) Lose: All attacking armies died, all non-attacking remain safe
    // By definition: attack_armies = lost + survived
    // Regardless of how many survive or are lost, the remaining number of armies is (original - attack)
    // In other words, once allocated for an attack, armies will never return to the region
    int remaining_armies = state.getOwnedRegions()[from].getNumArmies() - attack_armies;
    state.setArmies(from, remaining_armies);

    // If we won the battle, we need to update the owner and number of armies with how many survived
    if(survive_defend == 0)
    {
      std::cout << "Attack: SUCCESS\n";
      // Set number of armies in newly-acquired region
      // Add newly-acquired region to regions owned
      Region winnings = regions[to];
      winnings.setArmies(survive_attack);
      result.addNewOwnedRegion(winnings);
    }

    // If we lost the battle, we may update the other region's number of surviving armies (defense)
    else 
    {
      std::cout << "Attack: FAILED\n";
      //state.setArmies(to, survive_defend);   // Current implementation doesn't support this
    }
  }

  result.setMove(move);
  double wp = calculateWinPercentage(regions, result);
  result.setWinPercentage(wp);
}

void MCTSManager::simulateOpponentsTurn(State& state, std::vector<Region> regions, State& result)
{
  std::cout << "-- Simulating their turn --" << std::endl;;
  // @TODO: Implement
  result = state;
}

void MCTSManager::simulateBattle(int attack_armies, int defend_armies, int& survive_attack, int&survive_defend)
{
  // @TODO: Improve accuracy of battle simulation
  //        Current implementation does not handle battles where one side will clearly win (6->2)
  //        Should instead incrementally calculate and check to see if defense has lost
  double d1 = 0.6*attack_armies;
  double d2 = 0.6*attack_armies;
  double defend_destroyed = ceil((0.84*d1)+(0.16*d2));

  double a1 = 0.7*defend_armies;
  double a2 = 0.7*defend_armies;
  double attack_destroyed = ceil((0.84*a1)+(0.16*a2));
  
  survive_attack = attack_armies - attack_destroyed;
  survive_defend = defend_armies - defend_destroyed;
  if(survive_defend < 0) survive_defend = 0;
  
  std::cout << "attack_armies: " << attack_armies 
	    << " attack_destroyed: " << attack_destroyed 
	    << " survive_attack: " << survive_attack << std::endl;
  std::cout << "defend_armies: " << defend_armies
	    << " defend_destroyed: " << defend_destroyed
	    << " survive_defend: " << survive_defend << std::endl;
}

double MCTSManager::calculateWinPercentage(std::vector<Region> regions, State& state)
{
  // @TODO: Implement

  // Win percentage calculation - Initial thoughts:
  // 1) Based on battle?
  // 2) Based on number of owned regions vs total regions
  // 3) Based on number of our regions vs opponents regions

  return (state.getOwnedRegions().size() / regions.size());
}
std::string MCTSManager::findBestMove(Tree game_tree)
{
  // No moves by default to save time
  std::string best_move = "No moves\n";
  // Iterate immediate children, find highest win_percentage
  double temp_win_percentage = 0;
  TreeSiblingIterator best;
  TreeSiblingIterator child = game_tree.begin();
  while(child != game_tree.end())
  {
    if((*child).getWinPercentage() > temp_win_percentage)
    {
      best_move = " attack/transfer random1 random2 5";
      temp_win_percentage = (*child).getWinPercentage();
    }
    ++child;
  }
  return best_move;
}

void MCTSManager::printTree(Tree game_tree)
{
  TreeIterator node = game_tree.begin();
  while(node != game_tree.end())
  {
    std::cout << "Owner: "    << (*node).getName() 
	      << "   Win %: " << (*node).getWinPercentage() << std::endl;
    node++;
  }  
}

void MCTSManager::printTreeBracketed(const Tree& t)
{
  int headCount = t.number_of_siblings(t.begin());
  int headNum = 0;
  std::cout << "headCount: " << headCount << " headNum: " << headNum << std::endl;
  for(Tree::sibling_iterator iRoots = t.begin(); iRoots != t.end(); ++iRoots, ++headNum) {
    printSubtreeBracketed(t, iRoots);
    if (headNum != headCount) {
      std::cout << "Printing newline\n";
      std::cout << std::endl;
    }
    std::cout << "Next iteration\n";
  }
  std::cout << "Done printTreeBracketed\n";
}

void MCTSManager::printSubtreeBracketed(const Tree& t, Tree::iterator iRoot)
{
  std::cout << "Printing subtree" << std::endl;
  if(t.empty()) return;
  std::cout << "Tree not empty\n";
  if (t.number_of_children(iRoot) == 0) {
    std::cout << "Subtree has no children\n";
    std::cout << (*iRoot).getName();
  }
  else {
    std::cout << "Iterating, printing subtree\n";
    // parent
    std::cout << (*iRoot).getName();
    std::cout << "(";
    // child1, ..., childn
    int siblingCount = t.number_of_siblings(t.begin(iRoot));
    int siblingNum;
    Tree::sibling_iterator iChildren;
    for (iChildren = t.begin(iRoot), siblingNum = 0; iChildren != t.end(iRoot); ++iChildren, ++siblingNum) {
      // recursively print child 
      printSubtreeBracketed(t, iChildren);
      // comma after every child except the last one
      if (siblingNum != siblingCount ) {
	std::cout << ", ";
      }
    }
    std::cout << ")";
  }
}






  // /////////////////////
  // Second implementation of Selection process
  // Probably less efficient in the long term
  
  /*
  // Iterate all leafs and count total
  int number_of_leafs = 0;
  TreeLeafIterator leaf_itr = game_tree.begin();
  while(leaf_itr != game_tree.end())
  {
    number_of_leafs++;
    leaf_itr++;
  }

  // Pick random leaf
  int index = 0;
  int random = rand() % number_of_leafs;
  std::cout << "Randomly selected leaf at index: " << random << std::endl;
  leaf_itr = game_tree.begin();
  while(index != random)
  {
    leaf_itr++;
    index++;
  }
  std::cout << "There are " << number_of_leafs << " leafs\n";
  std::cout << "Got to leaf node: " << (*leaf_itr).getName() << std::endl; 
  */
