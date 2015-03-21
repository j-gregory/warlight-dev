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

  // Selection - Traverse "random" branch to leaf node (should eventually use win_percentage)
  TreeSiblingIterator leaf = game_tree.begin();
  while(leaf.number_of_children() > 0)
  {
    int index = 0;
    int random = rand() % leaf.number_of_children();
    std::cout << "Current node has " << leaf.number_of_children() << " children" << std::endl;
    std::cout << "Randomly chose " << index << " to traverse " << std::endl;
    while(index != random)
    {
      leaf++;
      index++;
    }
    // ///////
    // This doesn't seem right....
    TreeIterator loc = std::find(game_tree.begin(), game_tree.end(), (*leaf));
    leaf = game_tree.begin(loc);
    // ///////
  }
  std::cout << "Got to leaf node: " << (*leaf).getName() << std::endl; 
  
  // Expansion - Expand path by one node for a random move
  //tree<State>::iterator itr2 = game_tree.insert(game_tree.end(), State("second"));

  // Simulation - Simulate the rest of the game to determine the win_percentage of the node we added

  // Back Propagation - Update all parent nodes using win_percentage information we gained

  // Find our best move - Find child node with largest win_percentage, return that command
  return (name + findBestMove(game_tree));
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
