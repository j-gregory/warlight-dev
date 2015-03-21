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

  std::string getName()                 const { return name; }
  std::vector<Region> getRegionsOwned() const { return regions_owned; }
  float getWinPercentage()              const { return win_percentage; }

  void setName(std::string n);
  void setRegionsOwned(std::vector<Region> r);

 protected:
  std::string name;
  std::vector<Region> regions_owned;
  float win_percentage;
};

#endif  // __STATE_H_INCLUDED__
