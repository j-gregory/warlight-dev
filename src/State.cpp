#include "State.h"

State::State() : win_percentage(0.5)
{
  name = "";
  move = "No moves\n";
}

State::State(std::string n) : win_percentage(0.5)
{
  name = n;
  move = "No moves\n";
}

State::State(std::string n, std::vector<Region> r) : win_percentage(0.5)
{
  name = n;
  owned_regions = r;
  move = "No moves\n";
}

State::State(std::string n, std::vector<Region> r, double p)
{
  name = n;
  owned_regions = r;
  win_percentage = p;
  move = "No moves\n";
}

State::~State()
{

}

bool operator==(const State& s1, const State& s2)
{
  return ( (s1.name == s2.name) && 
	   (s1.owned_regions == s2.owned_regions) &&
	   //std::equal(s1.owned_regions.begin(), s1.owned_regions.end(), s2.owned_regions) &&
	   (s1.move == s2.move) &&
	   (s1.win_percentage == s2.win_percentage) );

}

void State::setName(std::string n)
{
  name = n;
  // strcpy(name, n);
}

void State::setOwnedRegions(std::vector<Region> r)
{
  owned_regions = r;
}

void State::setArmies(int region_id, int armies)
{
  for(auto& region : owned_regions)
  {
    if(region.getID() == region_id)
    {
      region.setArmies(armies);
      break;
    }
  }
}

void State::setMove(std::string m)
{
  move = m;
}

void State::setWinPercentage(double p)
{
  win_percentage = p;
}

void State::addNewOwnedRegion(Region region)
{
  owned_regions.push_back(region);
}
