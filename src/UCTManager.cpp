#include "UCTManager.h"

UCTManager::UCTManager()
{

}

UCTManager::~UCTManager()
{

}

std::string UCTManager::execute(std::string name, std::vector<Region> regions, double timelimit)
{
  std::cout << "Executing UCT\n";
  std::cout << "Initializing tree\n";

  // Create root node with current state of game
  Tree game_tree;
  game_tree.insert(game_tree.begin(), State(name, regions));
  std::cout << "Tree initialization complete\n";
  //kptree::print_tree_bracketed(test, std::cout);
  printTree(game_tree);

  /*! ------------------------------------------------------------------------------
   *  SELECTION
   *  Traverse "random" branch to leaf node (should eventually use win_percentage)
   *  ------------------------------------------------------------------------------
   */
  TreeSiblingIterator node_itr = game_tree.begin();
  while(node_itr.number_of_children() > 0)
  {
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
  }
  std::cout << "Randomly selected leaf: " << (*node_itr).getName() << std::endl;


  /*! ------------------------------------------------------------------------------
   *  EXPANSION
   *  Expand current path by one node representing a random move
   *  ------------------------------------------------------------------------------
   */
  State result;
  std::string move = getRandomMove((*node_itr), result);
  std::cout << "Executed random move: " << move;
  // Add new node to tree, iterate there
  node_itr = game_tree.append_child(node_itr, result);
  std::cout << "Now at new leaf: " << (*node_itr).getName() << std::endl;


  /*! ------------------------------------------------------------------------------
   *  SIMULATION
   *  Simulation the rest of the game to determine the win_percentage of added node
   *  ------------------------------------------------------------------------------
   */
  int steps = 0;
  while(steps < 5)
  {
    //simulateTurn();
    steps++;
  }


  /*! ------------------------------------------------------------------------------
   *  BACK PROPAGATION
   *  Update all parent nodes using win_percentage information we gained
   *  ------------------------------------------------------------------------------
   */
  //while(curr != game_tree.begin())
  //{
    //updateWinPercentage();
    //curr = curr.parent;     // Not curr--;  ??
  //}

  // Find our best move - Find child node with largest win_percentage, return that command
  return (name + findBestMove(game_tree));
}

std::string UCTManager::getRandomMove(State& state, State& result)
{
  result.setName("Test");
  return "No moves\n";
}


std::string UCTManager::findBestMove(Tree game_tree)
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

void UCTManager::printTree(Tree game_tree)
{
  TreeIterator node = game_tree.begin();
  while(node != game_tree.end())
  {
    std::cout << "Owner: "    << (*node).getName() 
	      << "   Win %: " << (*node).getWinPercentage() << std::endl;
    ++node;
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
