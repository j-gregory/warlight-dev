#include <string>
#include <iterator>
#include "OpponentBot.h"

using namespace std;


//int reinforcementThisTurn;	    //Reinforcement guessed for opponent in this turn
//int troopsLeftThisTurn;		    //Gets subtracted from every estimated attack / placement of the Opponent
//Region maxMomentRegion;		    //If no delta is recognized, vote the region with most troop concentration
//list<int> SuperRegionOwns;      //Super Regions speculted to be owned by the opponent Bot
//map<int, int> SuperRegionCost;	//1st parameter is cost of owning a superRegion, 2nd is the id of SuperRegion

//Constructor
OpponentBot::OpponentBot() : reinforcementThisTurn(5)
{
    ;
}

void OpponentBot::Handshake(Bot *b)
{
    bot = b;
}

string OpponentBot::GetName()
{
    return name;
}


void OpponentBot::SetName(string input)
{
    name = input;
}

//Adds a region to the list OpponentBot owns
void OpponentBot::AddRegion(int regionid)
{
    //add all regions to a list
    //from the list, decide how many super_regions are there, which one is easier to achieve ? use SuperRegion.getNumRegions()
    //make a super_region list, have super_region id ordered in the list based on the order of super_region to be achieved
    //if the region already belongs to the list, subtract 1 from the cost of owning the super_region.
    //If all super_regions in the list is achieved, do the calculation again with all neighboring area and which super_region is
    //easier to achieve.
    ;
    OpponentTotalRegion.push_back(regionid);

}

//Subtracts a region to the list OpponentBot owns
void OpponentBot::SubRegion(int regionid)
{
    //if a region is subtracted, is it subtracted from a super_region in list ? Add cost to the super_region list.
    //Is it the first region lost in Super_region, push it up in the ordered list.
    ;
}

//Getter method returns reinforcement per turn for the OpponentBot
int OpponentBot::GetReinforcement()
{
    return 0;
}

int OpponentBot::GetTroopsLeftThisTurn()
{
    return 0;
}
//sends you the region that has maximum threat from OpponentBot
int OpponentBot::GetMaxMomentRegion()
{
    return 0;
}

void OpponentBot::regionToSuperRegionMapping()
{
    std::vector<int> tempRegions;
    int tmpSuperRegion;

    for(int i=1; i< bot->super_regions.size(); i++)
    {
        tmpSuperRegion = bot->super_regions[i].getID();
        tempRegions = bot->super_regions[i].getRegions();

        std::cout << "SuperRegion # " << std::to_string(tmpSuperRegion) << "Regions # are :";

        for(int j=0; j < tempRegions.size(); j++)
        {
            std::cout << "," << std::to_string(tempRegions[j]);
            //regionToSuperRegionMap.push_back(regionToSrMapping(tmpSuperRegion, tempRegions(j)));
        }
        std::cout << endl;
        tempRegions.clear();
    }

    ;
}

//does internal book-keeping on all regions
void OpponentBot::Refresh()
{
    regionToSuperRegionMapping();       //create a quick list of tuples for region to superRegion to mapping

//    for(int i=0; i<OpponentTotalRegion.size(); i++)
//    {
//        find(bot->regions., regions.end(), )
//
//    }
}

//does internal book-keeping on a specific region, ignores the rest for quick runtime
void OpponentBot::Refresh(int Region)
{
    ;
}

//Gets called during update_map and setup_map opponent_starting_regions
void ObserveRegion(RegionOwner in, int RegionID)
{
    if(in == RegionOwner::Neutral)
    {
        ;//check if the RegionID belonged to OpponentBot, ifit did subtract
    }
    else if (in == RegionOwner::OppoentBot)
    {
        ;   //Add it to Region list, if it doesn't exist already
    }
    else if (in  == RegionOwner::PlayerBot)
    {
        ;   //check if the RegionID belonged to OpponentBot, ifit did subtract
    }

    //sort the super_region by the cost
    //std::sort(p_super_regions.begin(), p_super_regions.end());

    //create a list of regions to be acquired next

    //maintain a list of regions with troops in them

}

//called by the "debug opponent" argument
std::string  OpponentBot::printStatus()
{
    string result = "";

    result += "opponent's name :  " + name + '\n';
    result += "opponent's regions are : ";
    for(int i=0; i < OpponentTotalRegion.size(); i++)
    {
        result += ", " + std::to_string( OpponentTotalRegion[i] );
    }

    regionToSuperRegionMapping();

    result += "\n---------------\n";
    //for(int i=0)
    return result;
}

//call it in Parser::parseUpdateMap() & Parser::parseOpponentMoves
