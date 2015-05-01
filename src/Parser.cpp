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
    if (input_type == "Round")
      parseRoundNumber();
    if (input_type == "settings")
      parseSettings();
    else if (input_type == "setup_map")
      parseSetupMap();
    else if (input_type == "pick_starting_region")
      parsePickStartingRegions();
    else if (input_type == "update_map")
      parseUpdateMap();
    else if (input_type == "opponent_moves")    //kq: parseOpponentMoves() is called for the actual
      parseOpponentMoves();
    else if (input_type == "go")
      parseGo();
    else if (input_type == "debug")             //kq; flesh this out for bot opponent()
      bot->printStatus();
    bot->executeAction();
  }
}

/* ************************************ */

/* "Round #" Parser */
void Parser::parseRoundNumber()
{
#ifdef DEBUG_PRINT
  std::cout << "parseSettings\n";
#endif // DEBUG_PRINT
  int round;
  std::cin >> round;
  bot->setRoundNumber(round);
}

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
    bot->opponent_bot->SetStartingTroop(num_armies);

#ifdef DEBUG_PRINT
    std::cout << "settings starting_armies " << num_armies << "\n";
#endif // DEBUG_PRINT
  }
  else if (setting_type == "starting_regions")
  {
    // Nothing to do because pick_starting_region will supply same list
    // Could record these regions if you wanted to do some processing while engine is working
    // This process could be good because it wouldn't use time from timebank
    // Don't know what opponent is going to choose, but could prioritize all regions


    // Upon further analysis:
    //  1) They give you 5 starting regions to choose from
    //  2) Two players, each get to choose two regions
    //  3) They don't tell all of the choices your opponent made BUT you can deduce

    int region;
    while(std::cin  >> region)
    {
      bot->addStartingRegion(region);
      if (std::cin.peek()== '\n')
	break;
    }
  }
  else if (setting_type == "starting_pick_amount")
  {
    int amount;
    std::cin >> amount;
    bot->setPickAmount(amount);
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
  else if (setup_type == "wastelands")                  //kq: The Neutral lands that are started off at the beginning
    parseWastelands();                                  //kq: parseWasteLands() isn't implemented, you should use it for bot neutral zone.
  else if (setup_type == "opponent_starting_regions")
    parseOpponentStartingRegions();
}

/* "pick_starting_regions" Parser */
void Parser::parsePickStartingRegions()
{
#ifdef DEBUG_PRINT
  std::cout << "parseStartingRegions\n";
#endif // DEBUG_PRINT
  int region;
  int time_limit;
  std::cin >> time_limit;
  bot->startClock(time_limit);
  bot->resetAvailableRegions();
  while(std::cin  >> region)
  {
    bot->addAvailableRegion(region);    //kq: adding to avail region

    if (std::cin.peek() == '\n')
      break;
  }
  bot->analyzeAvailableRegions();       //kq: bookeeping of regions to either your bot or opponent's
  bot->setPhase("pickPreferredRegion"); //kq: This phase is implemented in Bot.cpp
}

/* "update_map" Parser */
void Parser::parseUpdateMap()       //kq: This needs to be fleshed out for opponent Bot
{
#ifdef DEBUG_PRINT
  std::cout <<"parseUpdate_Map\n";
#endif // DEBUG_PRINT
  std::string player_name;
  int region_id;
  int num_armies;
  bot->resetRegionsOwned();
  while (std::cin >> region_id >> player_name >> num_armies)
  {
    bot->updateRegion(region_id, player_name, num_armies);
    if (std::cin.peek()== '\n')
    {
      //bot->opponent_bot->Refresh();
      break;
    }
  }
}

/* "opponent_moves" Parser */// TODO (pavel#1#):

void Parser::parseOpponentMoves()   //kq: who is calling this? Is this good place for Opponent.GuesstimateStrength() ?
{
#ifdef DEBUG_PRINT
  std::cout << "parseOpponent_Moves\n";
#endif // DEBUG_PRINT
  std::string player_name;
  std::string action;
  int region_id;
  int num_armies;
  int to_region;
  while (std::cin.peek()!= '\n' && std::cin >> player_name >> action)
  {
    if (action == "place_armies")
    {
      std::cin >> region_id >> num_armies;
      bot->addArmies(region_id, num_armies);    //kq: regions[region_id].setArmies(regions[region_id].getNumArmies() + num_armies);
    }
    if (action == "attack/transfer")
    {
      std::cin >> region_id >> to_region >> num_armies;
      bot->moveArmies(region_id, to_region, num_armies);
    }
    if (std::cin.peek()== '\n')
      break;
  }
}

/* "go" Parser */
void Parser::parseGo()
{
  std::string phase;
  int time_limit;
  std::cin >> phase >> time_limit;
  bot->startClock(time_limit);
  bot->setPhase(phase);

  //kq: debug trace, take out later
  std::cout << "TRACE: Parser::parseGo()" << std::endl;;
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
  bot->SendStatus(Bot::BotSetupStages::SetSuperRegions);

  //Shouldn't analyze here
  //bot->analyzeSuperRegions();
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
  bot->analyzeSuperRegions();
//  Bot::BotSetupStages
  bot->SendStatus(Bot::BotSetupStages::SetRegions);
  //bot->analyzeRegions();
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
  bot->setPhase("findBorders"); //kq: Not implemented.
}

/* "setup_map wastelands" Parser */
/* Region IDs of neutral regions (>2 armies) */
//kq: Implement this
void Parser::parseWastelands()
{

}

/* "setup_map opponent_starting_regions" Parser */
/* Region IDs opponent has chosen */
void Parser::parseOpponentStartingRegions()
{
    int region;

    while(std::cin >> region)
    {
        bot->opponent_bot->AddRegion(region, 0);    //we are passing 0, b/c we don't really know the # of troops in that area until update_map phase
        if(std::cin.peek() == '\n')
        {
            //bot->opponent_bot->Refresh();
            break;
        }
    }
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


