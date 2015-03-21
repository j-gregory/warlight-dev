#include "State.h"

State::State() : win_percentage(0.5)
{
  name = "";
}

State::State(std::string n) : win_percentage(0.5)
{
  name = n;
}

State::State(std::string n, std::vector<Region> r) : win_percentage(0.5)
{
  name = n;
  regions_owned = r;
}

State::State(std::string n, std::vector<Region> r, double p)
{
  name = n;
  regions_owned = r;
  win_percentage = p;
}

State::~State()
{

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
