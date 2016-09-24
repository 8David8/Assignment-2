// DracView.c ... DracView ADT implementation

#include <stdlib.h>
#include <assert.h>
#include "Globals.h"
#include "Game.h"
#include "GameView.h"
#include "DracView.h"
#include "Map.h"
#include "string.h"
// #include "Map.h" ... if you decide to use the Map ADT
     
struct dracView {
    GameView gameState; //takes all the repetitive round, score, playerinfo etc. from GameView
    Map mapState; //gets the graph of the board from Map
    LocationID traps[NUM_MAP_LOCATIONS];
    LocationID vamps[NUM_MAP_LOCATIONS];
    char *pastPlays;
};
     

// Creates a new DracView to summarise the current state of the game
DracView newDracView(char *pastPlays, PlayerMessage messages[])
{
    DracView dracView = malloc(sizeof(struct dracView));

    dracView->gameState = newGameView (pastPlays, messages); //takes everything from newGameView
    dracView->mapState = newMap (pastPlays, messages); //takes everything from Map
    dracView->pastPlays = pastPlays;

    int index = 0;

    while (index < NUM_MAP_LOCATIONS) {
        dracView->traps[index] = 0;
        dracView->vamps[index] = 0;
    }

    return dracView;
}
     
     
// Frees all memory previously allocated for the DracView toBeDeleted
void disposeDracView(DracView toBeDeleted)
{
    disposeGameView (toBeDeleted->gameState);
    disposeMap (toBeDeleted->mapState);
    free (toBeDeleted);
    toBeDeleted = NULL;
}


//// Functions to return simple information about the current state of the game

// Get the current round
Round giveMeTheRound(DracView currentView)
{ 
    return getRound(currentView->gameState); //retrieves current round from GameView
}

// Get the current score
int giveMeTheScore(DracView currentView)
{
    return getScore(currentView->gameState); //retrieves current score from GameView
}

// Get the current health points for a given player
int howHealthyIs(DracView currentView, PlayerID player)
{
    return getHealth(currentView->gameState, player); //retrieves given player's health from GameView
}

// Get the current location id of a given player
LocationID whereIs(DracView currentView, PlayerID player)
{
    return getLocation(currentView->gameState, player); //retrieves given player's location from GameView
}

// Get the most recent move of a given player
void lastMove(DracView currentView, PlayerID player,
                 LocationID *start, LocationID *end)
{
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    LocationID playerTrail[TRAIL_SIZE];
    giveMeTheTrail(currentView, player, playerTrail);

    int currentRound = giveMeTheRound(currentView); //retrieves the current round from the round function

    if (currentRound == 0) { //Case 1 - round 0
        if (playerTrail[0] == UNKNOWN_LOCATION) { //the player may not have had a turn yet, so their trail is empty i.e. [-1,-1,-1,-1,-1,-1]
            *start = UNKNOWN_LOCATION;
            *end = UNKNOWN_LOCATION;   
        } else { //or else the player has had a turn, so trail would have 1 location i.e. [??,-1,-1,-1,-1,-1]
            *start = playerTrail[0];
            *end = UNKNOWN_LOCATION;
        }
    } else if (currentRound < 5) { //Case 2 - round 1-5 
        *start = playerTrail[currentRound-1]; //round 5 every player's trail is filled. round 1-4, trail has something, but is not filled
        *end = playerTrail[currentRound];     //therefore, we use the currentRound number to do this

    } else { //Case 3 - round 6 or more
        *start = playerTrail[4]; 
        *end = playerTrail[5];

        if (player == PLAYER_DRACULA) {
            *start = playerTrail[3];
            *end = playerTrail[4];

        }
    }
}

// Find out what minions are placed at the specified location
void whatsThere(DracView currentView, LocationID where,
                         int *numTraps, int *numVamps)
{
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    int i = 0;
    while (i < strlen(currentView->pastPlays)) {
        if ((i % 40) > 31) {
            if ((i % 40) % 8 == 3) {
                if (currentView->pastPlays[i] == 'T') {
                    currentView->traps[whereIs(currentView, (i % 40)/8)]--;
                } else if (currentView->pastPlays[i+1] == 'V') {
                    currentView->vamps[whereIs(currentView, (i % 40) / 8)]--;
                }
            }
        } else {
            if ((i % 40) % 8 == 3) {
                if (currentView->pastPlays[i] == 'T') {
                    currentView->traps[whereIs(currentView, (i % 40)/8)]++;
                } else if (currentView->pastPlays[i+1] == 'V') {
                    currentView->vamps[whereIs(currentView, (i % 40) / 8)]++;
                }
            } else if ((i % 40)% 8 == 6) {
                if (currentView->pastPlays[i] == 'M') { //trap left trail
                    currentView->traps[abbrevToID(&currentView->pastPlays[i - 244])]-=1;
                } else if (currentView->pastPlays[i]=='V') { //mature vampire
                    currentView->vamps[abbrevToID(&currentView->pastPlays[i - 244])]-=1;
                }
            }
        }
        i++;
    }
    
    *numTraps = currentView->traps[where];
    *numVamps = currentView->vamps[where];
}

//// Functions that return information about the history of the game

// Fills the trail array with the location ids of the last 6 turns
void giveMeTheTrail(DracView currentView, PlayerID player,
                            LocationID trail[TRAIL_SIZE])
{
    getHistory(currentView->gameState, player, trail);
}

//// Functions that query the map to find information about connectivity

// What are my (Dracula's) possible next moves (locations)
LocationID *whereCanIgo(DracView currentView, int *numLocations, int road, int sea)
{
    LocationID draculaLocation = whereIs(currentView, PLAYER_DRACULA);

    int currentRound = giveMeTheRound(currentView); //retrieves the current round from the round function

    if (currentRound == 0) { //Case 1 - Round 0, Dracula is always last to move and he cannot move to ST_JOSEPH_AND_ST_MARYS
        LocationID *place = malloc(sizeof(LocationID)*NUM_MAP_LOCATIONS);
        int i;

        while (i < NUM_MAP_LOCATIONS) {
            if (i != ST_JOSEPH_AND_ST_MARYS) {
                place[*numLocations++] = i;
                
            } 
            i++;
        }
        return place;
    }
    return connectedLocations(currentView->gameState, numLocations, draculaLocation, PLAYER_DRACULA, currentRound, road, 0, sea);
}

// What are the specified player's next possible moves
LocationID *whereCanTheyGo(DracView currentView, int *numLocations,
                           PlayerID player, int road, int rail, int sea)
{
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    LocationID hunterLocation = whereIs (currentView, player);

    int currentRound = giveMeTheRound(currentView); //retrieves the current round from the round function

    if (currentRound == 0) { //Case 1 - Round 0, where player has not made a move, therefore can move anywhere to start
        LocationID *place = malloc(sizeof(LocationID)*NUM_MAP_LOCATIONS);
        int i;

        while (i < NUM_MAP_LOCATIONS) {
            place[*numLocations++] = i;
            i++;
        }
        
        return place;
    } 

    return connectedLocations(currentView->gameState, numLocations, hunterLocation, player, currentRound, road, rail, sea);
}
