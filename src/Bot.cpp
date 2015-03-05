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

void Bot::addStartingRegion(unsigned no_region)
{
  starting_regions.push_back(no_region);
}

void Bot::addSuperRegion(unsigned no_super_region, int reward)
{
  while(super_regions.size() <= no_super_region)
  {
    super_regions.push_back(SuperRegion());
  }
  super_regions[no_super_region] = SuperRegion(reward);
}

void Bot::addRegion(unsigned no_region, unsigned no_super_region)
{
  while (regions.size() <= no_region)
  {
    regions.push_back(Region());
  }
  regions[no_region] = Region(no_region, no_super_region);
  super_regions[no_super_region].addRegion(no_region);
}

void Bot::addNeighbors(unsigned no_region, unsigned neighbors)
{
  regions[no_region].addNeighbors(neighbors);
  regions[neighbors].addNeighbors(no_region);
}

void Bot::addArmies(unsigned no_region, int num_armies)
{
  regions[no_region].setArmies(regions[no_region].getArmies() + num_armies);
}

void Bot::moveArmies(unsigned no_region, unsigned to_region, int num_armies)
{
  if (regions[no_region].getOwner() == regions[to_region].getOwner()
      && regions[no_region].getArmies() > num_armies)
  {
    regions[no_region].setArmies(regions[no_region].getArmies() - num_armies);
    regions[to_region].setArmies(regions[to_region].getArmies() + num_armies);
    
  }
  else if (regions[no_region].getArmies() > num_armies)
  {
    regions[no_region].setArmies(regions[no_region].getArmies() - num_armies);
  }
}

void Bot::startClock(int clock)
{
  // Keep track of time and prioritize
  // Pre-emptively interrupt so we don't run over time and lose turn!
}

void Bot::executeAction()
{
  if (phase == "")
    return;

  else if (phase == "pickPreferredRegion")
  {
    /*
    unsigned i;
    unsigned num_ans = 0;
    for (i = 0; i < starting_regions.size() && num_ans < 6; i++)
    {
      std::cout << starting_regions[i];
      num_ans++;
      if (num_ans < 6)
	std::cout << " ";
      else
      {
	std::cout << "\n";
	break;
      }
    }
    */
    if (starting_regions.size() <= 0)
    {
      std::cout << "No moves\n"; 
      std::cout.flush();
    }
    else
    {
      std::cout << std::to_string(starting_regions[0]) << "\n"; 
      std::cout.flush();
    }
    
    // Just delete all because you will add updated list in next iteration (if any)
    starting_regions.clear();
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

void Bot::updateRegion(unsigned no_region, std::string player_name, int num_armies)
{
  regions[no_region].setArmies(num_armies);
  regions[no_region].setOwner(player_name);
  if (player_name == bot_name)
    owned_regions.push_back(no_region);
}

void Bot::resetRegionsOwned()
{
  owned_regions.clear();
}
