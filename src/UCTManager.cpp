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
  tree<State> game_tree;
  game_tree.insert(game_tree.begin(), State(name, regions));
  //tree<State>::iterator itr2 = game_tree.insert(game_tree.end(), State("second"));
  //tree<State>::iterator itr3 = game_tree.insert(game_tree.end(), State("third"));
  std::cout << "Tree initialization complete\n";
  //kptree::print_tree_bracketed(test, std::cout);
  printTree(game_tree);

  // Selection - Choose "random" branch to traverse (should eventually use win_percentage)
  
  // Expansion - Expand path by one node using simulation

  // Simulation - Simulate the rest of the game to determine the win_percentage of the node we added

  // Back Propagation - Update all parent nodes using win_percentage information we gained

  // Find our best move - Find child node with largest win_percentage, return that command
  return (name + findBestMove(game_tree));
}

std::string UCTManager::findBestMove(tree<State> game_tree)
{
  // No moves by default to save time
  std::string best_move = "No moves\n";
  // Iterate immediate children, find highest win_percentage
  double temp_win_percentage = 0;
  tree<State>::sibling_iterator child = game_tree.begin();
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

void UCTManager::printTree(tree<State> game_tree)
{
  tree<State>::iterator node = game_tree.begin();
  while(node != game_tree.end())
  {
    std::cout << "Owner: "    << (*node).getName() 
	      << "   Win %: " << (*node).getWinPercentage() << std::endl;
    ++node;
  }  
}
