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
                        OpponentTotalRegion.push_back(tuple<int, int, int>(tmpSuperRegion, tempRegions[j], troopInThisRegion));

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
        std::sort(begin(regionToSuperRegionMap), end(regionToSuperRegionMap), TupleCompareAscending<1>());

    }//addig super_regions

    allMappingSetupDone = true;    //this routine will not be triggered from now on.

#ifndef DEBUG_CANCEL_PRINT

        std::cout << "Printing regionToSuperRegionMap :=" << endl;
        if(regionToSuperRegionMap.size() > 0)
        {
            output_list_contents(regionToSuperRegionMap);
        }
        std::cout << "Printing superRegionToRegionMap :=" << endl;
        if(superRegionToRegionMap.size() > 0)
        {
            output_list_contents(superRegionToRegionMap);
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

//Sets up a quick list of achievable Super Regions and their heuristic cost
void OpponentBot::calculateCost()
{
    std::vector<tuple<int>> tempListOfSuperRegionsToConsider;
    bool alreadyOwn = false;
    bool foundEntryInSuperRegionPursueList = false;
    int tempCostofSuperRegion = 0;
    int tempSuperRegion = 0;
    int tempSuperRegionToConsider = 0;
    int tempSuperRegionInPursueList = 0;
    int tempRegionToIgnore = 0;
    int tempRegionAudited = 0;
    SuperRegionPursueList.clear();      //start with cean slate
    //std::vector<tuple<int, int>>

    //take a list of superRegions
    for(unsigned int i=0; i< OpponentTotalRegion.size(); i++)    //iterate through regions you already own
    {
       tempSuperRegionToConsider = std::get<0>(OpponentTotalRegion.at(i));
       tempListOfSuperRegionsToConsider.push_back(tuple<int>(tempSuperRegionToConsider));
    }
    std::sort(begin(tempListOfSuperRegionsToConsider), end(tempListOfSuperRegionsToConsider), TupleCompareAscending<0>());
    //prune the vector
    for(unsigned int x=0; x< tempListOfSuperRegionsToConsider.size()-1; x++)
    {
        if(tempListOfSuperRegionsToConsider.at(x) == tempListOfSuperRegionsToConsider.at(x+1))
        {
            tempListOfSuperRegionsToConsider.erase(tempListOfSuperRegionsToConsider.begin()+(x+1));
        }
    }

    //use the list of SuperRegions & add all costs of those regions, excpept the ones you already own.
    for(unsigned int j=0; j<superRegionToRegionMap.size(); j++)
    {
        for(unsigned int l=0; l<tempListOfSuperRegionsToConsider.size(); l++)
        {

            tempSuperRegion = std::get<0>(superRegionToRegionMap.at(j));
            tempSuperRegionToConsider = std::get<0>(tempListOfSuperRegionsToConsider.at(l));
            //If you are looking at a member with same Super Region id
            //if(std::get<0>(superRegionToRegionMap.at(j)) == tempListOfSuperRegionsToConsider.at(l))
            if(tempSuperRegion == tempSuperRegionToConsider)
            {
                //check this row against all already owned region, they are not to be counted for cost calculation
                for(unsigned int k=0; k< OpponentTotalRegion.size(); k++)
                {
                    tempRegionToIgnore = std::get<1>(OpponentTotalRegion.at(k));
                    tempRegionAudited = std::get<1>(superRegionToRegionMap.at(j));
                    //If the region in this row already belongs to opponent ignore.
                    //if(std::get<1>(OpponentTotalRegion.at(k)) ==  std::get<1>(superRegionToRegionMap.at(j)))
                    if(tempRegionToIgnore == tempRegionAudited)
                    {
                        alreadyOwn = true;
                    }
                }

                //If you don't own it, add the troop in this region to total cost for this Super Region
                if(!alreadyOwn)
                {
                    tempCostofSuperRegion += std::get<2>(superRegionToRegionMap.at(j));

                    if(SuperRegionPursueList.size() == 0)   //we have no entry.
                    {
                        foundEntryInSuperRegionPursueList = true;
                        SuperRegionPursueList.push_back(tuple<int, int>(tempSuperRegionToConsider, tempCostofSuperRegion));
                    }
                    else
                    {
                        for(unsigned int m=0; m<SuperRegionPursueList.size(); m++)   //may be we have entry
                        {
                            tempSuperRegionInPursueList = std::get<0>(SuperRegionPursueList.at(m));
                            //if(tempListOfSuperRegionsToConsider.at(l) == std::get<0>(SuperRegionPursueList.at(m)))
                            if(tempSuperRegionToConsider == tempSuperRegionInPursueList)
                            {
                                foundEntryInSuperRegionPursueList = true;
                                std::get<1>(SuperRegionPursueList.at(m)) += tempCostofSuperRegion;
                            }
                        }
                    }

                    if(!foundEntryInSuperRegionPursueList)
                    {
                        SuperRegionPursueList.push_back(tuple<int, int>(tempSuperRegionToConsider, tempCostofSuperRegion));
                    }
                }

                tempCostofSuperRegion = 0;
                foundEntryInSuperRegionPursueList = false;
            }

        }

        alreadyOwn = false;
    }
    std::sort(begin(SuperRegionPursueList), end(SuperRegionPursueList), TupleCompareAscending<1>());

#ifndef DEBUG_CANCEL_PRINT

        std::cout << "Printing SuperRegionPursueList :=" << endl;
        if(SuperRegionPursueList.size() > 0)
        {
            for(unsigned y=0; y<SuperRegionPursueList.size(); y++)
            {
                std::cout << "SuperRegion # " << std::get<0>(SuperRegionPursueList.at(y)) << " Cost : " << std::get<1>(SuperRegionPursueList.at(y)) << endl;
            }
        }
        else
        {
            std::cout << "SuperRegionPursueList isn't populated" << endl;
        }

#endif
}//OpponentBot::calculateCost()

//does internal book-keeping on all regions
void OpponentBot::Refresh()
{
    //initial setup
    if(!allMappingSetupDone)
    {
        regionToSuperRegionMapping();       //create a quick list of tuples for region to superRegion to mapping
        calculateCost();
    }
    //update statments after update_map
    else
    {
        std::cout << "update of opponent_bot via update_map haven't been implemented yet" << endl;
    }

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

void OpponentBot::output_list_contents(std::vector<tuple<int, int, int>> &myList)
{
    for(const auto& e: myList)
    {
        std::cout << "SuperRegion ID# : "  << std::get<0>(e) << "Region ID# : "  << std::get<1>(e) <<  " Troop# : " << std::get<2>(e) << " Troop# " << endl;
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
            Refresh();
            //regionToSuperRegionMapping();               //This call should be replaced by Refresh()
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
