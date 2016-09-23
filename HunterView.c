// HunterView.c ... HunterView ADT implementation

#include <stdlib.h>
#include <assert.h>
#include "Globals.h"
#include "Game.h"
#include "GameView.h"
#include "HunterView.h"
#include "Map.h" //... if you decide to use the Map ADT
     
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
    return getRound(currentView->gameState); 
}

// Get the id of current player
PlayerID whoAmI(HunterView currentView)
{
   
    return getCurrentPlayer(currentView->gameState);

}

// Get the current score
int giveMeTheScore(HunterView currentView)
{
    return getScore(currentView->gameState);
}

// Get the current health points for a given player
int howHealthyIs(HunterView currentView, PlayerID player)
{
    
    return getHealth(currentView->gameState, player);
}

// Get the current location id of a given player
LocationID whereIs(HunterView currentView, PlayerID player)
{
    return getLocation(currentView->gameState, player);
}

//// Functions that return information about the history of the game

// Fills the trail array with the location ids of the last 6 turns
void giveMeTheTrail(HunterView currentView, PlayerID player,
                            LocationID trail[TRAIL_SIZE])
{

    return getHistory(currentView->gameState, player, trail);

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
   
    LocationID  *moves; //makes a pointer to where i will make the array of locations.
    Round currRound = getRound(currentView->gameState);
    if(currRound == 1) {         
        moves = malloc(sizeof(LocationID)*NUM_MAP_LOCATIONS);
        // if it's the first turn, the hunters are free to be placed anywhere.
/*
	int counter = 0;
	int checker =0;
	LocationID location =0;
	int done = FALSE;
	while (done != TRUE) {
            if (idToType(checker) == LAND) {
	        moves[counter] == LocationID(checker); // typecast it to add into the array.
                counter++;
		checker++;
	    } else if (idToType(checker)) == SEA || idToType(checker == UNKNOWN){
                checker++;
	    } else {
		done = TRUE;
	    }
	}
        //what i tried to do here is make a look that goes through the location type def
	// and finds which location would be valid and add it to the array.
*/
	int counter =0;
        while(counter != NUM_MAP_LOCATION) {
	    moves[counter] == LocationID(counter);
	    counter++;
	}
	

    } else {
	connectedLocation(currentView->gameState, numLocations, player, road, rail sea);       



    }
    return moves;
}
