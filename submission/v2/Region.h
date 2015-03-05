#ifndef REGION_H
#define REGION_H

#include <vector>
#include <string>

class Region
{  
  std::vector<int> neighbors;
  int id;
  int num_neighbors;
  int super_region;
  std::string owner;
  int armies;

 public:
  Region();
  Region(int p_id,int super_region);
  virtual ~Region();
  void addNeighbors(int n);
  void setArmies(int num_armies);
  void setOwner(std::string o);
  int getArmies();
  std::string getOwner();
  int getSuperRegion();
  std::vector<int>& getNeighbors();

 protected:

 private:

};

#endif // REGION_H
