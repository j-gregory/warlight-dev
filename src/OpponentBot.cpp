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

void OpponentBot::SetStartingTroop(int troop)
{
    if(!startingArmyDefined)
    {
        minTroopPerTurn = troop;
        startingArmyDefined = true;
    }
}
//Adds a region to the list OpponentBot owns
//1st param: Region id
//2nd param is 0, if this is the starting region for the opponent.
void OpponentBot::AddRegion(int regionid, int troops)
{

    int superid, index, iterateSuperId, iterateId, previous_troop, delta= 0;
    bool indexFound = false;

    if(!allMappingSetupDone)
    {
        OpponentQueueRegions.push_back(tuple<int, int>(regionid, troops));
    }
    else
    {

        if(regionToSuperRegionMap.size() > 0 && superRegionToRegionMap.size() > 0 )
        {
            superid = std::get<0>(regionToSuperRegionMap.at(regionid));
            std::get<2>(regionToSuperRegionMap.at(regionid)) = troops;  //resetting the troop# for the region
            std::get<3>(regionToSuperRegionMap.at(regionid)) = true;    //resetting own status of the region

            for(unsigned int i=0; i<superRegionToRegionMap.size(); i++)
            {
                iterateSuperId = std::get<0>(superRegionToRegionMap.at(i));
                if(iterateSuperId == superid)
                {
                    iterateId = std::get<1>(superRegionToRegionMap.at(i));
                    if(iterateId == regionid)
                    {
                        index = i;
                        break;
                    }
                }
            }
            std::get<2>(superRegionToRegionMap.at(index)) = troops;
            std::get<3>(superRegionToRegionMap.at(index)) = true;

            //add to OpponentTotalRegion - visibe + invisible
            if(OpponentTotalRegion.size() > 0)
            {
                for(unsigned int i=0; i< OpponentTotalRegion.size(); i++)
                {
                    iterateId = std::get<1>(OpponentTotalRegion.at(i));
                    if(iterateId == regionid)
                    {
                        std::get<2>(OpponentTotalRegion.at(i)) = troops;
                        indexFound = true;
                    }
                }
                if(!indexFound)
                {
                    OpponentTotalRegion.push_back(tuple<int, int, int>(superid, regionid, troops));
                }
            }
            else
            {
                OpponentTotalRegion.push_back(tuple<int, int, int>(superid, regionid, troops));
            }
            indexFound = false;


            //add to OpponentVisbleRegion
            if(OpponentVisbleRegion.size() > 0)
            {
                for(unsigned int i=0; i< OpponentVisbleRegion.size(); i++)
                {
                    iterateId = std::get<0>(OpponentVisbleRegion.at(i));
                    if(iterateId == regionid)
                    {
                        previous_troop = std::get<1>(OpponentVisbleRegion.at(i));
                        std::get<1>(OpponentVisbleRegion.at(i)) = troops;
                        delta = ((troops - previous_troop) > 0 ? (troops - previous_troop) : 0);
                        std::get<2>(OpponentVisbleRegion.at(i)) = delta;
                        indexFound = true;
                    }
                }
                if(!indexFound)
                {
                    OpponentVisbleRegion.push_back(tuple<int, int, int>(regionid, troops, 0));
                }
            }
            else
            {
                OpponentVisbleRegion.push_back(tuple<int, int, int>(regionid, troops, 0));
            }
        }
    }

}

//Subtracts a region to the list OpponentBot owns
void OpponentBot::SubRegion(int regionid, int troops)
{
    //delete from OpponentTotalRegion   //<int, int, int>
    //update regionToSuperRegionMap, superRegionToRegionMap
    //update SuperRegionOwns

    int superid, index, iterateSuperId, iterateId= 0;

    if(!allMappingSetupDone)
    {
        ;   //do nothing
    }
    else
    {

        if(regionToSuperRegionMap.size() > 0 && superRegionToRegionMap.size() > 0 )
        {
            superid = std::get<0>(regionToSuperRegionMap.at(regionid));
            std::get<2>(regionToSuperRegionMap.at(regionid)) = troops;  //resetting the troop# for the region
            std::get<3>(regionToSuperRegionMap.at(regionid)) = false;    //resetting own status of the region

            for(unsigned int i=0; i<superRegionToRegionMap.size(); i++)
            {
                iterateSuperId = std::get<0>(superRegionToRegionMap.at(i));
                if(iterateSuperId == superid)
                {
                    iterateId = std::get<1>(superRegionToRegionMap.at(i));
                    if(iterateId == regionid)
                    {
                        index = i;
                        break;
                    }
                }
            }
            std::get<2>(superRegionToRegionMap.at(index)) = troops;
            std::get<3>(superRegionToRegionMap.at(index)) = false;

            //erase from OpponentTotalRegion if it does exist
            if(OpponentTotalRegion.size() > 0)
            {
                for(unsigned int i=0; i< OpponentTotalRegion.size(); i++)
                {
                    iterateId = std::get<1>(OpponentTotalRegion.at(i));
                    if(iterateId == regionid)
                    {
                        OpponentTotalRegion.erase(OpponentTotalRegion.begin() + i);
                    }
                }
            }

            //If this region belonged to a Super Region, time to disocunt from the list of
            if(SuperRegionOwns.size() > 0)
            {
                iterateSuperId = SuperRegionOwns[superid];
                if(iterateSuperId == superid)
                {
                    SuperRegionOwns.erase(SuperRegionOwns.begin()+superid);
                }
            }

        }
    }
}

//sends you the region that has maximum threat from OpponentBot
//if 0 is returned, means no visible region from opponent borders the player_bot yet
int OpponentBot::GetMaxMomentRegion()
{
    int regionid;

    if(OpponentVisbleRegion.size() > 0)
    {
        //sort OpponentVisibleRegion by delta in descending order, giving us most threatning region at the top
        std::sort(begin(OpponentVisbleRegion), end(OpponentVisbleRegion), TupleCompareDescending<2>());
        regionid = std::get<0>(OpponentVisbleRegion.at(0));
        return regionid;
    }
    else
    {
        return 0;
    }
}

void OpponentBot::RecieveStatus(int msg)
{
    switch(msg)
    {
        case Bot::BotSetupStages::SetRegions:       //3
            setRegionsStatus = true;
            break;
        case Bot::BotSetupStages::SetSuperRegions:  //4
            setSuperRegionsSatus = true;
            break;
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
        std::sort(begin(OpponentTotalRegion), end(OpponentTotalRegion), TupleCompareAscending<1>());

    }//addig super_regions

    allMappingSetupDone = true;    //this routine will not be triggered from now on.

    /*
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
    */
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
                        break;
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
    std::sort(begin(SuperRegionPursueList), end(SuperRegionPursueList), TupleCompareAscending<1>());    //ordered, so lowest cost always ontop

    /*
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
    */
}//OpponentBot::calculateCost()



void OpponentBot::calculateCostNormal()
{
    std::vector<tuple<int>> tempListOfSuperRegionsToConsider;
    bool alreadyOwn = false;
    bool foundEntryInSuperRegionPursueList = false;
    int tempCostofSuperRegion = 0;
    //int tempSuperRegion = 0;
    int tempSuperRegionToConsider = 0;
    int tempSuperRegionInPursueList = 0;
    int tempRegionToIgnore = 0;
    int tempRegionAudited = 0;
    SuperRegionPursueList.clear();      //start with cean slate
    //std::vector<tuple<int, int>>


    //use the list of SuperRegions & add all costs of those regions, excpept the ones you already own.
    for(unsigned int j=0; j<superRegionToRegionMap.size(); j++)
    {
        //tempSuperRegion = std::get<0>(superRegionToRegionMap.at(j));
        //tempRegionAudited = std::get<1>(superRegionToRegionMap.at(j));

        //check this row against all already owned region, which are not to be counted for cost calculation
        for(unsigned int k=0; k< OpponentTotalRegion.size(); k++)
        {
            tempRegionToIgnore = std::get<1>(OpponentTotalRegion.at(k));
            tempRegionAudited = std::get<1>(superRegionToRegionMap.at(j));
            //If the region in this row already belongs to opponent ignore.
            //if(std::get<1>(OpponentTotalRegion.at(k)) ==  std::get<1>(superRegionToRegionMap.at(j)))
            if(tempRegionToIgnore == tempRegionAudited)
            {
                alreadyOwn = true;
                break;
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

        alreadyOwn = false;
    }
    std::sort(begin(SuperRegionPursueList), end(SuperRegionPursueList), TupleCompareAscending<1>());    //ordered, so lowest cost always ontop
    std::sort(SuperRegionOwns.begin(), SuperRegionOwns.begin());
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


void OpponentBot::simulate()
{
    int ik = 1;
    int tempSuperRegionPersuing = 0;
    int predictedSuperRegionCost = 0;
    int tempCurIterSuperRegion = 0;
    int tempCurIterRegion = 0;
    int tempRegionToIgnore = 0;
    int troopPresent = 0;
    int troopRequiredForAttack = 0;
    int troopLeftAfterAttack = 0;
    int neighborPossibleID = 0;
    int neighborID = 0;
    int knowNeighborsIndex = 0;
    bool alreadyOwn = false;
    bool neighborFound = false;

    std::vector<int> tempNeighbors;
    std::vector<tuple<int, int, int>> unConqueredRegions;                //1st param: region_id,2nd param: troop, 3rd param: super_region

    predictedSuperRegionCost = std::get<1>(SuperRegionPursueList.at(0));

    while(predictedSuperRegionCost == 0)
    {
        predictedSuperRegionCost = std::get<1>(SuperRegionPursueList.at(ik));
        tempSuperRegionPersuing = std::get<0>(SuperRegionPursueList.at(ik));     //cheapest Super Region to obtain
        ik++;
    }

    //find regions in this SuperRegion that haven't been conquered yet
    for(unsigned int i=0; i<superRegionToRegionMap.size(); i++)
    {
        tempCurIterSuperRegion = get<0>(superRegionToRegionMap.at(i));

        if(tempCurIterSuperRegion == tempSuperRegionPersuing)
        {
            tempCurIterRegion = get<1>(superRegionToRegionMap.at(i));

            for(unsigned int k=0; k< OpponentTotalRegion.size(); k++)
            {
                tempRegionToIgnore = std::get<1>(OpponentTotalRegion.at(k));
                //If the region in this row already belongs to opponent ignore.
                if(tempRegionToIgnore == tempCurIterRegion)
                {
                    alreadyOwn = true;
                    break;
                }
            }
            //this consideration of this region for expansionis further explored
            if(!alreadyOwn)
            {
                troopPresent = std::get<2>(superRegionToRegionMap.at(i));
                unConqueredRegions.push_back(tuple<int, int, int>(tempCurIterRegion, troopPresent, tempCurIterSuperRegion));
            }
        }
        tempCurIterSuperRegion = 0;
        tempCurIterRegion = 0;
        troopPresent = 0;
        alreadyOwn = false;
    }

    //sort unConqueredRegions by their troop strength, in ascending order - it will be easier to conquer
    std::sort(begin(unConqueredRegions), end(unConqueredRegions), TupleCompareAscending<1>());

    //see if any of those regions can be acquired
    for(unsigned int i=0; i<unConqueredRegions.size(); i++)
    {
        tempCurIterRegion = get<0>(unConqueredRegions.at(i));
        tempNeighbors = bot->regions[tempCurIterRegion].getNeighbors();

        //we want to check if any neighboring regions already belong to us
        for(unsigned int j=0; j<OpponentTotalRegion.size(); j++)
        {
            neighborPossibleID = std::get<1>(OpponentTotalRegion.at(j));
            for(unsigned int k=0; k<tempNeighbors.size(); k++)
            {
                neighborID = tempNeighbors[k];
                if(neighborID == neighborPossibleID)
                {
                    knowNeighborsIndex = j;
                    neighborFound = true;
                    break;
                }
            }
            if(neighborFound)
            {
                break;
            }
        }

        //If ther is a known neighbor that belongs to opponent, from where an attack can be launched
        if(knowNeighborsIndex !=0)
        {
            troopPresent = std::get<2>(OpponentTotalRegion.at(knowNeighborsIndex));
            troopRequiredForAttack = staticCostIndex[troopPresent];
            troopLeftAfterAttack = troopRequiredForAttack - PredictTroopDestroyed(troopRequiredForAttack, troopPresent);    //troop attack/transfer to new region

            //Do opponent have enough troops to attack?
            //Have I launched attack from this region before ?

            if((troopLeftAfterAttack > 1) && (troopRequiredForAttack < (troopsLeftThisTurn + troopPresent)))
            {
                troopsLeftThisTurn -= (troopRequiredForAttack - troopPresent);  //troop left behind where the attack was launched from
                tempCurIterSuperRegion = std::get<2>(OpponentTotalRegion.at(knowNeighborsIndex));

                //adjust current region troop
                std::get<2>(OpponentTotalRegion.at(knowNeighborsIndex)) = 1;    //This was guesstimated, a more clever value could substitute this
                //enter the guesstimated new acquisition of region to OpponentTotalRegion
                OpponentTotalRegion.push_back(tuple<int, int, int>(tempCurIterSuperRegion,tempCurIterRegion,troopLeftAfterAttack));
            }

        }

        knowNeighborsIndex = 0;
        neighborPossibleID = 0;
        neighborID = 0;
        troopPresent = 0;
        neighborFound = false;
    }

    if(troopsLeftThisTurn > 0)
    {
        int indexz = OpponentTotalRegion.size();
        int troopz = std::get<2>(OpponentTotalRegion.at(indexz));
        std::get<2>(OpponentTotalRegion.at(indexz)) = troopz + troopsLeftThisTurn;
    }

}

void OpponentBot::superRegionInspection()
{
    int cost, reward, id, iterated_id = 0;
    reinforcementThisTurn  = minTroopPerTurn;

    //Have we owned added any Super Region yet ?
    if(SuperRegionPursueList.size() > 0)
    {
        //sort the list by cost
        std::sort(begin(SuperRegionPursueList), end(SuperRegionPursueList), TupleCompareAscending<1>());
        do
        {
            cost = std::get<1>(SuperRegionPursueList.at(0));
            if(cost == 0)
            {
                id = std::get<0>(SuperRegionPursueList.at(0));
                SuperRegionOwns.push_back(id);
                SuperRegionPursueList.erase(SuperRegionPursueList.begin());
            }
        }while(cost == 0);
    }

    //Recalculate reinforcement this turn
    if(SuperRegionOwns.size() > 0)
    {
        for(unsigned int i=0; i<SuperRegionOwns.size(); i++)
        {
            id = SuperRegionOwns[i];
            for(unsigned int j=0; j<bot->super_regions.size(); j++)
            {
                iterated_id = bot->super_regions[j].getID();
                if(iterated_id == id)
                {
                    reward = bot->super_regions[j].getReward();
                    reinforcementThisTurn += reward;
                }
            }
        }
    }

}

//does internal book-keeping on all regions
void OpponentBot::Refresh()
{
    //initial setup
    if(!allMappingSetupDone)
    {

        regionToSuperRegionMapping();
        calculateCost();
        simulate();
#ifndef DEBUG_CANCEL_PRINT
        std::cout << "Refresh (Setting) :=" << endl;
#endif

    }
    //update statments after update_map
    else
    {
        superRegionInspection();
        calculateCostNormal();
        simulate();
#ifndef DEBUG_CANCEL_PRINT
        std::cout << "Refresh (Update Map) :=" << endl;
#endif
    }

}


//Gets called during update_map and setup_map opponent_starting_regions
void OpponentBot::ObserveRegion(RegionOwner owner, int RegionID, int troops)
{
    switch(owner)
    {
        case OpponentBot::RegionOwner::PlayerBot:
            SubRegion(RegionID, troops);
            break;
        case OpponentBot::RegionOwner::OppoentBot:
            AddRegion(RegionID, troops);
            break;
        case OpponentBot::RegionOwner::Neutral:
            SubRegion(RegionID, troops);
            break;
        default:
            break;
    }
}

//Call to this has been replaced by static table look-up served by staticCostIndex @ the header file
int OpponentBot::predictTroopRequirement(int defending_destroyed)
{
    int temp;
    temp = 3+(((defending_destroyed  - (defending_destroyed * LUCK_MODIFIER_FACTOR)) / (1 - LUCK_MODIFIER_FACTOR))/0.6);
    return temp;
}

//Originally copied from MCTSManager::simulateBattle
int OpponentBot::PredictTroopDestroyed(int attacking_armies, int defending_armies)
{
  int attacking_destroyed = 0;
  //int defending_destroyed = 0;


  // Calculate how much attacking armies are destroyed with 100% luck
  for(int i = 1; i <= defending_armies; i++)
  {
    double chance = (rand() % 100) / 10.0;  // Between 0 and 1
    //70% chance to destroy one attacking army
    if(chance < 0.7) attacking_destroyed++;
  }

  attacking_destroyed = (int) round( ((defending_armies * 0.7) * (1 - LUCK_MODIFIER))
				    + (attacking_destroyed * LUCK_MODIFIER) );

  if(attacking_destroyed >= attacking_armies)
  {

    attacking_destroyed = attacking_armies;
  }
  return attacking_destroyed;
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


//todo: Don't use OpponentToalRegion as filter, check the 4th tuple for ownership
//call it in Parser::parseUpdateMap() & Parser::parseOpponentMoves

