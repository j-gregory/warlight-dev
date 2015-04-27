#ifndef __SUPERREGION_H_INCLUDED__
#define __SUPERREGION_H_INCLUDED__

#include <vector>
#include "Region.h"
#include "Bot.h"

class Bot;

class SuperRegion
{
  int id;
  std::vector<int> regions;
  int num_regions;
  int reward;
  int costForYouToOwn;      //kq: Tells the # of regions left if you were to own it.
  int costForOppToOwn;      //kq: Tells the # of regions left for opponent to own it.

 public:
  SuperRegion();
  SuperRegion(int p_id, int p_reward);
  //SuperRegion(int p_id, std::vector<int> p_regions, int p_reward);
  virtual ~SuperRegion();

  /* Getters */
  int getID()                   const { return id; };
  std::vector<int> getRegions() const { return regions; };
  int getNumRegions()           const { return num_regions; };
  int getReward()               const { return reward; };
  int GetCostForYouToOwn()      const { return costForYouToOwn; };
  int GetCostForOppToOwn()      const { return costForOppToOwn; };

  /* Setters */
  void SetCostForYouToOwn(int cost) { costForYouToOwn = cost; };    //the cost of you and opponent does not add up to total cost, there could be neutral zones
  void SetCostForOppToOwn(int cost) { costForOppToOwn = cost; };

  void addRegion(int region_id);
  void print();

  bool operator< (const SuperRegion& other) const;

 protected:

 //private:             //kq: I commented this, after all private scope is implied by lack of mention for some members already

};



#endif // __SUPERREGION_H_INCLUDED__
