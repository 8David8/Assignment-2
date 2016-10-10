// DracView.c ... DracView ADT implementation

#include <stdlib.h>
#include <assert.h>
#include "Globals.h"
#include "Game.h"
#include "GameView.h"
#include "DracView.h"

#define DEATH 0
#define FIRST_ROUND 0
#define NO_RAIL 0

// each location has a limit of 3 traps and one immature vamp :D
#define TRAP_AND_VAMP 2  
#define TRAP_INDEX 0
#define VAMP_INDEX 1
#define NUM_CHAR_ENCOUNTER_HUNTER 4
#define NUM_CHAR_PER_PLAY 7
#define NUM_CHAR_NEW_LOCATION 2
#define PAST_PLAYS_DELIMITER ' '
#define NUM_CHAR_PLAYER 1

struct dracView {
    GameView gState; // takes all the repetitive round, score, playerinfo etc. from GameView
    // an array that stores all the Traps and Vampires in a give location
    int trapsAndVamp[NUM_MAP_LOCATIONS][TRAP_AND_VAMP];
};
     
static PlayerID convertNameAbbrevToID(char *abbrev);

// Creates a new DracView to summarise the current state of the game
DracView newDracView(char *pastPlays, PlayerMessage messages[])
{
    assert(pastPlays != NULL); assert(messages != NULL);

    DracView dView = malloc(sizeof(struct dracView));
    dView->gState = newGameView (pastPlays, messages); // takes everything from newGameView

    int index;
    for (index = 0; pastPlays[index] != '\0'; index += NUM_CHAR_PER_PLAY) {
        if (pastPlays[index] == PAST_PLAYS_DELIMITER) { index++; }

        char playerNameAbbrev[NUM_CHAR_PLAYER+1];
        playerNameAbbrev[0] = pastPlays[index]; playerNameAbbrev[1] = '\0';

        char newLocation[NUM_CHAR_NEW_LOCATION+1];
        newLocation[0] = pastPlays[index+1]; newLocation[1] = pastPlays[index+2];
        newLocation[2] = '\0';
        
        PlayerID currCharacter = convertNameAbbrevToID(playerNameAbbrev);
        LocationID updatedLocation = abbrevToID(newLocation);
      
        // if the current turn belongs to Dracula
        if (currCharacter == PLAYER_DRACULA) {
            // Dracula placed a trap
            if (pastPlays[index+3] == 'T') {
                dView->trapsAndVamp[updatedLocation][TRAP_INDEX]++;
            } 
            // Dracula placed a vampire
            if (pastPlays[index+4] == 'V') {
                dView->trapsAndVamp[updatedLocation][VAMP_INDEX]++;
            }
            // Trap expired
            if (pastPlays[index+5] == 'M') {
                // we have to get the location of Dracula 6 moves ago...
                // to do this we grab his trail, find his location 6 moves ago
                // and remove the trap at that location
                LocationID popTrapLocation[TRAIL_SIZE];
                giveMeTheTrail(dView, PLAYER_DRACULA, popTrapLocation);
                dView->trapsAndVamp[popTrapLocation[TRAIL_SIZE-1]][TRAP_INDEX]--;
            // Vampire Matures and WREAKS HAVOK D:
            } else if (pastPlays[index+5] == 'V') {
                // similarly, we grab the Dracula's location 6 moves ago
                // and remove the (once) immature vampire of the city
                // because he probably left to cause destruction in other places?... idk
                LocationID popTrapLocation[TRAIL_SIZE];
                giveMeTheTrail(dView, PLAYER_DRACULA, popTrapLocation);
                dView->trapsAndVamp[popTrapLocation[TRAIL_SIZE-1]][VAMP_INDEX]--;
            }
        // else its the hunter's turn :D
        } else {
            int encounterIndex;
            // we have to loop through the encounters 4 times (NUM_CHAR_ENCOUNTER_HUNTER) 
            // because there is a possibility that a hunter could go 
            // through multiple encounters in one turn
            for (encounterIndex = 0; encounterIndex < NUM_CHAR_ENCOUNTER_HUNTER; encounterIndex++) {
               // hunter encountered a trap, ouch
               // trap is then disarmed 
               // remove trap where it was originally
               if (pastPlays[encounterIndex+2] == 'T') {
                   dView->trapsAndVamp[updatedLocation][TRAP_INDEX]--;
               // hunter encounters an immature vampire
               // hunter kills it yay!
               // remove vampire out of the corresponding location
               } else if (pastPlays[encounterIndex+2] == 'V') {
                   dView->trapsAndVamp[updatedLocation][VAMP_INDEX]--;
               }
            }            
        }
    }
   return dView;
}
     
static PlayerID convertNameAbbrevToID(char *abbrev)
{
    PlayerID currPlayer; int firstIndex = 0;
    switch (abbrev[firstIndex]) {
        case 'G': currPlayer = PLAYER_LORD_GODALMING; break;
        case 'S': currPlayer = PLAYER_DR_SEWARD; break;
        case 'H': currPlayer = PLAYER_VAN_HELSING; break;
        case 'M': currPlayer = PLAYER_MINA_HARKER; break;
        case 'D': currPlayer = PLAYER_DRACULA; break;
        default: exit(1);  // not sure if exit(1) works
    }
    return currPlayer;
}
     
// Frees all memory previously allocated for the DracView toBeDeleted
void disposeDracView(DracView toBeDeleted)
{
    free(toBeDeleted->gState); free(toBeDeleted);
    toBeDeleted = NULL;
}

//// Functions to return simple information about the current state of the game

// Get the current round
Round giveMeTheRound(DracView currentView)
{ 
    return getRound(currentView->gState); //retrieves current round from GameView
}

// Get the current score
int giveMeTheScore(DracView currentView)
{
    return getScore(currentView->gState); //retrieves current score from GameView
}

// Get the current health points for a given player
int howHealthyIs(DracView currentView, PlayerID player)
{
    return getHealth(currentView->gState, player); //retrieves given player's health from GameView
}

// Get the current location id of a given player
LocationID whereIs(DracView currentView, PlayerID player)
{
    int trueLocation = getLocation(currentView->gState, player); //retrieves given player's location from GameView
   
    LocationID dracTrail[TRAIL_SIZE]; 
    giveMeTheTrail(currentView, player, dracTrail);

    switch (trueLocation) {
        case TELEPORT: trueLocation = CASTLE_DRACULA; break;
        case DOUBLE_BACK_1: trueLocation = dracTrail[1]; break;
        case DOUBLE_BACK_2: trueLocation = dracTrail[2]; break;
        case DOUBLE_BACK_3: trueLocation = dracTrail[3]; break;
        case DOUBLE_BACK_4: trueLocation = dracTrail[4]; break;
        case DOUBLE_BACK_5: trueLocation = dracTrail[5]; break;
        default: ;//Do nothing;
    }
    return trueLocation;
}

// Get the most recent move of a given player
void lastMove(DracView currentView, PlayerID player,
                 LocationID *start, LocationID *end)
{
    // get the most recent location
    LocationID mostRecentLocation = getLocation(currentView->gState, player);
    *end = mostRecentLocation;

    LocationID trail[TRAIL_SIZE];

    giveMeTheTrail(currentView, player, trail);

    // get the location before the most recent location
    LocationID mostRecentLocationBeforeMove = trail[1];
    *start = mostRecentLocationBeforeMove;

}

// Find out what minions are placed at the specified location
void whatsThere(DracView currentView, LocationID where,
                         int *numTraps, int *numVamps)
{
    if (where == SEA || where == UNKNOWN_LOCATION) {
        *numTraps = 0;
        *numVamps = 0;
    } else {
        *numTraps = currentView->trapsAndVamp[where][TRAP_INDEX];
        *numVamps = currentView->trapsAndVamp[where][VAMP_INDEX];
    }
}

//// Functions that return information about the history of the game

// Fills the trail array with the location ids of the last 6 turns
void giveMeTheTrail(DracView currentView, PlayerID player,
                            LocationID trail[TRAIL_SIZE])
{
    getHistory(currentView->gState, player, trail);
}

//// Functions that query the map to find information about connectivity

// What are my (Dracula's) possible next moves (locations)
LocationID *whereCanIgo(DracView currentView, int *numLocations, int road, int sea)
{
    LocationID draculaLocation = whereIs(currentView, PLAYER_DRACULA);
    int currentRound = giveMeTheRound(currentView); // retrieves the current round from the round function

    return connectedLocations(currentView->gState, numLocations,
                              draculaLocation, PLAYER_DRACULA,
                              currentRound, road, NO_RAIL, sea);
}

// What are the specified player's next possible moves
LocationID *whereCanTheyGo(DracView currentView, int *numLocations,
                           PlayerID player, int road, int rail, int sea)
{
    LocationID hunterLocation = whereIs(currentView, player);
    int currentRound = giveMeTheRound(currentView); // retrieves the current round from the round function

    return connectedLocations(currentView->gState, numLocations,
                              hunterLocation, player, 
                              currentRound, road, rail, sea);
}
