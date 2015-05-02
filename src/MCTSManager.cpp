#include "MCTSManager.h"

#include "string_util.hpp"

MCTSManager::MCTSManager()
{

}

MCTSManager::~MCTSManager()
{

}

std::string MCTSManager::execute(std::string name, std::vector<Region> all_regions, std::vector<int> owned_regions, double timelimit) //kq: This could be expanded with opponent_bot
{
  //std::cout << "Executing MCTS" << std::endl;
  //std::cout << "Initializing tree" << std::endl;

  // Create root node with current state of game
  Tree game_tree;
  game_tree.insert(game_tree.begin(), State("root", all_regions, owned_regions));
  //std::cout << "Tree initialization complete" << std::endl;

  //std::cout << "Printing tree\n";
  ////kptree::print_tree_bracketed(test, std::cout);
  //printTree(game_tree);
  ////printTreeBracketed(game_tree);

  bool our_turn = true;

  /* =============================================================================== */
  // This will be based on time, but for debuggin purposes just an arbitrary number of iterations
  int iterations = 0;
  //while(time is almost up)
  while(iterations < 10)
  {
    //std::cout << "*************** NEW ITERATION OF MCTS ****************" << std::endl;
    //std::cout << "***********     game_tree has " << game_tree.size() << " nodes " << std::endl;

    //printTree(game_tree);

    /*! ------------------------------------------------------------------------------
     *  SELECTION
     *  UCT (and win percentage) WILL BE USED IN THIS STEP TO BALANCE EXPLORATION AND EXPLOITATION
     *  For now, traverse "random" branch until random location or child node
     *  ------------------------------------------------------------------------------
     */
    //std::cout << "Starting SELECTION process" << std::endl;
    /*
    // SELECTION PROCESS - RANDOM
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
    */
    
    // SELECTION PROCESS - UCT
    ////TreeFixedDepthIterator node_itr = game_tree.begin_fixed(game_tree.begin(), 1);
    //std::cout << "Preparing to run UCT algorithm" << std::endl;
    TreeIterator node_itr = game_tree.begin();
    State selection = UCT(node_itr);
    //std::cout << "UCT Algorithm found selection state and returned" << std::endl;
    //std::cout << "Selection: " << selection.getName() << std::endl;
    node_itr = game_tree.begin();
    while((*node_itr) != selection) node_itr++;
    
    //std::cout << "Randomly selected leaf: " << (*node_itr).getName() << std::endl;
    
    
    /*! ------------------------------------------------------------------------------
     *  EXPANSION
     *  Expand current path by one node representing a random move
     *  ------------------------------------------------------------------------------
     */
    //std::cout << "Starting EXPANSION process\n";
    State result(name, all_regions, owned_regions, 0.5);
    if(our_turn)
    {
      //std::cout << "Expanding: Simulating our turn" << std::endl;
      simulateOurTurn((*node_itr), result);
    }
    else
    {
      //std::cout << "Expanding: Simulating opponent's turn" << std::endl;
      simulateOpponentsTurn((*node_itr), result);
      result.setName("opponent");
    }
    our_turn = (!our_turn);
    
    // Add new node to tree, iterate there
    //std::cout << "* Adding new node to game_tree from " << result.getName() << std::endl;
    node_itr = game_tree.append_child(node_itr, result);
    //std::cout << "Now at new leaf: " << (*node_itr).getName() << std::endl;
    
    
    /*! ------------------------------------------------------------------------------
     *  SIMULATION
     *  Simulate the rest of the game to determine the win_percentage of added node
     *  ------------------------------------------------------------------------------
     */
    //std::cout << "Starting SIMULATION process\n";
    State curr = result;
    State next;
    int steps = 0;
    // while(some amount of time)
    while(steps < 5)
    {
      // Their turn? Guess one of their moves
      if(steps%2 == 0) simulateOpponentsTurn(curr, next); //kq: This could be expanded with opponent_bot
      // Our turn? Simulate a random move
      else simulateOurTurn(curr, next);                   //kq: This could be expanded with opponent_bot
      
      curr = next;
      steps++;
    }
    
    
    /*! ------------------------------------------------------------------------------
     *  BACK PROPAGATION
     *  Update all parent nodes using win_percentage information we gained
     *  ------------------------------------------------------------------------------
     */
    //std::cout << "Starting BACK PROPAGATION process\n";
    double updated_win_percentage = next.getWinPercentage();
    while(node_itr != game_tree.begin())
    {
      (*node_itr).setWinPercentage(updated_win_percentage);
      node_itr = game_tree.parent(node_itr);
    }
    
    // Reset nodes' Q and numVisit values
    node_itr = game_tree.begin();
    while(game_tree.is_valid(node_itr))
    {
      (*node_itr).setNumVisits(0);
      (*node_itr).setQval(0.0);
      node_itr++;
    }
    // DONE SINGLE ITERATION OF MCTS
    iterations++;
  }

  /* =============================================================================== */

  // Find our best move - Find child node with largest win_percentage, return that command
  return findBestMove(game_tree);
}

State MCTSManager::UCT(TreeIterator& node_itr)
{
  //std::cout << "Starting UCT algorithm" << std::endl;

  if(node_itr.number_of_children() == 0) return (*node_itr);

  State selection;
  int time_left = 5;   // Could incorporate actual time later
  std::vector<State> envelope;
  double cost = 0;
  while(time_left > 0)
  {
    //std::cout << "Calling UCTHelper from node: " << (*node_itr).getName() << std::endl;
    cost = UCTHelper(node_itr, 3, envelope, selection);
    //std::cout << "Returned from UCTHelper with cost of " << cost << std::endl;
    //node_itr = node_itr.begin();
    while((*node_itr).getName() != "root") node_itr--;
    time_left--;
    //std::cout << "Returned iterator to head of tree, time left is " << time_left << std::endl;
  }
  return selection;
}


double MCTSManager::UCTHelper(TreeIterator& node_itr, int cutoff, std::vector<State> envelope, State& s_prime)
{
  //std::cout << "Starting UCTHelper" << std::endl;
  State s = (*node_itr);
  int num_children = node_itr.number_of_children();
  if(num_children == 0) return 0.0;
  if(cutoff == 0) return 5.0;  // @TODO: Fix this

  //std::cout << "Envelope size: " << envelope.size() << std::endl;

  std::vector<State> untried;
  if(std::find(envelope.begin(), envelope.end(), (*node_itr)) == envelope.end() || 
     (int)envelope.size() == 0)
  {
    //std::cout << "State is NOT in envelope, adding ..." << std::endl;
    (*node_itr).setNumVisits(0);
    envelope.push_back(*node_itr);

    // All <action, states> are applicable in our case
    for(int i = 0; i < num_children; i++)
    {
      node_itr++;
      (*node_itr).setQval(0.0);
      (*node_itr).setNumVisits(0);
      untried.push_back(*node_itr);
    }
  }

  // Return iterator to current "root"
  //node_itr = game_tree.begin_fixed(game_tree.begin(), 1);
  //node_itr = node_itr.begin();
  for(int i = 0; i < num_children; i++) node_itr--;

  int num_untried = (int)untried.size();
  //std::cout << "Untried actions: " << num_untried << std::endl;
  if(num_untried > 0)
  {
    //std::cout << "Choosing random <action, state>" << std::endl;
    // Choose random next state from untried
    int rand_index = rand() % num_untried;
    //std::cout << "rand_index: " << rand_index << std::endl;
    //s_prime = untried[rand_index];
    node_itr++;
    for(int i = 0; i != rand_index; i++)   // != rand_index
      node_itr++;
    s_prime = (*node_itr);
    //std::cout << "UCT - randomly chose state " << s_prime.getName() << std::endl;
  }
  else
  {
    //std::cout << "Choosing argmin <action, state> according to UCT formula" << std::endl;
    // Choose state with minimum (Q - C(log(n)/n') value
    double uct_val = 100000;
    for(int i = 0; i < num_children; i++)
    {
      node_itr++;
      double tmp_val = 100000;
      if((*node_itr).getNumVisits() == 0) 
	tmp_val = (s.getQval() - 2);
      else
	tmp_val = s.getQval() - 2*pow(log(s.getNumVisits()/(*node_itr).getNumVisits()), 0.5); 

      if(tmp_val < uct_val)
      {
	s_prime = (*node_itr);
	uct_val = tmp_val;
      }
    }
    while((*node_itr) != s_prime) node_itr--;
  }

  // No need to sample, already have next state
  // At this point, iterator should be at next state and s_prime should be set
  
  //std::cout << "Performing cost rollout (recursive UCT)" << std::endl;
  // Perform cost rollout using UCT with cutoff
  double cost_rollout = 1 + UCTHelper(node_itr, --cutoff, envelope, s_prime);

  //std::cout << "Updating UCT values of states" << std::endl;
  // Update current state's Q and n values
  int num_visits = (*node_itr).getNumVisits();
  (*node_itr).setQval(((num_visits*(*node_itr).getQval())+cost_rollout) / (1+num_visits));
  (*node_itr).incrementNumVisits();
  // Update original "root" state in node_itr
  s.incrementNumVisits();
  while((*node_itr) != s) node_itr--;
  (*node_itr).incrementNumVisits();
  // Return iterator to next state, do we need this?
  //while((*node_itr) != s_prime) node++;
  
  //std::cout << "Cost rollout for this state is " << cost_rollout << std::endl;

  return cost_rollout;
}

std::string MCTSManager::getAggressiveRandomMove(State& state)
{
  std::string rand_move = "No moves\n";

  // Look at state.owned_regions, pick a random valid move
  std::vector<Region> all_regions = state.getAllRegions();
  std::vector<int> owned_regions  = state.getOwnedRegions();
  int from_id = 0;
  int to = 0;
  int max_armies = 0;

  // Implementation for choosing region with largest number of armies
  int temp_region = 0;
  int temp_armies = 0;
  for(int i = 0; i < (int)owned_regions.size(); i++)
  {
    temp_region = all_regions[owned_regions[i]].getID();
    temp_armies = all_regions[owned_regions[i]].getNumArmies();
    if (temp_armies > max_armies)
    {
      from_id = temp_region;
      //from_index = i;
      max_armies = temp_armies;
    }
  }
  
  to = from_id;
  int attempts = 0;
  int num_neighbors = all_regions[from_id].getNumNeighbors();
  // Addition: never attack a region we already own (i.e., no transfers)
  while(std::find(owned_regions.begin(), owned_regions.end(), to) != owned_regions.end() || attempts < 10)
  {
    int rand_index = rand() % num_neighbors;
    to = all_regions[from_id].getNeighbors()[rand_index];
    attempts++;
  }

  return (all_regions[from_id].getOwner() +
	  " attack/transfer " +
	  std::to_string(from_id) + " " +
	  std::to_string(to) + " " +
	  std::to_string(max_armies-1) + "\n");
}

std::string MCTSManager::getImprovedRandomMove(State& state)
{

  // Convert owned regions to map, automatically sorts
  std::vector<Region> all_regions = state.getAllRegions();
  std::vector<int> owned_regions  = state.getOwnedRegions();
  std::map<int, int> armies_to_region;
  for(int i = 0; i < (int)owned_regions.size(); i++)
  {
    int region_id = owned_regions[i];
    int num_armies = all_regions[region_id].getNumArmies();
    armies_to_region[num_armies] = region_id;
  }
  
  // Iterate through owned regions based on largest number of armies, 
  // attempt to find random neighbor that we don't own so we can attack
  bool found_move = false;
  int from_id = 0;
  int to_id = 0;
  int max_armies = 0;
  for(std::map<int, int>::reverse_iterator map_itr = armies_to_region.rbegin();
      map_itr != armies_to_region.rend();
      ++map_itr)
  {
    from_id = map_itr->second;
    max_armies = map_itr->first;

    // Find (random) adjacent region
    to_id = from_id;
    int attempts = 0;
    int num_neighbors = all_regions[from_id].getNumNeighbors();
    // Addition: never attack a region we already own (i.e., no transfers)
    while(std::find(owned_regions.begin(), owned_regions.end(), to_id) != owned_regions.end())
    {
      if(attempts >= num_neighbors) break;
      //int rand_index = rand() % num_neighbors;
      //to_id = all_regions[from_id].getNeighbors()[rand_index];
      to_id = all_regions[from_id].getNeighbors()[attempts];
      attempts++;
    }

    if(attempts < num_neighbors) 
    {
      found_move = true;
      break;
    }
  }

  // If didn't find move, do some transfer
  if(!found_move)
  {
    std::map<int, int>::reverse_iterator map_itr = armies_to_region.rbegin();
    from_id = map_itr->second;
    max_armies = map_itr->first;
    int num_neighbors = all_regions[from_id].getNumNeighbors();
    int rand_index = rand() % num_neighbors;
    to_id = all_regions[from_id].getNeighbors()[rand_index];
  }

  return (all_regions[from_id].getOwner() +
	  " attack/transfer " +
	  std::to_string(from_id) + " " +
	  std::to_string(to_id) + " " +
	  std::to_string(max_armies-1) + "\n");
}

std::string MCTSManager::getTotallyRandomMove(State& state)
{  
  std::vector<Region> all_regions = state.getAllRegions();
  std::vector<int> owned_regions  = state.getOwnedRegions();

  int rand_index = rand() % (int)owned_regions.size();
  int from_id = owned_regions[rand_index];
  int region_armies = all_regions[from_id].getNumArmies();
  while(region_armies < 2) 
  {
    rand_index = rand() % (int)owned_regions.size();
    from_id = owned_regions[rand_index];
    region_armies = all_regions[from_id].getNumArmies();
  }
  rand_index = rand() % (all_regions[from_id].getNumNeighbors());
  int to_id = all_regions[from_id].getNeighbors()[rand_index];
  int max_armies = all_regions[from_id].getNumArmies();

  return (all_regions[from_id].getOwner() +
	  " attack/transfer " +
	  std::to_string(from_id) + " " +
	  std::to_string(to_id) + " " +
	  std::to_string(max_armies-1) + "\n");
}

std::string MCTSManager::getSeededRandomMove(State& state, int from_id)
{
  std::vector<Region> all_regions = state.getAllRegions();
  int region_armies = all_regions[from_id].getNumArmies();
  
  if(region_armies <= 2) return "No moves\n";

  int rand_index = rand() % (all_regions[from_id].getNumNeighbors());
  int to_id = all_regions[from_id].getNeighbors()[rand_index];

  int attack_armies = rand() % (region_armies-2);
  attack_armies++;

  return (all_regions[from_id].getOwner() +
	  " attack/transfer " +
	  std::to_string(from_id) + " " +
	  std::to_string(to_id) + " " +
	  std::to_string(attack_armies));
}

std::string MCTSManager::getSeededDirectedMove(State& state, int from_id)
{
  std::vector<Region> all_regions = state.getAllRegions();
  int att_eligible_armies = (all_regions[from_id].getNumArmies()-1);
  
  if(att_eligible_armies <= 2) return "No moves\n";

  // Sort list of neighbors by lowest number of troops
  std::map<int, int> armies_to_region;
  for(int i = 0; i < (int)all_regions[from_id].getNumNeighbors(); i++)
  {
    int region_id = all_regions[from_id].getNeighbors()[i];
    int num_armies = all_regions[region_id].getNumArmies();
    armies_to_region[num_armies] = region_id;
  }
  
  std::map<int, int>::iterator map_itr = armies_to_region.begin();
  int to_id = map_itr->second;
  int to_armies = map_itr->first;

  // Determine how many armies to attack with (if at all)
  if(att_eligible_armies <= to_armies) return "No moves\n";

  // Attack with all we got!
  //int attack_armies = att_eligible_armies;

  // Attack with some random amount
  //int attack_armies = rand() % att_eligible_armies;
  //attack_armies++;

  // Attack with some random amount more than the opposing region
  int diff = att_eligible_armies - to_armies;
  int eps = rand() % diff;
  int attack_armies = to_armies + eps;

  return (all_regions[from_id].getOwner() +
	  " attack/transfer " +
	  std::to_string(from_id) + " " +
	  std::to_string(to_id) + " " +
	  std::to_string(attack_armies));
}

void MCTSManager::simulateOurTurn(State& state, State& result)
{
  //std::cout << "-- Simulating our turn --" << std::endl;;
  // Get random, valid move
  std::string move = getImprovedRandomMove(state);

  // Get random number of random moves
  //std::string all_moves = "";
  std::vector<std::string> all_moves;
  std::string single_move;
  //int random_num_moves = rand() % (int)state.getOwnedRegions().size();
  //random_num_moves++;  // Want at least one random move
  //for(int i = 0; i < random_num_moves; i++)
  for(int i = 0; i < (int)state.getOwnedRegions().size(); i++)
  {
    // Could randomize seed also
    //single_move = getSeededRandomMove(state, state.getOwnedRegions()[i]);
    single_move = getSeededDirectedMove(state, state.getOwnedRegions()[i]);
    if(single_move == "No moves\n") 
      continue; //i--;
    else
      all_moves.push_back(single_move);
  }
  if((int)all_moves.size() == 0) all_moves.push_back(getImprovedRandomMove(state));

  std::string final_move = "";
  for(int i = 0; i < (int)all_moves.size(); i++)
  {
    // Produce resulting state for random valid move by simulating turn
    //std::cout << "Executing random move: " << move;

    std::vector<std::string> tokens = split(all_moves[i], ' ');
    if(tokens[1] == "attack/transfer")
    {
      int from = std::stoi(tokens[2]);
      int to = std::stoi(tokens[3]);
      int attacking_armies = std::stoi(tokens[4]);
      int defending_armies = state.getAllRegions()[to].getNumArmies();
      
      //std::cout << "Simulating attack from region " << from
      //          << " with " << attacking_armies << " armies"
      //	        << " to region " << to
      //	        << " that has " << defending_armies << " armies " << std::endl;
      
      int attacking_destroyed = 0;
      int defending_destroyed = 0;
      simulateBattle(attacking_armies, defending_armies, attacking_destroyed, defending_destroyed);
      
      int survive_attack = attacking_armies - attacking_destroyed;
      //if(survive_attack < 0) survive_attack = 0;
      int survive_defend = defending_armies - defending_destroyed;
      //if(survive_defend < 0) survive_defend = 0;
      
      //std::cout << "Battle Simulation Results: " << std::endl;
      //std::cout << "attacking_armies: "     << attacking_armies
      //	      << " attacking_destroyed: " << attacking_destroyed
      //	      << " survive_attack: "      << survive_attack << std::endl;
      //std::cout << "defending_armies: "     << defending_armies
      //	      << " defending_destroyed: " << defending_destroyed
      //	      << " survive_defend: "      << survive_defend << std::endl;
      
      // The outcome of an attack is ONE of the following:
      //   1) Win:  Move the surviving armies onto new region
      //   2) Lose: All attacking armies died, all non-attacking remain safe
      // By definition: attacking_armies = lost + survived
      // Regardless of how many survive or are lost, the remaining number of armies is (original - attack)
      // In other words, once allocated for an attack, armies will never return to the region
      
      // If we won the battle, we need to update the owner and number of armies with how many survived
      if(survive_defend <= 0)
      {
	//std::cout << "Attack: SUCCESS\n";
	// Update number of armies in resulting state
	int remaining_armies = state.getAllRegions()[from].getNumArmies() - attacking_armies;
	result.setArmies(from, remaining_armies);
	
	// Set number of armies in newly-acquired region
	result.setRegionOwner(to, state.getAllRegions()[from].getOwner());
	result.setArmies(to, survive_attack);
	// Add newly-acquired region to regions owned
	result.addNewOwnedRegion(to);
      }
      
      // If we lost the battle, we may update the other region's number of surviving armies (defense)
      else
      {
	//std::cout << "Attack: FAILED\n";
	// Update number of armies in current state
	int remaining_attacking_armies =
	  state.getAllRegions()[from].getNumArmies() - attacking_destroyed;  // attacking_armies?
	result.setArmies(from, remaining_attacking_armies);
	
	// Update number of armies in defending region
	int remaining_defending_armies =
	  state.getAllRegions()[to].getNumArmies() - defending_destroyed;
	// Current implementation doesn't support this because Region 'to' won't be in 'owned'
	result.setArmies(to, remaining_defending_armies);
      }
    }
    final_move += all_moves[i];
    if(i < ((int)all_moves.size()-1)) final_move += ", ";
  }
  final_move += "\n";

  //result.setMove(move);
  result.setMove(final_move);
  double wp = calculateWinPercentage(result.getAllRegions(), result);
  result.setWinPercentage(wp);
}

void MCTSManager::simulateOpponentsTurn(State& state, State& result)
{
  //std::cout << "-- Simulating their turn --" << std::endl;;
  // @TODO: Implement
  result = state;
}

void MCTSManager::simulateBattle(int attacking_armies, int defending_armies, int& attacking_destroyed, int&defending_destroyed)
{
  // According to wiki.warlight.net/index.php/Combat_Basics
  // Taken directly from doAttack(AttackTransferMove move) method in Warlight2 game engine
  // Then translated to fit our architecture

  // If attempting to attack with illegal number of armies (1), do nothing
  if (attacking_armies <= 1)
  {
    attacking_destroyed = 0;
    defending_destroyed = 0;
    return;
  }

  // Calculate how much attacking armies are destroyed with 100% luck
  for(int i = 1; i <= defending_armies; i++)
  {
    double chance = (rand() % 100) / 10.0;  // Between 0 and 1
    //70% chance to destroy one attacking army
    if(chance < 0.7) attacking_destroyed++;
  }

  // Calculate how much defending armies are destroyed with 100% luck
  for(int i = 1; i <= attacking_armies; i++)
  {
    double chance = (rand() % 100) / 10.0;  // Between 0 and 1
    //60% chance to destroy one defending army
    if(chance < 0.6) defending_destroyed++;
  }

  // Apply luck modifier to get actual amount of destroyed armies
  // Straight round method is used (instead of weighted random round)
  defending_destroyed = (int) round( ((attacking_armies * 0.6) * (1 - LUCK_MODIFIER))
				    + (defending_destroyed * LUCK_MODIFIER) );
  attacking_destroyed = (int) round( ((defending_armies * 0.7) * (1 - LUCK_MODIFIER))
				    + (attacking_destroyed * LUCK_MODIFIER) );

  if(attacking_destroyed >= attacking_armies)
  {
    if(defending_destroyed >= defending_armies)
      defending_destroyed = defending_armies - 1;

    attacking_destroyed = attacking_armies;
  }
}

double MCTSManager::calculateWinPercentage(std::vector<Region> regions, State& state)
{
  // @TODO: Implement

  // Win percentage calculation - Initial thoughts:
  // 1) Based on battle?
  // 2) Based on number of owned regions vs total regions
  // 3) Based on number of our regions vs opponents regions

  /*
  std::cout << "Win percentage calculation -  ownedregions: " << state.getOwnedRegions().size()
	    << "  totalregions: " << regions.size() 
	    << "  percentage: " << (state.getOwnedRegions().size() / (double)regions.size())
	    << std::endl;
  */
  return (state.getOwnedRegions().size() / (double)regions.size());
}
std::string MCTSManager::findBestMove(Tree game_tree)
{
  // No moves if game_tree is empty
  if(game_tree.size() == 0) return "No moves\n";

  std::string best_move;
  // Iterate immediate children, find highest win_percentage
  double temp_win_percentage = 0;
  //TreeSiblingIterator child = game_tree.begin();
  TreeFixedDepthIterator child = game_tree.begin_fixed(game_tree.begin(), 1);
  //while(child != game_tree.end())
  while(game_tree.is_valid(child))
  {
    if((*child).getWinPercentage() > temp_win_percentage)
    {
      best_move = (*child).getMove();
      temp_win_percentage = (*child).getWinPercentage();
    }
    ++child;
  }
  return best_move;
}

void MCTSManager::printTree(Tree game_tree)
{
  TreeIterator node = game_tree.begin();
  //while(node != game_tree.end())
  while(game_tree.is_valid(node))
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
  for(Tree::sibling_iterator iRoots = t.begin(); iRoots != t.end(); ++iRoots, ++headNum) {
    printSubtreeBracketed(t, iRoots);
    if (headNum != headCount) {
      std::cout << "Printing newline\n";
      std::cout << std::endl;
    }
  }
}

void MCTSManager::printSubtreeBracketed(const Tree& t, Tree::iterator iRoot)
{
  if(t.empty()) return;
  if (t.number_of_children(iRoot) == 0) {
    std::cout << (*iRoot).getName();
  }
  else {
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
