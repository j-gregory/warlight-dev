#ifndef BOT_H
#define BOT_H

#include <fstream>
#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>

#include "Parser.h"
#include "Region.h"
#include "SuperRegion.h"

class Bot
{
  std::ifstream in;

  double timebank;
  double time_per_move;
  int max_rounds;
  std::string bot_name;
  std::string opponent_bot_name;
  std::vector<SuperRegion> super_regions;
  std::vector<Region> regions;
  std::vector<int> starting_regions_received;
  std::vector<int> owned_regions;
  int armies_left;

  Parser parser;
  std::string phase;

 public:
  Bot();
  virtual ~Bot();
  
  void playGame();    // Plays a single game of Warlight
  void makeMoves();   // Makes moves for a single turn
  void endTurn();     // Indicates to the engine that it has made its moves

  // Setters
  void setTimebank(double time);
  void setTimePerMove(double time);
  void setMaxRounds(int max);
  void setBotName(std::string name);
  void setOpponentBotName(std::string name);
  void setArmiesLeft(int num_armies);
  void setPhase(std::string p_phase);

  void addRegion(unsigned no_region, unsigned no_super_region);
  void addSuperRegion(unsigned no_super_region, int reward);
  void addNeighbors(unsigned no_region, unsigned neighbors);
  void addArmies(unsigned no_region, int num_armies);
  void moveArmies(unsigned no_region, unsigned to_region, int num_armies);
  void addStartingRegion(unsigned no_region);
  void startDelay(int delay);
  void executeAction();
  void updateRegion(unsigned no_region, std::string player_name, int num_armies);
  void resetRegionsOwned();

 private:

};

#endif // BOT_H
