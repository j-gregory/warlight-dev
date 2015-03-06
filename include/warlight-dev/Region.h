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
  Region(int p_id, int super_region);
  virtual ~Region();

  /* Getters */
  std::vector<int>& getNeighbors() { return neighbors; };
  int getID()                      const { return id; };
  int getNumNeighbors()            const { return num_neighbors; };
  int getSuperRegionID()           const { return super_region; };
  std::string getOwner()           const { return owner; };
  int getNumArmies()               const { return armies; };

  /* Setters */
  void setOwner(std::string o);
  void setArmies(int num_armies);

  void addNeighbors(int n);
  void print();

 protected:

 private:

};

#endif // REGION_H
