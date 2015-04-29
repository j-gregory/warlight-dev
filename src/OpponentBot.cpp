#include <string>
#include <iterator>
#include <tuple>
#include "OpponentBot.h"

//#define DEBUG_CANCEL_PRINT

using namespace std;


//int reinforcementThisTurn;	    //Reinforcement guessed for opponent in this turn
//int troopsLeftThisTurn;		    //Gets subtracted from every estimated attack / placement of the Opponent
//Region maxMomentRegion;		    //If no delta is recognized, vote the region with most troop concentration
//list<int> SuperRegionOwns;      //Super Regions speculted to be owned by the opponent Bot
//map<int, int> SuperRegionCost;	//1st parameter is cost of owning a superRegion, 2nd is the id of SuperRegion, this has to be recalculated by refresh

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
//1st param: Region id
//2nd param is 0, if this is the starting region for the opponent.
void OpponentBot::AddRegion(int regionid, int troops)
{
    //add all regions to a list
    //from the list, decide how many super_regions are there, which one is easier to achieve ? use SuperRegion.getNumRegions()
    //make a super_region list, have super_region id ordered in the list based on the order of super_region to be achieved
    //if the region already belongs to the list, subtract 1 from the cost of owning the super_region.
    //If all super_regions in the list is achieved, do the calculation again with all neighboring area and which super_region is
    //easier to achieve.

    //OpponentTotalRegion.push_back(regionid, troops);
    OpponentQueueRegions.push_back(tuple<int, int>(regionid, troops));

}

//Subtracts a region to the list OpponentBot owns
void OpponentBot::SubRegion(int regionid, int troops)
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

void OpponentBot::RecieveStatus(int msg)
{
    switch(msg)
    {
        case Bot::BotSetupStages::SetRegions:       //3
            setRegionsStatus = true;
        case Bot::BotSetupStages::SetSuperRegions:  //4
            setSuperRegionsSatus = true;
        default:
            break;
    }
}

void OpponentBot::regionToSuperRegionMapping()  //This should be called only once
{
    std::vector<int> tempRegions;
    int tmpSuperRegion;
    int troopInThisRegion = 0;
    bool ownStatus = false;
    int bogusCost = (reinforcementThisTurn / OpponentQueueRegions.size());


    for(unsigned int i=1; i < (bot->super_regions.size()); i++)
    {
        tmpSuperRegion = bot->super_regions[i].getID();
        //get list of regions for each super_reigon
        tempRegions = bot->super_regions[i].getRegions();

        //check if any of those region already belong to opponent_bot
        for(unsigned int j=0; j < tempRegions.size(); j++)
        {
            //if there are regions in the queue....
            if(OpponentQueueRegions.size() > 0)
            {
                for(unsigned int p = 0; p < OpponentQueueRegions.size();  p++)
                {
                    if( std::get<0>(OpponentQueueRegions.at(p)) == tempRegions[j])
                    {
                        //std::cout << " Reigons : " << tempRegions[j];
                        ownStatus = true;
                        troopInThisRegion = std::get<1>(OpponentQueueRegions.at(p));

                        if(troopInThisRegion == 0){troopInThisRegion = bogusCost;}
                        OpponentTotalRegion.push_back(tuple<int, int>(tempRegions[j], troopInThisRegion));

                        OpponentQueueRegions.erase(OpponentQueueRegions.begin()+p);
                        //OpponentQueueRegions.pop_back();
                        break;
                    }
                }
            }

            if(!ownStatus)
            {
                troopInThisRegion = bot->regions[tempRegions[j]].getNumArmies();
                //Perhaps nobody has visibility to this region, we assign an arbitrary troop number of 3 to help calculate cost of obtaining an object
                if(troopInThisRegion == 0) {troopInThisRegion = 3;}
            }

            regionToSuperRegionMap.push_back(tuple<int, int, int, bool>(tmpSuperRegion, tempRegions[j], troopInThisRegion, ownStatus));

            troopInThisRegion = 0;
            ownStatus = false;
        }//adding regions

        tempRegions.clear();

        //create a list that is sorted according to Super Regions
        superRegionToRegionMap = regionToSuperRegionMap;
        //create a list that is sorted according to Region
        //regionToSuperRegionMap.sort();

    }//addig super_regions

#ifndef DEBUG_CANCEL_PRINT

        std::cout << "Printing regionToSuperRegionMap :=" << endl;
        if(regionToSuperRegionMap.size() > 0)
        {
            output_list_contents(regionToSuperRegionMap);
        }
        std::cout << "Printing OpponentQueueRegions :=" << endl;
        if(OpponentQueueRegions.size() > 0)
        {
            output_list_contents(OpponentQueueRegions);
        }
        std::cout << "Printing OpponentTotalRegion :=" << endl;
        if(OpponentTotalRegion.size() > 0)
        {
            output_list_contents(OpponentTotalRegion);
        }

#endif
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

    //todo: Recalculate the vector SuperRegionCost - this will helps us realize how much each Superregion will cost less and prioritize properly
}

//does internal book-keeping on a specific region, ignores the rest for quick runtime
void OpponentBot::Refresh(int Region)
{
    ;
}

//Gets called during update_map and setup_map opponent_starting_regions
void OpponentBot::ObserveRegion(RegionOwner in, int RegionID)
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

//a print out routine
void OpponentBot::output_list_contents(std::vector<tuple<int, int, int, bool>> &myList)
{
    for(const auto& e: myList)
    {
        std::cout << "Opponent Bot SuperRegion # : "  << std::get<0>(e) <<  " Region # : " << std::get<1>(e) << " Troop# " << std::get<2>(e) << " Own Status " << (std::get<3>(e) ? "Y" : "N") << endl;
    }
}

void OpponentBot::output_list_contents(std::vector<tuple<int, int>> &myList)
{
    for(const auto& e: myList)
    {
        std::cout << "Region ID# : "  << std::get<0>(e) <<  " Troop# : " << std::get<1>(e) << " Troop# " << endl;
    }
}

//called by the "debug opponent" argument
std::string  OpponentBot::printStatus()
{
    string result = "";

    result += "opponent's name :  " + name + '\n';
    result += "opponent's regions are : ";
    for(unsigned int i=0; i < OpponentQueueRegions.size(); i++)
    {
        result += ", " + std::to_string( std::get<0>(OpponentQueueRegions.at(i)) );
    }

    //regionToSuperRegionMapping();
    try
    {
        if(setRegionsStatus && setSuperRegionsSatus)
        {
            regionToSuperRegionMapping();               //This call should be replaced by Refresh()
            //std::cout << "all is good" << endl;
        }
        else
        {
            std::cout << "Either regions or super_regions weren't setup properly" << endl;
        }
    }
    catch(...)
    {
        std::cout << "Error occured and may/may not be related to segmentation fault." << endl;
        std::cout << "Please check that the bots, regions, super_regions and neighbors were set up properly" << endl;
    }

    result += "\n---------------\n";
    //for(int i=0)
    return result;
}

//call it in Parser::parseUpdateMap() & Parser::parseOpponentMoves
