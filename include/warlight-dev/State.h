#ifndef __STATE_H_INCLUDED__
#define __STATE_H_INCLUDED__

//#include <stdlib.h>
#include <string>
//#include <map>
//#include <vector>

#include "Region.h"

class State
{
 public:
  State();
  State(std::string n);
  State(std::string n, std::vector<Region> r);
  State(std::string n, std::vector<Region> r, double p);
  ~State();

  /* Getters */
  std::string getName()                 const { return name; }
  std::vector<Region> getRegionsOwned() const { return regions_owned; }
  std::string getMove()                 const { return move; }
  double getWinPercentage()             const { return win_percentage; }

  /* Setters */
  void setName(std::string n);
  void setRegionsOwned(std::vector<Region> r);
  void setMove(std::string m);
  void setWinPercentage(double p);

  friend bool operator== (const State& s1, const State& s2);

 protected:
  std::string name;
  std::vector<Region> regions_owned;
  std::string move;
  double win_percentage;
};

#endif  // __STATE_H_INCLUDED__
