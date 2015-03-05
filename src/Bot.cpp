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
  botName = name;
}

void Bot::setOpponentBotName(std::string name)
{
  opponentBotName = name;
}

void Bot::setArmiesLeft(int nbArmies)
{
  armiesLeft = nbArmies;
}

void Bot::setPhase(std::string pPhase)
{
  phase = pPhase;
}

/* ************************************ */

void Bot::addRegion(unsigned noRegion, unsigned noSuperRegion)
{
  while (regions.size() <= noRegion)
  {
    regions.push_back(Region());
  }
  regions[noRegion] = Region(noRegion, noSuperRegion);
  superRegions[noSuperRegion].addRegion(noRegion);
}

void Bot::addNeighbors(unsigned noRegion, unsigned neighbors)
{
  regions[noRegion].addNeighbors(neighbors);
  regions[neighbors].addNeighbors(noRegion);
}

void Bot::addSuperRegion(unsigned noSuperRegion, int reward)
{
  while(superRegions.size() <= noSuperRegion)
  {
    superRegions.push_back(SuperRegion());
  }
  superRegions[noSuperRegion] = SuperRegion(reward);
}

void Bot::addStartingRegion(unsigned noRegion)
{
  startingRegionsreceived.push_back(noRegion);
}

void Bot::startDelay(int delay)
{

}

void Bot::executeAction()
{
  if (phase == "")
    return;

  if (phase == "pickPreferredRegion")
  {
    unsigned i, nbAns=0;
    for (i = 0; i < startingRegionsreceived.size() && nbAns < 6; i++)
    {
      std::cout << startingRegionsreceived[i];
      nbAns++;
      if (nbAns < 6)
	std::cout << " ";
      else
      {
	std::cout << "\n";
	break;
      }
    }
  }

  if (phase == "place_armies")
  {
    std::cout << botName << " place_armies " << ownedRegions[0] << " " << armiesLeft << "\n";
  }

  if (phase == "attack/transfer")
  {
    std::cout << "No moves\n" ;
    // std::cout << botName << " attack/transfer " << from << " " << to << " "<< armiesMoved;
  }
  phase.clear();
}

void Bot::updateRegion(unsigned noRegion, std::string playerName, int nbArmies)
{
  regions[noRegion].setArmies(nbArmies);
  regions[noRegion].setOwner(playerName);
  if (playerName == botName)
    ownedRegions.push_back(noRegion);
}

void Bot::addArmies(unsigned noRegion, int nbArmies)
{
  regions[noRegion].setArmies(regions[noRegion].getArmies() + nbArmies);
}

void Bot::moveArmies(unsigned noRegion, unsigned toRegion, int nbArmies)
{
  if (regions[noRegion].getOwner() == regions[toRegion].getOwner()
      && regions[noRegion].getArmies() > nbArmies)
  {
    regions[noRegion].setArmies(regions[noRegion].getArmies() - nbArmies);
    regions[toRegion].setArmies(regions[toRegion].getArmies() + nbArmies);
    
  }
  else if (regions[noRegion].getArmies() > nbArmies)
  {
    regions[noRegion].setArmies(regions[noRegion].getArmies() - nbArmies);
  }
}

void Bot::resetRegionsOwned()
{
  ownedRegions.clear();
}
