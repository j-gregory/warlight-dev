#include "Parser.h"
#include <iostream>
#include <stdlib.h>
#include "Bot.h"


Parser::Parser()
{
  //ctor
}

Parser::~Parser()
{
  //dtor
}

void Parser::initParser(Bot* bot)
{
  theBot = bot;
}

/* **************** */
/* Parse controller */
/* **************** */
void Parser::parseInput()
{
  std::string inputType;
  while (std::cin >> inputType)
  {
    if (inputType == "settings")
      parseSettings();
    else if (inputType == "setup_map")
      parseSetupMap();
    else if (inputType == "pick_starting_regions")
      parseStartingRegions();
    else if (inputType == "update_map")
      parseUpdateMap();
    else if (inputType == "opponent_moves")
      parseOpponentMoves();
    else if (inputType == "go")
      parseGo();
    theBot->executeAction();
  }
}

/* ************************************ */

/* "settings" Parser */
void Parser::parseSettings()
{
#ifdef DEBUG_PRINT
  std::cout << "parseSettings\n";
#endif // DEBUG_PRINT
  std::string settingType;
  std::string bot_name;
  int nbArmies;
  std::cin >> settingType;

  if (settingType == "timebank")
  {
    //std::cin >> timebank;
    //theBot->setTimebank(timebank);
  }
  else if (settingType == "time_per_move")
  {
    //std::cin >> time_per_move;
    //theBot->setTimePerMove(time_per_move);
  }
  else if (settingType == "max_rounds")
  {
    //std::cin >> max;
    //theBot->setMaxRounds(max);
  }
  else if (settingType == "your_bot")
  {
    std::cin >> bot_name;
    theBot->setBotName(bot_name);
  }
  else if (settingType == "opponent_bot")
  {
    std::cin >> bot_name;
    theBot->setOpponentBotName(bot_name);
  }
  else if (settingType == "starting_armies")
  {
    std::cin >> nbArmies;
    theBot->setArmiesLeft(nbArmies);
#ifdef DEBUG_PRINT
    std::cout << "settings starting_armies " << nbArmies << "\n";
#endif // DEBUG_PRINT
  }
  else if (settingType == "starting_regions")
  {
    // Do something
  }
  else if (settingType == "starting_pick_amount")
  {
    // Do something
  }
}

/* "setup_map" Parser */
void Parser::parseSetupMap()
{
#ifdef DEBUG_PRINT
  std::cout <<"parseSetupMap\n";
#endif // DEBUG_PRINT
  std::string setupType;
  std::cin >> setupType;
  if (setupType == "super_regions")
    parseSuperRegions();
  else if (setupType == "regions")
    parseRegions();
  else if (setupType == "neighbors")
    parseNeighbors();
  else if (setupType == "wastelands")
    parseWastelands();
  else if (setupType == "opponent_starting_regions")
    parseOpponentStartingRegions();
}

/* "pick_starting_regions" Parser */
void Parser::parseStartingRegions()
{
#ifdef DEBUG_PRINT
  std::cout << "parseStartingRegions\n";
#endif // DEBUG_PRINT
  int region;
  int delay;
  std::cin >> delay;
  theBot->startDelay(delay);
  while(std::cin  >> region)
  {
    theBot->addStartingRegion(region);
    if (std::cin.peek()== '\n')
      break;
  }
  theBot->setPhase("pickPreferredRegion");
}

/* "update_map" Parser */
void Parser::parseUpdateMap()
{
#ifdef DEBUG_PRINT
  std::cout <<"parseUpdate_Map\n";
#endif // DEBUG_PRINT
  std::string playerName;
  int noRegion, nbArmies;
  theBot->resetRegionsOwned();
  while (std::cin >> noRegion >> playerName >> nbArmies)
  {
    theBot->updateRegion(noRegion, playerName, nbArmies);
    if (std::cin.peek()== '\n')
      break;
  }
}

/* "opponent_moves" Parser */
void Parser::parseOpponentMoves()
{
#ifdef DEBUG_PRINT
  std::cout << "parseOpponent_Moves\n";
#endif // DEBUG_PRINT
  std::string playerName, action;
  int noRegion, nbArmies, toRegion;
  while (std::cin.peek()!= '\n' && std::cin >> playerName >> action)
  {
    if (action == "place_armies")
    {
      std::cin >> noRegion >> nbArmies;
      theBot->addArmies(noRegion, nbArmies);
    }
    if (action == "attack/transfer")
    {
      std::cin >> noRegion >> toRegion >> nbArmies;
      theBot->moveArmies(noRegion, toRegion, nbArmies);
    }
    if (std::cin.peek()== '\n')
      break;
  }
}

/* "go" Parser */
void Parser::parseGo()
{
  std::string phase;
  int delay;
  std::cin >> phase >> delay;
  theBot->startDelay(delay);
  theBot->setPhase(phase);
}

/* ************************************ */

/* "setup_map super_regions" Parser */
/* Superregion IDs with rewards     */
void Parser::parseSuperRegions()
{
  int super,reward;
#ifdef DEBUG_PRINT
  std::cout << "parseSuperRegions\n";
#endif // DEBUG_PRINT
  
  while(std::cin >> super >> reward)
  {
    theBot->addSuperRegion(super, reward);
    if (std::cin.peek()== '\n')
      break;
  }
}

/* "setup_map regions" Parser */
/* Region IDs with superregion IDs */
void Parser::parseRegions()
{
  int super,region;
#ifdef DEBUG_PRINT
  std::cout << "parseRegions\n";
#endif // DEBUG_PRINT
  
  while(std::cin  >> region >> super)
  {
    theBot->addRegion(region, super);
    if (std::cin.peek()== '\n')
      break;
  }
}

/* "setup_map neighbors" Parser */
/* Region IDs with neighboring region IDs */
void Parser::parseNeighbors()
{
#ifdef DEBUG_PRINT
  std::cout <<  "parseNeighbors\n";
#endif // DEBUG_PRINT
  int region;
  std::string neighbors;
  std::vector<std::string> neighbors_flds;
  while(std::cin  >> region >> neighbors)
  {
    neighbors_flds = splitString(neighbors, neighbors_flds, ',');
    for (unsigned i = 0; i < neighbors_flds.size(); i++)
      theBot->addNeighbors(region, atoi(neighbors_flds[i].c_str()));
    if (std::cin.peek()== '\n')
      break;
  }
  neighbors_flds.clear();
  theBot->setPhase("findBorders");
}

/* "setup_map wastelands" Parser */
/* Region IDs of neutral regions (>2 armies) */
void Parser::parseWastelands()
{

}

/* "setup_map opponent_starting_regions" Parser */
/* Region IDs opponent has chosen */
void Parser::parseOpponentStartingRegions()
{

}

/* ************************************ */

std::vector<std::string>& Parser::splitString(std::string String, std::vector<std::string>& flds, char delim)
{
  if (!flds.empty())
    flds.clear();
  std::string buf = "";
  unsigned i = 0;
  while (i < String.length())
  {
    if (String[i] != delim)
      buf += String[i];
    else
    {
      flds.push_back(buf);
      buf = "";
    }
    i++;
  }
  if (!buf.empty())
    flds.push_back(buf);
  return flds;
}


