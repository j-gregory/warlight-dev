#ifndef __BOT_H__
#define __BOT_H__

#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <stdio.h>
#include <string>
#include <vector>

#include "Parser.h"
#include "Region.h"
#include "SuperRegion.h"
#include "UCTManager.h"

class Bot
{
  std::ifstream in;

  int round_number;
  double timebank;
  double time_per_move;
  int max_rounds;
  std::string bot_name;
  std::string opponent_bot_name;
  std::vector<SuperRegion> super_regions;
  std::vector<SuperRegion> p_super_regions;
  std::vector<Region> regions;
  std::vector<int> prev_avail_regions;
  std::vector<int> curr_avail_regions;
  std::map<int, std::string> belief_regions;
  std::vector<int> owned_regions;
  std::vector<int> opponent_regions;
  int armies_left;
  int pick_amount;

  Parser parser;
  std::string phase;

  UCTManager* uct;

 public:
  Bot();
  virtual ~Bot();
  
  void playGame();    // Plays a single game of Warlight
  void makeMoves();   // Makes moves for a single turn
  void endTurn();     // Indicates to the engine that it has made its moves

  /* Setters */
  void setRoundNumber(int round);
  void setTimebank(double time);
  void setTimePerMove(double time);
  void setMaxRounds(int max);
  void setBotName(std::string name);
  void setOpponentBotName(std::string name);
  void setArmiesLeft(int num_armies);
  void setPickAmount(int amount);
  void setPhase(std::string p_phase);

  void addStartingRegion(unsigned region_id);
  void addAvailableRegion(unsigned region_id);
  void addSuperRegion(unsigned super_region_id, int reward);
  void addRegion(unsigned region_id, unsigned super_region_id);
  void addNeighbors(unsigned region_id, unsigned neighbors);
  void addArmies(unsigned region_id, int num_armies);

  void analyzeSuperRegions();
  void analyzeRegions();
  void analyzeAvailableRegions();

  void startClock(int clock);
  void executeAction();
  void moveArmies(unsigned region_id, unsigned to_region, int num_armies);
  void updateRegion(unsigned region_id, std::string player_name, int num_armies);
  void resetAvailableRegions();
  void resetRegionsOwned();

  void printStatus();

 private:

};

#endif // __BOT_H__
