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
  regions_owned = r;
  move = "No moves\n";
}

State::State(std::string n, std::vector<Region> r, double p)
{
  name = n;
  regions_owned = r;
  win_percentage = p;
  move = "No moves\n";
}

State::~State()
{

}

bool operator==(const State& s1, const State& s2)
{
  return ( (s1.name == s2.name) && 
	   (s1.regions_owned == s2.regions_owned) &&
	   //std::equal(s1.regions_owned.begin(), s1.regions_owned.end(), s2.regions_owned) &&
	   (s1.move == s2.move) &&
	   (s1.win_percentage == s2.win_percentage) );

}

void State::setName(std::string n)
{
  name = n;
  // strcpy(name, n);
}

void State::setRegionsOwned(std::vector<Region> r)
{
  regions_owned = r;
}

void State::setMove(std::string m)
{
  move = m;
}

void State::setWinPercentage(double p)
{
  win_percentage = p;
}