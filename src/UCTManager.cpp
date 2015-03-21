#include "UCTManager.h"

UCTManager::UCTManager()
{

}

UCTManager::~UCTManager()
{

}

bool UCTManager::execute(std::string name, std::vector<Region> regions, double timelimit, std::string result)
{
  std::cout << "Executing UCT\n";

  std::cout << "Initializing tree\n";
  // Create root node with current state of game

  /*
  tree<State> test;
  test.insert(test.begin(), State("test"));
  tree<State>::iterator itr2 = test.insert(test.end(), State("second"));
  tree<State>::iterator itr3 = test.insert(test.end(), State("third"));
  tree<State>::iterator printout = test.begin();
  while(printout != test.end())
  {
    std::cout << (*printout).getName() << std::endl;
    ++printout;
  }
  //kptree::print_tree_bracketed(test, std::cout);
  */

  tree<State> game_tree;
  game_tree.insert(game_tree.begin(), State("root"));
  tree<State>::iterator itr2 = game_tree.insert(game_tree.end(), State("second"));
  tree<State>::iterator itr3 = game_tree.insert(game_tree.end(), State("third"));
  tree<State>::iterator printout = game_tree.begin();
  std::cout << "Tree built, now printout\n";
  while(printout != game_tree.end())
  {
    std::cout << (*printout).getName() << "  " << (*printout).getWinPercentage() << std::endl;
    
    ++printout;
  }

  
  std::cout << "Initialization complete\n";

  /*
  tree<Node>::iterator loc = find(root.begin(), root.end(), to_add);
  if(loc != root.end()) 
  {
    tree<Node>::sibling_iterator sib = root.begin(loc);
    while(sib != root.end(loc)) 
    {
      cout << (*sib).getName() << endl;
      ++sib;
    }
  }
  */


  result = name + "attack/transfer random1 random2 5";


  // Selection
  
  // Expansion

  // Simulation

  // Back Propagation

  // Choose best move

  // Save move to <result>

  return true;
}

