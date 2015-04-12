#ifndef __STATE_H_INCLUDED__
#define __STATE_H_INCLUDED__

//#include <stdlib.h>
#include <algorithm>
#include <string>
//#include <map>
//#include <vector>

#include "Region.h"

class State
{
 public:
  State();
  State(std::string n);
  State(std::string n, std::vector<Region> all, std::vector<int> owned);
  State(std::string n, std::vector<Region> all, std::vector<int> owned, double p);
  ~State();

  /* Getters */
  std::string getName()                 const { return name; }
  std::vector<Region> getAllRegions()   const { return all_regions; }
  std::vector<int> getOwnedRegions()    const { return owned_regions; }
  std::string getMove()                 const { return move; }
  double getWinPercentage()             const { return win_percentage; }

  /* Setters */
  void setName(std::string n);
  void setOwnedRegions(std::vector<int> r);
  void setArmies(int region_id, int armies);
  void setMove(std::string m);
  void setWinPercentage(double p);

  void addNewOwnedRegion(int region_id);

  friend bool operator== (const State& s1, const State& s2);

 protected:
  std::string name;
  std::vector<Region> all_regions;
  std::vector<int> owned_regions;
  std::string move;
  double win_percentage;
};

#endif  // __STATE_H_INCLUDED__
