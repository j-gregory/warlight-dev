#ifndef __STATE_H_INCLUDED__
#define __STATE_H_INCLUDED__

//#include <stdlib.h>
#include <algorithm>
#include <string>
//#include <map>
//#include <vector>

#include "Region.h"
#include "OpponentBot.h"

class OpponentBot;
class State
{

 protected:
  std::string name;
  std::vector<Region> all_regions;
  std::vector<int> owned_regions;	//kq: legacy list is vector<Region>
  std::string move;
  double win_percentage;
  int num_visits;
  double Qval;

  //Use either this
   OpponentBot *opponent_bot;          //kq: introduced so that in mcts->execute() can take advantage of :
                                        //GetMaxMomentRegion()   - to bias the UCT simulation with a node where opponent carries most momentum
                                        //GetReinforcement() - to take account of maximum # of oppoent reinforcement that can take place on a region to be attacked
                                        //GetTroopsLeftTurn() - can be used only if opponent_bot is passed by val instead of passed by ref
  //Or use this
  int opponent_reinforcement;
  int opponent_momentumRegion;

 public:
  State();
  State(std::string n);
  State(std::string n, std::vector<Region> all, std::vector<int> owned);
  State(std::string n, std::vector<Region> all, std::vector<int> owned, double p);
  State(std::string n, std::vector<Region> all, std::vector<int> owned, double p, OpponentBot *opponentBot);
  State(std::string n, std::vector<Region> all, std::vector<int> owned, double p, int enemyReinforcement, int enemyRegionConcentrated);
  ~State();

  /* Getters */
  std::string getName()                 const { return name; }
  std::vector<Region> getAllRegions()   const { return all_regions; }
  std::vector<int> getOwnedRegions()    const { return owned_regions; }
  std::string getMove()                 const { return move; }
  double getWinPercentage()             const { return win_percentage; }
  int getNumVisits()                    const { return num_visits; }
  double getQval()                      const { return Qval; }

  /* Setters */
  void setName(std::string n);
  void setOwnedRegions(std::vector<int> r); //kq: umm w/o using vector<Region> we loose the neighbored regions. good idea ?
  void setArmies(int region_id, int armies);
  void setMove(std::string m);
  void setWinPercentage(double p);
  void setNumVisits(int n);
  void setQval(double q);

  void addNewOwnedRegion(int region_id);  //kq: originally input param was type Region
  void incrementNumVisits() { num_visits++; }

  friend bool operator== (const State& s1, const State& s2);
  friend bool operator!= (const State& s1, const State& s2);


};

#endif  // __STATE_H_INCLUDED__
