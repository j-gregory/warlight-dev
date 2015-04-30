#include "State.h"

State::State() : win_percentage(0.5), num_visits(0), Qval(0.0)
{
  name = "";
  move = "No moves\n";
}

State::State(std::string n) : win_percentage(0.5), num_visits(0), Qval(0.)
{
  name = n;
  move = "No moves\n";
}

State::State(std::string n, std::vector<Region> all, std::vector<int> owned) : win_percentage(0.5), num_visits(0), Qval(0.0)
{
  name = n;
  all_regions = all;
  owned_regions = owned;
  move = "No moves\n";
}

State::State(std::string n, std::vector<Region> all, std::vector<int> owned, double p) : num_visits(0), Qval(0.0)
{
  name = n;
  all_regions = all;
  owned_regions = owned;
  win_percentage = p;
  move = "No moves\n";
}

State::State(std::string n, std::vector<Region> all, std::vector<int> owned, double p, OpponentBot *opponentBot) : num_visits(0), Qval(0.0)
{
  name = n;
  all_regions = all;
  owned_regions = owned;
  win_percentage = p;
  move = "No moves\n";
  opponent_bot = opponentBot;
}

State::State(std::string n, std::vector<Region> all, std::vector<int> owned, double p, int enemyReinforcement, int enemyRegionConcentrated) : num_visits(0), Qval(0.0), opponent_reinforcement(enemyReinforcement), opponent_momentumRegion(enemyRegionConcentrated)
{
  name = n;
  all_regions = all;
  owned_regions = owned;
  win_percentage = p;
  move = "No moves\n";
}

State::~State()
{

}

bool operator==(const State& s1, const State& s2)
{
  return ( (s1.name == s2.name) &&
	   (s1.all_regions == s2.all_regions) &&
	   (s1.owned_regions == s2.owned_regions) &&
	   //std::equal(s1.owned_regions.begin(), s1.owned_regions.end(), s2.owned_regions) &&
	   (s1.move == s2.move) &&
	   (s1.win_percentage == s2.win_percentage) );
}

bool operator!=(const State& s1, const State& s2)
{
  return ( (s1.name != s2.name) ||
	   (s1.all_regions != s2.all_regions) ||
	   (s1.owned_regions != s2.owned_regions) ||
	   (s1.move != s2.move) ||
	   (s1.win_percentage != s2.win_percentage) );
}

void State::setName(std::string n)
{
  name = n;
  // strcpy(name, n);
}

void State::setOwnedRegions(std::vector<int> r)
{
  owned_regions = r;
}

void State::setArmies(int region_id, int armies)
{
  // @TODO: CAN SOMEONE CONFIRM ENGINE COMPILES WITH C+11
  /*
  for(auto& region : all_regions)
  {
    if(region.getID() == region_id)
    {
      region.setArmies(armies);
      break;
    }
  }
  */
  for(int i = 0; i < (int)all_regions.size(); i++)
  {
    if(all_regions[i].getID() == region_id)
    {
      all_regions[i].setArmies(armies);
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

void State::setNumVisits(int n)
{
  num_visits = n;
}

void State::setQval(double q)
{
  Qval = q;
}

void State::addNewOwnedRegion(int region_id)
{
  owned_regions.push_back(region_id);
}
