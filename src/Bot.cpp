#include "Bot.h"

Bot::Bot()
{
  // printf("ctor\n");
}

Bot::~Bot()
{
  //printf("dtor\n");
  //dtor
}

/* Plays a single game of Warlight */
void Bot::playGame()
{
  parser.initParser(this);
  parser.parseInput();
}    

/* Makes moves for a single turn */
void Bot::makeMoves()
{
  
}

/* Indicates to the engine that it has made its move */
void Bot::endTurn()
{
  
}

/* ************************************ */

/* Setters */
void Bot::setRoundNumber(int round)
{
  round_number = round;
}
void Bot::setTimebank(double time)
{
  timebank = time;
}

void Bot::setTimePerMove(double time)
{
  time_per_move = time;
}

void Bot::setMaxRounds(int max)
{
  max_rounds = max;
}

void Bot::setBotName(std::string name)
{
  bot_name = name;
}

void Bot::setOpponentBotName(std::string name)
{
  opponent_bot_name = name;
}

void Bot::setArmiesLeft(int num_armies)
{
  armies_left = num_armies;
}

void Bot::setPickAmount(int amount)
{
  pick_amount = amount;
}

void Bot::setPhase(std::string p_phase)
{
  phase = p_phase;
}

/* ************************************ */

void Bot::addStartingRegion(unsigned region_id)
{
  prev_avail_regions.push_back(region_id);
  belief_regions[region_id] = "none";
}

void Bot::addAvailableRegion(unsigned region_id)
{
  curr_avail_regions.push_back(region_id);
}

void Bot::addSuperRegion(unsigned super_region_id, int reward)
{
  while(super_regions.size() <= super_region_id)
  {
    super_regions.push_back(SuperRegion(0, 0));
  }
  super_regions[super_region_id] = SuperRegion(super_region_id, reward);
}

void Bot::addRegion(unsigned region_id, unsigned super_region_id)
{
  while (regions.size() <= region_id)
  {
    regions.push_back(Region(0, 0));
  }
  regions[region_id] = Region(region_id, super_region_id);
  super_regions[super_region_id].addRegion(region_id);
}

void Bot::addNeighbors(unsigned region_id, unsigned neighbors)
{
  regions[region_id].addNeighbors(neighbors);
  regions[neighbors].addNeighbors(region_id);
}

void Bot::addArmies(unsigned region_id, int num_armies)
{
  regions[region_id].setArmies(regions[region_id].getNumArmies() + num_armies);
}

void Bot::startClock(int clock)
{
  // Keep track of time and prioritize
  // Pre-emptively interrupt so we don't run over time and lose turn!
}

void Bot::analyzeSuperRegions()
{
  // Copy SuperRegions (with Regions)
  p_super_regions = super_regions;

  // Sort SuperRegions based on bonus (ascending)
  std::sort(p_super_regions.begin(), p_super_regions.end());
  /*
  std::cout << "\nPrioritized: ";
  for(int i = 0; i < p_super_regions.size(); i++)
    std::cout << p_super_regions[i].getID() << " ";
  std::cout << "\n";
  */
}

void Bot::analyzeRegions()
{
  // For now, just printouts for debugging
  std::cout << "\nStarting: \n";
  for(int i = 0; i < super_regions.size(); i++)
  {
    std::vector<int> regions = super_regions[i].getRegions();
    for(int j = 0; j < regions.size(); j++)
      std::cout << regions[j] << " ";
    std::cout << "\n";
  }

  std::cout << "\nPrioritized: \n";
  for(int i = 0; i < p_super_regions.size(); i++)
  {
    std::vector<int> regions = p_super_regions[i].getRegions();
    for(int j = 0; j < regions.size(); j++)
      std::cout << regions[j] << " ";
    std::cout << "\n";
  }

}

void Bot::analyzeAvailableRegions()
{ 
  for(int i = 0; i < curr_avail_regions.size(); i++)
  {
    int region = curr_avail_regions[i];
    if(std::find(prev_avail_regions.begin(), prev_avail_regions.end(), region) == prev_avail_regions.end())
    {
      /* prev_avail_regions does NOT contain region in curr_avail_regions contains */
      if(belief_regions[region] != bot_name)
      {
	opponent_regions.push_back(region);
	belief_regions[region] = opponent_bot_name;
      }
    }
  }
  // Store currently available as previously available for next iteration
  prev_avail_regions.clear();
  prev_avail_regions = curr_avail_regions;
  //curr_avail_regions.clear();
}

void Bot::executeAction()
{
  if (phase == "")
    return;

  else if (phase == "pickPreferredRegion")
  {
    // Find SuperRegion with smallest bonus (smallest number of regions)
    // And choose Region from that SuperRegion
    for (int i = 0; i < p_super_regions.size(); i++)
    {
      //std::cout << "Investigating super region: " << p_super_regions[i].getID() << "\n";
      std::vector<int> regions = p_super_regions[i].getRegions();
      /*
      std::cout << "Regions in super region: ";
      for (int k = 0; k < regions.size(); k++)
	std::cout << regions[k] << " ";
      */
      for (int j = 0; j < curr_avail_regions.size(); j++)
      {
	int candidate = curr_avail_regions[j];
	//std::cout << "\nCandidate: " << candidate << "\n";
	if(std::find(regions.begin(), regions.end(), candidate) != regions.end())
	{
	  std::cout << std::to_string(candidate) << "\n";
	  std::cout.flush();
	  return;
	}
      }
    }   
  }

  else if (phase == "place_armies")
  {
    std::cout << bot_name << " place_armies " << owned_regions[0] << " " << armies_left << "\n";
  }

  else if (phase == "attack/transfer")
  {
    std::cout << "No moves\n" ;
    // std::cout << botName << " attack/transfer " << from << " " << to << " "<< armiesMoved;
  }
  phase.clear();
}

void Bot::moveArmies(unsigned region_id, unsigned to_region, int num_armies)
{
  if (regions[region_id].getOwner() == regions[to_region].getOwner()
      && regions[region_id].getNumArmies() > num_armies)
  {
    regions[region_id].setArmies(regions[region_id].getNumArmies() - num_armies);
    regions[to_region].setArmies(regions[to_region].getNumArmies() + num_armies);
    
  }
  else if (regions[region_id].getNumArmies() > num_armies)
  {
    regions[region_id].setArmies(regions[region_id].getNumArmies() - num_armies);
  }
}

void Bot::updateRegion(unsigned region_id, std::string player_name, int num_armies)
{
  regions[region_id].setArmies(num_armies);
  regions[region_id].setOwner(player_name);
  if (player_name == bot_name)
    owned_regions.push_back(region_id);
}

void Bot::resetAvailableRegions()
{
  curr_avail_regions.clear();
}

void Bot::resetRegionsOwned()
{
  owned_regions.clear();
}

void Bot::printStatus()
{
  std::cout << "Timebank: " << timebank << "\n";
  std::cout << "Time/move: " << time_per_move << "\n";
  std::cout << "Max rounds: " << max_rounds << "\n";
  std::cout << "Bot name: " << bot_name << "\n";
  std::cout << "Opponent: " << opponent_bot_name << "\n";
  std::cout << "Num superregions: " << super_regions.size() << "\n";
  std::cout << "Starting armies: " << armies_left << "\n";
  //std::cout << "Num starting regions: " << starting_regions.size() << "\n";
  std::cout << "Pick amount: " << pick_amount << "\n";
  //p_super_regions.print();
}
