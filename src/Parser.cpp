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

void Parser::initParser(Bot* b)
{
  bot = b;
}

/* **************** */
/* Parse controller */
/* **************** */
void Parser::parseInput()
{
  std::string input_type;
  while (std::cin >> input_type)
  {
    if (input_type == "settings")
      parseSettings();
    else if (input_type == "setup_map")
      parseSetupMap();
    else if (input_type == "pick_starting_regions")
      parseStartingRegions();
    else if (input_type == "update_map")
      parseUpdateMap();
    else if (input_type == "opponent_moves")
      parseOpponentMoves();
    else if (input_type == "go")
      parseGo();
    bot->executeAction();
  }
}

/* ************************************ */

/* "settings" Parser */
void Parser::parseSettings()
{
#ifdef DEBUG_PRINT
  std::cout << "parseSettings\n";
#endif // DEBUG_PRINT
  std::string setting_type;
  std::string bot_name;
  int num_armies;
  std::cin >> setting_type;

  if (setting_type == "timebank")
  {
    double timebank;
    std::cin >> timebank;
    bot->setTimebank(timebank);
  }
  else if (setting_type == "time_per_move")
  {
    double time;
    std::cin >> time;
    bot->setTimePerMove(time);
  }
  else if (setting_type == "max_rounds")
  {
    double max;
    std::cin >> max;
    bot->setMaxRounds(max);
  }
  else if (setting_type == "your_bot")
  {
    std::cin >> bot_name;
    bot->setBotName(bot_name);
  }
  else if (setting_type == "opponent_bot")
  {
    std::cin >> bot_name;
    bot->setOpponentBotName(bot_name);
  }
  else if (setting_type == "starting_armies")
  {
    std::cin >> num_armies;
    bot->setArmiesLeft(num_armies);
#ifdef DEBUG_PRINT
    std::cout << "settings starting_armies " << num_armies << "\n";
#endif // DEBUG_PRINT
  }
  else if (setting_type == "starting_regions")
  {
    // Do something
  }
  else if (setting_type == "starting_pick_amount")
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
  std::string setup_type;
  std::cin >> setup_type;
  if (setup_type == "super_regions")
    parseSuperRegions();
  else if (setup_type == "regions")
    parseRegions();
  else if (setup_type == "neighbors")
    parseNeighbors();
  else if (setup_type == "wastelands")
    parseWastelands();
  else if (setup_type == "opponent_starting_regions")
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
  bot->startDelay(delay);
  while(std::cin  >> region)
  {
    bot->addStartingRegion(region);
    if (std::cin.peek()== '\n')
      break;
  }
  bot->setPhase("pickPreferredRegion");
}

/* "update_map" Parser */
void Parser::parseUpdateMap()
{
#ifdef DEBUG_PRINT
  std::cout <<"parseUpdate_Map\n";
#endif // DEBUG_PRINT
  std::string player_name;
  int no_region;
  int num_armies;
  bot->resetRegionsOwned();
  while (std::cin >> no_region >> player_name >> num_armies)
  {
    bot->updateRegion(no_region, player_name, num_armies);
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
  std::string player_name;
  std::string action;
  int no_region;
  int num_armies; 
  int to_region;
  while (std::cin.peek()!= '\n' && std::cin >> player_name >> action)
  {
    if (action == "place_armies")
    {
      std::cin >> no_region >> num_armies;
      bot->addArmies(no_region, num_armies);
    }
    if (action == "attack/transfer")
    {
      std::cin >> no_region >> to_region >> num_armies;
      bot->moveArmies(no_region, to_region, num_armies);
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
  bot->startDelay(delay);
  bot->setPhase(phase);
}

/* ************************************ */

/* "setup_map super_regions" Parser */
/* Superregion IDs with rewards     */
void Parser::parseSuperRegions()
{
  int super;
  int reward;
#ifdef DEBUG_PRINT
  std::cout << "parseSuperRegions\n";
#endif // DEBUG_PRINT
  
  while(std::cin >> super >> reward)
  {
    bot->addSuperRegion(super, reward);
    if (std::cin.peek()== '\n')
      break;
  }
}

/* "setup_map regions" Parser */
/* Region IDs with superregion IDs */
void Parser::parseRegions()
{
  int super;
  int region;
#ifdef DEBUG_PRINT
  std::cout << "parseRegions\n";
#endif // DEBUG_PRINT
  
  while(std::cin  >> region >> super)
  {
    bot->addRegion(region, super);
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
      bot->addNeighbors(region, atoi(neighbors_flds[i].c_str()));
    if (std::cin.peek()== '\n')
      break;
  }
  neighbors_flds.clear();
  bot->setPhase("findBorders");
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


