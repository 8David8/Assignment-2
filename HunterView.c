// HunterView.c ... HunterView ADT implementation

#include <stdlib.h>
#include <assert.h>
#include "Globals.h"
#include "Game.h"
#include "GameView.h"
#include "HunterView.h"
// #include "Map.h" ... if you decide to use the Map ADT
     
struct hunterView {
   GameView gameState;
   Map mapState;

};
     

// Creates a new HunterView to summarise the current state of the game
HunterView newHunterView(char *pastPlays, PlayerMessage messages[])
{
    assert(pastPlays != NULL);
    assert(messages != NULL);

    HunterView hunterView = malloc(sizeof(struct hunterView));
    
    hunterView->gameState = newGameView(pastPlays, messages);
    hunterView->mapState = newMap(pastPlays, messages);
    
    return hunterView;



}
    
// Frees all memory previously allocated for the HunterView toBeDeleted
void disposeHunterView(HunterView toBeDeleted)
{
    //COMPLETE THIS IMPLEMENTATION
    free( toBeDeleted );
    toBeDeleted = NULL;
    return;
}


//// Functions to return simple information about the current state of the game

// Get the current round
Round giveMeTheRound(HunterView currentView)
{
    return (currentView->round);
}

// Get the id of current player
PlayerID whoAmI(HunterView currentView)
{
    PlayerID curr = 0;
    Round currTurn = getTurn(currView);
    if (currTurn % NUM_PLAYERS == 1) {
    	curr = 1;
    } else if (currTurn % NUM_PLAYERS == 2) {
	curr = 2
    } else if (currTurn % NUM_PLAYERS == 3) {
	curr = 3
    } else if (currTurn % NUM_PLAYERS == 4) {
	curr = 4
    } else {
	curr = 0;
    }
    return curr;
}

// Get the current score
int giveMeTheScore(HunterView currentView)
{
    return currentView->score;
}

// Get the current health points for a given player
int howHealthyIs(HunterView currentView, PlayerID player)
{
    int health =0;
    health = currentView->playerStats[player]->health;
    return health;
}

// Get the current location id of a given player
LocationID whereIs(HunterView currentView, PlayerID player)
{
    LocationID location = 0;
    location = currentView->playerStats[player]->location;
    return location;
}

//// Functions that return information about the history of the game

// Fills the trail array with the location ids of the last 6 turns
void giveMeTheTrail(HunterView currentView, PlayerID player,
                            LocationID trail[TRAIL_SIZE])
{
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
}

//// Functions that query the map to find information about connectivity

// What are my possible next moves (locations)
LocationID *whereCanIgo(HunterView currentView, int *numLocations,
                        int road, int rail, int sea)
{
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    return NULL;
}

// What are the specified player's next possible moves
LocationID *whereCanTheyGo(HunterView currentView, int *numLocations,
                           PlayerID player, int road, int rail, int sea)
{
    assert(currentView != NULL); // make sure that it's not empty
    assert(player >= 0 && player <= NUM_PLAYERS); //checks to see if the player ID is legit
    assert(message !=NULL); // makes sure message is not zero. 

    Round currRound = getRound(currentView);
    if(currRound == FIRST_ROUND) { // think someone hash defined it in the gameView. This checks for first round.
        
        
        

    }
    return NULL;
}
