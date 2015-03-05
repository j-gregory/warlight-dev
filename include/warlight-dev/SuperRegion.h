#ifndef SUPERREGION_H_INCLUDED
#define SUPERREGION_H_INCLUDED

#include <vector>
#include "Region.h"

class SuperRegion
{
  std::vector<int> regions;
  int num_regions;
  int reward;

 public:
  SuperRegion();
  SuperRegion(int p_reward);
  virtual ~SuperRegion();
  void addRegion(int no_region);
  int getNbRegions();

 protected:

 private:

};



#endif // SUPERREGION_H_INCLUDED
