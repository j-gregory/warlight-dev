#include "UCTManager.h"

#include "string_util.hpp"

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
  std::string move = getRandomMove(*node_itr);

  // Produce resulting state for random valid move by simulating turn
  std::cout << "Executed random move: " << move;
  State result(name, regions, 0.5);
  simulateTurn((*node_itr), move, regions, result);

  // Add new node to tree, iterate there
  node_itr = game_tree.append_child(node_itr, result);
  std::cout << "Now at new leaf: " << (*node_itr).getName() << std::endl;


  /*! ------------------------------------------------------------------------------
   *  SIMULATION
   *  Simulate the rest of the game to determine the win_percentage of added node
   *  ------------------------------------------------------------------------------
   */
  int steps = 0;
  // while(time is almost up)
  while(steps < 5)
  {
    // @TODO: simulate();
    steps++;
  }


  /*! ------------------------------------------------------------------------------
   *  BACK PROPAGATION
   *  Update all parent nodes using win_percentage information we gained
   *  ------------------------------------------------------------------------------
   */
  //while(node_itr != game_tree.begin())
  //{
    //Calculate new win percentage
    //double new_win_percentage;
    //(*node_itr).setWinPercentage(new_win_percentage);
    //curr = curr.parent;
  //}

  // Find our best move - Find child node with largest win_percentage, return that command
  return (name + findBestMove(game_tree));
}

std::string UCTManager::getRandomMove(State& state)
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

void UCTManager::simulateTurn(State& state, std::string move, std::vector<Region> regions, State& result)
{
  std::cout << "Parsing " << move << std::endl;
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

    // Set new number of armies in region attacked from    
    //int total_armies = state.getOwnedRegions()[from].getNumArmies();
    //int lost_armies = attack_armies-survive_attack;
    //int remaining_armies = total_armies-lost_armies-survive_attack;
    int remaining_armies = 
      state.getOwnedRegions()[from].getNumArmies() - attack_armies - (2*survive_attack);
    state.setArmies(from, remaining_armies);

    if(survive_defend == 0)
    {
      std::cout << "Attack: SUCCESS\n";
      // Set number of armies in newly-acquired region
      // Add newly-acquired region to regions owned
      Region winnings = regions[to];
      winnings.setArmies(survive_attack);
      result.addNewOwnedRegion(winnings);
    }
    else std::cout << "Attack: FAILED\n";
  }

  result.setMove(move);
  // @TODO: Win percentage calculation based on battle?
  result.setWinPercentage(0.5);
}

void UCTManager::simulateBattle(int attack_armies, int defend_armies, int& survive_attack, int&survive_defend)
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
